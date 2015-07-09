/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: cmmsearch.cpp,v 1.2 2008/08/22 06:07:38 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file cmmsearch.cpp
 *
 * @date 2008/06/24 15:40:35
 * @version $Revision: 1.2 $ 
 * @brief 
 *  
 **/
#include <stdio.h>
#include <stdlib.h>
#include "nshead.h"
#include "tinyse.h"
#include "mc_pack.h"
#include "mc_pack_rp.h"
#include <ub_log.h>
#include "ul_sign.h"


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
class cmmsearch_update_handle_t:public DEFAULT_UPDATE_HANDLE
{

};

/**
* @brief 检索命令处理句柄
*/
class cmmsearch_query_handle_t : public DEFAULT_QUERY_HANDLE
{
    int parse_query(const pointer_t query_cmd,
                    ts_buffer_t &req_detail,
                    basic_req_info *basic_info,
                    vector < ts_terminfo_t > &term_list);
};

int cmmsearch_query_handle_t::parse_query(const pointer_t query_cmd,
                                        ts_buffer_t &req_detail,
                                        basic_req_info *basic_info,
                                        vector < ts_terminfo_t > &term_list)
{
    int opret = 0;
    nshead_t *req_nshead = (nshead_t *) query_cmd;

	bsl::ResourcePool rp;
	req_detail.reset();
	mc_pack_t *req_mc = mc_pack_open_w_rp(2, req_detail._buf, req_detail._size, &rp);
	if(MC_PACK_PTR_ERR(req_mc)){
        UB_LOG_WARNING("[function:parse_query>>mc_pack_open_w_rp][ret:][desc:mc_pack_open_w_rp error][intput:]");
        return -2;
    }

    opret = mc_pack_json2pack((char *)(req_nshead+1), req_mc);
    if(opret<0){
        UB_LOG_WARNING("[function:parse_query>>mc_pack_json2pack][ret:][desc:mc_pack_json2pack error][intput:]");
        return -2;
    }
    //cmd
    snprintf(basic_info->cmd_str, TS_CMDSTR_MAXLEN, "%s",  mc_pack_get_str(req_mc, "cmd"));
    if(MC_PACK_PTR_ERR(basic_info->cmd_str))
    {
        return -2;
    }


    //page_no
    opret= mc_pack_get_uint32(req_mc, "page_no", &basic_info->page_no);
    if(opret < 0)
    {
        return -2;
    }
    //num_per_page
    opret= mc_pack_get_uint32(req_mc, "num_per_page", &basic_info->num_per_page );
    if(opret < 0)
    {
        return -2;
    }

    //term_list
    const char *keyword = mc_pack_get_str(req_mc, "query");
    if(MC_PACK_PTR_ERR(keyword))
    {
        return -2;
    }

    const char delimiters[] = " .,;:!-";
    char *running;
    char *token;
    running = strdup(keyword);
    ts_terminfo_t terminfo;
    token = strsep(&running, delimiters);
    while(NULL != token)
    {
        creat_sign_fs64(token, strlen(token), &terminfo.term.sign1,
                        &terminfo.term.sign2);
        term_list.push_back(terminfo);
        UB_LOG_DEBUG("qterm[%s] [%u:%u]",token, terminfo.term.sign1,
                     terminfo.term.sign2);
        token = strsep(&running, delimiters);
    }
	if (running)
		free(running);

    mc_pack_close(req_mc);
    return 0;
}



/**
* @brief 扩展句柄
*/
ts_handle_t ts_so_cmmsearch_handle = 
{
	global_init_func, 		          /**< 全局初始化函数     */
	NULL,		                      /**< 检索线程初始化函数       */
	NULL,		                      /**< 更新线程初始化函数       */
	NULL,		                      /**< 定时线程处理函数       */
	new cmmsearch_query_handle_t(),		  /**< 检索处理句柄       */
	new cmmsearch_update_handle_t()		  /**< 更新处理句柄       */

};











/* vim: set ts=4 sw=4 sts=4 tw=100 */
