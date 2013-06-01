/***************************************************************************
 * 
 * Copyright (c) 2007 Baidu.com, Inc. All Rights Reserved
 * $Id: mc_pack_core.c,v 1.20 2008/09/04 06:51:09 zhouzr Exp $ 
 * 
 **************************************************************************/



/**
 * @file mc_pack_core.c
 * @author zhouzr(zhouzerui@baidu.com)
 * @date 2007/12/27 14:35:38
 * @version $Revision: 1.20 $ 
 * @brief mc_pack core api
 * <pre> 
 *   一个 pack 的结构如下：
 *    ____________________________________
 *   | mc_pack_t | internal_item_t | key1 |
 *   |___________|_________________|______|
 *   | value1 | key2  | key2.key3         |
 *   |________|_______|___________________|
 *   | value2 | key2.key4 | value3        |
 *   |________|___________|_______________|
 *   | ... | key5 | key5:0 | value4       |
 *   |_____|______|________|______________|
 *   | key5:1 | value5 | ...              |
 *   |________|________|__________________|
 * </pre> 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "mc_pack_core.h"

#define MC_TAG_LEN 4
#define MC_PACK_TAG "PCK"
#define MC_ITEM_TAG "ITM"

/**
* @brief datapack 中数据项类型
*   datapack内部数据类型，对外不可见
*  
*/
typedef struct _internal_item_t {
	char tag[MC_TAG_LEN];	  /**< 用于与 mc_pack_t 区分       */
	int type;				  /**< see mc_pack_item_type_t      */
	u_int size;				  /**< 大小       */
	u_int offset;			  /**< 在 pack 中的偏移       */
	char data[0];			  /**< field_name + '\\0' + data       */
} internal_item_t;

#define likely(x) __builtin_expect(!!(x), 1)

#define PACK_ITEM_OFFSET ((size_t)((mc_pack_t *)0)->data)
#define ITEM_NAME_OFFSET ((size_t)((internal_item_t *)0)->data)
#define ITEM_DATA_OFFSET(p) ((size_t)((internal_item_t *)0)->data \
		+ strlen(((internal_item_t *)p)->data) + 1)
#define GET_ROOT_PACK(p) ((mc_pack_t *) \
		 ((char *) p - ((internal_item_t *) p)->offset))
#define TYPE_IS_PACK(type) (MC_PT_PCK == type \
		|| MC_PT_OBJ == type || MC_PT_ARR == type)
#define TYPE_NOT_PACK(type) (! TYPE_IS_PACK(type))
#define TYPE_IS_SUBPACK(type) (MC_PT_OBJ == type || MC_PT_ARR == type)
#define NOT_OBJECT(p) (get_type(p) != MC_PT_OBJ)
#define NOT_ARRAY(p) (get_type(p) != MC_PT_ARR)
#define IS_ARRAY(p) (get_type(p) == MC_PT_ARR)
#define NOT_NTSTR_ITEM(p) (0 != \
		*((char *)p + ((internal_item_t *)p)->size - 1))

/* check if array/object in pack      */
#define INVALID_SUB(pack, sub) ((char *)sub - ((internal_item_t *)sub)->offset \
		< (char *)pack)

/* FIXME 对齐问题，Fix in future */
#define AO_ITEM_COUNT(p) (*(u_int *) ((char *) p + ITEM_DATA_OFFSET(p)))
#define AO_CLEAR_COUNT(p) do { \
	*(u_int *) ((char *) p + ITEM_DATA_OFFSET(p)) = 0; \
} while (0)
#define AO_INCREASE_COUNT(p) do { \
	++(*(u_int *) ((char *) p + ITEM_DATA_OFFSET(p))); \
} while (0)
#define AO_DECREASE_COUNT(p) do { \
	--(*(u_int *) ((char *) p + ITEM_DATA_OFFSET(p))); \
} while (0)


static inline int
match_name(const char *s1, int len1, const char *s2, int len2)
{
	static u_char map[256] = {
		0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
		48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
		64, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
		112,113,114,115,116,117,118,119,120,121,122, 91, 92, 93, 94, 95,
		96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
		112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
		128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
		144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
		160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
		176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
		192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
		208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
		224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
		240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};
	
	if (len1 < len2 || map[(int)s1[len2 - 1]] != map[(int)s2[len2 - 1]]) {
		return 0;
	}

	while (*s1 && *s2 && map[(int)*s1] == map[(int)*s2]) {
		++s1;
		++s2;
	}
	if (*s1 != *s2) {
		return 0;
	}
	return 1;
}


/**
 * @brief check name's validation
 *		检查字段名合法性，不包含'.'和':'
 *
 * @param [in] name   : const char*
 * @return  int 
 * @retval  retval 
 *		- 0 : valid
 *		- !=0 : invalid
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/27 21:53:34
**/
static int
chk_name(const char *name)
{
	static const char delim[] = {MC_PACK_OBJDELIM, MC_PACK_ARRDELIM, '\0'};

	if (NULL == name) {
		return -1;
	}

	if (NULL == strpbrk(name, delim)) {
		return 0;
	}

	return -1;
}

/**
 * @brief get type of pack
 *
 * @param [in] ppack   : const mc_pack_t*
 * @return  int 
 * @retval  mc_pack_type_t 
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/31 15:19:17
**/
static inline int
get_type(const mc_pack_t * ppack)
{
	if (NULL == ppack) {
		return MC_PT_BAD;
	}
	if (MC_PACK_PTR_ERR(ppack) < 0) {
		return MC_PT_BAD;
	}

	if (strncmp(ppack->tag, MC_PACK_TAG, MC_TAG_LEN) == 0) {
		return MC_PT_PCK;
	}
	if (strncmp(ppack->tag, MC_ITEM_TAG, MC_TAG_LEN) == 0) {
		return ((internal_item_t *) ppack)->type;
	}

	return MC_PT_BAD;
}

/**
 * @brief write full name
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] name   : const char*
 * @param [out] full_name   : char*
 * @param [in] size   : u_int
 * @return  int 
 * @retval  errno 
 *		- >0 : name len
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/31 13:47:10
**/
static inline int
get_full_name(const mc_pack_t *ppack, const char *name, char *full_name, u_int size)
{
	u_int used = 0;
	const char *p = ((internal_item_t *) ppack)->data;

	full_name[0] = 0;
	switch (get_type(ppack)) {
		case MC_PT_ARR:
			while (*p && used < size) {
				full_name[used++] = *p++;
			}
			full_name[used++] = MC_PACK_ARRDELIM;
			p = name;
			while (*p && used < size) {
				full_name[used++] = *p++;
			}
			full_name[used] = 0;
			break;
		case MC_PT_OBJ:
			while (*p && used < size) {
				full_name[used++] = *p++;
			}
			full_name[used++] = MC_PACK_OBJDELIM;
			p = name;
			while (*p && used < size) {
				full_name[used++] = *p++;
			}
			full_name[used] = 0;
			break;
		case MC_PT_PCK:
			p = name;
			while (*p && used < size) {
				full_name[used++] = *p++;
			}
			full_name[used] = 0;
			break;
		default:
			return MC_PE_BAD_TYPE;
	}

	return used;
}

