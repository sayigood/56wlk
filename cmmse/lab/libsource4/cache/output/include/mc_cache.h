/**
 * @file
 * @brief 提供cache支持机制（多用于前端检索），cache单元定长，并按照LRU原则来更新数据
 */

#ifndef __MC_CACHE_H_
#define __MC_CACHE_H_

#include "mc_type.h"
#include "mc_const.h"
#include <ul_def.h>

 
/**
 * 初始化cache结构
 *
 *  @param[in] capacity  cache 的容积
 *  @param[in] unitsize  每个单元的大小
 *  @param[out] 无
 *  @return cache指针
 *  - 非NULL 表示成功
 *  - NULL  表示失败
 */ 
mc_cache* mc_creat_cache(u_int capacity,u_int unitsize);

/**
 * 销毁cache结构
 *
 *  @param[in] pcache  指向要销毁容器的指针
 *  @param[out] 无
 *  @return 操作返回值
 * - RT_NOTICE_PROCESSED  成功
 * - RT_NOTICE_NONE_PROCESSED 传入的指针为空
 */ 
int mc_destroy_cache(mc_cache* pcache);

/**
 * 清空cache
 *
 *  @param[in] pcache  cache指针
 *  @param[out] 无
 *  @return RT_NOTICE_PROCESSED
 */ 
int mc_clean_cache(mc_cache* pcache);

/**
 * 将一个条目存入cache,如果cache满,将最老的置换出来
 *
 *  @param[in] pcache  cache指针
 *  @param[in] sign    签名id(两个int)
 *  @param[in] pdata   待存入的数据
 *  @param[in] size    数据的大小
 *  @param[out] 无
 *  @return 操作返回值
 * - RT_NOTICE_PROCESSED      成功
 * - RT_NOTICE_NONE_PROCESSED 待存入的数据已经在cache中存在
 * - others                   失败信息,具体定义可参见mc_const.h
 */ 
int mc_additem(mc_cache* pcache,u_int* sign,void* pdata,u_int size);

/**
 * 在cache中查找条目,取得签名对应的cache数据
 *
 *  @param[in]  pcache  cache指针
 *  @param[in]  sign    签名id(两个int)
 *  @param[in]  pdata   存储数据的buf
 *  @param[in]  size    buf的大小
 *  @param[out] pdata   取出的cache数据
 *  @return 操作返回值
 * - RT_NOTICE_PROCESSED      成功
 * - RT_NOTICE_NONE_PROCESSED 找不到数据
 * - RT_ERROR_GENERAL_ERROR   其他错误
 */ 
int mc_seekitem(mc_cache* pcache,u_int* sign,void* pdata,u_int size);

/**
 * 从cache中删除某个条目
 *
 *  @param[in]  pcache  cache指针
 *  @param[in]  sign    签名id(两个int)
 *  @param[out] 无
 *  @return 操作返回值
 * - RT_NOTICE_PROCESSED      成功
 * - RT_NOTICE_NONE_PROCESSED 找不到数据
 * - others                   其他错误,具体定义可参见mc_const.h
 */ 
int mc_removeitem(mc_cache* pcache,u_int* sign);

/**
 * 修改cache中某个条目的内容
 *
 *  @param[in]  pcache  cache指针
 *  @param[in]  sign    签名id(两个int)
 *  @param[in]  opfunc  修改内容所使用的函数
 *  @param[in]  opinfo  修改信息,用做opfunc的参数
 *  @param[out] 无
 *  @return 操作返回值
 * - RT_NOTICE_PROCESSED      成功
 * - RT_NOTICE_NONE_PROCESSED 找不到数据
 * - others                   其他错误,具体定义可参见mc_const.h
 *  @see ::mc_creat_cache  ::mc_clean_cache  ::mc_additem  ::mc_seekitem  ::mc_removeitem  ::mc_modifyitem ::mc_getwashout
 */ 
int mc_modifyitem(mc_cache* pcache,u_int* sign,modfunc opfunc,void* opinfo);

/**
 * 得到即将被置换出来的数据块
 *
 *  @param[in]  pcache  cache指针
 *  @param[in]  pdata   存储数据的buf
 *  @param[in]  size    buf的大小
 *  @param[out] pdata   取出的cache数据
 *  @return 操作返回值
 * - RT_NOTICE_PROCESSED      成功
 * - RT_NOTICE_NONE_PROCESSED 空间不满,无需要置换的数据
 *  @see ::mc_creat_cache  ::mc_clean_cache  ::mc_additem  ::mc_seekitem  ::mc_removeitem  ::mc_modifyitem ::mc_getwashout
 */ 
int mc_getwashout(mc_cache* pcache,void* pdata,u_int size);

/**
 * 保存cache结构到文件
 *
 *  @param[in] path  路径
 *  @param[in] fname  保存的文件名
 *  @param[in] pcache  要保存的cache
 *  @return 操作返回值
 * - RT_NOTICE_PROCESSED      成功
 * - others                   错误,具体定义可参见mc_const.h
 */
int mc_save_cache(const char * path, const char *fname, const mc_cache* pcache);
/**
 * 从文件中读取cache结构
 *
 *  @param[in] path  路径
 *  @param[in] fname  保存的文件名
 *  @param[in/out] capacity  cache 的容积，传入值表示用户需求，传出值表示实际值。传入零表示使用上次的配置
 *  @param[in/out] unitsize  每个单元的大小，传入值表示用户需求，传出值表示实际值。传入零表示使用上次的配置
 *  @return cache指针
 *  - 非NULL 表示成功
 *  - NULL  表示失败
 */
mc_cache* mc_load_cache(const char * path, const char *fname, u_int *capacity,u_int *unitsize);

/**
 * 得到即将被置换出来的数据块
 *
 *  @param[in]  pcache  cache指针
 *  @param[in]  pdata   存储数据的buf
 *  @param[in]  size    buf的大小
 *  @param[out] pdata   取出的cache数据
 *  @return 操作返回值
 * - RT_NOTICE_PROCESSED      成功
 * - RT_NOTICE_NONE_PROCESSED cache为空，或pdata是空指针
 * - others                   错误,具体定义可参见mc_const.h
 */ 
int mc_get_last_item(mc_cache* pcache,void* pdata,u_int size, u_int sign[2]);
#endif

