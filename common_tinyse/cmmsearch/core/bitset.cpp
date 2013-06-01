/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: bitset.cpp,v 1.1 2008/10/20 01:56:18 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file bitset.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/08/30 16:58:23
 * @version $Revision: 1.1 $ 
 * @brief 
 *  
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "tinyse.h"



#define BITSET_SUFFIX_FILE ".bitset"
int ts_bitset_t::init(uint32 data_maxnum,const char* dir ,const char *file)
{

	_num = data_maxnum;

	char path[TS_FILEPATH_MAXSIZE];
	snprintf(path,sizeof(path),"%s/%s%s",dir,file,BITSET_SUFFIX_FILE);
	int fd = open(path,O_CREAT|O_RDWR,0664);
	if (fd<0) 
	{
		//	UB_LOG_FATAL("unable to open mmap file[%s]fd[%d]",path,fd);
		return -1; 
	}   

	size_t dsize = (size_t) (_num/8+1);
	lseek(fd,dsize-1,SEEK_SET);
	::write(fd,"",1);
	_array =  (char*)mmap( NULL, dsize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0 );
	if (_array == MAP_FAILED)
	{
		//	UB_LOG_FATAL("unable to map size[%u]",dsize);
		return -2; 
	}   


	return 0;
}

void ts_bitset_t::set(uint32 id,bool value)
{
	int pos = id % 8;
	int off = id / 8;
	if (value)
		*(_array+off) = (*(_array + off)) | ( ((int)value)<<(pos)  ) ;
	else
		*(_array+off) = (*(_array + off)) & ( ~(((int)1)<<(pos))  ) ;
}
bool ts_bitset_t::get(uint32 id)
{
	int pos = id % 8;
	int off = id / 8;
	return ((*(_array + off))>>pos) & 1;
}
int ts_bitset_t::reset()
{
	return 0;
}






















/* vim: set ts=4 sw=4 sts=4 tw=100 */
