/***************************************************************************
 * 
 * Copyright (c) 2007 Baidu.com, Inc. All Rights Reserved
 * $Id: mc_pack_core.h,v 1.9 2008/05/10 06:58:26 zhouzr Exp $ 
 * 
 **************************************************************************/



/**
 * @file mc_pack_core.h
 * @author zhouzr(zhouzerui@baidu.com)
 * @date 2007/12/25 21:04:37
 * @version $Revision: 1.9 $ 
 * @brief mc_pack core api
 *  
 **/
#ifndef __MC_PACK_CORE_H
#define __MC_PACK_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <sys/types.h>

#define MC_PACK_VERSION (0x0000)
#define MC_PACK_OBJDELIM ','
#define MC_PACK_ARRDELIM ':'

#define MC_PACK_FIELD_NAME_LEN_MAX (1024)

typedef int mc_int32_t;
typedef unsigned int mc_uint32_t;
typedef long long mc_int64_t;
typedef unsigned long long mc_uint64_t;

enum mc_pack_type_t {
	/* pack type       */
	MC_PT_BAD = 0x00,		  /**< 非法       */
	MC_PT_PCK = 0x01,		  /**< pack       */
	MC_PT_OBJ = 0x02,		  /**< object       */
	MC_PT_ARR = 0x04,		  /**< array       */

	/* item type       */
	MC_IT_BAD = 0x00,		  /**< 非法       */
	MC_IT_BIN = 0x10,		  /**< 二进制       */
	MC_IT_TXT = 0x20,		  /**< 文本       */

	MC_IT_SGN = 0x11,		  /**< 有符号       */
	MC_IT_UNS = 0x12,		  /**< 无符号       */
	MC_IT_32B = 0x14,		  /**< 32位       */
	MC_IT_64B = 0x18,		  /**< 64位       */

	MC_IT_I32 = MC_IT_SGN | MC_IT_32B,		  /**< int32       */
	MC_IT_U32 = MC_IT_UNS | MC_IT_32B,		  /**< uint32       */
	MC_IT_I64 = MC_IT_SGN | MC_IT_64B,		  /**< int64       */
	MC_IT_U64 = MC_IT_UNS | MC_IT_64B		  /**< uint64       */
};

enum mc_pack_error_t {
	MC_PE_SUCCESS = 0,			  /**< 成功       */
	MC_PE_NOT_FOUND = -1,		  /**< 未找到       */
	MC_PE_BAD_PARAM = -2,		  /**< 错误的参数，空指针等       */
	MC_PE_BAD_NAME = -3,		  /**< 名字不合法       */
	MC_PE_BAD_TYPE = -4,		  /**< 错误的类型       */
	MC_PE_NO_SPACE = -5,		  /**< 空间不足       */
	MC_PE_NAME_TOO_LONG = -6,	  /**< 名字太长       */
	MC_PE_ALREADY_EXIST = -7,	  /**< 已存在       */
	MC_PE_BAD_DATA = -8,		  /**< 数据错误       */
	MC_PE_NULL_POINTER = -9,	  /**< 空指针       */
	MC_PE_FOUND_BUT_TYPE = -10,	  /**< 空指针       */
	MC_PE_UNKNOWN = -255		  /**< 未知错误       */
};

/**
* @brief mc_pack_item_t
*   用于遍历的接口
*   当 type 为 array/object 时，value_size 保存的是 item_count
*   key 为名字，value 为指向 array/object 的指针
*  
*/
typedef struct _mc_pack_item_t {
	int type;			  /**< see mc_pack_type_t       */
	u_int value_size;	  /**< value 数据的大小       */
	const char *key;			  /**< 指向字段名的指针       */
	const char *value;		  /**< 指向数据的指针       */
} mc_pack_item_t;

typedef struct _mc_pack_t {
	char tag[4];
	u_int item_count;
	u_int bytes_left;
	u_int cur_pos;
	u_int internal_item_count;
	u_short version;
	u_short reserved;
	char data[0];
} mc_pack_t;

/**
 * @brief convert pointer to errno
 *
 * @param [in] ptr   : const void*
 * @return  inline int 
 * @retval  errno
 *		- 0 : good pointer
 *		- <0 : errno, see mc_pack_error_t
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/13 17:32:48
**/
static inline int MC_PACK_PTR_ERR(const void *ptr)
{
	if (NULL == ptr) {
		return MC_PE_NULL_POINTER;
	}

	return (((long)ptr >= -255 && (long)ptr < 0) ? (long)ptr : 0);
}

