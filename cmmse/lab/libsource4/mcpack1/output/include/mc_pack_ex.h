/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: mc_pack_ex.h,v 1.3 2008/04/09 05:54:54 zhouzr Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file mc_pack_ex.h
 * @author zhouzr(zhouzerui@baidu.com)
 * @date 2008/01/29 16:20:46
 * @version $Revision: 1.3 $ 
 * @brief mc_pack extended api
 *  
 **/


#ifndef  __MC_PACK_EX_H_
#define  __MC_PACK_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#include "mc_pack_core.h"

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
 * @author zhouzr
 * @date 2008/01/14 10:22:13
**/
const char *mc_pack_perror(int err);

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
 * @author zhouzr
 * @date 2008/04/08 22:31:10
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
 * @author zhouzr
 * @date 2008/01/25 12:19:42
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
 * @author zhouzr
 * @date 2008/01/25 12:22:02
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
 * @author zhouzr
 * @date 2008/03/07 18:25:01
**/
mc_pack_t * mc_pack_creatf(char *buf, u_int buf_size, const char *fmt, ...)
	__attribute__((format(printf, 3, 4)));

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
 * @author zhouzr
 * @date 2008/01/31 21:56:47
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
 * @author zhouzr
 * @date 2008/03/17 20:04:36
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
 * @author zhouzr
 * @date 2008/01/31 22:00:30
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
 * @author zhouzr
 * @date 2008/03/17 20:05:42
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
 * @author zhouzr
 * @date 2008/01/31 22:02:11
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
 * @author zhouzr
 * @date 2008/02/29 16:51:09
**/
const char *mc_pack_get_subkey(const char *key);

#ifdef __cplusplus
}
#endif

#endif  //__MC_PACK_EX_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
