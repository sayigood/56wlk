/**
 * @file default.cpp
 * @author
 * @date 2013.01.06 09:21:07
 * @version $Revision: 1.6 $ 
 * @brief 默认实现
 *  
 **/
#include "nshead.h"
#include "mc_pack.h"
#include "mc_pack_rp.h"
#include "ul_sign.h"
#include <ub_log.h>
#include <algorithm>
#include "tinyse.h"



int DEFAULT_GLOBAL_INIT()
{
    return 0;
}

int DEFAULT_THREAD_INIT()
{
    return 0;
}

int DEFAULT_SESSION_INIT()
{
    return 0;
};

int DEFAULT_QUERY_HANDLE::get_cmd_str(nshead_t *req_head, char *cmd_str)
{
    //req
	bsl::ResourcePool rp;
	mc_pack_t *mc = mc_pack_open_r_rp((char *)(req_head+1), req_head->body_len, &rp);
    if(MC_PACK_PTR_ERR(mc)){
        UB_LOG_WARNING("[ret:][desc:mc_pack_open_r_rp error][intput:]");
        return -1;
    }
    //cmd
    int ret = mc_pack_get_uint32(mc, "cmd", &cmd_str);
    if(ret < 0){
        return ret;
    }
    
    mc_pack_close(mc);
    return 0;
}

int DEFAULT_QUERY_HANDLE::parse_query(char *cmd_str,
                                      const pointer_t query_cmd,
                                      basic_req_info *basic_info,
                                      vector < ts_terminfo_t > &term_list)
{
    nshead_t *req_nshead = (nshead_t *) query_cmd;

    //req
	bsl::ResourcePool rp;
	mc_pack_t *mc = mc_pack_open_r_rp((char *)(req_nshead+1), req_nshead->body_len, &rp);
    if(MC_PACK_PTR_ERR(mc)){
        UB_LOG_WARNING("[function:parse_query>>mc_pack_open_r_rp][ret:][desc:mc_pack_open_r_rp error][intput:]");
        return -2;
    }
    //print input
    char in_query[1024*1024];
    mc_pack_pack2json_gbk(mc, in_query, 1024*1024);
    ub_log_pushnotice("[SE INPUT:", " %s]", in_query );

    //cmd
    cmd_str = mc_pack_get_str(mc, "cmd");
    if(MC_PACK_PTR_ERR(cmd_str))
    {
        return -2;
    }
    
    //page_no
    page_no = mc_pack_get_uint32(mc, "page_no");
    if(MC_PACK_PTR_ERR(page_no))
    {
        return -2;
    }  
    //num_per_page
    num_per_page = mc_pack_get_uint32(mc, "num_per_page");
    if(MC_PACK_PTR_ERR(num_per_page))
    {
        return -2;
    }
    //填充结果时使用   
    snprintf(basic_info->cmd_str, TS_CMDSTR_MAXLEN, "%s", cmd);
    basic_info->page_no  = page_no;
    basic_info->num_per_page = num_per_page;
    
    //term_list
    const char *keyword = mc_pack_get_str(mc, "query");
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
        
    mc_pack_close(mc);
    return 0;
};

