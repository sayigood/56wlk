/**
 * @file
 * @brief 字符串映射词典
 */
/**
 * @brief 字符串映射词典是将字符串加入词典时会返回一个与之对应的整数，并实现字符串和对应的整数之间的相互查找。
 */

#ifndef STRDICT_H
#define STRDICT_H

#include <ul_dict.h>


typedef struct UL_STRDICT_T
{
	unsigned int m_bufsize;
	unsigned int m_curpos;
	unsigned int m_count;
	char* m_strbuf;
	Sdict_build * m_dict;
}ul_strdict_t;

/**
 *  得到字典当前存储的节点数量
 *  
 *  @param[in]  pstrdict  字典的指针
 *  @param[out] 无
 *  @return 节点数量
 */

int ul_strd_size(const ul_strdict_t * pstrdict);

/**
 *  创建字符串映射词典
 *  
 *  @param[in]  hashnum  词典的哈希桶的大小
 *  @param[in]  bufsize  词典缓冲区的初始大小及每次增加的字节数
 *  @param[out] 无
 *  @return 字典指针
 * - 非NULL   成功
 * - NULL     失败
 */ 
ul_strdict_t * ul_strd_create(int hashnum,int bufsize);


/**
 *  销毁字符串映射词典
 *  
 *  @param[in]  pstrdict  字典的指针
 *  @param[out] 无
 *  @return 无
 */ 
void ul_strd_destroy(ul_strdict_t *pstrdict);

/**
 *  向字符串映射词典中插入一个字符串
 *  
 *  @param[in]  pstrdict  指向词典的指针
 *  @param[in]  str       待插入的字符串
 *  @param[in]  pcode     用于存储与字符串对应整数的空间
 *  @param[out] pcode     存放有与字符串对应的整数
 *  @return 操作返回值
 * - 1   成功
 * - 0   字符串已存在
 * - -1  失败
 */    
int ul_strd_insert(ul_strdict_t* pstrdict,const char * const str,int* pcode);

 
/**
 *  清空字符串映射词典
 *  
 *  @param[in]  pstrdict  指向词典的指针
 *  @param[out] 无
 *  @return 无
 */ 
void ul_strd_reset(ul_strdict_t *pstrdict);

 
/**
 *  通过字符串查找与其对应的code
 *  
 *  @param[in]  pstrdict  指向词典的指针
 *  @param[in]  str       待查找整数的字符串
 *  @param[out] 无
 *  @return code
 * - >=0   查找到的与字符串对应的code
 * - -1  失败
 */ 
int ul_strd_getcode(const ul_strdict_t*pstrdict,const char* str);


 
/**
 *  通过code查找与其对应的字符串
 *  
 *  @param[in]  pstrdict  指向词典的指针
 *  @param[in]  code      待查找字符串的code
 *  @param[out] 无
 *  @return 指向查找到的字符串的指针
 * - 非NULL   成功
 * - NULL     失败
 *  @note the string will be change when new string is insert the dict.BE SURE NOT to keep this string after a new string had been added to the strdict.
 */
const char* ul_strd_getstr(const ul_strdict_t* pstrdict,const int code);

#endif
