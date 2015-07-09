/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: ts_helper.h,v 1.6 2009/11/18 20:27:50 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file ts_helper.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/08/05 11:51:07
 * @version $Revision: 1.6 $ 
 * @brief 提供给用户的帮助类
 *  
 **/


#ifndef  __TS_HELPER_H_
#define  __TS_HELPER_H_
/**
* @brief 默认的query handle
*/
class DEFAULT_QUERY_HANDLE:public ts_query_handle_t
{
  public:

    virtual int parse_query(const pointer_t query_cmd,
                            ts_buffer_t &req_detail,
                            basic_req_info *basic_info,
                            vector < ts_terminfo_t > &term_list);

    virtual int merge_ind_list(const pointer_t query_cmd,
                                vector < ts_terminfo_t > &term_list,
                                ts_ind_reader_t &ind_reader,
                                vector<ts_index_t> *merged_list) ;


    virtual int index_filt(const pointer_t query_cmd,
                            ts_buffer_t &brief,
                           vector < ts_index_t > *merged_list,
                           vector < ts_index_t > *filted_list);

    virtual int adjust_weight(const pointer_t query_cmd,
                              const vector < ts_terminfo_t > &term_list,
                              vector < ts_index_t > *filted_list);

	virtual int init_abs(const pointer_t query_cmd,ts_buffer_t &result) ;
    
    virtual int fill_basic_res( const pointer_t query_cmd, 
                                basic_req_info *basic_info,
                                vector < ts_index_t > *filted_list,
                                ts_buffer_t & res);    

    virtual int add_abs(const pointer_t query_cmd, int i,
                        const ts_buffer_t & fulltext,
                        const ts_buffer_t & brief, ts_buffer_t & result);

	virtual int fini_abs(const pointer_t query_cmd,ts_buffer_t &result) ;

  private:


};

/**
* @brief 默认的update handle
*/
class DEFAULT_UPDATE_HANDLE:public ts_update_handle_t
{
  public:
    virtual int get_cmd_str(nshead_t *req_head, const char *cmd_str);

    virtual int parse_add(pointer_t update_cmd,
                          uint32 & id,
                          vector < ts_terminfo_t > &term_list,
                          ts_buffer_t & brief, ts_buffer_t & fulltext);


    virtual int parse_mod(pointer_t update_cmd,
                          uint32 & id,
                          vector < ts_terminfo_t > &term_list,
                          ts_buffer_t & brief, ts_buffer_t & fulltext);

    virtual int parse_modbasic(pointer_t update_cmd,
                               uint32 & id, ts_buffer_t & brief);

    virtual int parse_del(pointer_t update_cmd, vector < uint32 > &id_list);


    virtual int parse_undel(pointer_t update_cmd, vector < uint32 > &id_list);

    virtual int parse_keywords(const void *keywords, vector<ts_terminfo_t> &term_list);

};


/**
* @brief 默认的全局初始化函数
*/
int DEFAULT_GLOBAL_INIT();

/**
* @brief 默认的线程初始化函数
*/
int DEFAULT_THREAD_INIT();

/**
* @brief 默认的会话初始化函数
*/
int DEFAULT_SESSION_INIT();


/////////////////////////////////////////////////////////

int nlp_queryparser_init(char *worddict_path,char *bsdict_path,char *stopword_path,uint32 max_term_count);
int nlp_queryparser_thread_init();
int nlp_queryparser_parse(vector<ts_terminfo_t>& term_list,char* query,uint32 len);


int nlp_docparser_parse(vector < ts_terminfo_t > &term_list,char *content, uint32 len);

int nlp_docparser_init(char *worddict_path,char *felt_path,char *stopword_path, 
		uint32 max_term_count);

///////////////////////////////////////////
int init_data_accessor(int thread_num,ts_runtime_t &rt);
int set_data(const char *key, void *data);
void *get_data(const char *key);

/////////////////////////////////////////////////////////
uint32 get_inner_id(const uint32 outer_id);
uint32 get_outer_id(const uint32 inner_id);
uint32 max_inner_id();
uint32 max_outer_id();
void get_brief(const uint32 inner_id,void *brief);












#endif //__TS_HELPER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