class sort_less_t
{
  public:
    bool operator() (const ts_terminfo_t & a, const ts_terminfo_t & b)const
    {   
        return a.ind_num < b.ind_num;
    }   
};
int DEFAULT_QUERY_HANDLE::merge_ind_list(const pointer_t query_cmd,
                                vector < ts_terminfo_t > &term_list,
                                ts_ind_reader_t &ind_reader,
                                vector<ts_index_t> *merged_list) 
{

    //get ind_num
    for(vector < ts_terminfo_t >::iterator iter = term_list.begin();
        iter != term_list.end(); ++iter)
    {
        iter->ind_num = ind_reader.read_ind_num(iter->term);
		if ((int)iter->ind_num<0)
		{
			UB_LOG_WARNING("ind_reader.read_ind_num [%u:%u] failed.",iter->term.sign1,iter->term.sign2);
		}      

        if(0 == iter->ind_num)
        {
            UB_LOG_DEBUG("no result.term[%u:%u]", iter->term.sign1,
                         iter->term.sign2);
            return 0;
        } 

    }
	//找出最短term
    std::sort(term_list.begin(), term_list.end(), sort_less_t());
	
	vector < ts_index_t > *lst1 = ((vector < ts_index_t > *)get_data("lst1"));
	vector < ts_index_t > *lst3 = ((vector < ts_index_t > *)get_data("lst3"));
	lst1->clear();
    lst3->clear();
	
	int k = 0;	
	for (vector<ts_terminfo_t>::iterator  term_it = term_list.begin();
		term_it != term_list.end();++term_it)
	{
	
		if (0 == k) {
			if (1 == term_list.size())
			{
				ts_ind_reader_t::iterator ind_it2 = 
						ind_reader.begin(term_it->term,TS_INDEX_SHORTEST_NUM);
				lst3->resize(ind_it2.size());
				std::copy(ind_it2,ind_reader.end(),lst3->begin());
			}

		} else if (1 == k) {

			ts_ind_reader_t::iterator ind_it1 = 
							ind_reader.begin(term_list.begin()->term,TS_INDEX_SHORTEST_NUM);
			ts_ind_reader_t::iterator ind_it2 = ind_reader.begin(term_it->term,TS_INDEX_MAXNUM);;

			while (ind_it1 != ind_reader.end() && ind_it2 != ind_reader.end())
			{
				ts_index_t& ind1 = *ind_it1;
				ts_index_t& ind2 = *ind_it2;

				if (ind1.id > ind2.id)
				{
					++ind_it2;
				} else if (ind1.id < ind2.id){
					++ind_it1;
				} else {
					ts_index_t tmp ;
					tmp.id = ind1.id;
					tmp.sweight = ind1.sweight + ind2.sweight;
					lst3->push_back(tmp);			
					++ind_it1;
					++ind_it2;
				}

			}
		} else {
			vector < ts_index_t >::iterator ind_it1 = lst1->begin();
			ts_ind_reader_t::iterator ind_it2 = ind_reader.begin(term_it->term,TS_INDEX_MAXNUM);;
			while (ind_it1 != lst1->end() && ind_it2 != ind_reader.end())
			{
				ts_index_t& ind1 = *ind_it1;
				ts_index_t& ind2 = *ind_it2;
				if (ind1.id > ind2.id)
				{
					++ind_it2;
				} else if (ind1.id < ind2.id){
					++ind_it1;
				} else {
					ts_index_t tmp ;
					tmp.id = ind1.id;
					tmp.sweight = ind1.sweight + ind2.sweight;
					lst3->push_back(tmp);			
					++ind_it1;
					++ind_it2;
				}

			}
		}
		{///swap
			vector < ts_index_t > *tmp = lst1;
			lst1 = lst3;
			lst3 = tmp;
			lst3->clear();
		}
		k++;
	}
	int old_size = merged_list->size();
	merged_list->resize(old_size+lst1->size());
	std::copy(lst1->begin(),lst1->end(),merged_list->begin()+old_size);

	return 0;
}


int DEFAULT_QUERY_HANDLE::index_filt(const pointer_t query_cmd,
                                     vector < ts_index_t > *merged_list,
                                     vector < ts_index_t > *filted_list)
{
    nshead_t *req_nshead = (nshead_t *) query_cmd;
    mc_pack_t *mc = (mc_pack_t *) & req_nshead[1];

    uint32 pubtime = mc_pack_get_uint32(mc, "pubtime");
    if(MC_PACK_PTR_ERR(pubtime))
    {
        return -1;
    }

    ts_buffer_t brief(g_cfg.brief_size);
    brief_t *pbrief = (brief_t *)brief._buf;
    for(vector < ts_index_t >::iterator iter = merged_list->begin();
        iter != merged_list->end(); ++iter)
    {
        get_brief(iter->id, brief._buf);
        if(pbrief->pubtime == pubtime)
        {
            //std::copy(iter,iter,filted_list.end());
            filted_list->push_back(*iter);
        }
    };
    
    return 0;
}


int DEFAULT_QUERY_HANDLE::adjust_weight(const pointer_t query_cmd,
                                        const vector < ts_terminfo_t >
                                        &term_list, 
                                        vector < ts_index_t > *filted_list)
{
    return 0;
}

int DEFAULT_QUERY_HANDLE::fill_basic_res(const pointer_t query_cmd, 
                                        basic_req_info *basic_info,
                                        ts_buffer_t & res)
{
    bsl::ResourcePool rp;
    mc_pack_t *mc = mc_pack_open_rw_rp(res._buf, res._size, &rp);
    if(MC_PACK_PTR_ERR(mc)){
        UB_LOG_WARNING("[ret:][desc:mc_pack_open_r_rp error][intput:]");
        return -2;
    }

    page_no = basic_info->page_no;
    num_per_page = basic_info->num_per_page;
    

    int start_num = num_per_page * page_no;
    int ret_num = filted_list->size() - start_num;

    ret_num = ret_num > num_per_page ? num_per_page : ret_num;
    ret_num = ret_num < 0 ? 0 : ret_num;
    
    mc_pack_put_string(mc, "cmd", basic_info->cmd_str);
    mc_pack_put_uint32(mc, "disp_num", filted_list->size());    
    mc_pack_put_uint32(mc, "ret_num", ret_num);
    
    basic_info->ret_num = ret_num;
    basic_info->disp_num = disp_num;
    
    mc_pack_close(mc);
    res._used = mc_pack_get_size(mc);
    
    return 0;
}


