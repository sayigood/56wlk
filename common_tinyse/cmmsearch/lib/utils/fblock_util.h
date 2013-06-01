/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: fblock_util.h,v 1.1 2008/08/22 04:02:42 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file fblock_util.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/07/30 17:32:56
 * @version $Revision: 1.1 $ 
 * @brief 实现高速顺序io
 *  
 **/


#ifndef  __FBLOCK_UTIL_H_
#define  __FBLOCK_UTIL_H_

#include "fileblock.h"
#include "mfiles.h"

#include "tinyse.h"
#define DEFAULT_BUF_SIZE (1024*1024*10)
class fblock_writer
{
  private:
	fblock_t * m_lib;
	char *m_buf;
	int m_bufsize;
	int m_bufused;
  public:
	int init (fblock_t * hd, int buffer_size = DEFAULT_BUF_SIZE);
	int write (const void *item, int item_size);
	int fini ();
};

class fblock_reader
{
  private:
	fblock_t * m_lib;
	bool m_done;

	char *m_buf;
	int m_bufsize;
	int m_readnum;
	int m_curblk;
	int m_curpos;

  public:
	int init (fblock_t * hd, int buffer_size = DEFAULT_BUF_SIZE);	//initial
	int get_item_and_next (void *item, int item_size);
	bool is_done ();
	int fini ();

};

class mfiles_writer
{
  private:
	mfiles_t * m_lib;
	char *m_buf;
	int m_bufsize;
	int m_bufused;
  public:
	int init (mfiles_t * hd, int buffer_size = DEFAULT_BUF_SIZE);
	int write (const void *item, int item_size);
	int fini ();
};


class mfiles_reader
{

  private:
	mfiles_t * m_lib;
	bool m_done;

	char *m_buf;
	int m_bufsize;
	int m_readsize;
	uint64 m_offset;
	int m_curpos;

  public:
	int init (mfiles_t * hd, int buffer_size = DEFAULT_BUF_SIZE);	//initial
	int get_item_and_next (void *item, int item_size);
	bool is_done ();
	int fini ();
};


















#endif  //__FBLOCK_UTIL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
