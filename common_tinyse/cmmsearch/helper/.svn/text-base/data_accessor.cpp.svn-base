/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: data_accessor.cpp,v 1.3 2009/03/11 03:20:23 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file data_accessor.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/08/09 12:51:27
 * @version $Revision: 1.3 $ 
 * @brief 获取自定义数据的方法
 *  
 **/

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>


#include <ub_server.h>
#include <ub_auth.h>
#include <ub_misc.h>
#include <ub_proccmd.h>
#include "hashmap.h"
#include "tinyse.h"
#include "ul_sign.h"
using namespace ansx;
pointer_t *g_data_map = NULL;
ts_runtime_t *pruntime = NULL; //在主程序中初始化

static const uint32 DMEM_BASE = 1024 + 1;

inline uint32 _dhash_fun(const sign64_t & key)
{
    return (key.sign1 + key.sign2) % DMEM_BASE;
}

inline int _dhash_cmp_fun(const sign64_t & k1, const sign64_t & k2)
{
    return ((k1.sign1 == k2.sign1) && (k1.sign2 == k2.sign2)) ? 0 : 1;
}

int init_data_accessor(int thread_num,ts_runtime_t &rt)
{

    g_data_map = new pointer_t[thread_num];
    if(NULL == g_data_map)
    {
        return -1;
    }
    for(int i = 0; i < thread_num; i++)
    {
        g_data_map[i] = new ansx::hash_map < sign64_t, pointer_t > ();
        ((hash_map < sign64_t, pointer_t > *)g_data_map[i])->create(DMEM_BASE,
                                                                    _dhash_fun,
                                                                    _dhash_cmp_fun);
    }
	pruntime = &rt	;

    return 0;
}

int set_data(const char *key, pointer_t data)
{
    hash_map < sign64_t, pointer_t > *usr_data_map =
        (hash_map < sign64_t, pointer_t > *)ub_server_get_user_data();

    if(NULL == usr_data_map)
    {
        return -1;
    }
    sign64_t sign;
    creat_sign_fs64((char *)key, strlen(key), &sign.sign1, &sign.sign2);
    int opret = usr_data_map->set(sign, data);
    if(HASH_SUCCESS != opret )
        return -2;
    return 0;
}


pointer_t get_data(const char *key)
{

    hash_map < sign64_t, pointer_t > *usr_data_map =
        (hash_map < sign64_t, pointer_t > *)ub_server_get_user_data();

    if(NULL == usr_data_map)
    {
        return NULL;
    }
    sign64_t sign;
    creat_sign_fs64((char *)key, strlen(key), &sign.sign1, &sign.sign2);
    pointer_t data = (pointer_t) - 1;
    int opret = usr_data_map->get(sign, &data);
    if(HASH_SUCCESS != opret )
        return NULL;
    return data;
}



uint32 get_inner_id(const uint32 outer_id)
{
	return pruntime->cm.get_inner_id(outer_id);
}


uint32 get_outer_id(const uint32 inner_id)
{
    return pruntime->cm.get_outer_id(inner_id);
}


uint32 max_inner_id()
{
	return pruntime->cm.i_size();
}


uint32 max_outer_id()
{
    return pruntime->cm.o_size();
}

void get_brief(const uint32 inner_id,void *brief)
{
    pruntime->mg.read(inner_id, brief);
}
/* vim: set ts=4 sw=4 sts=4 tw=100 */
