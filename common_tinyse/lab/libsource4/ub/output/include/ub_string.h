/////////////////////////////////////////////////////////////////////////////

#ifndef __UB_STRING_H__
#define __UB_STRING_H__
#include <string.h>
//#ifndef size_t
//#define size_t unsigned int
//#endif

//string related function
#ifndef HAVE_STRLCPY
/**
 * @brief 安全的字符串拷贝函数，会在目标位置最后置0
 *
 * @param [out] t   : 字串拷贝的目标位置
 * @param [in] s   : 需要拷贝的字符串
 * @param [in] n   : 目标串位置的最大空间长度，如果比需要拷贝的字符串长度小会截断，最后位置置０
 * @return  需要拷贝的字符串长度，不是结果的长度 
**/
extern size_t strlcpy(register char* t, register const char* s, register size_t n);
#endif

/**
 * @brief 连接字符串
 *
 * @param [out] dest   : 目标字符串
 * @param [in] src   : 需要连接的字符串
 * @param [in] n   : 目标串位置的最大空间长度,n过小会截断 
 * @return  0没有截断　-1有截断
**/
extern int ub_strcat(register char *dest, register const char *src, register size_t n);

/**
 * @brief 获取字符串str的长度，最长不超过maxlen-1
 *
 * @param [in] str   : 需要取长度str
 * @param [in] maxlen   : 最大的长度
 * @return  字符串str的长度，超过maxlen-1，返回maxlen-1
**/
extern int ub_strnlen(register char *str, register size_t maxlen);

/**
 * @brief 转化字符串成16进制的形式 
 *
 * @param [out] dst   : 目标字符串
 * @param [in] dlen   : 目标字符串的最大长度
 * @param [in] src   : 需要转化的字符串
 * @param [in] slen   : 需要转化的字符串的长度
 * @return 0表示成功，非0失败  
**/
extern int ub_strtohex(char *dst, const size_t dlen, char *src, const size_t slen);
#endif

/* vim: set ts=4 sw=4 tw=0 noet: */
