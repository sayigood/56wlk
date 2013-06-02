#include "adcodeMap.h"

#include <iconv.h>
#include <string.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>

int adcodeMap::loadMapLoc(){
	string::size_type pos = 0;
	string pattern = "\t";
	string line, word_1, word_2;
	map<string, string>::iterator iter;
	ifstream ifile;
	ifile.open(this->fileLoc.c_str());
	if( !ifile ) {
		return -1;
	}
	while(getline(ifile, line)) {
		pos = line.find(pattern);
		if (pos != -1) {
			word_1 = line.substr(0,pos);
			word_2 = line.substr(pos+1,line.length());
			iter = mapLoc.find(word_1);
			if(iter != mapLoc.end()) {
				word_2 = word_2+","+iter->second;
				iter->second = word_2;
			} else {
				this->mapLoc.insert(pair<string, string>(word_1, word_2));
			}	
		}
	}
	ifile.close();
	return 0;
}

int adcodeMap::loadAdcode(){
	string::size_type pos = 0;
	string pattern = "\t";
	string line, word_1, word_2;
	map<string, string>::iterator iter;
	
	ifstream ifile;
	ifile.open(this->fileAdcode.c_str());
	if( !ifile ) {
		return -1;
	}

	while(getline(ifile, line)) {
		pos = line.find(pattern);
		if (pos != -1) {
			word_1 = line.substr(0,pos);
			word_2 = line.substr(pos+1,line.length());
			iter = mapAdcode.find(word_2);
                        if(iter != mapAdcode.end()) {
                                word_1 = word_1+","+iter->second;
				iter->second = word_1;
                        } else {
				this->mapAdcode.insert(pair<string, string>(word_2, word_1));
			}
		}
	}
	ifile.close();
	return 0;
}

int adcodeMap::init(string path, string conf) {
	Ul_confdata *pconf;
    	pconf = ul_initconf(1000);
        int ret;
	ret = ul_readconf(path.c_str(), conf.c_str(), pconf);
	if (ret < 0) {
		FATAL_LOG_NLP("load config file error!");
                return -1;
	}
	char buf[100];
        string adcode_map, loc_map;
	ul_getconfstr(pconf, "adcode_map_path", buf);
	adcode_map = buf;
	ul_getconfstr(pconf, "loc_map_path", buf);
	loc_map = buf;
	ul_freeconf(pconf);
	this->fileLoc = loc_map;
	this->fileAdcode = adcode_map;
	ret = loadMapLoc();
	if (ret < 0) {
		FATAL_LOG_NLP("load loc_map file error!");
                return -1;
	}

	ret = loadAdcode();
	if (ret < 0) {
		FATAL_LOG_NLP("load adcode_map file error!");
                return -1;
	}

	return 0;
}

vector<string> adcodeMap::findAdcode(string loc){
	vector<string> codes; 
	if (this->mapAdcode.empty() || this->mapLoc.empty()) {
		return codes;
	}

	map<string, string>::iterator iter;

	// directly using adcode-map	
	string::size_type pos = 0;
	string pattern = ",";
	string line, word_1, word_2;
	iter = mapAdcode.find(loc);
	if(iter != this->mapAdcode.end()) {
		line = iter->second;
		pos = line.find(pattern);
		while (pos != -1) {
			word_1 = line.substr(0,pos);
			word_2 = line.substr(pos+1,line.length());
			codes.push_back(word_1);
			line = word_2;
			pos = line.find(pattern);
		}
		codes.push_back(line);
	}

	// using loc-map to relocate the element
	vector<string> locs;
	iter = this->mapLoc.find(loc);
	if(iter != this->mapLoc.end()) {
		line  = iter->second;
		pos = line.find(pattern);
		while (pos != -1) {
			word_1 = line.substr(0,pos);
			word_2 = line.substr(pos+1,line.length());
			locs.push_back(word_1);
			line = word_2;
			pos = line.find(pattern);
		}
		locs.push_back(line);
	}
	if(!locs.empty()) {
		for(int i = 0; i < locs.size(); i++) {
			loc = locs[i];
			iter = mapAdcode.find(loc);
			if(iter != this->mapAdcode.end()) {
				line = iter->second;
				pos = line.find(pattern);
				while (pos != -1) {
					word_1 = line.substr(0,pos);
					word_2 = line.substr(pos+1,line.length());
					codes.push_back(word_1);
					line = word_2;
					pos = line.find(pattern);
				}
				codes.push_back(line);
			}
		}
	}

	sort(codes.begin(), codes.end());
	vector<string>:: iterator viter = unique(codes.begin(), codes.end());
	codes.erase(viter, codes.end());

	return codes;
}