/**
 * @brief init a pack
 *		在指定的缓冲区上初始化一个 datapack
 *
 * @param [in] buf   : char*
 *		缓冲区
 * @param [in] buf_size   : u_int
 *		缓冲区大小，应至少比 sizeof(mc_pack_t) 大
 * @return  mc_pack_t* 
 *		指向 pack 结构的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/26 17:36:36
**/
mc_pack_t *mc_pack_create(char *buf, u_int buf_size);

/**
 * @brief get pack size
 *		取得 pack 的大小，只适用于 pack，不适用于 array/object
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 结构的指针
 * @return  int 
 * @retval errno   
 *		- >0 : pack size
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 13:16:59
**/
int mc_pack_get_size(const mc_pack_t * ppack);

/**
 * @brief get item count of pack
 *		取得 pack 的 item count，可用于 array/object
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 结构的指针
 * @return  int 
 * @retval errno   
 *		- >=0 : item count
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 13:38:59
**/
int mc_pack_get_item_count(const mc_pack_t * ppack);

/**
 * @brief init a array item in a pack
 *		在一个 pack 中创建一个数组型的 item
 *
 * @param [in] ppack   : mc_pack_t*
 *		datapack 指针
 * @param [in] name   : const char*
 *		数组名，ppack 为数组时 name 必须为 NULL
 * @return  mc_pack_t* 
 *		指向 array 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/26 22:20:50
**/
mc_pack_t *mc_pack_put_array(mc_pack_t * ppack, const char *name);

/**
 * @brief get array from pack
 *		从 pack 中取得 array 的指针
 *
 * @param [in] ppack   : const mc_pack_t*
 *		datapack 指针
 * @param [in] name   : const char*
 *		数组名
 * @return  mc_pack_t* 
 *		指向 array 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/02 15:31:21
**/
mc_pack_t *mc_pack_get_array(const mc_pack_t * ppack, const char *name);

/**
 * @brief get array from array
 *		从 array 中取得子 array 的指针
 *
 * @param [in] ppack   : const mc_pack_t*
 *		array 指针
 * @param [in] arr_index   : u_int
 * @return  mc_pack_t* 
 *		指向 array 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/03 14:04:51
**/
mc_pack_t *mc_pack_get_array_arr(const mc_pack_t * ppack, u_int arr_index);

/**
 * @brief init a object item in a pack
 *		在一个 pack 中创建一个结构类型的 item
 *
 * @param [in] ppack   : mc_pack_t*
 *		datapack 指针
 * @param [in] name   : const char*
 *		结构名，ppack 为数组时 name 必须为 NULL
 * @return  mc_pack_t* 
 *		指向 object 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/26 22:24:28
**/
mc_pack_t *mc_pack_put_object(mc_pack_t * ppack, const char *name);

/**
 * @brief get object from pack
 *		从 pack 中取得 object 的指针
 *
 * @param [in] ppack   : const mc_pack_t*
 *		datapack 指针
 * @param [in] name   : const char*
 *		结构名
 * @return  mc_pack_t* 
 *		指向 object 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/02 15:33:17
**/
mc_pack_t *mc_pack_get_object(const mc_pack_t * ppack, const char *name);

/**
 * @brief get object from array
 *		从 array 中取得 object 的指针
 *
 * @param [in] ppack   : const mc_pack_t*
 *		array 指针
 * @param [in] arr_index   : u_int
 * @return  mc_pack_t* 
 *		指向 object 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/03 14:07:49
**/
mc_pack_t *mc_pack_get_object_arr(const mc_pack_t * ppack, u_int arr_index);

/**
 * @brief check if a pointer is valid
 *		判断一个指针指向的是否为合法的 mc_pack_t 或 array/object mc_pack_item_t
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 array/object 的指针
 * @param [in] buf_size   : u_int
 * @return  int 
 * @retval validation
 *		- 1 : valid
 *		- 0 : invalid
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/27 15:35:00
**/
int mc_pack_valid(const mc_pack_t * ppack, u_int buf_size);

/**
 * @brief add int32 into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : mc_int32_t
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 11:52:21
**/
int mc_pack_put_int32(mc_pack_t * ppack, const char *name, mc_int32_t value);

/**
 * @brief get int32 from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [out] result   : mc_int32_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 15:30:37
**/
int mc_pack_get_int32(const mc_pack_t * ppack, const char *name, mc_int32_t * result);

/**
 * @brief add uint32 into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [in] value   : mc_uint32_t
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 11:58:48
**/
int mc_pack_put_uint32(mc_pack_t * ppack, const char *name, mc_uint32_t value);

/**
 * @brief get uint32 from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [out] result   : mc_uint32_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 10:49:27
**/
int mc_pack_get_uint32(const mc_pack_t * ppack, const char *name, mc_uint32_t * result);

