/**
 *  =============================================================
 *
 *  Copyright (c) 2011-2012 Panguso.com. All rights reserved.
 *
 *      FileName:  test_main.cpp
 *   Description:  Fuck the world
 *       Created:  2012年07月11日 16:44:45
 *       Version:  
 *      Revision:  #1;#4
 *        AUTHOR:  liuzhangxun(liuzhangxun@panguso.com)
 *
 *  =============================================================
**/
#include "get_type.h"
#include <iostream>
using namespace std;

int main()
{
	GetType my_gt;
	int ret;
    const char* path = "./conf";
    const char*  file = "mappb.conf";
	ret=my_gt.init(path, file);
	if(ret!=0)
	{
		FATAL_LOG_NLP("init dup_dict && type_dict failed");
		return -1;
	}
		
	ifstream fin;
	fin.open("./data/type_input");
	if( !fin) 
	{
		FATAL_LOG_NLP("cannot open file input");
		return -1; 
	}

	vector<string> v_seg_word;
	vector<string> v_type_code;
	string line,word,type_code;
	while(getline(fin,line))
	{
		v_seg_word.clear();
		v_type_code.clear();

		istringstream stream(line);
		while(stream >> word)
		{
			v_seg_word.push_back(word);
		}

		my_gt.process(v_seg_word, v_type_code, type_code);

		//以下用于输出结果，查看
		vector<string>::iterator iter;
		for(iter = v_type_code.begin(); iter != v_type_code.end()-1 && iter != v_type_code.end(); iter++)
		{
			cout<<*iter<<"\t";
		}
        if(iter != v_type_code.end())
        {
		    cout<<*iter<<"\t\t\t"<<type_code<<endl;
        }
        else
            cout << endl;
	}
	return 0;
}
