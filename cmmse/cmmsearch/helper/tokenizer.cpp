/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: tokenizer.cpp,v 1.6 2009/03/16 10:02:44 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file tokenizer.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/08/05 11:56:49
 * @version $Revision: 1.6 $ 
 * @brief 默认分词相关的类
 *  
 **/
#include "scwdef.h"
#include "isegment.h"
#include "wp_felt.h"
#include "tinyse.h"
#include "ul_ccode.h"
#include "ul_log.h"
#include "ul_sign.h"
#include "thread_specific.h"
#include "hashmap.h"
#include "stopword.h"
#define MAX_TERM_COUNT 10000
#define IS_ENG(ch)              (((ch) >= 'a' && (ch) <= 'z') || ((ch) >= 'A' && (ch) <='Z') )
#define IS_DIGITAL(ch)          ((ch) - '0' >=0 && (ch) - '0' <= 9)
static const int MEM_BASE = 1024 * 10 + 1;

inline uint32 _mhash_fun(const sign64_t & key)
{
	return (key.sign1 + key.sign2) % MEM_BASE;
}

inline int _mhash_cmp_fun(const sign64_t & k1, const sign64_t & k2)
{
	return ((k1.sign1 == k2.sign1) && (k1.sign2 == k2.sign2)) ? 0 : 1;
}
/////////////////////////////

typedef struct
{
	search_seg_data_t *psegData ;
	char *phraseoffreq ;
	char *parsedquery;
	ansx::hash_map < sign64_t, uint32 > map;
} nlp_queryparser_buffer_t;

typedef struct 
{
	wp_dict_pack_t *pdictPack ;
	uint32 max_term_count;
	thread_specific<nlp_queryparser_buffer_t> *buffer;
	stop_word_t sdict;

}nlp_queryparser_t;

nlp_queryparser_t* queryparser = NULL;

void *creat_queryparser_buffer()
{
	nlp_queryparser_buffer_t *tmp = new nlp_queryparser_buffer_t();
	if ( init_segData(tmp->psegData, tmp->phraseoffreq,queryparser->max_term_count) < 0 )
	{
		return NULL;
	}
	tmp->parsedquery = new char[queryparser->max_term_count * 64];
	int opret = tmp->map.create(MEM_BASE, _mhash_fun, _mhash_cmp_fun);
	if(opret < 0)
		return NULL;


	return tmp;

};

int nlp_queryparser_init(char *worddict_path,char *bsdict_path,char *stopword_path,uint32 max_term_count)
{
	queryparser = new nlp_queryparser_t();
	if (NULL == queryparser)
		return -1;
	queryparser->max_term_count = max_term_count;

	bool atFelt = false;
	if( init_dictPack(worddict_path, bsdict_path, queryparser->pdictPack, atFelt) < 0)
	{
		return -2;
	}
	queryparser->buffer = new thread_specific<nlp_queryparser_buffer_t>(creat_queryparser_buffer);
	if (!queryparser->buffer)
		return -3;

	return queryparser->sdict.init(stopword_path);;
}
int nlp_queryparser_thread_init()
{
/*
	queryparser->buffer = new thread_specific<nlp_queryparser_buffer_t>(creat_queryparser_buffer);
	if (!queryparser->buffer)
		return -1;
*/
	return 0;

}

int nlp_queryparser_parse(vector<ts_terminfo_t>& term_list,char* query,uint32 len)
{

	nlp_queryparser_buffer_t *buffer = queryparser->buffer->ts_get ();
	if (NULL == buffer)
	{
		return -1;
	}

    ul_trans2bj(query,query);
    ul_trans2bj_ext(query, query);
    ul_trans2lower(query,query);

//	scw_segment_words(queryparser->pdictPack->pScwDict, buffer->psegData->m_out,query,len);
	//wp_felt_hfword_p(&(buffer->psegData->m_felt), buffer->psegData->m_out, queryparser->pdictPack->pFeltDict);
		//todo : 是否需要
	if (ns_get_seg_pos(query,len,buffer->psegData,queryparser->pdictPack)<0)
	{
		ul_writelog(UL_LOG_WARNING,"ns_get_seg_pos failed.");;
		return -1;
	}

	int query_is_orig_name = 0;
	if( wp_get_query(queryparser->pdictPack,buffer->psegData, 
				0, SEARCH_IN_CT, buffer->phraseoffreq, query_is_orig_name) < 0)
	{   
		ul_writelog(UL_LOG_WARNING,"wp_get_query failed.");
		return -2;
	}   


	ts_terminfo_t terminfo;
	search_seg_data_t *p = buffer->psegData ;
	buffer->map.reset();
    for (int i = 0; i <  p->m_felt_merge.m_termcount; i++)
    {   
        int pos = GET_TERM_POS (*(((u_int *) p->m_felt_merge.m_termpos)+i));
        int len = GET_TERM_LEN (*(((u_int *) p->m_felt_merge.m_termpos)+i));
        p->m_felt_merge.m_buf[pos + len] = 0;

		creat_sign_fs64 (p->m_felt_merge.m_buf+pos, len, &terminfo.term.sign1, &terminfo.term.sign2);

		if (len<=1 && !IS_DIGITAL(p->m_felt_merge.m_buf[pos]) 
			&& !IS_ENG(p->m_felt_merge.m_buf[pos]))//去除标点等单字符
			continue;

        p->m_felt_merge.m_buf[pos + len] = ' ';


		if (0 == queryparser->sdict.find(terminfo.term.sign1, terminfo.term.sign2))//停用词
			continue;

		uint32 added = 0;
		int opret = buffer->map.get(terminfo.term, &added);
		if(ansx::HASH_VAL_NO_EXIST == opret)
		{
			term_list.push_back(terminfo);
			opret = buffer->map.set(terminfo.term, added);
			if(ansx::HASH_SUCCESS != opret )
				return -3;
		}



    }   
 
	return 0;
}


