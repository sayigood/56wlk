/**
 *  =============================================================
 *
 *  Copyright (c) 2011-2012 Panguso.com. All rights reserved.
 *
 *      FileName:  addr.cpp
 *   Description:  Fuck the world
 *       Created:  2012年07月12日 13:11:09
 *       Version:  
 *      Revision:  #1;#4
 *        AUTHOR:  liuzhangxun(liuzhangxun@panguso.com)
 *
 *  =============================================================
**/
#ifndef _GETADDR_H 
#define _GETADDR_H
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "af_common.h"
#include "trie.h"

using namespace std;

class GetAddr{
	public:
		int init(const char*, const char*);
        trie_tree* getTree(){
            return tree;
        };
        void to_lower(const char*, char*);
        int process(vector<string>, const char *, vector<string> &);
	private:
		trie_tree *tree;
        vector<string> filter_dict;
};

#endif
