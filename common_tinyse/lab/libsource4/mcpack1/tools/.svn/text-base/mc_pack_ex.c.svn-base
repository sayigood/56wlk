/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: mc_pack_ex.c,v 1.7 2008/09/08 12:15:56 zhouzr Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file mc_pack_ex.c
 * @author zhouzr(zhouzerui@baidu.com)
 * @date 2008/01/29 16:30:29
 * @version $Revision: 1.7 $ 
 * @brief mc_pack extended api
 *  
 **/


#include <stdio.h>
#include <string.h>

#include "mc_pack_ex.h"

static const char *ERR_STR[] = {
	"MCP: Success",
	"MCP: Not found",
	"MCP: Bad parameter",
	"MCP: Bad name",
	"MCP: Wrong type",
	"MCP: Not enough space",
	"MCP: Name too long",
	"MCP: Already exist",
	"MCP: Bad data",
	"MCP: Null pointer",
	"MCP: Found but wrong type",
};

const char*
mc_pack_perror(int err)
{
	if (err <= 0 && err >= -10) {
		return ERR_STR[-err];
	}

	return "MCP: Unknow error";
}

static struct _type_name_t {
	int type;
	const char *name;
} TNMAP[] = {
	{MC_PT_PCK, "pack"},
	{MC_PT_OBJ, "object"},
	{MC_PT_ARR, "array"},
	{MC_IT_BIN, "raw"},
	{MC_IT_TXT, "str"},
	{MC_IT_I32, "int32"},
	{MC_IT_U32, "uint32"},
	{MC_IT_I64, "int64"},
	{MC_IT_U64, "uint64"},
	{0, NULL}
};

const char*
mc_pack_ptype(int type)
{
	struct _type_name_t *ptn = TNMAP;
	while (ptn->name) {
		if (type == ptn->type) {
			return ptn->name;
		}
		++ptn;
	}
	return "unknown";
}

int
mc_pack_cmp_str(const mc_pack_t *ppack, const char *name, const char *value)
{
	const char *s = NULL;
	int err = 0;

	if (NULL == value) {
		return MC_PE_BAD_PARAM;
	}

	s = mc_pack_get_str(ppack, name);
	err = MC_PACK_PTR_ERR(s);
	if (err) {
		return err;
	}
	if (strcmp(s, value) == 0) {
		return 0;
	}
	return 1;
}

int
mc_pack_casecmp_str(const mc_pack_t *ppack, const char *name, const char *value)
{
	const char *s = NULL;
	int err = 0;

	if (NULL == value) {
		return MC_PE_BAD_PARAM;
	}

	s = mc_pack_get_str(ppack, name);
	err = MC_PACK_PTR_ERR(s);
	if (err) {
		return err;
	}
	if (strcasecmp(s, value) == 0) {
		return 0;
	}
	return 1;
}

enum _fmt_type_t {
	FMT_PS = 1,			  /**< %s       */
	FMT_PD = 2,			  /**< %d       */
	FMT_PU = 3,			  /**< %u       */
	FMT_PLLD = 4,		  /**< %lld       */
	FMT_PLLU = 5,		  /**< %llu       */
	FMT_PP = 6,			  /**< %p       */
	FMT_PLAIN = 7,		  /**< string not contain %       */
	FMT_PPPU = 8,		  /**< %p%u       */
	FMT_BAD = -1		  /**< invalid       */
};

#define NOT_DELIM(ch) (MC_PACK_OBJDELIM != ch \
		&& MC_PACK_ARRDELIM != ch && 0 != ch)

static inline int is_fmt_ps(const char *fmt, u_int s, u_int e)
{
	if (e - s != 1) {
		return 0;
	}
	return ('%' == fmt[s] && 's' == fmt[e]);
}

static inline int is_fmt_pd(const char *fmt, u_int s, u_int e)
{
	if (e - s != 1) {
		return 0;
	}
	return ('%' == fmt[s] && 'd' == fmt[e]);
}

static inline int is_fmt_pu(const char *fmt, u_int s, u_int e)
{
	if (e - s != 1) {
		return 0;
	}
	return ('%' == fmt[s] && 'u' == fmt[e]);
}

