/**
 * @file op_update.cpp
 * @author
 * @date 2013.01.06 09:20:49
 * @version $Revision: 1.8 $ 
 * @brief 处理更新线程命令
 *  
 **/


#include "pub.h"



/**
 * @brief 索引命令重建函数
 *        用于命令回放或者外部数据重放
 * @param [in] cmd_str   : string
 * @param [in] req_head   : nshead_t*
 * @param [in] req_buf   : ub_buff_t*
 * @param [in/out] res_head   : nshead_t*
 * @param [in/out] res_buf   : ub_buff_t*
 * @return  STATIC int 
 * @retval   
**/
int
ts_rebuild(const char* cmd_str, uint32 inner_id,
           nshead_t * req_head,
           ub_buff_t * req_buf, nshead_t * res_head, ub_buff_t * res_buf)
{
    ASSERT(cmd_str == NULL && req_head && req_buf && res_head && res_buf);


    int opret = 0;
    uint32 outer_id;
    vector < ts_terminfo_t > term_list; ///todo : 可能影响性能
    ts_buffer_t brief(g_cfg.brief_size);
    ts_buffer_t fulltext(g_cfg.fulltext_maxsize);

    ts_update_handle_t *hd = g_runtime.handle->update_handle;
	if (!strcmp(cmd_str, "add"))
	{
	    opret = hd->parse_add(req_head, outer_id, term_list, brief, fulltext);
	} else if (!strcmp(cmd_str, "mod"))	{
	    opret = hd->parse_mod(req_head, outer_id, term_list, brief, fulltext);
	} else
	{
		return -1;
	}
    if(opret < 0)
    {
        UB_LOG_WARNING("hd->parse failed.ret[%d]", opret);
        return 0;
    }

    inner_id = g_runtime.cm.get_inner_id(outer_id);
    if(inner_id <= 0)
    {
        UB_LOG_WARNING("cm.get_inner_id failed.outer_id[%d]ret[%d]", outer_id,
                       opret);
        return 0;
    }


    //ri
    ts_ri_item_t item;
    for(vector < ts_terminfo_t >::iterator iter = term_list.begin();
        iter != term_list.end(); ++iter)
    {
        item.weight = iter->weight;
        item.id = inner_id;
        item.term = iter->term;

        opret = g_runtime.mem_ri.write(item);
        if(opret < 0)
        {
            UB_LOG_WARNING
                ("mem_ri.write failed.term[%u:%u]id[%u]wt[%u]ret[%d]",
                 item.term.sign1, item.term.sign2, inner_id, item.weight,
                 opret);
            return opret;
        }

	}
	//di , 没有必要重建
  
	//mg，没有必要重建

    return 0;
}



/**
 * @brief 添加数据
 *
 * @param [in] cmd_str   : string
 * @param [in] req_head   : nshead_t*
 * @param [in] req_buf   : ub_buff_t*
 * @param [in/out] res_head   : nshead_t*
 * @param [in/out] res_buf   : ub_buff_t*
 * @return  STATIC int 
 * @retval   
**/
STATIC int
ts_add(const char* cmd_str,
       nshead_t * req_head, ub_buff_t * req_buf, 
       nshead_t * res_head, ub_buff_t * res_buf)
{
    ASSERT(cmd_str == NULL && req_head && req_buf && res_head && res_buf);


    int opret = 0;

    uint32 outer_id, inner_id;
    vector < ts_terminfo_t > term_list; ///todo : 可能影响性能
    ts_buffer_t brief(g_cfg.brief_size);
    ts_buffer_t fulltext(g_cfg.fulltext_maxsize);

    ts_update_handle_t *hd = g_runtime.handle->update_handle;

    opret = hd->parse_add(req_head, outer_id, term_list, brief, fulltext);
    if(opret < 0)
    {
        UB_LOG_WARNING("hd->parse_add failed.ret[%d]", opret);
        return 0;
    }
	

    if(g_runtime.cm.i_size() >= g_cfg.data_maxnum)
    {
        UB_LOG_WARNING("inner_id[%d] too large", inner_id);
        return -1;
    }

    opret = g_runtime.cm.alloc(outer_id, inner_id);
    if(opret < 0)//分配id失败
    {
        UB_LOG_TRACE("cm.alloc failed.outer_id[%d]ret[%d]", outer_id,
                       opret);
		// 尝试重新获取inner_id，如果可以获取成功，则认为是程序上次运行时未处理完的命令
		// 这里不能只用cm.get_inner_id，否则可能出现致命错误
		{//先去除老数据
			inner_id = g_runtime.cm.get_inner_id(outer_id);
			if(inner_id <= 0)
			{
				UB_LOG_WARNING("get_inner_id failed.outer_id[%d]", outer_id);
				return -2;
			} else
			{
				UB_LOG_DEBUG("get_inner_id[%d].outer_id[%d]", inner_id, outer_id);
			}

			g_runtime.mod_table.set(inner_id, 1);
		}
		opret = g_runtime.cm.realloc(outer_id, inner_id);
		if(inner_id <= 0)
		{
			UB_LOG_WARNING("cm.get_inner_id failed.outer_id[%d]ret[%d]", outer_id,
					opret);
			return 0;
		}
    }

    //ri
    ts_ri_item_t item;
    for(vector < ts_terminfo_t >::iterator iter = term_list.begin();
        iter != term_list.end(); ++iter)
    {
        item.weight = iter->weight;
        item.id = inner_id;
        item.term = iter->term;

        opret = g_runtime.mem_ri.write(item);
        if(opret < 0)
        {
            UB_LOG_WARNING
                ("mem_ri.write failed.term[%u:%u]id[%u]wt[%u]ret[%d]",
                 item.term.sign1, item.term.sign2, inner_id, item.weight,
                 opret);
            return opret;
        }

    }
    //di
    opret = g_runtime.di.write(inner_id, fulltext._buf, fulltext._used);
    if(opret < 0)
    {
        UB_LOG_WARNING("di.write failed.id[%d]size[%d]opret[%d]", inner_id,
                       fulltext._used, opret);
        return opret;
    }
    //mg
    g_runtime.mg.write(inner_id, brief._buf);

    return 0;
}

