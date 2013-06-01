/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: ts_runtime.h,v 1.1 2008/10/20 01:49:15 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file include/ts_runtime.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/08/30 16:52:31
 * @version $Revision: 1.1 $ 
 * @brief 
 *  
 **/


#ifndef  __TS_RUNTIME_H_
#define  __TS_RUNTIME_H_







/**
* @brief 全局数据
*/
typedef struct ts_runtime_t
{

    ts_handle_t *handle;          /**< 扩展句柄       */


    ts_cm_t cm;                   /**<  id分配      */
    ts_mg_t mg;                   /**<  mg      */
    ts_di_t di;                   /**<  di      */
    ts_bk_t bk;                   /**< 命令备份        */


    ts_mem_ri_t mem_ri;           /**<  内存索引      */
    ts_disk_ri_t day_ri[3];       /**<  day库索引      */
    ts_disk_ri_t mon_ri[2];       /**<  mon库索引      */

    ts_bitset_t del_table;        /**<  已删除数据表      */
    ts_bitset_t mod_table;        /**<  已修改数据表      */


    int day_curdir;               /**<  当前day库 0 或 1      */
    int mon_curdir;               /**<  当前mon库 0 或 1      */
    int need_merge:1;             /**<  是否需要merge 到 mon库       */
    int need_load_day:1;          /**<  启动时是否需要load day到mon库       */

	uint32 last_cmd_sign;
} ts_runtime_t;













#endif  //__INCLUDE_TS_RUNTIME_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
