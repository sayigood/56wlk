/**
 * @file
 * @brief 提供线程级的内存管理功能的memory函数族，在debug模式下可有效检测内存泄露，在release模式下为系统调用的封装
 */
#ifndef __UL_MEM__
#define __UL_MEM__

#include <stdio.h>

#ifdef __MEM_DEBUG__
#define ul_malloc(size) ul_debug_malloc(size, __FILE__, __LINE__)
#define ul_calloc(num, size) ul_debug_calloc(num, size, __FILE__, __LINE__)
#define ul_realloc(ptr, size) ul_debug_realloc(ptr, size, __FILE__, __LINE__)
#define ul_free(mem_ptr) ul_debug_free(mem_ptr)
#define ul_dump_mem_info(buff, buff_len) ul_debug_dump_mem_info(buff, buff_len)
#else
#define ul_malloc(size) ul_release_malloc(size)
#define ul_calloc(num, size) ul_release_calloc(num, size)
#define ul_realloc(ptr, size) ul_release_realloc(ptr, size)
#define ul_free(mem_ptr) ul_release_free(mem_ptr)
#define ul_dump_mem_info(buff, buff_len)	0
#endif


/**
 *  release模式下的malloc，与malloc用法相同
 *  
 *  @param[in]  size  开辟空间大小
 *  @param[out] 无
 *  @return 开辟的指针
 * - 非NULL   成功
 * - NULL     失败
 */
void *ul_release_malloc(size_t size);

/**
 *  release模式下的calloc，与calloc用法相同
 *  
 *  @param[in]  num   开辟连续空间的块数
 *  @param[in]  size  每块大小
 *  @param[out] 无
 *  @return 开辟的指针
 * - 非NULL   成功
 * - NULL     失败
 */
void *ul_release_calloc(size_t num, size_t size);


/**
 *  release模式下的realloc，与realloc用法相同
 *  
 *  @param[in]  ptr   原来的数据
 *  @param[in]  size  重新分配的大小
 *  @param[out] 无
 *  @return 开辟的指针
 * - 非NULL   成功
 * - NULL     失败
 */
void *ul_release_realloc(void *ptr, size_t size);

/**
 *  release模式下的free，与free用法相同
 *  
 *  @param[in]  ptr   原来的数据指针
 *  @param[out] 无
 *  @return 无
 */
void ul_release_free(void *ptr);


/**
 *  debug模式下的malloc，分配时将内存串成链表，用于内存监控管理
 *  
 *  @param[in]  size        开辟空间大小
 *  @param[in]  alloc_file  当前文件名
 *  @param[in]  alloc_line  当前行
 *  @param[out] 无
 *  @return 开辟的指针
 * - 非NULL   成功
 * - NULL     失败
 */
void *ul_debug_malloc(size_t size, char *alloc_file, int alloc_line);

/**
 *  debug模式下的calloc，分配时将内存串成链表，用于内存监控管理
 *  
 *  @param[in]  num   开辟连续空间的块数
 *  @param[in]  size  每块大小
 *  @param[in]  alloc_file  当前文件名
 *  @param[in]  alloc_line  当前行
 *  @param[out] 无
 *  @return 开辟的指针
 * - 非NULL   成功
 * - NULL     失败
 */
void *ul_debug_calloc(size_t num, size_t size, char *alloc_file, int alloc_line);


/**
 *  debug模式下的realloc，分配时将内存串成链表，用于内存监控管理
 *  
 *  @param[in]  ptr   原来的数据
 *  @param[in]  size  重新分配的大小
 *  @param[in]  alloc_file  当前文件名
 *  @param[in]  alloc_line  当前行
 *  @param[out] 无
 *  @return 开辟的指针
 * - 非NULL   成功
 * - NULL     失败
 */
void *ul_debug_realloc(void *ptr, size_t size, char *alloc_file, int alloc_line);


/**
 *  debug模式下的free，释放资源同时修改管理链表，用于内存监控管理
 *  
 *  @param[in]  ptr   原来的数据
 *  @param[out] 无
 *  @return 无
 */
void ul_debug_free(void *ptr);

/**
 *  debug模式输出应用程序的内存使用信息
 *  
 *  @param[in]  buff      buf指针
 *  @param[in]  buff_len  buf长度
 *  @param[out] buff      输出内存使用信息
 *  @return 无
 */
int  ul_debug_dump_mem_info(char *buff, int buff_len);

#endif
