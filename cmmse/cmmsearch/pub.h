/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: pub.h,v 1.7 2009/11/18 20:27:50 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file pub.h
 *
 * @date 2008/04/08 11:26:08
 * @version $Revision: 1.7 $ 
 * @brief 统一的头文件
 *  
 **/


#ifndef  __PUB_H_
#define  __PUB_H_
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <dlfcn.h>

#include <nshead.h>
#include <ub_server.h>
#include <ub_auth.h>
#include <ub_misc.h>
#include <ub_proccmd.h>
#include "ul_thr.h"
#include "ul_sign.h"
#include "lock.h"
#include "hashmap.h"
#include "tinyse.h"
#include "debug.h"
#include "utils.h"
#define DEFAULT_CONF_DIR "conf/"
#define DEFAULT_CONF_NAME PROJNAME".conf"



/**
* @brief 全局配置
*/
typedef struct cfg_data_t
{
    char conf_dir[TS_FILEPATH_MAXSIZE];           /**<  配置文件路径   */
    char conf_name[TS_FILEPATH_MAXSIZE];          /**<  配置文件名      */
    int log_level;                                /**<  日志级别      */
    char log_dir[TS_FILEPATH_MAXSIZE];            /**<  日志路径     */
    char log_name[TS_FILEPATH_MAXSIZE];           /**<  日志名      */
    char iplist_path[TS_FILEPATH_MAXSIZE];        /**<  提交线程的ip白名单      */
    char limits_path[TS_FILEPATH_MAXSIZE];        /**<  limits路径     */


    ub_svr_t svr_update;                          /**<  提交线程的配置句柄      */
    ub_svr_t svr_query;                           /**<  查询线程的配置句柄      */

    int query_readbuf_size;                       /**<  查询线程数据接收缓冲区大小      */
    int query_writebuf_size;                      /**<  查询线程数据发送缓冲区大小      */
    int update_readbuf_size;                      /**<  更新线程数据接收缓冲区大小      */
    int update_writebuf_size;                     /**<  更新线程数据发送缓冲区大小     */

    int ontime_sleep;                             /**<  ontime线程监控的时间间隔      */


    char dump01_flag[TS_FILEPATH_MAXSIZE];        /**<  如果存在这个文件，就dump01      */
    int dump01_tmeval;                            /**<  0,1目录dump的时间间隔      */

    int dump2_tm;                                 /**<  dump 到2目录的时间点      */
    int dump2_min_tmeval;                         /**<  2次dump到2目录的最短时间间隔      */
    uint32 memory_maxsize;                        /**<  开辟的内存数量,以M为单位     */

    uint32 brief_size;                            /**<  brief表项的大小      */
    uint32 fulltext_maxsize;                      /**<  存储的全文数据的最大大小      */
    uint32 data_maxnum;                           /**<  支持的数据最大个数      */

	int static_index;                             /**<  是否是静态索引:0,否;1,是      */

    char data_path[TS_FILEPATH_MAXSIZE];          /**<  数据文件的路径     */
    char data_type[TS_TYPENAME_MAXLEN];           /**<  数据名称      */
    char so_path[TS_TYPENAME_MAXLEN];             /**<  扩展的路径      */


} cfg_data_t;



/** 以下是一些全局变量和函数的声明 */

extern pthread_mutex_t g_ts_lock;         /**<  全局锁  */

extern cfg_data_t g_cfg;                  /**<  全局配置      */

extern ts_runtime_t g_runtime;            /**<  全局数据      */


extern pointer_t *g_data_map;             /**< 线程buffer的映射表       */


int query_thread_init();                  /**< query线程初始化函数       */
int op_query();                           /**< query线程主处理函数      */


extern const ub_cmd_map_t TS_UPDATE_CMD_MAP[];
                                          /**< 更新命令映射      */

int update_thread_init();                 /**< 更新线程初始化函数       */
int op_update();                          /**< 更新线程主处理函数       */


int cfg_init(int conf_build);             /**< 配置初始化      */
int cfg_check();                          /**< 配置检查      */
int data_init();                          /**< 全局数据初始化      */
uint32 get_memory_size();                 /**< 获取模块内存使用量     */


void *ontime_thread(void *);              /**< 定时线程处理函数      */

int resend_cmd();		                  /**< 回放命令，在更新线程启动以后才能调用，因为涉及了用户自定义的线程数据       */

int ts_rebuild(int cmd_no, uint32 inner_id, nshead_t * req_head,
       ub_buff_t * req_buf, nshead_t * res_head, ub_buff_t * res_buf);		  /**<  重放命令句柄      */




#endif //__PUB_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