/**
 * @brief add uint64 into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : mc_int64_t
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 11:59:14
**/
int mc_pack_put_int64(mc_pack_t * ppack, const char *name, mc_int64_t value);

/**
 * @brief get int64 from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [out] result   : mc_int64_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 10:58:34
**/
int mc_pack_get_int64(const mc_pack_t * ppack, const char *name, mc_int64_t * result);

/**
 * @brief add uint64 into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : mc_uint64_t
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 11:59:39
**/
int mc_pack_put_uint64(mc_pack_t * ppack, const char *name, mc_uint64_t value);

/**
 * @brief get uint64 from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [out] result   : mc_uint64_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 11:07:05
**/
int mc_pack_get_uint64(const mc_pack_t * ppack, const char *name, mc_uint64_t * result);

/**
 * @brief add a string into pack
 *		向 pack 中加入字符串，会将结尾的 '\\0' 也加入
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : const char*
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 13:51:48
**/
int mc_pack_put_str(mc_pack_t * ppack, const char *name, const char *value);

/**
 * @brief add a string into pack
 *		将 value 的前 n 个字节加入 pack 中，会自动补 '\\0'
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : const char*
 * @param [in] len   : u_int
 *		长度
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/28 18:22:29
**/
int mc_pack_put_nstr(mc_pack_t * ppack, const char *name, const char *value, u_int len);

/**
 * @brief get a string from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @return  const char* 
 *		指向 string 的指针
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:08:39
**/
const char *mc_pack_get_str(const mc_pack_t * ppack, const char *name);

/**
 * @brief get a string from pack with default value
 *		取得字符串，如果未找到，返回默认值
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [in] def_value   : const char*
 *		默认值，如果未找到，返回def_value
 * @return  const char* 
 *		指向 string 的指针
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:10:26
**/
const char *mc_pack_get_str_def(const mc_pack_t * ppack, const char *name, const char *def_value);

/**
 * @brief add raw data into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] rawdata   : const void*
 * @param [in] size   : u_int
 *		raw data 大小
 * @return  int 
 *		- 0 : succeed
 *		- <0 : failed
 * @retval errno   
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/02 21:40:01
**/
int mc_pack_put_raw(mc_pack_t * ppack, const char *name, const void *rawdata, u_int size);

/**
 * @brief get raw data from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [out] size   : u_int*
 *		raw data 的大小
 * @return  const void* 
 *		指向 raw data 的指针
 * @retval   MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:12:40
**/
const void *mc_pack_get_raw(const mc_pack_t * ppack, const char *name, u_int *size);

/**
 * @brief get int32 from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_int32_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/31 15:58:55
**/
int mc_pack_get_int32_arr(const mc_pack_t * ppack, u_int arr_index, mc_int32_t * result);

/**
 * @brief get uint32 from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_uint32_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/31 16:00:42
**/
int mc_pack_get_uint32_arr(const mc_pack_t * ppack, u_int arr_index, mc_uint32_t * result);

/**
 * @brief get int64 from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_int64_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/31 16:01:48
**/
int mc_pack_get_int64_arr(const mc_pack_t * ppack, u_int arr_index, mc_int64_t * result);

/**
 * @brief get uint64 from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_uint64_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/31 16:03:25
**/
int mc_pack_get_uint64_arr(const mc_pack_t * ppack, u_int arr_index, mc_uint64_t * result);

/**
 * @brief get string from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @return  const char* 
 *		指向 string 的指针
 * @retval   MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:14:51
**/
const char *mc_pack_get_str_arr(const mc_pack_t * ppack, u_int arr_index);

/**
 * @brief get string from array with default value
 *		用数组下标取得字符串，如果未找到，返回默认值
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [in] def_value   : const char*
 *		默认值，如果未找到，返回def_value
 * @return  const char* 
 *		指向 string 的指针
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:15:56
**/
const char *mc_pack_get_str_arr_def(const mc_pack_t * ppack, u_int arr_index, const char *def_value);

/**
 * @brief get raw data from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] size   : u_int*
 *		raw data 的大小
 * @return  const void* 
 *		指向 raw data 的指针
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:17:41
**/
const void *mc_pack_get_raw_arr(const mc_pack_t * ppack, u_int arr_index, u_int *size);

/**
 * @brief get first item
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/05 21:34:26
**/
int mc_pack_first_item(const mc_pack_t * ppack, mc_pack_item_t * pitem);

/**
 * @brief get next item
 *
 * @param [in] pitem_now   : const mc_pack_item_t*
 * @param [out] pitem_out   : mc_pack_item_t*
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/05 21:35:17
**/
int mc_pack_next_item(const mc_pack_item_t * pitem_now, mc_pack_item_t * pitem_out);

