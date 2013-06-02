/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: talk.cpp,v 1.1 2008/08/22 04:03:35 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file talk.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/07/08 11:34:59
 * @version $Revision: 1.1 $ 
 * @brief与as交互
 *  
 **/


#include "nshead.h"
#include "mynet.h"
#include "pub.h"
#include "talk.h"




int
talkwith (int &fd, nshead_t * req, nshead_t * res, int siz)
{
	int timeout = 1000000;
	int ret = 0;

	if (fd < 0)
	{
		fd = MyConnectO (opt_data.ip, opt_data.port, &timeout, "tinyse-qtool");
		if (fd <= 0)
		{
			ul_writelog (UL_LOG_WARNING, "connect err[%m] ret[%d]", fd);
			return -1;
		}
		int on = 1;

		setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof (on));
		struct linger li;

		memset (&li, 0, sizeof (li));
		li.l_onoff = 1;
		li.l_linger = 0;
		setsockopt (fd, SOL_SOCKET, SO_LINGER, (const char *) &li,
					sizeof (li));
	}

	req->log_id = rand ();
	ret = nshead_write (fd, req, 1000);

	if (ret != 0)
	{
		ul_writelog (UL_LOG_WARNING, "write err[%m], ret[%d]", ret);
		goto fail;
	}
	ret = nshead_read (fd, res, siz, 10000);
	if (ret != 0)
	{
		ul_writelog (UL_LOG_WARNING, "read err[%m], ret[%d]", ret);
		goto fail;
	}
	{//短连接
		close (fd);
		fd = -1;
	}

	return 0;
  fail:
	if (fd > 0)
	{
		close (fd);
	}
	fd = -1;
	return -1;
}












/* vim: set ts=4 sw=4 sts=4 tw=100 */