int DEFAULT_QUERY_HANDLE::add_abs(const pointer_t query_cmd, int i,
                                  const ts_buffer_t & fts,
                                  const ts_buffer_t & brs, ts_buffer_t & res)
{
    char name[16];
    snprintf(name, sizeof(name), "result%d", i);

    //TODO:1、输入i没作用了；2、每次都open mcpack性能可以能有问题
    bsl::ResourcePool rp;
    mc_pack_t *mc = mc_pack_open_rw_rp(res._buf, res._size, &rp);
    if(MC_PACK_PTR_ERR(mc)){
        UB_LOG_WARNING("[function:parse_query>>mc_pack_open_r_rp][ret:][desc:mc_pack_open_r_rp error][intput:]");
        return -2;
    }

    mc_pack_t *tmp = mc_pack_put_object(mc, name);
    if(MC_PACK_PTR_ERR(tmp) < 0)
    {
        return MC_PACK_PTR_ERR(tmp);
    }
    mc_pack_copy_item((mc_pack_t *) fts._buf, tmp, NULL);
    mc_pack_close(mc);
    res._used = mc_pack_get_size(mc);

    return 0;
}


int DEFAULT_QUERY_HANDLE::init_abs(const pointer_t query_cmd,ts_buffer_t &result) 
{
	return 0;
}


int DEFAULT_QUERY_HANDLE::fini_abs(const pointer_t query_cmd,ts_buffer_t &result) 
{
	return 0;
}



int DEFAULT_UPDATE_HANDLE::parse_add(pointer_t update_cmd,
                                     uint32 & id,
                                     vector < ts_terminfo_t > &term_list,
                                     ts_buffer_t & brief,
                                     ts_buffer_t & fulltext)
{

    nshead_t *req_nshead = (nshead_t *) update_cmd;
    mc_pack_t *mc = (mc_pack_t *) & req_nshead[1];

    //id
    id = mc_pack_get_uint32(mc, "id");
    if(MC_PACK_PTR_ERR(id))
    {
        return -1;
    }
    
    //brief
    uint32 pubtime,msgsign1,msgsign2,mark;
    mc_pack_t *mc_brief = mc_pack_get_obj(mc, "brief");
    if(MC_PACK_PTR_ERR(mc_brief))
    {
        return -2;
    }

    pubtime = mc_pack_get_uint32(mc_brief, "pubtime");
    if(MC_PACK_PTR_ERR(pubtime))
    {
        return -3;
    }
    msgsign1 = mc_pack_get_uint32(mc_brief, "msg_sign1");
    if(MC_PACK_PTR_ERR(msgsign1))
    {
        return -4;
    }
    msgsign2 = mc_pack_get_uint32(mc_brief, "msg_sign2");
    if(MC_PACK_PTR_ERR(msgsign2))
    {
        return -5;
    }
    mark = mc_pack_get_uint32(mc_brief, "mark");
    if(MC_PACK_PTR_ERR(mark))
    {
        return -6;
    }    
    
    brief_t *pbrief  = (brief_t *)brief._buf;
    
    pbrief->pubtime  = pubtime;
    pbrief->msgsign1 = msgsign1;
    pbrief->msgsign2 = msgsign2;
    pbrief->mark = mark;

    //fulltext
	if (mc_pack_get_size(mc) > (int)fulltext._size)
	{
		return -7;
	}
    memcpy(fulltext._buf, mc, mc_pack_get_size(mc));
    fulltext._used = mc_pack_get_size(mc);

    //term_list
    const char *keywords = mc_pack_get_str(mc, "keywords");
    if(MC_PACK_PTR_ERR(keywords))
    {
        return -8;
    }

	int opret = parse_keywords(keywords, term_list);
    if(opret){
        return -9;
    }   
    
    return 0;
}


int DEFAULT_UPDATE_HANDLE::parse_mod(pointer_t update_cmd,
                                     uint32 & id,
                                     vector < ts_terminfo_t > &termlist,
                                     ts_buffer_t & brief,
                                     ts_buffer_t & fulltext)
{
    return parse_add(update_cmd,id,termlist,brief,fulltext);
}