/**
 * @brief 修改命令
 *
 * @param [in] cmd_str   : string
 * @param [in] req_head   : nshead_t*
 * @param [in] req_buf   : ub_buff_t*
 * @param [in/out] res_head   : nshead_t*
 * @param [in/out] res_buf   : ub_buff_t*
 * @return  STATIC int 
 * @retval   
**/
STATIC int
ts_mod(const char* cmd_str,
       nshead_t * req_head, ub_buff_t * req_buf, 
       nshead_t * res_head, ub_buff_t * res_buf)
{

    ASSERT(cmd_str == NULL && req_head && req_buf && res_head && res_buf);

    int opret = 0;
    uint32 outer_id, inner_id;
    vector < ts_terminfo_t > term_list; ///todo : 可能影响性能
    ts_buffer_t brief(g_cfg.brief_size);
    ts_buffer_t fulltext(g_cfg.fulltext_maxsize);
    ts_update_handle_t *hd = g_runtime.handle->update_handle;

    opret = hd->parse_mod(req_head, outer_id, term_list, brief, fulltext);
    if(opret < 0)
    {
        UB_LOG_WARNING("hd->parse_mod failed.ret[%d]", opret);
        return 0;
    }

    if(g_runtime.cm.i_size() >= g_cfg.data_maxnum)
    {
        UB_LOG_WARNING("inner_id[%d] too large", inner_id);
        return -1;
    }
	//先去除老数据
	inner_id = g_runtime.cm.get_inner_id(outer_id);
	if(inner_id <= 0)
	{
		UB_LOG_WARNING("get_inner_id failed.outer_id[%d]", outer_id);
		return -2;
	} else
	{
		UB_LOG_DEBUG("get_inner_id[%d].outer_id[%d]", inner_id, outer_id);
	}

	g_runtime.mod_table.set(inner_id, 1);

	//重新分配id
    opret = g_runtime.cm.realloc(outer_id, inner_id);
    if(opret < 0)
    {
        UB_LOG_WARNING("cm.alloc failed.outer_id[%d]ret[%d]", outer_id,
                       opret);
        return 0;
    }


    //ri
    ts_ri_item_t item;
    for(vector < ts_terminfo_t >::iterator iter = term_list.begin();
        iter != term_list.end(); ++iter)
    {
        item.weight = iter->weight;
        item.id = inner_id;
        item.term = iter->term;
        opret = g_runtime.mem_ri.write(item);
        if(opret < 0)
        {
            UB_LOG_WARNING
                ("mem_ri.write failed.term[%u:%u]id[%u]wt[%u]ret[%d]",
                 item.term.sign1, item.term.sign2, inner_id, item.weight,
                 opret);
            return opret;
        }

    }
    //di
    opret = g_runtime.di.write(inner_id, fulltext._buf, fulltext._size);
    if(opret < 0)
    {
        UB_LOG_WARNING("di.write failed.id[%d]size[%d]ret[%d]", inner_id,
                       fulltext._size, opret);
        return opret;
    }
    //mg
    g_runtime.mg.write(inner_id, brief._buf);

    return 0;
}

