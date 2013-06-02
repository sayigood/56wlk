/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: cache_util.cpp,v 1.1 2008/08/28 08:09:23 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file cache_util.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/08/26 20:08:07
 * @version $Revision: 1.1 $ 
 * @brief 
 *  
 **/
#include <assert.h>
#include "mc_cache.h"
#include "ul_func.h"
#include "cache_util.h"

void mc_destroy_cache(mc_cache* pcache)
{
    assert(pcache);
    assert( (pcache->capacity>0) && (pcache->unitsize>0));
            
    //     initialize hash table
	ul_free(pcache->hashtable.table);
	ul_free(pcache->pnodelist);
	ul_free(pcache->pinfolist);
	ul_free(pcache);
 	return;   
}





















/* vim: set ts=4 sw=4 sts=4 tw=100 */
