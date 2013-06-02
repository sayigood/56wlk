/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: pub.h,v 1.2 2008/08/25 07:18:08 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file pub.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/07/03 14:07:41
 * @version $Revision: 1.2 $ 
 * @brief 
 *  
 **/


#ifndef  __PUB_H_
#define  __PUB_H_



#include "myopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>



using namespace std;

#define MAX_TERM_COUNT 64
#define MAX_TERM_LEN   64
#define  MAX_TEXT_LENGTH 128
#define FILEPATH_MAXLEN 128

#define DEFAULT_AUTHORITY 100
#define DEFAULT_IDF  7



typedef struct options_data_t
{
	char cmd[16];
	char query[10240];
	char ip[32];
	int port;
	char data_type[64];
	int page_no;
} options_data_t;
extern options_data_t opt_data;





#endif  //__PUB_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