/**
 * @brief 修改基本数据命令
 *
 * @param [in] cmd_str   : string
 * @param [in] req_head   : nshead_t*
 * @param [in] req_buf   : ub_buff_t*
 * @param [in/out] res_head   : nshead_t*
 * @param [in/out] res_buf   : ub_buff_t*
 * @return  STATIC int 
 * @retval   
**/
STATIC int
ts_modbasic(const char* cmd_str,
            nshead_t * req_head, ub_buff_t * req_buf, 
            nshead_t * res_head, ub_buff_t * res_buf)
{

    ASSERT(cmd_str == NULL && req_head && req_buf && res_head && res_buf);
    // bkup cmd
	// 不必备份命令

    int opret = 0;
    uint32 outer_id;
    uint32 inner_id;
    ts_buffer_t brief(g_cfg.brief_size);
    ts_update_handle_t *hd = g_runtime.handle->update_handle;

    opret = hd->parse_modbasic(req_head, outer_id, brief);
    if(opret < 0)
    {
        UB_LOG_WARNING("hd->parse_modbasic failed.id[%d]ret[%d]", outer_id,
                       opret);
        return 0;
    }


    inner_id = g_runtime.cm.get_inner_id(outer_id);
    if(inner_id <= 0)
    {
        UB_LOG_WARNING("cm.get_inner_id failed.outer_id[%d]ret[%d]", outer_id,
                       opret);
        return 0;
    }


    g_runtime.mg.write(inner_id, brief._buf);



    return 0;
}
/**
 * @brief 批量删除数据
 *
 * @param [in] cmd_str   : string
 * @param [in] req_head   : nshead_t*
 * @param [in] req_buf   : ub_buff_t*
 * @param [in/out] res_head   : nshead_t*
 * @param [in/out] res_buf   : ub_buff_t*
 * @return  STATIC int 
 * @retval   
**/
STATIC int
ts_del(const char* cmd_str,
       nshead_t * req_head, ub_buff_t * req_buf, 
       nshead_t * res_head, ub_buff_t * res_buf)
{

    ASSERT(cmd_str == NULL && req_head && req_buf && res_head && res_buf);

    int opret = 0;
    uint32 inner_id;
    vector < uint32 > id_list;  ///todo : 可能影响性能
    ts_update_handle_t *hd = g_runtime.handle->update_handle;
    id_list.reserve(TS_BATCH_MAXNUM);
    id_list.clear();
    opret = hd->parse_del(req_head, id_list);
    if(opret < 0)
    {
        UB_LOG_WARNING("hd->parse_del failed.ret[%d]", opret);
        return 0;
    }
	if (id_list.size() > TS_BATCH_MAXNUM)
	{
		UB_LOG_WARNING("id_list.size(%ld) > %d",id_list.size(),TS_BATCH_MAXNUM);
		return 0;
	}


    for(vector < uint32 >::iterator iter = id_list.begin();
        iter != id_list.end(); ++iter)
    {
        inner_id = g_runtime.cm.get_inner_id(*iter);
        if(inner_id <= 0)
        {
            UB_LOG_WARNING("get_inner_id failed.outer_id[%d]", *iter);
            continue;
        } else
        {
            UB_LOG_DEBUG("get_inner_id[%d].outer_id[%d]", inner_id, *iter);
        }

        g_runtime.del_table.set(inner_id, 1);

    }

    return 0;
}