/**
 * @brief get a item from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] name   : const char*
 * @return  internal_item_t* 
 * @retval  MC_PACK_PTR_ERR(retval) 
 *		- 0 : found
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 09:55:11
**/
static internal_item_t *
get_item(const mc_pack_t * ppack, const char *name)
{
	char full_name[MC_PACK_FIELD_NAME_LEN_MAX];		  /* 考虑效率，没有初始化       */
	char *p = NULL;
	char *bound = NULL;
	int fn_len = 0;
	int pack_type = 0;
	internal_item_t *pitem = NULL;
	const mc_pack_t *root_pack = NULL;

	if (NULL == ppack || NULL == name) {
		return (internal_item_t *)MC_PE_BAD_PARAM;
	}

	pack_type = get_type(ppack);
	if (TYPE_NOT_PACK(pack_type)) {
		return (internal_item_t *)MC_PE_BAD_TYPE;
	}

	if ('\0' == name[0]) {
		return (internal_item_t *)MC_PE_BAD_NAME;
	}

		  /* get full name       */
	fn_len = get_full_name(ppack, name, full_name, sizeof(full_name));
	if (fn_len >= (int)sizeof(full_name)) {
		return (internal_item_t *)MC_PE_NAME_TOO_LONG;
	}

		  /* get root pack       */
	root_pack = ppack;
	if (TYPE_IS_SUBPACK(pack_type)) {
		root_pack = GET_ROOT_PACK(ppack);
	}
	bound = (char *)root_pack + root_pack->cur_pos;

	for (p = MC_PT_PCK == pack_type ? (char *)ppack->data : (char *)ppack;
			likely(p < bound); p += pitem->size) {
		pitem = (internal_item_t *) p;
		if (match_name(pitem->data,
					pitem->size - sizeof(internal_item_t), full_name, fn_len) == 0) {
			continue;
		}
		return pitem;
	}

	return (internal_item_t *)MC_PE_NOT_FOUND;
}

/**
 * @brief add item into pack
 *		因为加入 item 的过程都类似，使用同一个函数来完成不同类型 item 的添加
 *
 * @param [in] ppack   : mc_pack_t*
 *		pack指针，可以是指向 array 或 object 类型的
 * @param [in] name   : const char*
 *		名字
 * @param [in] type   : int
 *		类型 mc_pack_item_type_t
 * @param [in] data   : const char*
 *		数据，array 和 object 时为空
 * @param [in] size   : u_int
 *		数据大小
 * @return  mc_pack_t* 
 * @retval  MC_PACK_PTR_ERR(retval) 
 *		- 0 : 添加失败
 *		- <0 : 添加成功，array 和 object 返回指向 item 的指针，其它返回 ppack
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 10:29:22
**/
static mc_pack_t *
put_item(mc_pack_t * ppack, const char *name, int type, const char *data, u_int size)
{
	char full_name[MC_PACK_FIELD_NAME_LEN_MAX];		  /* 考虑效率，没有初始化       */
	int pack_type = 0;
	internal_item_t *pitem = NULL;
	u_int fname_len = 0;
	u_int total_len = 0;
	mc_pack_t *root_pack = NULL;

	if (NULL == ppack || NULL == name) {
		return (mc_pack_t *)MC_PE_BAD_PARAM;
	}

	pack_type = get_type(ppack);
	if (TYPE_NOT_PACK(pack_type)) {
		return (mc_pack_t *)MC_PE_BAD_TYPE;
	}

	if ('\0' == name[0] || chk_name(name)) {
		return (mc_pack_t *)MC_PE_BAD_NAME;
	}

		  /* check for duplicate name       */
	pitem = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(pitem) == 0) {
		return (mc_pack_t *)MC_PE_ALREADY_EXIST;
	}

		  /* get full name       */
	fname_len = get_full_name(ppack, name, full_name, sizeof(full_name));
	if (fname_len >= (int)sizeof(full_name)) {
		return (mc_pack_t *)MC_PE_NAME_TOO_LONG;
	}

		  /* get root pack       */
	root_pack = ppack;
	if (TYPE_IS_SUBPACK(pack_type)) {
		root_pack = GET_ROOT_PACK(ppack);
	}

		  /* check length       */
	total_len = fname_len + 1 + ITEM_NAME_OFFSET;
	if (data) { /* with data       */
		total_len += size;
	} else { /* array/object, alloc one int to record item count       */
		total_len += sizeof(int);
	}

	if (total_len > root_pack->bytes_left) {
		return (mc_pack_t *)MC_PE_NO_SPACE;
	}

		  /* add name, no data       */
	pitem = (internal_item_t *)((char *)root_pack + root_pack->cur_pos);
	strncpy(pitem->tag, MC_ITEM_TAG, MC_TAG_LEN);
	pitem->type = type;
	pitem->size = total_len;
	pitem->offset = root_pack->cur_pos;
	strncpy(pitem->data, full_name, fname_len + 1);

	if (data) {		  /* add data       */
		memcpy(pitem->data + fname_len + 1, data, size);
		if (MC_IT_TXT == type) {
			// make sure txt is NULL-terminated
			// pitem->data[fname_len + 1 + size - 1]
			pitem->data[fname_len + size] = 0;
		}
	} else {	  /* item count init       */
		AO_CLEAR_COUNT(pitem);
	}

		  /* adjust item count       */
	if (MC_PT_PCK == pack_type) {
		root_pack->item_count++;
	} else {
		AO_INCREASE_COUNT(ppack);
	}
		  /* adjust root pack       */
	root_pack->internal_item_count++;
	root_pack->cur_pos += total_len;
	root_pack->bytes_left -= total_len;

	return (mc_pack_t *)pitem;
}

mc_pack_t *
mc_pack_create(char *buf, u_int buf_size)
{
	mc_pack_t *ppack = NULL;

	if (NULL == buf) {
		return (mc_pack_t *)MC_PE_BAD_PARAM;
	}
	if (sizeof(mc_pack_t) + sizeof(internal_item_t) >= buf_size) {
		return (mc_pack_t *)MC_PE_NO_SPACE;
	}

	memset(buf, 0, sizeof(mc_pack_t));
	ppack = (mc_pack_t *) buf;

	strncpy(ppack->tag, MC_PACK_TAG, MC_TAG_LEN);
	ppack->item_count = 0;
	ppack->internal_item_count = 0;
	ppack->bytes_left = buf_size - PACK_ITEM_OFFSET;
	ppack->cur_pos = PACK_ITEM_OFFSET;
	ppack->version = MC_PACK_VERSION;

	return ppack;
}

int
mc_pack_get_size(const mc_pack_t * ppack)
{
	if (NULL == ppack) {
		return MC_PE_BAD_PARAM;
	}
	if (get_type(ppack) != MC_PT_PCK) {			  /* only pack       */
		return MC_PE_BAD_TYPE;
	}

	return (int)ppack->cur_pos;
}

int
mc_pack_get_item_count(const mc_pack_t * ppack)
{
	int pack_type = 0;

	if (NULL == ppack) {
		return MC_PE_BAD_PARAM;
	}

	pack_type = get_type(ppack);
	if (MC_PT_PCK == pack_type) {			  /* pack       */
		return (int)ppack->item_count;
	}

	if (MC_PT_ARR == pack_type || MC_PT_OBJ == pack_type) {	  /* array/object       */
		return (int)AO_ITEM_COUNT(ppack);
	}

	return MC_PE_BAD_TYPE;
}