////////////////////////////////////////////////////////////////////////
/**
* @brief 建库文档分词器
*   1，线程不安全；2，支持停用词;3,使用混合词buffer和建库粘接
*  
*/
typedef struct nlp_docparser_t
{
	scw_worddict_t * pwdict ;
	Sdict_search   * pfdict;
	scw_out_t *pout;
	wp_felt_t felt;
	ansx::hash_map < sign64_t, uint32 > map;
	stop_word_t sdict;
	uint32 max_term_count;
} nlp_docparser_t;

nlp_docparser_t *docparser = NULL;
int nlp_docparser_init(char *worddict_path,char *felt_path,char *stopword_path, 
		uint32 max_term_count)
{
	docparser = new nlp_docparser_t();
	if(NULL == docparser)
		return -1;
	docparser->max_term_count = max_term_count;

	int opret = docparser->map.create(MEM_BASE, _mhash_fun, _mhash_cmp_fun);
	if(opret < 0)
		return -1;

	if((docparser->pwdict=scw_load_worddict(worddict_path))==NULL)
	{
		return -1;
	}
	int scw_out_flag = SCW_OUT_ALL | SCW_OUT_PROP;
	if((docparser->pout=scw_create_out(max_term_count, scw_out_flag))==NULL)
	{
		return -1;
	}

	//初始化粘接相关的量
    docparser->pfdict = ds_load( felt_path, "felt" );
    if(NULL == docparser->pfdict )
    {   
        return -1; 
	}   
	docparser->felt.m_buf = new char[max_term_count];
	docparser->felt.m_size = max_term_count;
	docparser->felt.m_curpos = 0;
	docparser->felt.m_termcount = 0;
	docparser->felt.m_termoffsets = new int[max_term_count];
	docparser->felt.m_termpos = new int[max_term_count];

	
	return docparser->sdict.init(stopword_path);
}






int nlp_docparser_parse(vector < ts_terminfo_t > &term_list,
		char *content, uint32 len)
{

	char word[1024];
	ts_terminfo_t terminfo;
	docparser->map.reset();

    ul_trans2bj(content,content);
    ul_trans2bj_ext(content, content);
    ul_trans2lower(content,content);

	//分词
	scw_segment_words(docparser->pwdict,docparser->pout,content,len);
	//基本词
	for( uint32 i = 0; i<docparser->pout->wsbtermcount; i++)
	{
		int pos = GET_TERM_POS(docparser->pout->wsbtermpos[i]);
		int len = GET_TERM_LEN(docparser->pout->wsbtermpos[i]);
		strncpy(word, docparser->pout->wordsepbuf + pos, len);
		word[len] = 0;
		creat_sign_fs64(word, strlen(word),
				&terminfo.term.sign1, &terminfo.term.sign2);


		if (0 == docparser->sdict.find(terminfo.term.sign1, terminfo.term.sign2))//停用词
			continue;
		if (len<=1 && !IS_DIGITAL(word[0]) && !IS_ENG(word[0]))//去除标点等单字符
			continue;

		uint32 added = 0;
		int opret = docparser->map.get(terminfo.term, &added);
		if(ansx::HASH_VAL_NO_EXIST == opret)
		{
			term_list.push_back(terminfo);
			opret = docparser->map.set(terminfo.term, added);
			if(opret < 0)
				return -3;
		}
	}
	//词/词组
	for( uint32 i = 0; i<docparser->pout->wpbtermcount; i++)
	{
		int pos = GET_TERM_POS(docparser->pout->wpbtermpos[i]);
		int len = GET_TERM_LEN(docparser->pout->wpbtermpos[i]);
		strncpy(word, docparser->pout->wpcompbuf + pos, len);
		word[len] = 0;
		creat_sign_fs64(word, strlen(word),
				&terminfo.term.sign1, &terminfo.term.sign2);


		if (0 == docparser->sdict.find(terminfo.term.sign1, terminfo.term.sign2))//停用词
			continue;
		if (len<=1 && !IS_DIGITAL(word[0]) && !IS_ENG(word[0]))//去除标点等单字符
			continue;

		uint32 added = 0;
		int opret = docparser->map.get(terminfo.term, &added);
		if(ansx::HASH_VAL_NO_EXIST == opret)
		{
			term_list.push_back(terminfo);
			opret = docparser->map.set(terminfo.term, added);
			if(opret < 0)
				return -3;
		}
	}
	//粘接
	docparser->felt.m_curpos = 0;
	docparser->felt.m_termcount = 0;
	wp_felt_hfword_p( &docparser->felt, docparser->pout, docparser->pfdict );
	for (int i=0;i<docparser->felt.m_termcount;i++)
	{
        int pos = GET_TERM_POS(docparser->felt.m_termpos[i]);
        int len = GET_TERM_LEN(docparser->felt.m_termpos[i]);

        strncpy(word, docparser->felt.m_buf+pos, len);
        word[len] = 0;
		creat_sign_fs64(word, strlen(word),
				&terminfo.term.sign1, &terminfo.term.sign2);
		uint32 added = 0;
		int opret = docparser->map.get(terminfo.term, &added);
		if(ansx::HASH_VAL_NO_EXIST == opret)
		{
			term_list.push_back(terminfo);
			opret = docparser->map.set(terminfo.term, added);
			if(ansx::HASH_SUCCESS != opret )
				return -3;
		}

	}
	return 0;
}







/* vim: set ts=4 sw=4 sts=4 tw=100 */
