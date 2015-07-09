/**
 * @file op_query.cpp
 * @author 
 * @date 2013.01.06 08:37:17
 * @version $Revision: 1.9 $ 
 * @brief 检索线程处理文件
 *  
 **/
#include <algorithm>
#include "pub.h"


/**
 * @brief 查询命令处理
 *
 * @param [in] cmd_no   : int 命令号
 * @param [in] req_head   : nshead_t*
 * @param [in] req_buf   : ub_buff_t*
 * @param [in/out] res_head   : nshead_t*
 * @param [in/out] res_buf   : ub_buff_t*
**/
STATIC int
process_query(nshead_t * req_head, ub_buff_t * req_buf,
       nshead_t * res_head, ub_buff_t * res_buf)
{
	
    int opret = 0;
	struct timeval s,e;
    basic_req_info *basic_info;

    ts_query_handle_t *hd = g_runtime.handle->query_handle;

    // parse query
	gettimeofday(&s,NULL);
	ts_buffer_t req_detail(req_buf->buf, req_buf->size);
    vector < ts_terminfo_t > term_list; ///todo: 可能影响性能
    opret = hd->parse_query(req_head, req_detail, basic_info, term_list);
    if(opret < 0)
    {
        UB_LOG_WARNING("hd->parse_query failed.ret[%d]", opret);
        return opret;
    }

    if(term_list.size() > TS_QTERM_MAXNUM)
    {
        UB_LOG_WARNING("term_list[%ld>%d] too large", term_list.size(),
                       TS_QTERM_MAXNUM);
        return -1;
	}
	gettimeofday(&e,NULL);
	ub_log_pushnotice("parse_query(us)","%lu",TIME_US_DIFF(s,e));
    
    // get ri libs
    vector < ts_ri_t * > ri_libs;

    ri_libs.push_back(&g_runtime.mem_ri);

    if(0 == g_runtime.mon_curdir || 1 == g_runtime.mon_curdir)
    {
        ri_libs.push_back(&g_runtime.mon_ri[g_runtime.mon_curdir]);
    }

    if(g_runtime.need_merge)    //can be not thread-safe
    {
        ri_libs.push_back(&g_runtime.day_ri[2]);
    }

    vector < ts_index_t > *merged_list =
        ((vector < ts_index_t > *)get_data("merged_list"));
    vector < ts_index_t > *filted_list =
        ((vector < ts_index_t > *)get_data("filted_list"));

    merged_list->clear();
	filted_list->clear();
	gettimeofday(&s,NULL);
    for(vector < ts_ri_t * >::iterator iter = ri_libs.begin();
        iter != ri_libs.end(); ++iter)
    {

		ts_ind_reader_t ind_reader(*iter);
		// merge
        opret = hd->merge_ind_list(req_head,term_list,ind_reader,merged_list);
        if(opret < 0)
        {   
            UB_LOG_WARNING("merge_ind_list failed.ret[%d]",opret);
            return opret;
        }

    }
    gettimeofday(&e,NULL);
    ub_log_pushnotice("merge_ind_list(us)","%lu",TIME_US_DIFF(s,e));
	// washout del/undel nodes
	gettimeofday(&s,NULL);
	int k = 0;
	for(vector < ts_index_t >::iterator i = merged_list->begin();
        i != merged_list->end(); ++i)
	{   

		if (1 == g_runtime.mod_table.get(i->id)) continue;

		bool bit = g_runtime.del_table.get(i->id);
		if( !strcmp(basic_info->cmd_str, "qsearchall")
		    ||(!strcmp(basic_info->cmd_str, "qsearchdel") && bit)
		    ||(!strcmp(basic_info->cmd_str, "qsearch") && !bit))
		{   
			merged_list->at(k++) = *i; 
		}   
	}   
	merged_list->resize(k);
    UB_LOG_DEBUG("merged_list.size[%ld]", merged_list->size());
	gettimeofday(&e,NULL);
    ub_log_pushnotice("washout_del(us)","%lu",TIME_US_DIFF(s,e));
    
	// filt
	gettimeofday(&s,NULL);
    filted_list->reserve(merged_list->size());
    filted_list->clear();
    ts_buffer_t brief(g_cfg.brief_size);
    opret = hd->index_filt(req_head, brief, merged_list, filted_list);
    if(opret < 0)
    {
        UB_LOG_WARNING("hd->index_filt failed.ret[%d]", opret);
        return opret;
	}
	gettimeofday(&e,NULL);
	ub_log_pushnotice("index_filt(us)","%lu",TIME_US_DIFF(s,e));
    
    // adjust
	gettimeofday(&s,NULL);     
	opret =
		hd->adjust_weight(req_head, term_list,filted_list);
	if(opret < 0)
	{
		UB_LOG_WARNING("hd->adjust_weight failed.ret[%d]", opret);
		return opret;
	}
	gettimeofday(&e,NULL);
	ub_log_pushnotice("adjust_weight(us)","%lu",TIME_US_DIFF(s,e));

    // res
	gettimeofday(&s,NULL);
    ts_buffer_t fulltext(g_cfg.fulltext_maxsize);
    ts_buffer_t res(res_buf->buf, res_buf->size);

    // --page    
    opret = hd->fill_basic_res(req_head, basic_info, filted_list, res);
    if (opret<0)
	{
		UB_LOG_WARNING("hd->init_abs failed.ret[%d]", opret);
		return opret;
	}

    /*if(0 == ret_num)
    {
        res_head->body_len = sizeof(ts_head_t);
        return 0;
    }*/    
    
    // --abs
	opret = hd->init_abs(req_head,res);
	if (opret<0)
	{
		UB_LOG_WARNING("hd->init_abs failed.ret[%d]", opret);
		return opret;
	}
    unsigned int i = 0;
    int start_num = basic_info->num_per_page*basic_info->page_no;
    for(vector < ts_index_t >::iterator iter =
        filted_list->begin() + start_num;
        iter != filted_list->end() && i < basic_info->ret_num; ++iter, ++i)
    {
        fulltext._used = fulltext._size;
        opret = g_runtime.di.read(iter->id, fulltext._buf, fulltext._used);
        if(opret < 0)
        {
            UB_LOG_WARNING("di.read failed.ret[%d]", opret);
            return opret;
        }

        g_runtime.mg.read(iter->id, brief._buf);
        brief._used = brief._size;

        opret = hd->add_abs(req_head, i, fulltext, brief, res);
        if(opret < 0)
        {
            UB_LOG_WARNING("hd->add_abs failed.i[%d]ret[%d]", i, opret);
            return opret;
        }

        fulltext.reset();
        brief.reset();
    }

	opret = hd->fini_abs(req_head,res);
	if (opret<0)
	{
		UB_LOG_WARNING("hd->fini_abs failed.ret[%d]", opret);
		return opret;
	}
	gettimeofday(&e,NULL);
	ub_log_pushnotice("add_abs(us)","%lu",TIME_US_DIFF(s,e));

    res_head->body_len = res._used;
    return 0;
}