mc_pack_t *
mc_pack_put_array(mc_pack_t * ppack, const char *name)
{
	char idx_name[16];
	u_int idx;

	if (IS_ARRAY(ppack)) {
		if (name) {
			return (mc_pack_t *)MC_PE_BAD_TYPE;
		}
		idx = AO_ITEM_COUNT(ppack);
		snprintf(idx_name, sizeof(idx_name), "%u", idx);
		return put_item(ppack, idx_name, MC_PT_ARR, NULL, 0);
	}

	return put_item(ppack, name, MC_PT_ARR, NULL, 0);
}

mc_pack_t *
mc_pack_put_object(mc_pack_t * ppack, const char *name)
{
	char idx_name[16];
	u_int idx;

	if (IS_ARRAY(ppack)) {
		if (name) {
			return (mc_pack_t *)MC_PE_BAD_TYPE;
		}
		idx = AO_ITEM_COUNT(ppack);
		snprintf(idx_name, sizeof(idx_name), "%u", idx);
		return put_item(ppack, idx_name, MC_PT_OBJ, NULL, 0);
	}

	return put_item(ppack, name, MC_PT_OBJ, NULL, 0);
}

mc_pack_t *
mc_pack_get_array(const mc_pack_t * ppack, const char *name)
{
	long err = 0;
	mc_pack_t *ap = NULL;
	const mc_pack_t *ppack_root = ppack;
	
	ap = (mc_pack_t *) get_item(ppack, name);
	err = MC_PACK_PTR_ERR(ap);
	if (err < 0) {
		return (mc_pack_t *)err;
	}
	if (NOT_ARRAY(ap)) {
		return (mc_pack_t *)MC_PE_FOUND_BUT_TYPE;
	}
	if (get_type(ppack) != MC_PT_PCK) { // array/object
		ppack_root = GET_ROOT_PACK(ppack);
	}
	if (INVALID_SUB(ppack_root, ap)) {
		return (mc_pack_t *)MC_PE_BAD_DATA;
	}

	return ap;
}

mc_pack_t *
mc_pack_get_array_arr(const mc_pack_t * ppack, u_int arr_index)
{
	char name[16];
	long err = 0;
	mc_pack_t *ap = NULL;
	const mc_pack_t *ppack_root = NULL;

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return (mc_pack_t *)MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	ap = (mc_pack_t *) get_item(ppack, name);
	err = MC_PACK_PTR_ERR(ap);
	if (err < 0) {
		return (mc_pack_t *)err;
	}
	if (NOT_ARRAY(ap)) {
		return (mc_pack_t *)MC_PE_FOUND_BUT_TYPE;
	}
	ppack_root = GET_ROOT_PACK(ppack);
	if (INVALID_SUB(ppack_root, ap)) {
		return (mc_pack_t *)MC_PE_BAD_DATA;
	}

	return ap;
}

mc_pack_t *
mc_pack_get_object(const mc_pack_t * ppack, const char *name)
{
	long err = 0;
	mc_pack_t *op = NULL;
	const mc_pack_t *ppack_root = ppack;
	
	op = (mc_pack_t *) get_item(ppack, name);
	err = MC_PACK_PTR_ERR(op);
	if (err < 0) {
		return (mc_pack_t *)err;
	}
	if (NOT_OBJECT(op)) {
		return (mc_pack_t *)MC_PE_FOUND_BUT_TYPE;
	}
	if (get_type(ppack) != MC_PT_PCK) { // array/object
		ppack_root = GET_ROOT_PACK(ppack);
	}
	if (INVALID_SUB(ppack_root, op)) {
		return (mc_pack_t *)MC_PE_BAD_DATA;
	}

	return op;
}

mc_pack_t *
mc_pack_get_object_arr(const mc_pack_t * ppack, u_int arr_index)
{
	mc_pack_t *op = NULL;
	const mc_pack_t *ppack_root = NULL;
	long err = 0;
	char name[16];

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return (mc_pack_t *)MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	op = (mc_pack_t *) get_item(ppack, name);
	err = MC_PACK_PTR_ERR(op);
	if (err < 0) {
		return (mc_pack_t *)err;
	}
	if (NOT_OBJECT(op)) {
		return (mc_pack_t *)MC_PE_FOUND_BUT_TYPE;
	}
	ppack_root = GET_ROOT_PACK(ppack);
	if (INVALID_SUB(ppack_root, op)) {
		return (mc_pack_t *)MC_PE_BAD_DATA;
	}

	return op;
}

int
mc_pack_valid(const mc_pack_t * ppack, u_int buf_size)
{
	int ret = 0;

	if (MC_PACK_PTR_ERR(ppack)) {
		return 0;
	}

	if (buf_size < sizeof(mc_pack_t) && strncmp(ppack->tag, MC_ITEM_TAG, MC_TAG_LEN)) {
		return 0;
	}

	ret = get_type(ppack);

	if (MC_PT_PCK == ret) {
		  /* check buf size       */
		if (ppack->cur_pos + ppack->bytes_left > buf_size) {
			return 0;
		}
		return 1;
	}

	if (MC_PT_ARR == ret || MC_PT_OBJ == ret) {
		return 1;
	}

	return 0;
}

int
mc_pack_put_int32(mc_pack_t * ppack, const char *name, mc_int32_t value)
{
	mc_pack_t *ret = NULL;
	char idx_name[16];
	u_int idx;

	if (NULL == ppack) {
		return MC_PE_BAD_PARAM;
	}

	if (IS_ARRAY(ppack)) {
		if (name) {
			return MC_PE_BAD_TYPE;
		}
		idx = AO_ITEM_COUNT(ppack);
		snprintf(idx_name, sizeof(idx_name), "%u", idx);
		ret = put_item(ppack, idx_name, MC_IT_I32, (const char *) &value, sizeof(int));
		return MC_PACK_PTR_ERR(ret);
	}

	ret = put_item(ppack, name, MC_IT_I32, (const char *) &value, sizeof(int));
	return MC_PACK_PTR_ERR(ret);
}

int
mc_pack_put_uint32(mc_pack_t * ppack, const char *name, mc_uint32_t value)
{
	mc_pack_t *ret = NULL;
	char idx_name[16];
	u_int idx;

	if (NULL == ppack) {
		return MC_PE_BAD_PARAM;
	}

	if (IS_ARRAY(ppack)) {
		if (name) {
			return MC_PE_BAD_TYPE;
		}
		idx = AO_ITEM_COUNT(ppack);
		snprintf(idx_name, sizeof(idx_name), "%u", idx);
		ret = put_item(ppack, idx_name, MC_IT_U32, (const char *) &value, sizeof(u_int));
		return MC_PACK_PTR_ERR(ret);
	}

	ret = put_item(ppack, name, MC_IT_U32, (const char *) &value, sizeof(u_int));
	return MC_PACK_PTR_ERR(ret);
}

