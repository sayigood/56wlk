#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "af_common.h"

using namespace std;

class extendMap
{
public:
	 int init(string path, string file);
	 int process(vector<string> &segWords);
	 //extendMap(string map);
	 int loadExtMap();
	 string findExtend(string word);
	 //string filterWord(string word);
	 //string UTF8ToGBK(string word);
	 //int isChinese(string word);
	 //int getExtend(vector<string> &segWords);
private:
	 string fileExt;
	 map<string, string> mapExt;
};
