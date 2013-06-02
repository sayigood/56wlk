/**
 *  =============================================================
 *
 *  Copyright (c) 2011-2012 Panguso.com. All rights reserved.
 *
 *      FileName:  get_type.cpp
 *   Description:  Fuck the world
 *       Created:  2012年07月11日 16:44:02
 *       Version:  
 *      Revision:  #1;#4
 *        AUTHOR:  liuzhangxun(liuzhangxun@panguso.com)
 *
 *  =============================================================
**/
#include "get_type.h"
const int CONF_NUM = 1024;

int GetType::init(const char* path,const char* file)
{
	if ((dup_dict = odb_creat(MAX_DICT_NUM)) == NULL) 
	{
		FATAL_LOG_NLP("dup_dict create error");
		return -1;
	}

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
    string type_dict;
    ul_getconfstr(pconf, "type_dict_path", buf);
    type_dict=buf;
    ul_freeconf(pconf);

	ifstream fin;
	fin.open(type_dict.c_str());
	if( !fin ){
        FATAL_LOG_NLP("open type_dict failed");
		return -1;
	}

    string line;
    string type_code,type;
	while( getline(fin,line) ){
		istringstream stream(line);
		stream>>type_code>>type;
		u_int id1;
		u_int id2;
		Fealib_signature::create_sign64(id1, id2, 1, const_cast<char*>(type.c_str()));
		sodict_snode_t snode;
		snode.sign1 = id1;
		snode.sign2 = id2;
		snode.cuint1 = atoi(type_code.c_str());
		if (odb_seek(dup_dict, &snode) == ODB_SEEK_OK)
		{
			continue;
		}
		odb_add(dup_dict, &snode, 1);
	}
    fin.close();
	return 0;
}

inline string GetType::add_zero(string str)
{
    string new_str;
    if(str.size()%2 != 0)
    {
        new_str="0"+str;
        return new_str;
    }
    else
    {
        return str;
    }
}

int GetType::process(vector<string>v_seg_word,vector<string> &v_type_code,string &res_type_code)
{
    u_int id1;
	u_int id2;

	char word[MAX_QUERY_LEN]={0};
	vector<string>::iterator iter = v_seg_word.begin();
	while(iter != v_seg_word.end())
	{
		for(int len=3;len>0;len--)
		{
			if(len == 3)
			{
				if((iter+2) < v_seg_word.end())
				{
					snprintf(word, MAX_QUERY_LEN, "%s%s%s", iter->c_str(), (iter+1)->c_str(), (iter+2)->c_str());
				}
				else
				{
					continue;
				}
			}
			else if(len == 2)
			{
				if((iter+1) < v_seg_word.end())
				{
					snprintf(word, MAX_QUERY_LEN, "%s%s", iter->c_str(), (iter+1)->c_str());
				}
				else
				{
					continue;
				}
			}
			else
			{
				snprintf(word, MAX_QUERY_LEN, "%s", iter->c_str());
			}
			Fealib_signature::create_sign64(id1, id2, 1, (const char*)word);
			sodict_snode_t snode;
			snode.sign1 = id1;
			snode.sign2 = id2;
			char typecode[MAX_QUERY_LEN]="";
			if (odb_seek(dup_dict, &snode) == ODB_SEEK_OK)
			{
				snprintf(typecode, sizeof(typecode), "%u", snode.cuint1);
                string out_typecode = add_zero(string(typecode)); 
                
                vector<string>::iterator found; 
                found = find( v_type_code.begin(), v_type_code.end(), out_typecode );
                if ( found == v_type_code.end() )
				    v_type_code.push_back(out_typecode);
				iter=iter+(len-1);
				res_type_code = out_typecode;
				break;
			}
		}
		iter++;
	}
	return 0;
}	