static inline int is_fmt_plld(const char *fmt, u_int s, u_int e)
{
	if (e - s != 3) {
		return 0;
	}
	return ('%' == fmt[s] && 'l' == fmt[s + 1]
			&& 'l' == fmt[s + 2] && 'd' == fmt[e]);
}

static inline int is_fmt_pllu(const char *fmt, u_int s, u_int e)
{
	if (e - s != 3) {
		return 0;
	}
	return ('%' == fmt[s] && 'l' == fmt[s + 1]
			&& 'l' == fmt[s + 2] && 'u' == fmt[e]);
}

static inline int is_fmt_pp(const char *fmt, u_int s, u_int e)
{
	if (e - s != 1) {
		return 0;
	}
	return ('%' == fmt[s] && 'p' == fmt[e]);
}

static inline int is_fmt_plain(const char *fmt, u_int s, u_int e)
{
	if (e < s) {
		return 0;
	}
	while (s <= e) {
		if ('%' == fmt[s]) {
			return 0;
		}
		++s;
	}
	return 1;
}

static inline int is_fmt_pppu(const char *fmt, u_int s, u_int e)
{
	if (e - s != 3) {
		return 0;
	}
	return ('%' == fmt[s] && 'p' == fmt[s + 1] && '%' == fmt[s + 2] && 'u' == fmt[e]);
}

static int get_fmt_type(const char *fmt, u_int s, u_int e)
{
	if (is_fmt_ps(fmt, s, e)) {
		return FMT_PS;
	}
	if (is_fmt_pd(fmt, s, e)) {
		return FMT_PD;
	}
	if (is_fmt_pu(fmt, s, e)) {
		return FMT_PU;
	}
	if (is_fmt_plld(fmt, s, e)) {
		return FMT_PLLD;
	}
	if (is_fmt_pllu(fmt, s, e)) {
		return FMT_PLLU;
	}
	if (is_fmt_pp(fmt, s, e)) {
		return FMT_PP;
	}
	if (is_fmt_plain(fmt, s, e)) {
		return FMT_PLAIN;
	}
	if (is_fmt_pppu(fmt, s, e)) {
		return FMT_PPPU;
	}

	return FMT_BAD;
}

int
count_fmt_field(const char *fmt)
{
	int cnt = 1;

	while (fmt && *fmt) {
		if (NOT_DELIM(*fmt)) {
			++fmt;
			continue;
		}
		++cnt;
		++fmt;
	}

	return cnt / 2;
}

int
mc_pack_vputf(mc_pack_t *ppack, const char *fmt, va_list arg)
{
	int put_count = 0;
	int ret = 0;
	char key[MC_PACK_FIELD_NAME_LEN_MAX];
	int got_key = 0;
	u_int key_len = 0;
	u_int pre_pos = 0;
	u_int i = 0;
	char *raw = NULL;

	if (NULL == ppack || NULL == fmt) {
		return MC_PE_BAD_PARAM;
	}

	i = -1;
	do {
		++i;
		if (NOT_DELIM(fmt[i])) {
			continue;
		}

		ret = get_fmt_type(fmt, pre_pos, i - 1);
		if (got_key) {
			switch (ret) {
				case FMT_PS:
					ret = mc_pack_put_str(ppack, key, va_arg(arg, char*));
					if (ret) {
						return put_count;
					}
					++put_count;
					break;

				case FMT_PD:
					ret = mc_pack_put_int32(ppack, key, va_arg(arg, int));
					if (ret) {
						return put_count;
					}
					++put_count;
					break;

				case FMT_PU:
					ret = mc_pack_put_uint32(ppack, key, va_arg(arg, u_int));
					if (ret) {
						return put_count;
					}
					++put_count;
					break;

				case FMT_PLLD:
					ret = mc_pack_put_int64(ppack, key, va_arg(arg, mc_int64_t));
					if (ret) {
						return put_count;
					}
					++put_count;
					break;

				case FMT_PLLU:
					ret = mc_pack_put_uint64(ppack, key, va_arg(arg, mc_uint64_t));
					if (ret) {
						return put_count;
					}
					++put_count;
					break;

				case FMT_PPPU:
					raw = va_arg(arg, char*);
					ret = mc_pack_put_raw(ppack, key, raw, va_arg(arg, u_int));
					if (ret) {
						return put_count;
					}
					++put_count;
					break;

				default:
					return put_count;
			}
			got_key = 0;
		} else {
			switch (ret) {
				case FMT_PLAIN:
					key_len = i - pre_pos;
					if (key_len >= MC_PACK_FIELD_NAME_LEN_MAX) {
						key_len = MC_PACK_FIELD_NAME_LEN_MAX - 1;
					}
					strncpy(key, fmt + pre_pos, key_len);
					key[key_len] = 0;
					break;

				case FMT_PS:
					snprintf(key, sizeof(key), "%s", va_arg(arg, char*));
					key[sizeof(key) - 1] = 0;
					break;

				default:
					// not got key, return count
					return put_count;
			}
			got_key = 1;
		}

		pre_pos = i + 1;
	} while (fmt[i]);

	return put_count;
}