int DEFAULT_UPDATE_HANDLE::parse_modbasic(pointer_t update_cmd,
                                          uint32 & id, ts_buffer_t & brief)
{
	nshead_t *req_nshead = (nshead_t *) update_cmd;
	mc_pack_t *mc = (mc_pack_t *) & req_nshead[1];

	//id
    id = mc_pack_get_uint32(mc, "id");
    if(MC_PACK_PTR_ERR(id))
    {
        return -1;
    }
    
	//brief
    uint32 pubtime,msgsign1,msgsign2,mark;
    mc_pack_t *mc_brief = mc_pack_get_obj(mc, "brief");
    if(MC_PACK_PTR_ERR(mc_brief))
    {
        return -2;
    }

    pubtime = mc_pack_get_uint32(mc_brief, "pubtime");
    if(MC_PACK_PTR_ERR(pubtime))
    {
        return -3;
    }
    msgsign1 = mc_pack_get_uint32(mc_brief, "msg_sign1");
    if(MC_PACK_PTR_ERR(msgsign1))
    {
        return -4;
    }
    msgsign2 = mc_pack_get_uint32(mc_brief, "msg_sign2");
    if(MC_PACK_PTR_ERR(msgsign2))
    {
        return -5;
    }
    mark = mc_pack_get_uint32(mc_brief, "mark");
    if(MC_PACK_PTR_ERR(mark))
    {
        return -6;
    }    
    
    brief_t *pbrief  = (brief_t *)brief._buf;
    
    pbrief->pubtime  = pubtime;
    pbrief->msgsign1 = msgsign1;
    pbrief->msgsign2 = msgsign2;
    pbrief->mark = mark;

	return 0;
}

int DEFAULT_UPDATE_HANDLE::parse_del(pointer_t update_cmd,
		vector < uint32 > &id_list)
{
	nshead_t *req_nshead = (nshead_t *) update_cmd;
    uint32 *ids = (uint32 *) & req_nshead[1];
    uint32 num = req_nshead->body_len / sizeof(uint32);
    id_list.resize(num);
    std::copy(ids, ids + num, id_list.begin());

    return 0;
}


int DEFAULT_UPDATE_HANDLE::parse_undel(pointer_t update_cmd,
                                       vector < uint32 > &id_list)
{
    return parse_del(update_cmd, id_list);
}

int DEFAULT_UPDATE_HANDLE::parse_keywords(
        const void *keywords, 
        vector<ts_terminfo_t> &term_list)
{
	if(NULL == keywords)
    {
		return -1;
	}
	mc_pack_t *mc=(mc_pack_t*)keywords;
    if(MC_PACK_PTR_ERR(mc))
    {
        return -2;
    }

	const char *token;
    char fni_token[64];
	ts_terminfo_t terminfo;
	mc_pack_item_t item;
	int opret = mc_pack_first_item(mc, &item);
	while(opret==0)
	{
		const mc_pack_t *arr;
		opret = mc_pack_get_array_from_item(&item, &arr);
		if(opret<0)
		{
			UB_LOG_WARNING("[ret:%d][desc:get pack from item failed][input:]", opret);
			return -3;
		}
		int size = mc_pack_get_item_count(arr);
        for(int i=0; i<size; i++)
        {
            mc_pack_t *obj = mc_pack_get_object_arr(arr, i);
            if(MC_PACK_PTR_ERR(obj)) {
                return -4;
            }
            token = mc_pack_get_str(obj, "key");
            if(MC_PACK_PTR_ERR(token)) {
                return -5;
            }
            opret = mc_pack_get_uint32(obj, "w", &terminfo.weight);
            if(opret)
            {
                UB_LOG_WARNING("[ret:%d][desc:get uint32 from pack failed]", opret);
                return -6;
            }
            *fni_token = '\0';
            snprintf(fni_token, sizeof(fni_token), "%s_%s", item.key, token);
            creat_sign_fs64((char*)fni_token, strlen(fni_token), &terminfo.term.sign1, &terminfo.term.sign2);
            term_list.push_back(terminfo);
            UB_LOG_DEBUG("uterm[%u:%u], term[%d], count[%d]", 
                        terminfo.term.sign1, terminfo.term.sign2, fni_token, i);
        }

		opret = mc_pack_next_item(&item, &item);
	}

	return 0;
}



/* vim: set ts=4 sw=4 sts=4 tw=100 */