int
mc_pack_put_int64(mc_pack_t * ppack, const char *name, mc_int64_t value)
{
	mc_pack_t *ret = NULL;
	char idx_name[16];
	u_int idx;

	if (NULL == ppack) {
		return MC_PE_BAD_PARAM;
	}

	if (IS_ARRAY(ppack)) {
		if (name) {
			return MC_PE_BAD_TYPE;
		}
		idx = AO_ITEM_COUNT(ppack);
		snprintf(idx_name, sizeof(idx_name), "%u", idx);
		ret = put_item(ppack, idx_name, MC_IT_I64, (const char *) &value, sizeof(int64_t));
		return MC_PACK_PTR_ERR(ret);
	}

	ret = put_item(ppack, name, MC_IT_I64, (const char *) &value, sizeof(int64_t));
	return MC_PACK_PTR_ERR(ret);
}

int
mc_pack_put_uint64(mc_pack_t * ppack, const char *name, mc_uint64_t value)
{
	mc_pack_t *ret = NULL;
	char idx_name[16];
	u_int idx;

	if (NULL == ppack) {
		return MC_PE_BAD_PARAM;
	}

	if (IS_ARRAY(ppack)) {
		if (name) {
			return MC_PE_BAD_TYPE;
		}
		idx = AO_ITEM_COUNT(ppack);
		snprintf(idx_name, sizeof(idx_name), "%u", idx);
		ret = put_item(ppack, idx_name, MC_IT_U64, (const char *) &value, sizeof(mc_uint64_t));
		return MC_PACK_PTR_ERR(ret);
	}

	ret = put_item(ppack, name, MC_IT_U64, (const char *) &value, sizeof(mc_uint64_t));
	return MC_PACK_PTR_ERR(ret);
}

static int
put_str(mc_pack_t *ppack, const char *name, const char *value, u_int size)
{
	mc_pack_t *ret = NULL;
	char idx_name[16];
	u_int idx;

	if (NULL == ppack || NULL == value) {
		return MC_PE_BAD_PARAM;
	}

	if (IS_ARRAY(ppack)) {
		if (name) {
			return MC_PE_BAD_TYPE;
		}
		idx = AO_ITEM_COUNT(ppack);
		snprintf(idx_name, sizeof(idx_name), "%u", idx);
		ret = put_item(ppack, idx_name, MC_IT_TXT, value, size);
		return MC_PACK_PTR_ERR(ret);
	}

	ret = put_item(ppack, name, MC_IT_TXT, value, size);
	return MC_PACK_PTR_ERR(ret);
}

int
mc_pack_put_str(mc_pack_t *ppack, const char *name, const char *value)
{
	if (NULL == value) {
		return MC_PE_BAD_PARAM;
	}
	return put_str(ppack, name, value, strlen(value) + 1);
}

int
mc_pack_put_nstr(mc_pack_t *ppack, const char *name, const char *value, u_int len)
{
	u_int slen = 0;

	if (NULL == value) {
		return MC_PE_BAD_PARAM;
	}
	slen = strlen(value);
	len = len > slen ? slen : len;

	return put_str(ppack, name, value, len + 1);
}

int
mc_pack_put_raw(mc_pack_t * ppack, const char *name, const void *rawdata, u_int size)
{
	mc_pack_t *ret = NULL;
	char idx_name[16];
	u_int idx;

	if (NULL == ppack || NULL == rawdata) {
		return MC_PE_BAD_PARAM;
	}

	if (IS_ARRAY(ppack)) {
		if (name) {
			return MC_PE_BAD_TYPE;
		}
		idx = AO_ITEM_COUNT(ppack);
		snprintf(idx_name, sizeof(idx_name), "%u", idx);
		ret = put_item(ppack, idx_name, MC_IT_BIN, (const char *) rawdata, size);
		return MC_PACK_PTR_ERR(ret);
	}

	ret = put_item(ppack, name, MC_IT_BIN, (const char *) rawdata, size);
	return MC_PACK_PTR_ERR(ret);
}

#define IVALUE(ip, typ) (*(typ *)((char *)ip + ITEM_DATA_OFFSET(ip)))
#define SET_INT_VAL(res, typ, val) do { \
	*(typ *)res = (typ)val; \
} while (0)

static inline mc_int64_t TO_I64(internal_item_t *ip)
{
	char *p = (char *)ip + ITEM_DATA_OFFSET(ip);
	char *end = NULL;
	int64_t val;

	val = strtoll(p, &end, 10);
	  /* if there is invalid char left or an underflow occurs, return INT64_MAX     */
	if (*end != '\0' || INT64_MIN == val) {
		val = INT64_MAX;
	}
	return val;
}

static inline mc_uint64_t TO_U64(internal_item_t *ip)
{
	char *p = (char *)ip + ITEM_DATA_OFFSET(ip);
	char *end = NULL;
	mc_uint64_t val;

	if ('-' == *p) {
		return UINT64_MAX;
	}

	val = strtoull(p, &end, 10);
	  /* if there is invalid char left, return UINT64_MAX     */
	if (*end != '\0') {
		val = UINT64_MAX;
	}
	return val;
}

