/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: utils.h,v 1.1 2008/08/22 04:02:42 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file utils.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/04/26 22:12:29
 * @version $Revision: 1.1 $ 
 * @brief 
 *  
 **/


#ifndef  __UTILS_H_
#define  __UTILS_H_








#define TS_MAX(x,y) do{  (x) = ( (x)>=(y)?(x):(y)   )+1; }while(0)
#define TIME_SEC_DIFF(pre, cur) ((cur.tv_sec)-(pre.tv_sec))
#define TIME_MS_DIFF(pre, cur) (((cur.tv_sec)-(pre.tv_sec))*1000 + \
        (cur.tv_usec)/1000 - (pre.tv_usec)/1000)
#define TIME_US_DIFF(pre, cur) (((cur.tv_sec)-(pre.tv_sec))*1000000 + \
        (cur.tv_usec) - (pre.tv_usec))


#if !defined safe_exit
	#define safe_exit(ret) do{ raise(SIGKILL); }while(1)
#endif



/** 文件是否存在 */
bool _is_fileexist( const char* filename );


#endif  //__UTILS_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