int
mc_pack_putf(mc_pack_t *ppack, const char *fmt, ...)
{
	va_list arg;
	int ret = 0;

	va_start(arg, fmt);
	ret = mc_pack_vputf(ppack, fmt, arg);
	va_end(arg);

	return ret;
}

int
mc_pack_vgetf(const mc_pack_t *ppack, const char *fmt, va_list arg)
{
	int get_count = 0;
	int ret = 0;
	char key[MC_PACK_FIELD_NAME_LEN_MAX];
	int got_key = 0;
	u_int key_len = 0;
	u_int pre_pos = 0;
	u_int i = 0;
	const char **s = NULL;

	if (NULL == ppack || NULL == fmt) {
		return MC_PE_BAD_PARAM;
	}

	i = -1;
	do {
		++i;
		if (NOT_DELIM(fmt[i])) {
			continue;
		}

		ret = get_fmt_type(fmt, pre_pos, i - 1);
		if (got_key) {
			switch (ret) {
				case FMT_PP:
					s = va_arg(arg, const char**);
					*s = mc_pack_get_str(ppack, key);
					ret = MC_PACK_PTR_ERR(*s);
					if (ret) {
						return get_count;
					}
					++get_count;
					break;

				case FMT_PD:
					ret = mc_pack_get_int32(ppack, key, va_arg(arg, int*));
					if (ret) {
						return get_count;
					}
					++get_count;
					break;

				case FMT_PU:
					ret = mc_pack_get_uint32(ppack, key, va_arg(arg, u_int*));
					if (ret) {
						return get_count;
					}
					++get_count;
					break;

				case FMT_PLLD:
					ret = mc_pack_get_int64(ppack, key, va_arg(arg, mc_int64_t*));
					if (ret) {
						return get_count;
					}
					++get_count;
					break;

				case FMT_PLLU:
					ret = mc_pack_get_uint64(ppack, key, va_arg(arg, mc_uint64_t*));
					if (ret) {
						return get_count;
					}
					++get_count;
					break;

				case FMT_PPPU:
					s = va_arg(arg, const char**);
					*s = mc_pack_get_raw(ppack, key, va_arg(arg, u_int*));
					ret = MC_PACK_PTR_ERR(*s);
					if (ret) {
						return get_count;
					}
					++get_count;
					break;

				default:
					return get_count;
			}
			got_key = 0;
		}
		else {
			switch (ret) {
				case FMT_PLAIN:
					key_len = i - pre_pos;
					if (key_len >= MC_PACK_FIELD_NAME_LEN_MAX) {
						key_len = MC_PACK_FIELD_NAME_LEN_MAX - 1;
					}
					strncpy(key, fmt + pre_pos, key_len);
					key[key_len] = 0;
					break;

				default:
					// not got key, return count
					return get_count;
			}
			got_key = 1;
		}

		pre_pos = i + 1;
	} while (fmt[i]);

	return get_count;
}

int
mc_pack_getf(const mc_pack_t *ppack, const char *fmt, ...)
{
	va_list arg;
	int ret = 0;

	va_start(arg, fmt);
	ret = mc_pack_vgetf(ppack, fmt, arg);
	va_end(arg);

	return ret;
}