/**
 * @brief 批量反删除数据
 *
 * @param [in] cmd_str   : string
 * @param [in] req_head  : nshead_t*
 * @param [in] req_buf   : ub_buff_t*
 * @param [in/out] res_head   : nshead_t*
 * @param [in/out] res_buf    : ub_buff_t*
 * @return  STATIC int 
 * @retval   
**/
STATIC int
ts_undel(const char* cmd_str,
         nshead_t * req_head, ub_buff_t * req_buf, 
         nshead_t * res_head, ub_buff_t * res_buf)
{

    ASSERT(cmd_str == NULL && req_head && req_buf && res_head && res_buf);


    int opret = 0;
    uint32 inner_id;
    vector < uint32 > id_list;
    ts_update_handle_t *hd = g_runtime.handle->update_handle;

    id_list.reserve(TS_BATCH_MAXNUM);
    opret = hd->parse_undel(req_head, id_list);
    if(opret < 0)
    {
        UB_LOG_WARNING("hd->parse_del failed.ret[%d]", opret);
        return 0;
    }
	if (id_list.size() > TS_BATCH_MAXNUM)
	{
		UB_LOG_WARNING("id_list.size(%ld) > %d",id_list.size(),TS_BATCH_MAXNUM);
		return 0;
	}

    for(vector < uint32 >::iterator iter = id_list.begin();
        iter != id_list.end(); ++iter)
    {
        inner_id = g_runtime.cm.get_inner_id(*iter);
        if(inner_id <= 0)
        {
            UB_LOG_WARNING("get_inner_id failed.outer_id[%d]", *iter);
            continue;
        } else
        {
            UB_LOG_DEBUG("get_inner_id[%d].outer_id[%d]", inner_id, *iter);
        }
        g_runtime.del_table.set(inner_id, 0);

    }

    return 0;
}

int check_memory()
{
	if (get_memory_size() >= g_cfg.memory_maxsize)
		return -1;
    return 0;
};

/**
 * @brief 更新线程初始化函数
 *
 * @return  int 
 * @retval   
**/
int 
update_thread_init()
{
    ans::lock _lock(&g_ts_lock);
    int opret = 0;

    opret = g_runtime.handle->update_thread_init();
    if(opret < 0)
    {
        return opret;
    }

    //return resend_cmd();///命令回放，只做一次
    return 0;
}


int process_update( 
        nshead_t * req_head, ub_buff_t * req_buf, 
        nshead_t * res_head, ub_buff_t * res_buf
    )
{
    int ret = 0;
    char cmd_str[TS_CMDSTR_MAXLEN];
    ts_update_handle_t *hd = g_runtime.handle->update_handle;
    ret = hd->get_cmd_str(req_head, cmd_str);
    if(ret < 0){
        return ret;
    }    

    if(!strcmp(cmd_str, "add")){
        ret = ts_add(cmd_str, req_head, req_buf, res_head, res_buf);
    }
    if(!strcmp(cmd_str, "mod")){
        ret = ts_mod(cmd_str, req_head, req_buf, res_head, res_buf);
    }
    if(!strcmp(cmd_str, "modbasic")){
        ret = ts_mod(cmd_str, req_head, req_buf, res_head, res_buf);
    }    
    if(!strcmp(cmd_str, "del")){
        ret = ts_del(cmd_str, req_head, req_buf, res_head, res_buf);
    }
    if(!strcmp(cmd_str, "undel")){
        ret = ts_undel(cmd_str, req_head, req_buf, res_head, res_buf);
    }

    return ret;
}

/**
 * @brief 更新命令处理函数
 *
 * @return  int 
 * @retval   =0 成功 ； !=0 失败
 * @see 
 * @author yanghy
 * @date 2008/03/09 20:39:31
**/
int 
op_update()
{
    ans::lock _lock(&g_ts_lock);
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
        FATAL_RETURN("get req_head[%ld] || res_head[%ld] failed.",
                     (long)req_head, (long)res_head);
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
    ub_log_setbasic(UB_LOG_SVRNAME, "%s", g_cfg.svr_update.svr_name);

    ub_log_pushnotice("dlen", "%d", req_head->body_len);

    *res_head = *req_head;
    strncpy(res_head->provider, req_head->provider,
            sizeof(res_head->provider));
    res_head->body_len = 0;
    res_head->reserved = 0;

    ///检查内存是不是满了，满了就直接拒绝提交命令
    opret = check_memory();
    if(0 != opret)
    {
		UB_LOG_WARNING("memory full.waiting to dump memory");
        return opret;
    }
    
    struct timeval total_s,total_e;
    gettimeofday(&total_s, NULL);
    opret = process_update(req_head, &req_buf, res_head, &res_buf); //index_type的处理全部是一种处理逻辑
    if(opret != 0){
            UB_LOG_WARNING("[function:op_update]>>>process_update failed! errno:%d", opret);
            return -1;
    }
    gettimeofday(&total_e, NULL);
    ub_log_setbasic(UB_LOG_PROCTIME, "%luus", TIME_US_DIFF(total_s, total_e));
    
    return opret;
  
}






/* vim: set ts=4 sw=4 sts=4 tw=100 */
