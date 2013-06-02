/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: bitset.h,v 1.3 2008/08/26 10:54:55 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file bitset.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/06/19 11:52:57
 * @version $Revision: 1.3 $ 
 * @brief 
 *  
 **/


#ifndef  __BITSET_H_
#define  __BITSET_H_
#define BITSET_SUFFIX_FILE ".bitset"

class ts_bitset_t
{
private:
	uint32 _num ;
	char *_array;
public:
	int init(uint32 data_maxnum,const char* dir ,const char *file)
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

	void set(uint32 id,bool value)
	{
		int pos = id % 8;
		int off = id / 8;
		if (value)
			*(_array+off) = (*(_array + off)) | ( ((int)value)<<(pos)  ) ;
		else
			*(_array+off) = (*(_array + off)) & ( ~(((int)1)<<(pos))  ) ;
	}
	bool get(uint32 id)
	{
		int pos = id % 8;
		int off = id / 8;
		return ((*(_array + off))>>pos) & 1;
	}
	int reset()
	{
		return 0;
	}

};
















#endif  //__BITSET_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