/**
 * @brief get item by index
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] idx   : u_int
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/05 21:35:44
**/
int mc_pack_get_by_idx(const mc_pack_t * ppack, u_int idx, mc_pack_item_t * pitem);

/**
 * @brief close a pack
 *		发送 pack 前必须调用此函数，将 pack 的剩余空间置为 0
 *
 * @param [in] ppack   : mc_pack_t*
 * @return  int 
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/08 21:01:27
**/
int mc_pack_close(mc_pack_t * ppack);

/**
 * @brief reopen a pack
 *		重新打开一个 close 了的 pack
 *
 * @param [in] ppack   : mc_pack_t*
 * @param [in] buf_size   : u_int
 * @return  int 
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/13 17:54:02
**/
int mc_pack_reopen(mc_pack_t * ppack, u_int buf_size);

/**
 * @brief copy item from one pack to another
 *		从一个 pack 中拷贝数据项到另一个 pack
 *
 * @param [in] src_pack   : const mc_pack_t*
 *		可以是 pack/array/object
 * @param [in] dst_pack   : mc_pack_t*
 *		可以是 pack/array/object，类型必须与 src_pack 一致
 * @param [in] name   : const char*
 *		如果为 NULL，则 copy 所有子元素
 * @return  int 
 * @retval  errno 
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/24 18:26:25
**/
int mc_pack_copy_item(const mc_pack_t * src_pack, mc_pack_t * dst_pack, const char *name);

/**
 * @brief copy item from one array to another
 *
 * @param [in] src_arr   : const mc_pack_t*
 *		source array
 * @param [in] dst_arr   : mc_pack_t*
 *		destination array
 * @param [in] idx   : u_int
 *		array index
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/07 21:20:15
**/
int mc_pack_copy_item_arr(const mc_pack_t * src_arr, mc_pack_t * dst_arr, u_int idx);

/**
 * @brief get item by name
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] name   : const char*
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/02/13 11:54:16
**/
int mc_pack_get_item(const mc_pack_t * ppack, const char *name, mc_pack_item_t *pitem);

/**
 * @brief get item by array index
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] idx   : u_int
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/21 15:49:23
**/
int mc_pack_get_item_arr(const mc_pack_t * ppack, u_int idx, mc_pack_item_t *pitem);

/**
 * @brief reset a pack
 *
 * @param [in] ppack   : mc_pack_t*
 * @return  int 
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/03/03 16:24:45
**/
int mc_pack_reset(mc_pack_t *ppack);

/**
 * @brief callback function for mc_pack_foreach
 *
 * @param [in] pitem   : const mc_pack_item_t*
 *		one item in pack
 * @param [in] arg   : void*
 *		argument for callback
 * @return  int 
 * @retval   
 *		- <0 : stop and return to caller of foreach
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/17 17:37:12
**/
typedef int (*mc_pack_foreach_callback)(const mc_pack_item_t *pitem, void *arg);

/**
 * @brief for each item in pack do sth.
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] cb   : mc_pack_foreach_callback
 *		callback function
 * @param [in] arg   : void*
 *		argument for callback
 * @return  int 
 * @retval   
 *		- <0 : err returned by cb
 *		- >=0 : sum of retval of cb
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/17 17:36:48
**/
int mc_pack_foreach(const mc_pack_t *ppack, mc_pack_foreach_callback cb, void *arg);

/**
 * @brief get int32 from item
 *
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_int32_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/23 15:05:50
**/
int mc_pack_get_int32_from_item(const mc_pack_item_t *pitem, mc_int32_t *res);

/**
 * @brief get uint32 from item
 *
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_uint32_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/23 15:06:43
**/
int mc_pack_get_uint32_from_item(const mc_pack_item_t *pitem, mc_uint32_t *res);

/**
 * @brief get int64 from item
 *
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_int64_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/23 15:07:02
**/
int mc_pack_get_int64_from_item(const mc_pack_item_t *pitem, mc_int64_t *res);

/**
 * @brief get uint64 from item
 *
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_uint64_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/23 15:07:20
**/
int mc_pack_get_uint64_from_item(const mc_pack_item_t *pitem, mc_uint64_t *res);

/**
 * @brief put item into pack
 *
 * @param [in] pack   : mc_pack_t*
 * @param [in] pitem   : const mc_pack_item_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 *	会取 item.key 的最末级名字，不自动处理子 pack 的情况
 * @author zhouzr
 * @date 2008/05/10 15:00:55
**/
int mc_pack_put_item(mc_pack_t *pack, const mc_pack_item_t *pitem);

#ifdef __cplusplus
}
#endif

#endif	//__MC_PACK_CORE_H

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
