#include "extendMap.h"

#include <iconv.h>
#include <string.h>
#include <stdlib.h>

#include <vector>

int extendMap::init(string path, string file) {
	Ul_confdata *pconf;
	pconf = ul_initconf(1024);
	int ret = 0;
	ret = ul_readconf(path.c_str(), file.c_str(), pconf);
	if (ret < 0) {
		FATAL_LOG_NLP("load config file error!");
		return -1;
	}
	char buf[100];
	ul_getconfstr(pconf, "extend_dict_path", buf);
	fileExt = buf;	
	ret = loadExtMap();	
	if (ret < 0) {		
		FATAL_LOG_NLP("load ext_map file error!");
		return -1;
	}
	return 0;
}

int extendMap::loadExtMap(){
	string::size_type pos = 0;
	string pattern = "\t";
	string delimiter = ",";
	string line, word_1, word_2;
	map<string, string>::iterator iter;
	ifstream ifile;
	ifile.open(this->fileExt.c_str());
	if(!ifile) {
		return -1;
	}
	while(getline(ifile, line)) {
		pos = line.find(pattern);
		if (pos != -1) {
			word_1 = line.substr(0,pos);
			word_2 = line.substr(pos+1,line.length());
			iter = mapExt.find(word_1);
			if(iter != mapExt.end()) {
				word_2 = word_2+delimiter+iter->second;
				iter->second = word_2;
			} else {
				this->mapExt.insert(pair<string, string>(word_1, word_2));
			}	
		}
	}
	ifile.close();
	return 0;
}

string extendMap::findExtend(string word){
	if (this->mapExt.empty()) {
		return NULL;
	}
	// directly find the element
	map<string, string>::iterator iter;
	iter = mapExt.find(word);
	if(iter != this->mapExt.end()) {
		return iter->second;
	} else {
		return "";
	}
}

int extendMap::process(vector<string> &segWords) {
	if (mapExt.empty() || segWords.empty()) {
		return 0;
	}
	string word, wordExtend;
	vector<string> extends;
	string delimiter = ",";
	string::size_type pos = 0;
	for (int i = 0; i < segWords.size(); i++)
	{
		word = segWords[i];
		wordExtend = findExtend(word);
		if (wordExtend.size() > 0) 
		{
			pos = wordExtend.find(delimiter);
			while (pos != -1) {
				extends.push_back(wordExtend.substr(0,pos));
                        	wordExtend = wordExtend.substr(pos+1,wordExtend.length());
				pos = wordExtend.find(delimiter);
			} 
			extends.push_back(wordExtend);
		}
	}
	for (int i = 0; i < extends.size(); i++) {
		segWords.push_back(extends[i]);
	}
	return 1;
}

//string extendMap::UTF8ToGBK(string utf8)
//{
//    string ans;
//    char *src = new char;
//    strcpy(src, utf8.c_str());
//    int len = strlen(src)*2+1;
//    char *dst = (char *)malloc(len);
//    if(dst == NULL)
//    {
//        return ans;
//    }
//    memset(dst, 0, len);
//    char *in = src;
//    char *out = dst;
//    size_t len_in = strlen(src);
//    size_t len_out = len;
//     
//    iconv_t cd = iconv_open("GBK", "UTF-8");
//    if ((iconv_t)-1 == cd)
//    {
//        free(dst);
//        return ans;
//    }
//    int n = iconv(cd, &in, &len_in, &out, &len_out);
//    if( n < 0 )
//    {
//        return ans;
//    }
//    else
//    {
//        ans = dst;
//    }
//      
//    free(dst);
//    iconv_close(cd);
//    return ans;
//}
//
//int extendMap::isChinese(string str) {
//	if (str.empty()) {
//		return 0;
//	}
//	int len = str.size();
//        short high = 0, low = 0;
//        unsigned int code = 0;
//        for(int i=0; i<len; i++)
//        {
//        	if(str[i]>=0 || i==len-1)
//        	{
//			return 0;
//        	} else {
//			high = (short)(str[i]+256);
//			low = (short)(str[i+1]+256);
//			code = high*256 + low;
//			if (code>=0xB0A1 && code<=0xF7FE || code>=0x8140 && code<=0xA0FE || code>=0xAA40 && code<=0xFEA0) {
//				i++;
//			} else {
//				return 0;
//			}
//		}
//	}
//	return 1;	
//}
