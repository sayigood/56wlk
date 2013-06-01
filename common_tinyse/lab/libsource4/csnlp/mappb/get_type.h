/**
 *  =============================================================
 *
 *  Copyright (c) 2011-2012 Panguso.com. All rights reserved.
 *
 *      FileName:  get_type.h
 *   Description:  Fuck the world
 *       Created:  2012年07月11日 16:44:36
 *       Version:  
 *      Revision:  #1;#4
 *        AUTHOR:  liuzhangxun(liuzhangxun@panguso.com)
 *
 *  =============================================================
**/
#ifndef _GETTYPE_H
#define _GETTYPE_H

#include <string.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include "ul_sign.h"
#include "af_common.h"
#include "af_sign.h"
#include "ul_thr.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

const int MAX_DICT_NUM = 2048;
const int MAX_QUERY_LEN= 1024;
using namespace std;

class GetType{
	public:
		int init(const char*, const char*);
		int process(vector<string>, vector<string>&, string&);
        inline string add_zero(string);
		
        sodict_build_t * dup_dict;
	private:
};

#endif
