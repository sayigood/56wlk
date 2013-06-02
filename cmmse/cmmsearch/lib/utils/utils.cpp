/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: utils.cpp,v 1.1 2008/08/22 04:02:42 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file utils.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/06/27 13:27:15
 * @version $Revision: 1.1 $ 
 * @brief 
 *  
 **/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"
bool _is_fileexist( const char* filename )
{
	struct stat fst;
	if( stat( filename, &fst )==0 )
		return true;
	return false;
}

























/* vim: set ts=4 sw=4 sts=4 tw=100 */