static int get_i32(internal_item_t *ip, void *res)
{
	switch (get_type((mc_pack_t *) ip)) {
		case MC_IT_I32:
			SET_INT_VAL(res, mc_int32_t, IVALUE(ip, mc_int32_t));
			break;
		case MC_IT_U32:
			if (IVALUE(ip, mc_uint32_t) <= INT32_MAX) {
				SET_INT_VAL(res, mc_int32_t, IVALUE(ip, mc_uint32_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_I64:
			if (IVALUE(ip, mc_int64_t) <= INT32_MAX && IVALUE(ip, mc_int64_t) >= INT32_MIN) {
				SET_INT_VAL(res, mc_int32_t, IVALUE(ip, mc_int64_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_U64:
			if (IVALUE(ip, mc_uint64_t) <= INT32_MAX) {
				SET_INT_VAL(res, mc_int32_t, IVALUE(ip, mc_uint64_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_TXT:
			if (TO_I64(ip) <= INT32_MAX
					&& TO_I64(ip) >= INT32_MIN) {
				SET_INT_VAL(res, mc_int32_t, TO_I64(ip));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		default:
			return MC_PE_FOUND_BUT_TYPE;
	}

	return MC_PE_SUCCESS;
}

static int get_u32(internal_item_t *ip, void *res)
{
	switch (get_type((mc_pack_t *) ip)) {
		case MC_IT_I32:
			if (IVALUE(ip, mc_int32_t) >= 0) {
				SET_INT_VAL(res, mc_uint32_t, IVALUE(ip, mc_int32_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_U32:
			SET_INT_VAL(res, mc_uint32_t, IVALUE(ip, mc_uint32_t));
			break;
		case MC_IT_I64:
			if (IVALUE(ip, mc_int64_t) <= UINT32_MAX && IVALUE(ip, mc_int64_t) >= 0) {
				SET_INT_VAL(res, mc_uint32_t, IVALUE(ip, mc_int64_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_U64:
			if (IVALUE(ip, mc_uint64_t) <= UINT32_MAX) {
				SET_INT_VAL(res, mc_uint32_t, IVALUE(ip, mc_uint64_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_TXT:
			if (TO_U64(ip) <= UINT32_MAX) {
				SET_INT_VAL(res, mc_uint32_t, TO_U64(ip));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		default:
			return MC_PE_FOUND_BUT_TYPE;
	}

	return MC_PE_SUCCESS;
}

static int get_i64(internal_item_t *ip, void *res)
{
	switch (get_type((mc_pack_t *) ip)) {
		case MC_IT_I32:
			SET_INT_VAL(res, mc_int64_t, IVALUE(ip, mc_int32_t));
			break;
		case MC_IT_U32:
			SET_INT_VAL(res, mc_int64_t, IVALUE(ip, mc_uint32_t));
			break;
		case MC_IT_I64:
			SET_INT_VAL(res, mc_int64_t, IVALUE(ip, mc_int64_t));
			break;
		case MC_IT_U64:
			if (IVALUE(ip, mc_uint64_t) <= INT64_MAX) {
				SET_INT_VAL(res, mc_int64_t, IVALUE(ip, mc_uint64_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_TXT:
			if (TO_I64(ip) < INT64_MAX) {  /* == INT64_MAX means err       */
				SET_INT_VAL(res, mc_int64_t, TO_I64(ip));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		default:
			return MC_PE_FOUND_BUT_TYPE;
	}

	return MC_PE_SUCCESS;
}

static int get_u64(internal_item_t *ip, void *res)
{
	switch (get_type((mc_pack_t *) ip)) {
		case MC_IT_I32:
			if (IVALUE(ip, mc_int32_t) >= 0) {
				SET_INT_VAL(res, mc_uint64_t, IVALUE(ip, mc_int32_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_U32:
			SET_INT_VAL(res, mc_uint64_t, IVALUE(ip, mc_uint32_t));
			break;
		case MC_IT_I64:
			if (IVALUE(ip, mc_int64_t) >= 0) {
				SET_INT_VAL(res, mc_uint64_t, IVALUE(ip, mc_int64_t));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		case MC_IT_U64:
			SET_INT_VAL(res, mc_uint64_t, IVALUE(ip, mc_uint64_t));
			break;
		case MC_IT_TXT:
			if (TO_U64(ip) < UINT64_MAX) {  /* == UINT64_MAX means err       */
				SET_INT_VAL(res, mc_uint64_t, TO_U64(ip));
			} else {
				return MC_PE_FOUND_BUT_TYPE;
			}
			break;
		default:
			return MC_PE_FOUND_BUT_TYPE;
	}

	return MC_PE_SUCCESS;
}

int
mc_pack_get_int32(const mc_pack_t * ppack, const char *name, mc_int32_t * result)
{
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == name || NULL == result) {
		return MC_PE_BAD_PARAM;
	}

	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return MC_PACK_PTR_ERR(ip);
	}

	return get_i32(ip, result);
}

int
mc_pack_get_uint32(const mc_pack_t * ppack, const char *name, mc_uint32_t * result)
{
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == name || NULL == result) {
		return MC_PE_BAD_PARAM;
	}

	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return MC_PACK_PTR_ERR(ip);
	}

	return get_u32(ip, result);
}

int
mc_pack_get_int64(const mc_pack_t * ppack, const char *name, mc_int64_t * result)
{
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == name || NULL == result) {
		return MC_PE_BAD_PARAM;
	}

	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return MC_PACK_PTR_ERR(ip);
	}

	return get_i64(ip, result);
}

int
mc_pack_get_uint64(const mc_pack_t * ppack, const char *name, mc_uint64_t * result)
{
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == name || NULL == result) {
		return MC_PE_BAD_PARAM;
	}

	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return MC_PACK_PTR_ERR(ip);
	}

	return get_u64(ip, result);
}

const char *
mc_pack_get_str(const mc_pack_t * ppack, const char *name)
{
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == name) {
		return (const char *)MC_PE_BAD_PARAM;
	}

	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return (const char *)ip;
	}
	if (get_type((mc_pack_t *) ip) != MC_IT_TXT) {
		return (const char *)MC_PE_FOUND_BUT_TYPE;
	}
	if (NOT_NTSTR_ITEM(ip)) {
		return (const char *)MC_PE_BAD_DATA;
	}

	return (const char *) ip + ITEM_DATA_OFFSET(ip);
}

const char *
mc_pack_get_str_def(const mc_pack_t * ppack, const char *name, const char *def_value)
{
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == name) {
		return (const char *)MC_PE_BAD_PARAM;
	}

	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return def_value;
	}
	if (get_type((mc_pack_t *) ip) != MC_IT_TXT) {
		return def_value;
	}
	if (NOT_NTSTR_ITEM(ip)) {
		return def_value;
	}

	return (const char *) ip + ITEM_DATA_OFFSET(ip);
}

const void *
mc_pack_get_raw(const mc_pack_t * ppack, const char *name, u_int *size)
{
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == name || NULL == size) {
		return (const void *)MC_PE_BAD_PARAM;
	}

	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return (const void *)ip;
	}
	if (get_type((mc_pack_t *) ip) != MC_IT_BIN) {
		return (const void *)MC_PE_FOUND_BUT_TYPE;
	}

	*size = ip->size - ITEM_DATA_OFFSET(ip);
	return (const void *)((char *)ip + ITEM_DATA_OFFSET(ip));
}

int
mc_pack_get_int32_arr(const mc_pack_t * ppack, u_int arr_index, mc_int32_t * result)
{
	char name[16];
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == result) {
		return MC_PE_BAD_PARAM;
	}

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return MC_PACK_PTR_ERR(ip);
	}

	return get_i32(ip, result);
}

int
mc_pack_get_uint32_arr(const mc_pack_t * ppack, u_int arr_index, mc_uint32_t * result)
{
	char name[16];
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == result) {
		return MC_PE_BAD_PARAM;
	}

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return MC_PACK_PTR_ERR(ip);
	}

	return get_u32(ip, result);
}

int
mc_pack_get_int64_arr(const mc_pack_t * ppack, u_int arr_index, mc_int64_t * result)
{
	char name[16];
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == result) {
		return MC_PE_BAD_PARAM;
	}

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return MC_PACK_PTR_ERR(ip);
	}

	return get_i64(ip, result);
}

int
mc_pack_get_uint64_arr(const mc_pack_t * ppack, u_int arr_index, mc_uint64_t * result)
{
	char name[16];
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == result) {
		return MC_PE_BAD_PARAM;
	}

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return MC_PACK_PTR_ERR(ip);
	}

	return get_u64(ip, result);
}

const char *
mc_pack_get_str_arr(const mc_pack_t * ppack, u_int arr_index)
{
	char name[16];
	internal_item_t *ip = NULL;

	if (NULL == ppack) {
		return (const char *)MC_PE_BAD_PARAM;
	}

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return (const char *)MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return (const char *)ip;
	}
	if (get_type((mc_pack_t *) ip) != MC_IT_TXT) {
		return (const char *)MC_PE_FOUND_BUT_TYPE;
	}
	if (NOT_NTSTR_ITEM(ip)) {
		return (const char *)MC_PE_BAD_DATA;
	}

	return (const char *)ip + ITEM_DATA_OFFSET(ip);
}

const char *
mc_pack_get_str_arr_def(const mc_pack_t * ppack, u_int arr_index, const char *def_value)
{
	char name[16];
	internal_item_t *ip = NULL;

	if (NULL == ppack) {
		return (const char *)MC_PE_BAD_PARAM;
	}

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return (const char *)MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return def_value;
	}
	if (get_type((mc_pack_t *) ip) != MC_IT_TXT) {
		return def_value;
	}
	if (NOT_NTSTR_ITEM(ip)) {
		return def_value;
	}

	return (const char *)ip + ITEM_DATA_OFFSET(ip);
}

const void *
mc_pack_get_raw_arr(const mc_pack_t * ppack, u_int arr_index, u_int *size)
{
	char name[16];
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == size) {
		return (const void *)MC_PE_BAD_PARAM;
	}

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return (const void *)MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", arr_index);
	ip = get_item(ppack, name);
	if (MC_PACK_PTR_ERR(ip) < 0) {
		return (const void *)ip;
	}
	if (get_type((mc_pack_t *) ip) != MC_IT_BIN) {
		return (const void *)MC_PE_FOUND_BUT_TYPE;
	}

	*size = ip->size - ITEM_DATA_OFFSET(ip);
	return (const void *)((char *)ip + ITEM_DATA_OFFSET(ip));
}

#if 0
static void
change_offset(internal_item_t *pitem, u_int off)
{
	while (get_type((mc_pack_t *)pitem) != MC_PT_BAD) {
		pitem->offset -= off;
		pitem = (internal_item_t *)((char *)pitem + pitem->size);
	}
}

/**
 * @brief delete item from pack
 *		根据名字删除数据项，如果名字是 array/object ，会删除其子数据项
 *
 * @param [in] ppack   : mc_pack_t*
 * @param [in] name   : const char*
 * @return  int 
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/02 14:33:58
**/
static int
item_del(mc_pack_t * ppack, const char *name)
{
	mc_pack_t *ppack_root = ppack;
	char full_name[MC_PACK_FIELD_NAME_LEN_MAX];		  /* 考虑效率，没有初始化       */
	int pack_type = 0;
	int ret = 0;
	char *p = NULL;
	internal_item_t *pitem = NULL;
	int moveoff = 0;
	int removed = 0;
	int removed_ever = 0;
	u_int size = 0;
	u_int i = 0;


	if (NULL == ppack || NULL == name) {
		return MC_PE_BAD_PARAM;
	}

	pack_type = get_type(ppack);
	if (TYPE_NOT_PACK(pack_type)) {
		return MC_PE_BAD_TYPE;
	}

	if ('\0' == name[0]) {
		return MC_PE_BAD_NAME;
	}

		  /* get full name       */
	ret = get_full_name(ppack, name, full_name, sizeof(full_name));
	if (ret >= (int)sizeof(full_name)) {
		return MC_PE_NAME_TOO_LONG;
	}

	ppack_root = ppack;
		  /* if array/object, get root pack       */
	if (TYPE_IS_SUBPACK(pack_type)) {
		ppack_root = GET_ROOT_PACK(ppack_root);
	}

	do {
		removed = 0;
		p = (char *) ppack_root->data;

		for (i = 0; i < ppack_root->internal_item_count; ++i) {
			if (p >= (char *)ppack_root + ppack_root->cur_pos) {		  /* 检查边界       */
				break;
			}

			pitem = (internal_item_t *) p;
			if (strncasecmp(pitem->data, full_name, strlen(full_name)) == 0) {
				/* 只匹配前缀，删除所有子数据项       */

				size = pitem->size;
				moveoff = ppack_root->cur_pos - size - ((char *) pitem - (char *) ppack_root);
				if (moveoff < 0) {
					return MC_PE_UNKNOWN;
				}

				ppack_root->cur_pos -= size;
				ppack_root->bytes_left += size;
				ppack_root->internal_item_count--;
				memmove((char *) pitem, (char *) pitem + size, (size_t)moveoff);
				//memset((char *) pitem + moveoff, 0, size); /* 确保不会有脏数据 */ /* 脏点儿不要紧 */
				change_offset(pitem, size);

				removed = 1;
				removed_ever = 1;
				break;
			}
			p += pitem->size;
		}
	} while (removed);		  /* 循环删除所有 full_name 开头的数据项       */

	if (removed_ever) {
		/* adjust item count       */
		if (MC_PT_PCK == pack_type) {
			ppack_root->item_count--;
		}
		if (MC_PT_ARR == pack_type || MC_PT_OBJ == pack_type) {
			AO_DECREASE_COUNT(ppack);
		}
		return MC_PE_SUCCESS;
	}

	return MC_PE_NOT_FOUND;
}

int
mc_pack_del_item(mc_pack_t * ppack, const char *name)
{
	return item_del(ppack, name);
}
#endif

/**
 * @brief internal_item_t to mc_pack_item_t
 *
 * @param [in] inter   : const internal_item_t*
 * @param [out] ext   : mc_pack_item_t*
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/02/13 11:52:56
**/
static inline int
inter2ext(const internal_item_t *inter, mc_pack_item_t *ext)
{
	switch (get_type((mc_pack_t *)inter)) {
		case MC_PT_ARR:
		case MC_PT_OBJ:		  /* array/object       */
			ext->type = inter->type;
			ext->value_size = AO_ITEM_COUNT(inter);
			ext->key = (char *)inter->data;
			ext->value = (char *)inter;
			break;
		case MC_IT_TXT:
		case MC_IT_BIN:		  /* bin/txt       */
		case MC_IT_I32:
		case MC_IT_U32:
		case MC_IT_I64:
		case MC_IT_U64:
			ext->type = inter->type;
			ext->value_size = inter->size - ITEM_DATA_OFFSET(inter);
			ext->key = (char *)inter->data;
			ext->value = (char *)inter + ITEM_DATA_OFFSET(inter);
			break;
		default:
			return MC_PE_NOT_FOUND;
	}

	return MC_PE_SUCCESS;
}

static inline internal_item_t *
ext2inter(const mc_pack_item_t *ext)
{
	return (internal_item_t *)((char *)ext->key - ITEM_NAME_OFFSET);
}

int
mc_pack_first_item(const mc_pack_t * ppack, mc_pack_item_t * pitem)
{
	static const char delim[] = {MC_PACK_OBJDELIM, MC_PACK_ARRDELIM, '\0'};
	int pack_type = 0;
	int prefix_len = 0;
	char prefix[MC_PACK_FIELD_NAME_LEN_MAX];
	const internal_item_t *pitem_edit = NULL;
	const mc_pack_t *ppack_root = NULL;
	const char *pnow = NULL;
	const char *pend = NULL;

	if (NULL == ppack || NULL == pitem) {
		return MC_PE_BAD_PARAM;
	}

	pack_type = get_type(ppack);
	if (TYPE_NOT_PACK(pack_type)) { /* only pack/array/object       */
		return MC_PE_BAD_TYPE;
	}

	if (mc_pack_get_item_count(ppack) <= 0) {
		return MC_PE_NOT_FOUND;
	}

	prefix[0] = 0;
	if (MC_PT_PCK == pack_type) { /* pack       */
		pitem_edit = (internal_item_t *)ppack->data;
		return inter2ext(pitem_edit, pitem);
	}
	/* array/object       */
	pitem_edit = (internal_item_t *) ppack;
	prefix_len = snprintf(prefix, sizeof(prefix), "%s%c",
			pitem_edit->data,
			MC_PT_OBJ == pitem_edit->type ? MC_PACK_OBJDELIM : MC_PACK_ARRDELIM
			);
	ppack_root = GET_ROOT_PACK(ppack);

	pnow = (char *)pitem_edit;
	pend = (char *)ppack_root + ppack_root->cur_pos;
	for (;likely(pnow < pend); pnow += pitem_edit->size) {
		pitem_edit = (internal_item_t *) pnow;
		if (strncasecmp(pitem_edit->data, prefix, prefix_len) == 0) {
			/* check if in same level       */
			if (strpbrk(pitem_edit->data + prefix_len, delim) == NULL) {
				break;
			}
		}
	}

	if (pnow >= pend) {		  /* not found       */
		pitem_edit = NULL;
	}
		  /* 记录结果       */
	return inter2ext(pitem_edit, pitem);
}

/* 找到最后一个分隔符的位置       */
static inline int last_delim(const char *s, int len)
{
	int i;
	for (i = len - 1; i >= 0; --i) {
		if (MC_PACK_OBJDELIM == s[i] || MC_PACK_ARRDELIM == s[i]) {
			return i;
		}
	}
	return -1;
}

int
mc_pack_next_item(const mc_pack_item_t * pitem_now, mc_pack_item_t * pitem_out)
{
	static const char delim[] = {MC_PACK_OBJDELIM, MC_PACK_ARRDELIM, '\0'};
	int prefix_len = 0;
	char prefix[MC_PACK_FIELD_NAME_LEN_MAX];
	const internal_item_t *pitem_edit = NULL;
	const mc_pack_t *ppack_root = NULL;
	const char *pnow = NULL;
	const char *pend = NULL;

	if (NULL == pitem_now || NULL == pitem_out) {
		return MC_PE_BAD_PARAM;
	}

	pitem_edit = (internal_item_t *)((char *)pitem_now->key - ITEM_NAME_OFFSET);
	if (MC_PT_BAD == get_type((mc_pack_t *)pitem_edit)) {
		return MC_PE_BAD_TYPE;
	}

	prefix_len = snprintf(prefix, sizeof(prefix), "%s", pitem_edit->data);
	prefix_len = last_delim(prefix, prefix_len) + 1;
	prefix[prefix_len] = 0;

	ppack_root = GET_ROOT_PACK(pitem_edit);
	pnow = (char *)pitem_edit + pitem_edit->size;
	pend = (char *)ppack_root + ppack_root->cur_pos;
	for (;likely(pnow < pend); pnow += pitem_edit->size) {
		pitem_edit = (internal_item_t *) pnow;
		if (strncasecmp(pitem_edit->data, prefix, prefix_len) == 0) {
			/* check if in same level       */
			if (strpbrk(pitem_edit->data + prefix_len, delim) == NULL) {
				break;
			}
		}
	}

	if (pnow >= pend) { /* not found       */
		pitem_edit = NULL;
	}
		  /* 记录结果       */
	return inter2ext(pitem_edit, pitem_out);
}

int
mc_pack_get_by_idx(const mc_pack_t * ppack, u_int idx, mc_pack_item_t * pitem)
{
	int ret = 0;
	u_int i = 0;

	if (NULL == ppack || NULL == pitem) {
		return MC_PE_BAD_PARAM;
	}

	ret = get_type(ppack);
	if (TYPE_NOT_PACK(ret)) {
		return MC_PE_BAD_TYPE;
	}

	ret = mc_pack_first_item(ppack, pitem);
	if (ret < 0) {
		return MC_PE_NOT_FOUND;
	}
	if (0 == idx) {
		return MC_PE_SUCCESS;
	}

	for (i = 0; i < idx; ++i) {
		ret = mc_pack_next_item(pitem, pitem);
		if (ret < 0) {
			return MC_PE_NOT_FOUND;
		}
	}
	return MC_PE_SUCCESS;
}

int
mc_pack_close(mc_pack_t * ppack)
{
	if (NULL == ppack) {
		return MC_PE_BAD_PARAM;
	}

	if (get_type(ppack) != MC_PT_PCK) {
		return MC_PE_BAD_TYPE;
	}

	ppack->bytes_left = 0;
	return MC_PE_SUCCESS;
}

int
mc_pack_reopen(mc_pack_t * ppack, u_int buf_size)
{
	if (NULL == ppack) {
		return MC_PE_BAD_PARAM;
	}
	if (MC_PT_PCK == get_type(ppack)) {
		if (ppack->cur_pos + ppack->bytes_left > buf_size) {
			return MC_PE_BAD_PARAM;
		}
	}
	else {
		return MC_PE_BAD_TYPE;
	}

	ppack->bytes_left = buf_size - ppack->cur_pos;
	return MC_PE_SUCCESS;
}

/**
 * @brief pack copy
 *
 * @param [in] src   : const mc_pack_t*
 * @param [in] dst   : mc_pack_t*
 * @param [in] name   : const char*
 *		如果为 NULL，则 copy 所有子元素
 * @return  int 
 * @retval  errno 
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/24 16:01:47
**/
static int
pack_copy(const mc_pack_t * src, mc_pack_t * dst, const char *name)
{
	int ret = 0;
	int ret1 = 0;
	mc_pack_item_t itm;
	internal_item_t *ip = NULL;
	mc_pack_t *pp = NULL;
	char *value = NULL;

	if (NULL == src || NULL == dst) {
		return MC_PE_BAD_PARAM;
	}

	// 如果 src 和 dst 不是合法的 pack，返回 bad_type
	ret = get_type(src);
	if (TYPE_NOT_PACK(ret)) {
		return MC_PE_BAD_TYPE;
	}
	ret1 = get_type(dst);
	if (TYPE_NOT_PACK(ret1)) {
		return MC_PE_BAD_TYPE;
	}
	// src, dst 都是 array 或都不是 array, 否则 bat_type
	if ((MC_PT_ARR == ret) ^ (MC_PT_ARR == ret1)) {
		return MC_PE_BAD_TYPE;
	}

	// 如果 name 是 NULL，遍历 src，递归调用 pack_copy
	if (NULL == name) {
		ret = mc_pack_first_item(src, &itm);
		while (MC_PE_SUCCESS == ret) {
			ret = pack_copy(src, dst,
					// 取最末一级的名字
					itm.key + last_delim(itm.key, strlen(itm.key)) + 1);
			if (ret) {
				return ret;
			}
			ret = mc_pack_next_item(&itm, &itm);
		}

		return MC_PE_SUCCESS;
	}

	// 否则在 src 中查找 name
	ip = get_item(src, name);
	ret = MC_PACK_PTR_ERR(ip);
	if (ret) {
		return ret;
	}

	ret = get_type((mc_pack_t *)ip);
	switch (ret) {
		// 如果 item 是 object/array 在 dst 中加入 name，递归调用 pack_copy 加入子元素
		case MC_PT_ARR:
			pp = put_item(dst, name, MC_PT_ARR, NULL, 0);
			ret = MC_PACK_PTR_ERR(pp);
			if (ret) {
				return ret;
			}
			return pack_copy((mc_pack_t *)ip, pp, NULL);

		case MC_PT_OBJ:
			pp = put_item(dst, name, MC_PT_OBJ, NULL, 0);
			ret = MC_PACK_PTR_ERR(pp);
			if (ret) {
				return ret;
			}
			return pack_copy((mc_pack_t *)ip, pp, NULL);

			// 如果 item 是 txt/bin 调用 put_item 加入 dst
		case MC_IT_TXT:
			if (NOT_NTSTR_ITEM(ip)) {
				return MC_PE_BAD_DATA;
			}
			value = (char *)ip + ITEM_DATA_OFFSET(ip);
			pp = put_item(dst, name, MC_IT_TXT, value, strlen(value) + 1);
			return MC_PACK_PTR_ERR(pp);

		case MC_IT_BIN:
		case MC_IT_I32:
		case MC_IT_U32:
		case MC_IT_I64:
		case MC_IT_U64:
			pp = put_item(dst, name, ret, (char *)ip + ITEM_DATA_OFFSET(ip),
					ip->size - ITEM_DATA_OFFSET(ip));
			return MC_PACK_PTR_ERR(pp);

		default:
			return MC_PE_BAD_DATA;
	}
	
	//return MC_PE_SUCCESS;
}

int
mc_pack_copy_item(const mc_pack_t *src_pack, mc_pack_t *dst_pack, const char *name)
{
	return pack_copy(src_pack, dst_pack, name);
}

int
mc_pack_copy_item_arr(const mc_pack_t *src_arr, mc_pack_t *dst_arr, u_int idx)
{
	char name[16];
	snprintf(name, sizeof(name), "%u", idx);
	return pack_copy(src_arr, dst_arr, name);
}

int
mc_pack_get_item(const mc_pack_t * ppack, const char *name, mc_pack_item_t *pitem)
{
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == name || NULL == pitem) {
		return MC_PE_BAD_PARAM;
	}

	ip = get_item(ppack, name);

	return inter2ext(ip, pitem);
}

int
mc_pack_get_item_arr(const mc_pack_t * ppack, u_int idx, mc_pack_item_t *pitem)
{
	char name[16];
	internal_item_t *ip = NULL;

	if (NULL == ppack || NULL == pitem) {
		return MC_PE_BAD_PARAM;
	}

		  /* only array       */
	if (NOT_ARRAY(ppack)) {
		return MC_PE_BAD_TYPE;
	}

	snprintf(name, sizeof(name), "%u", idx);
	ip = get_item(ppack, name);

	return inter2ext(ip, pitem);
}

int
mc_pack_reset(mc_pack_t *ppack)
{
	if (get_type(ppack) != MC_PT_PCK) {
		return MC_PE_BAD_PARAM;
	}

	ppack->item_count = 0;
	ppack->internal_item_count = 0;
	ppack->bytes_left += ppack->cur_pos - PACK_ITEM_OFFSET;
	ppack->cur_pos = PACK_ITEM_OFFSET;

	return MC_PE_SUCCESS;
}

int
mc_pack_foreach(const mc_pack_t *ppack, mc_pack_foreach_callback cb, void *arg)
{
	int ret = 0;
	int retcnt = 0;
	int pack_type = 0;
	int prefix_len = 0;
	char prefix[MC_PACK_FIELD_NAME_LEN_MAX];
	const char *pnow = NULL;
	const char *pend = NULL;
	const mc_pack_t *root_pack = NULL;
	const internal_item_t *pitem = NULL;
	mc_pack_item_t item;

	if (NULL == ppack || NULL == cb) {
		return MC_PE_BAD_PARAM;
	}

	pack_type = get_type(ppack);
	if (TYPE_NOT_PACK(pack_type)) {
		return MC_PE_BAD_TYPE;
	}

	prefix_len = 0;
	prefix[0] = 0;
	root_pack = ppack;
	pitem = (internal_item_t *)ppack;

	if (TYPE_IS_SUBPACK(pack_type)) {
		root_pack = GET_ROOT_PACK(ppack);
		prefix_len = snprintf(prefix, sizeof(prefix), "%s%c",
				pitem->data,
				MC_PT_OBJ == pitem->type ? MC_PACK_OBJDELIM : MC_PACK_ARRDELIM
				);
	}
	pend = (char *)root_pack + root_pack->cur_pos;

	for (pnow = MC_PT_PCK == pack_type ? (char *)ppack->data : (char *)ppack;
			likely(pnow < pend); pnow += pitem->size) {
		pitem = (internal_item_t *)pnow;
		if (strncasecmp(pitem->data, prefix, prefix_len) == 0) {
			if (inter2ext(pitem, &item)) {
				return MC_PE_BAD_DATA;
			}
			ret = cb(&item, arg);
			if (ret < 0) {
				return ret;
			} else {
				retcnt += ret;
			}
		}
	}

	return retcnt;
}

int
mc_pack_get_int32_from_item(const mc_pack_item_t *pitem, mc_int32_t *res)
{
	internal_item_t *ip = NULL;
	if (NULL == pitem || NULL == res) {
		return MC_PE_BAD_PARAM;
	}
	ip = ext2inter(pitem);
	return get_i32(ip, res);
}

int
mc_pack_get_uint32_from_item(const mc_pack_item_t *pitem, mc_uint32_t *res)
{
	internal_item_t *ip = NULL;
	if (NULL == pitem || NULL == res) {
		return MC_PE_BAD_PARAM;
	}
	ip = ext2inter(pitem);
	return get_u32(ip, res);
}

int
mc_pack_get_int64_from_item(const mc_pack_item_t *pitem, mc_int64_t *res)
{
	internal_item_t *ip = NULL;
	if (NULL == pitem || NULL == res) {
		return MC_PE_BAD_PARAM;
	}
	ip = ext2inter(pitem);
	return get_i64(ip, res);
}

int
mc_pack_get_uint64_from_item(const mc_pack_item_t *pitem, mc_uint64_t *res)
{
	internal_item_t *ip = NULL;
	if (NULL == pitem || NULL == res) {
		return MC_PE_BAD_PARAM;
	}
	ip = ext2inter(pitem);
	return get_u64(ip, res);
}

static inline const char *
subkey(const char *key)
{
	int len = strlen(key);
	int pos;

	for (pos = len - 1; pos >= 0; --pos) {
		if (MC_PACK_OBJDELIM == key[pos] || MC_PACK_ARRDELIM == key[pos]) {
			break;
		}
	}

	return key + pos + 1;
}

int
mc_pack_put_item(mc_pack_t *pack, const mc_pack_item_t *pitem)
{
	mc_pack_t *ret = NULL;

	if (NULL == pack || NULL == pitem) {
		return MC_PE_BAD_PARAM;
	}
	if (NULL == pitem->key) {
		return MC_PE_BAD_PARAM;
	}

	if (TYPE_IS_SUBPACK(pitem->type)) {
		ret = put_item(pack, subkey(pitem->key), pitem->type, NULL, 0);
	} else {
		ret = put_item(pack, subkey(pitem->key), pitem->type,
				pitem->value, pitem->value_size);
	}

	return MC_PACK_PTR_ERR(ret);
}


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