//TODO:处理命令号的时候。。。
STATIC void 
set_res_error(uint32 err_no)
{

    //nshead_t *res_head = (nshead_t *) ub_server_get_write_buf();
    //if(NULL == res_head)
    //{
        //UB_LOG_FATAL("res_head is NULL");
        //return;
    //}

    //ts_head_t *pres = (ts_head_t *) (res_head + 1);
    //pres->err_no = err_no;

}

/**
 * @brief 线程初始化函数
 *
 * @return  int 
 * @retval   
**/
int 
query_thread_init()
{
    int opret = 0;
    opret = g_runtime.handle->query_thread_init();
    if(opret < 0)
    {
        return -1;
    }
	void *tmp = new vector < ts_index_t > (TS_INDEX_SHORTEST_NUM);
	if (NULL == tmp)
	{
		return -2;
	}
    opret = set_data("lst1",tmp);
    if(opret < 0)
    {
        return -3;
    }

	tmp = new vector < ts_index_t > (TS_INDEX_SHORTEST_NUM);
	if (NULL == tmp)
	{
		return -6;
	}
    opret = set_data("lst3",tmp );
    if(opret < 0)
    {
        return -7;
    }
	tmp = new vector < ts_index_t > (TS_INDEX_SHORTEST_NUM);
	if (NULL == tmp)
	{
		return -8;
	}

    opret = set_data("merged_list",tmp);
    if(opret < 0)
    {
        return -9;
    }
	tmp = new vector < ts_index_t > (TS_INDEX_SHORTEST_NUM);
	if (NULL == tmp)
	{
		return -10;
	}
    opret = set_data("filted_list",tmp);
    if(opret < 0)
    {
        return -11;
    }
    return 0;
}

/**
 * @brief 主处理函数
 *
 * @return  int 
 * @retval   
**/
int 
op_query()
{
    int opret = 0;

    nshead_t *req_head;
    nshead_t *res_head;
    ub_buff_t req_buf;
    ub_buff_t res_buf;
    in_addr_t req_ip;

    req_head = (nshead_t *) ub_server_get_read_buf();
    res_head = (nshead_t *) ub_server_get_write_buf();
    if(NULL == req_head || NULL == res_head)
    {
        UB_LOG_FATAL("get req_head[%ld] || res_head[%ld] failed.",
                     (long)req_head, (long)res_head);
        return -1;
    }
    req_buf.buf = (char *)(req_head + 1);
    req_buf.size = ub_server_get_read_size() - sizeof(nshead_t);
    res_buf.buf = (char *)(res_head + 1);
    res_buf.size = ub_server_get_write_size() - sizeof(nshead_t);

    //设置一些log需要的字段
    char ip_str[20];
    ip_str[0] = 0;
    req_ip = ub_server_get_ip();
    inet_ntop(AF_INET, &req_ip, ip_str, sizeof(ip_str));
    ub_log_setbasic(UB_LOG_REQIP, "%s", ip_str);
    ub_log_setbasic(UB_LOG_LOGID, "%u", req_head->log_id);
    ub_log_setbasic(UB_LOG_REQSVR, "%s", req_head->provider);
    ub_log_setbasic(UB_LOG_SVRNAME, "%s", g_cfg.svr_query.svr_name);

    ub_log_pushnotice("req_dlen", "%d", req_head->body_len);
    *res_head = *req_head;
    strncpy(res_head->provider, req_head->provider,
            sizeof(res_head->provider));
    res_head->body_len = 0;
    res_head->reserved = 0;

	//处理查询
	struct timeval total_s,total_e;
	gettimeofday(&total_s, NULL);
	opret = process_query(req_head, &req_buf, res_head, &res_buf);
    if(opret != 0){
            UB_LOG_WARNING("[function:op_update]>>>process_query failed! errno:%d", opret);
            return -1;
    }    
	gettimeofday(&total_e, NULL);
	ub_log_setbasic(UB_LOG_PROCTIME, "%luus", TIME_US_DIFF(total_s, total_e));
    
    return opret;
}




/* vim: set ts=4 sw=4 sts=4 tw=100 */
