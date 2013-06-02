#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "af_common.h"

using namespace std;

class adcodeMap
{
public:
	 int loadMapLoc();
	 int loadAdcode();
	 vector<string> findAdcode(string loc);
	 string filterWord(string word);
	 //string UTF8ToGBK(string word);
	 //int isChinese(string word);
	 int init(string path, string file);
	 int process(vector<string> segWords, vector<string>& adcodes);
private:
	 string fileLoc;
	 string fileAdcode;
	 map<string, string> mapLoc;
	 map<string, string> mapAdcode;
};