mc_pack_t *
mc_pack_creatf(char *buf, u_int buf_size, const char *fmt, ...)
{
	int ret = 0;
	int fld_cnt = 0;
	mc_pack_t *pack = NULL;
	va_list arg;

	pack = mc_pack_create(buf, buf_size);
	if (MC_PACK_PTR_ERR(pack)) {
		return NULL;
	}

	fld_cnt = count_fmt_field(fmt);
	if (fld_cnt <= 0) {
		return NULL;
	}

	va_start(arg, fmt);
	ret = mc_pack_vputf(pack, fmt, arg);
	va_end(arg);

	if (ret != fld_cnt) {
		return NULL;
	}

	return pack;
}

static int
print_item(const mc_pack_item_t *pitem, char *str, u_int size)
{
	int ret = 0;
	int i, bytes;
	const int BIN_PRINT_BYTES = 8;

	switch (pitem->type) {
		case MC_IT_TXT:
			ret = snprintf(str, size, "%s:%s ", pitem->key, pitem->value);
			break;
		case MC_IT_I32:
			ret = snprintf(str, size, "%s:%d ", pitem->key, *(mc_int32_t *)pitem->value);
			break;
		case MC_IT_U32:
			ret = snprintf(str, size, "%s:%u ", pitem->key, *(mc_uint32_t *)pitem->value);
			break;
		case MC_IT_I64:
			ret = snprintf(str, size, "%s:%lld ", pitem->key, *(mc_int64_t *)pitem->value);
			break;
		case MC_IT_U64:
			ret = snprintf(str, size, "%s:%llu ", pitem->key, *(mc_uint64_t *)pitem->value);
			break;
		case MC_IT_BIN:
#define CHK_LEFT(all, used) do { \
	if ((int)all <= (int)used) { \
		return (int)all; \
	} \
} while (0)
			bytes = (int)pitem->value_size > BIN_PRINT_BYTES ? BIN_PRINT_BYTES : (int)pitem->value_size;
			ret = snprintf(str, size, "%s:BIN[", pitem->key);
			CHK_LEFT(size, ret);

			for (i = 0; i < bytes; ++i) {
				ret += snprintf(str + ret, size - ret, "0x%02hhX ", pitem->value[i]);
				CHK_LEFT(size, ret);
			}
			if ((int)pitem->value_size > BIN_PRINT_BYTES) {
				ret += snprintf(str + ret, size - ret, "...");
				CHK_LEFT(size, ret);
			}
			ret += snprintf(str + ret, size - ret, "] ");
			break;
		default:
			return 0;
	}
	CHK_LEFT(size, ret);
	return ret;
}

static int output_pack(const mc_pack_t *ppack, char *str, u_int size)
{
	mc_pack_item_t itm;
	int ret = 0;
	int size_left = (int)size;
	int used = 0;

	if (0 == size) {
		return 0;
	}

	ret = mc_pack_first_item(ppack, &itm);
	while (MC_PE_SUCCESS == ret) {
		if (MC_PT_OBJ == itm.type || MC_PT_ARR == itm.type) {
			ret = output_pack((const mc_pack_t *)itm.value, str + used, (u_int)size_left);
		}
		else {
			ret = print_item(&itm, str + used, (u_int)size_left);
		}

		if (size_left <= ret) {
			return used + ret;
		}

		used += ret;
		size_left -= ret;
		ret = mc_pack_next_item(&itm, &itm);
	}

	return used;
}

int mc_pack_pack2str(const mc_pack_t *ppack, char *str, u_int size)
{
	const u_int PACKSIZE_MAX = 100 * 1024 * 1024;

	if (NULL == ppack || NULL == str) {
		return MC_PE_BAD_PARAM;
	}

	if ( ! mc_pack_valid(ppack, PACKSIZE_MAX)) {
		return MC_PE_BAD_TYPE;
	}

	str[0] = 0;

	return (output_pack(ppack, str, size) < (int)size ? MC_PE_SUCCESS : MC_PE_NO_SPACE);
}

const char *
mc_pack_get_subkey(const char *key)
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


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
