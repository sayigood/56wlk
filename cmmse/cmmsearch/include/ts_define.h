/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: ts_define.h,v 1.2 2009/11/18 20:27:50 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file include/tinyse_define.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/06/19 09:23:31
 * @version $Revision: 1.2 $ 
 * @brief 宏定义文件
 *  
 **/


#ifndef  __TS_DEFINE_H_
#define  __TS_DEFINE_H_



#ifndef uchar
#define uchar      unsigned char
#endif

#ifndef uint8
#define uint8	   unsigned char
#endif

#ifndef uint16
#define uint16     unsigned short
#endif

#ifndef uint32
#define uint32     unsigned int
#endif

#ifndef uint64
#define uint64     unsigned long long
#endif
static const uint32 TS_CMDSTR_MAXLEN      = 64;                 /**< 命令号最大长度 */
static const uint32 TS_TERM_MAXSIZE       = 32;                 /**< term的最大长度 */
static const uint32 TS_QTERM_MAXNUM       = 38;                 /**< query term的最大个数 */
static const uint32 TS_FILEPATH_MAXSIZE   = 1024;               /**< 文件路径名的最大长度 */
static const uint32 TS_TYPENAME_MAXLEN    = 64;                 /**< 数据类型名的最大长度       */
static const uint32 TS_RI_MAXNUM          = 5;                  /**< 支持的ri库的最大数目      */
static const uint32 TS_RI_BLOCK_NUM       = 1000;               /**< 每个ri库的term块包含的term的个数       */
static const uint32 TS_INDEX_MAXNUM       = (1 << 20) * 10;     /**< 索引拉链的最大个数       */
static const uint32 TS_BATCH_MAXNUM       = 100;                /**< 批量删除/反删除的最大个数      */
static const uint32 TS_INDEX_SHORTEST_NUM = 100000;             /**< 索引截断的长度       */
static const uint32 TS_INDEX_IOBLOCK_NUM  = 128*1024*2;         /**< 索引合并时每次读取的索引个数      */

static const uint32 TS_REQ_BUF_MAXSIZE    = 1024*1024;      /**<  req body大小*/

static const char NEED_LOAD_DAY[] = "need_load_day";      /**< 是否需要载入day标记文件名      */
static const char NEED_MERGE[]    = "need_merge";         /**< 是否需要merge标记文件名       */
static const char DAY_CURDIR[]    = "day_curdir";         /**< 当前day库标记文件名       */
static const char MON_CURDIR[]    = "mon_curdir";         /**< 当前mon库标记文件名       */



#endif //__TS_DEFINE_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
