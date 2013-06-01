/***************************************************************************
 *
 *
 **************************************************************************/



/**
 * @file mc_pack_ex.h
 * @brief mc_pack extended api
 *
 **/


#ifndef  __MC_PACK_EX_H_
#define  __MC_PACK_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#include "mc_pack_c.h"

    /**
     * @brief get error info string by errno
     *
     * @param [in] err   : int
     *		errno
     * @return  const char*
     *		error info string
     * @retval
     *		如果错误号未定义，返回 "MCP: Unknow error"
     * @see
     * @note
    **/
    const char *mc_pack_perror(int err);
    const char *mc_pack_perror_ptr(const mc_pack_t * ptr);

    /**
     * @brief get type info string by type
     *
     * @param [in] type   : int
     *		type, see mc_pack_type_t
     * @return  const char*
     *		type info string
     * @retval
     *		如果未定义，返回 "unknown"
     * @see
     * @note
    **/
    const char *mc_pack_ptype(int type);

    /**
     * @brief compare string value in pack
     *		比较 pack 中的字符串，区分大小写
     *
     * @param [in] ppack   : const mc_pack_t*
     * @param [in] name   : const char*
     * @param [in] value   : const char*
     * @return  int
     * @retval  errno
     *		- 0 : equal
     *		- 1 : not equal
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_pack_cmp_str(const mc_pack_t *ppack, const char *name, const char *value);

    /**
     * @brief compare string value in pack
     *		比较 pack 中的字符串，不区分大小写
     *
     * @param [in] ppack   : const mc_pack_t*
     * @param [in] name   : const char*
     * @param [in] value   : const char*
     * @return  int
     * @retval  errno
     *		- 0 : equal
     *		- 1 : not equal
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_pack_casecmp_str(const mc_pack_t *ppack, const char *name, const char *value);

    /**
     * @brief create a pack from a buffer & put something like putf
     *
     * @param [in] buf   : char*
     *		buffer
     * @param [in] buf_size   : u_int
     *		size of buffer
     * @param [in] fmt   : const char*
     *		"key:value.key:value" 形式
     *		key 可以是字符串或 %s
     *		value 取值 %s %d %u %lld %llu 含义与 printf 一致
     * @return  mc_pack_t*
     * @retval  pack
     *		- !NULL : pack
     *		- NULL : failed
     * @see
     * @note
    **/
    mc_pack_t *
    mc_pack_creatf(int version, char *buf, u_int buf_size, char * tmp, u_int tmp_size, const char *fmt, ...);


    /**
     * @brief 批量 put item
     *
     * @param [in] ppack   : mc_pack_t*
     * @param [in] fmt   : const char*
     *		"key:value.key:value" 形式
     *		key 可以是字符串或 %s
     *		value 取值 %s %d %u %lld %llu 含义与 printf 一致
     *		value 取值 %p%u 时代表 raw pointer 和 size
     * @return  int
     * @retval   errno
     *		- >=0 : 成功 put 的个数
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_pack_putf(mc_pack_t *ppack, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

    /**
     * @brief same as mc_pack_putf, except for using va_list arg
     *
     * @param [in] ppack   : mc_pack_t*
     * @param [in] fmt   : const char*
     * @param [in] arg   : va_list
     * @return  int
     * @retval   errno
     *		- >=0 : 成功 put 的个数
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_pack_vputf(mc_pack_t *ppack, const char *fmt, va_list arg);

    /**
     * @brief 批量 get item
     *
     * @param [in] ppack   : const mc_pack_t*
     * @param [in] fmt   : const char*
     *		"key:value.key:value" 形式
     *		key 是字符串
     *		value 取值 %p %d %u %lld %llu 含义与 scanf 一致
     *		使用 %p 获取字符串，传入 char** 类型
     *		使用 %p%u 获取 raw，传入 char** 和 u_int*
     * @return  int
     * @retval   errno
     *		- >=0 : 成功 get 的个数
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_pack_getf(const mc_pack_t *ppack, const char *fmt, ...) __attribute__((format(scanf, 2, 3)));

    /**
     * @brief same as mc_pack_getf, except for using va_list arg
     *
     * @param [in] ppack   : const mc_pack_t*
     * @param [in] fmt   : const char*
     * @param [in] arg   : va_list
     * @return  int
     * @retval   errno
     *		- >=0 : 成功 get 的个数
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_pack_vgetf(const mc_pack_t *ppack, const char *fmt, va_list arg);

    /**
     * @brief pack to string
     *		将 pack 的内容输出为 key:value 形式的字符串
     *
     * @param [in] ppack   : const mc_pack_t*
     * @param [out] str   : char*
     *		输出的字符串缓冲区
     * @param [in] size   : u_int
     * @return  int
     * @retval	 errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_pack_pack2str(const mc_pack_t *ppack, char *str, u_int size);

    /**
     * @brief get last sub name
     *
     * @param [in] key   : const char*
     *		pointer to full name
     * @return  const char*
     *		pointer to last sub name
     * @retval
     * @see
     * @note
    **/
    const char *mc_pack_get_subkey(const char *key);

#ifdef __cplusplus
}
#endif

#endif  //__MC_PACK_EX_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
