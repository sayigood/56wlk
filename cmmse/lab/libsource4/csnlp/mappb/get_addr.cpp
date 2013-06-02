/**
 *  =============================================================
 *
 *  Copyright (c) 2011-2012 Panguso.com. All rights reserved.
 *
 *      FileName:  get_addr.cpp
 *   Description:  Fuck the world
 *       Created:  2012年07月12日 13:21:44
 *       Version:  
 *      Revision:  #1;#4
 *        AUTHOR:  liuzhangxun(liuzhangxun@panguso.com)
 *
 *  =============================================================
**/

#include "get_addr.h"
const int CONF_NUM = 1024;
const int QUERY_LEN = 100;

int GetAddr::init(const char* path,const char* file)
{
	Ul_confdata *pconf;
	pconf = ul_initconf(CONF_NUM);
	int ret;
	ret = ul_readconf(path, file, pconf);
	if (ret < 0) 
	{
		FATAL_LOG_NLP("load config file error!");
		return -1;
	}
    char buf[100];
	string loc_map_dict;
	ul_getconfstr(pconf, "loc_map_path", buf);
	loc_map_dict=buf;

    string adcode_map_dict; 
    ul_getconfstr(pconf, "adcode_map_path", buf);
    adcode_map_dict=buf;

    string suffix_dict; 
    ul_getconfstr(pconf, "suffix_path", buf);
    suffix_dict=buf;

    string extendMap_dict;
    ul_getconfstr(pconf, "extend_dict_path", buf);
    extendMap_dict=buf;

    ul_freeconf(pconf);

	ifstream fin1;
	fin1.open(loc_map_dict.c_str());
	if( !fin1 )
	{
		FATAL_LOG_NLP("open loc_map_dict failed");
		return -1; 
	}

    ifstream fin2;
    fin2.open(adcode_map_dict.c_str());
    if( !fin2 )
    {
        FATAL_LOG_NLP("open adcode_map_dict failed");
        return -1; 
    }

    ifstream fin3;
    fin3.open(suffix_dict.c_str());
    if( !fin3 )
    {
        FATAL_LOG_NLP("open suffix_dict failed");
        return -1; 
    }

    ifstream fin4;
    fin4.open(extendMap_dict.c_str());
    if( !fin4 )
    {
        FATAL_LOG_NLP("open extendMap_dict failed, errno:%d, dir:%s", errno, extendMap_dict.c_str());
        return -1;
    }

	tree = init_trie_tree();
	if(!tree)
	{
		FATAL_LOG_NLP("init_trie_tree failed");
		return -1;
	}

	string line;
    string word_1,word_2;
    string pattern = "\t";
    string::size_type pos = 0;
	trie_node *node;
	while(getline(fin1,line))
	{
        pos = line.find(pattern);
        if (pos != -1) {
            word_1 = line.substr(0,pos);
		    node = add_word(tree, word_1.c_str());
            if(!node)
            {
                FATAL_LOG_NLP("add_word failed");
                return -1;
            }
            ++node->preq;
        }
	}

    while(getline(fin2,line))
    {
        pos = line.find(pattern);
        if (pos != -1) {
            word_2 = line.substr(pos+1,line.length());
            node = add_word(tree, word_2.c_str());
            if(!node)
            {
                FATAL_LOG_NLP("add_word failed");
                return -1;
            }
            ++node->preq;
        }
    }
   
    while(getline(fin3,line))
    {
        filter_dict.push_back(line);
    }

    while(getline(fin4,line))
    {
        pos = line.find(pattern);
        if (pos != -1) {
            word_1 = line.substr(0,pos);
            node = add_word(tree, word_1.c_str());
            if(!node)
            {
                FATAL_LOG_NLP("add_word failed");
                return -1;
            }   
            ++node->preq;
        }
    }

    fin1.close();
    fin2.close();
    fin3.close();
    fin4.close();
	return 0;
}

void GetAddr::to_lower(const char* query, char* query_lower)
{
    while(*query)
    {
        if( !((*((unsigned char*)query) & 0x80) >> 7) )
        {
            if(*query > 'A' && *query < 'Z')
            {
                *query_lower = *query + 32;
                query++;
                query_lower++;
            }
            else
                *query_lower++ = *query++;
        }
        else
            *query_lower++ = *query++;
    }
}

int GetAddr::process(vector<string> v_seg_word, const char *query, vector<string> &v_addr)
{
    assert(query);

    trie_tree * tree = getTree();

    vector<string>::iterator found;
    for(vector<string>::iterator iter=v_seg_word.begin(); iter<v_seg_word.end(); iter++)
    {
        found = find( filter_dict.begin(), filter_dict.end(), *iter);
        if ( found == filter_dict.end() )
        {
            found = find(v_addr.begin(), v_addr.end(), *iter);
            if( found == v_addr.end())
                v_addr.push_back(*iter);
        }
    }

    trie_node *root_node = &(tree->root_node);
    trie_node *cur_node = &(tree->root_node);

    trie_node *chunk_node;
    int offset;
    char query_lower[QUERY_LEN]={0};
    to_lower(query, query_lower);

    char *p = (char *)query_lower;
    char *p_end = (char *)query_lower;
    char *p_end_temp;

    while(*p_end)
    {
        //当前节点若没有子节点了，表示终结了此次遍历，直接退回即可
        if(!(cur_node->chunk_size))
        {
            //若当前节点不是根节点，说明是遍历到preq>=1时继续行进的，需要退回到根节点；同时将起始标志置到p_end处
            if(cur_node!=root_node)
            {
                cur_node=root_node;
                p=p_end_temp;
                continue;
            }
            break;
        }
        offset = (unsigned char)*p_end % cur_node->chunk_size;
        chunk_node = cur_node->chunk_nodes[offset];
        while(chunk_node)
        {
            if(chunk_node->key == *p_end)
            {
                if(chunk_node->preq >= 1)
                {
                    string addr;
                    addr.assign(p,p_end-p+1);
                    p_end_temp=p_end+1;
                    
                    found = find( v_addr.begin(), v_addr.end(), addr );
                    if ( found == v_addr.end() )
                        v_addr.push_back(addr);
                }
                break;
            }
            chunk_node = chunk_node->next;
        }
        if(chunk_node)
        {
            ++p_end;
            cur_node = chunk_node;
        }
        else
        {
            p++;
            p_end=p;
            cur_node = root_node;
        }
    }
    return 0;
}