string adcodeMap::filterWord(string word) {
	int word_len = 3;
	if( word.length() <= 2*word_len ) {
		return word;
	}

	vector<string> postfix;
	postfix.push_back("省");
	postfix.push_back("市");
	postfix.push_back("区");
	postfix.push_back("县");
	int len = word.length();
	string element = word.substr(len-word_len,len);
	for(int i = 0; i < postfix.size(); i++) {
		if (element == postfix[i]) {
			word = word.substr(0,len-word_len);
			break;
		}
	}
	return word;
}

int adcodeMap::process(vector<string> segWords, vector<string>& adcodes) {
	vector<string> codes;
	string word, gbkword;
	string pattern = ",";
//	string::size_type pos = 0;
	for (int i = 0; i < segWords.size(); i++)
	{
		word = segWords[i];
//		gbkword = UTF8ToGBK(word);
//		if ( gbkword.empty() ) {
//			continue;
//		} else {
//			if (!isChinese(gbkword)) {
//				continue;
//			}
//		}	
//		word = filterWord(word);
		codes = findAdcode(word);
		if ( !codes.empty() ) {
			for(int j = 0; j < codes.size(); j++) {
				adcodes.push_back(codes[j]);
			}
		}

	}

	bool find = false;
	string postfix = "0100";
	string str = "", pstr = "";
	int prov_adcode_len = 2;
	int city_adcode_len = 4;
	int normal_adcode_len = 6;
	vector<string> pcodes, padcodes;
	vector<string>::iterator viter, piter;
	for(viter = adcodes.begin(); viter != adcodes.end(); ) {
		str = (*viter);
		if( str.length() < normal_adcode_len ) {
			pcodes.push_back(str);
			viter = adcodes.erase(viter);
		} else {
			viter = viter+1;
		}
	}

	if(adcodes.size() == 0) {
		for(piter = pcodes.begin(); piter != pcodes.end(); piter++ ) {
			pstr = (*piter);
			pstr = pstr.substr(0,prov_adcode_len)+postfix;
			adcodes.push_back(pstr);
		}
		return 1;
	} else {
		for(piter = pcodes.begin(); piter != pcodes.end(); piter++ ) {
			pstr = (*piter);
			find = false;
			for(viter = adcodes.begin(); viter != adcodes.end(); viter++ ) {
				str = (*viter);
				if(str.substr(0,prov_adcode_len) == pstr.substr(0,prov_adcode_len)){
					padcodes.push_back(str);
					find = true;
				}
			}
			if(!find) {
				pstr = pstr.substr(0,2)+"0100";
				adcodes.push_back(pstr);
			}
		}
	}
	if(padcodes.size() > 0) {
		for(viter = padcodes.begin(); viter != padcodes.end(); viter++ ) {
			adcodes.push_back(*viter);
		}
	}
		
	int max_num = 1;
	map<string,int> elenumMap;
	map<string, int>::iterator miter;
	for(viter = adcodes.begin(); viter != adcodes.end(); viter++) {
		str = (*viter);
		str = str.substr(0,city_adcode_len);
		miter = elenumMap.find(str);
                if(miter == elenumMap.end()) {
			elenumMap.insert(pair<string, int>(str, 1));
		} else {
			miter->second += 1;
			if (miter->second > max_num) {
				max_num = miter->second;
			}	
		}	
	}
	if (max_num > 1) {
		for(viter = adcodes.begin(); viter != adcodes.end(); ) {
                	str = (*viter);
                	str = str.substr(0,city_adcode_len);
                	miter = elenumMap.find(str);
                	if(miter != elenumMap.end() && miter->second != max_num) {
				viter = adcodes.erase(viter);
                	} else { 
                        	viter++;
			}
                }
		sort(adcodes.begin(), adcodes.end());
		viter = unique(adcodes.begin(), adcodes.end());
		adcodes.erase(viter, adcodes.end());
	}
    int max_adcode_num = 3;
    while(adcodes.size() > max_adcode_num) {
        adcodes.pop_back();
    }
	return 1;
}

//string adcodeMap::UTF8ToGBK(string utf8)
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
//int adcodeMap::isChinese(string str) {
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
////        	} else {
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
