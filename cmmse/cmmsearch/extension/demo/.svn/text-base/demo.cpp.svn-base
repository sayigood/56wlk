/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: demo.cpp,v 1.2 2008/08/22 06:07:38 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file demo.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/06/24 15:40:35
 * @version $Revision: 1.2 $ 
 * @brief 
 *  
 **/
#include <stdio.h>
#include <stdlib.h>
#include "tinyse.h"


/**
 * @brief 全局初始化函数
 *
 * @return  int 
 * @retval   
**/
int global_init_func() 
{

	return 0;
};

/**
* @brief 更新命令处理句柄
*/
class demo_update_handle_t:public DEFAULT_UPDATE_HANDLE
{

};

/**
* @brief 检索命令处理句柄
*/
class demo_query_handle_t : public DEFAULT_QUERY_HANDLE
{

};

/**
* @brief 扩展句柄
*/
ts_handle_t ts_so_demo_handle = 
{
	global_init_func, 		          /**< 全局初始化函数     */
	NULL,		                      /**< 检索线程初始化函数       */
	NULL,		                      /**< 更新线程初始化函数       */
	NULL,		                      /**< 定时线程处理函数       */
	new demo_query_handle_t(),		  /**< 检索处理句柄       */
	new demo_update_handle_t()		  /**< 更新处理句柄       */

};











/* vim: set ts=4 sw=4 sts=4 tw=100 */
