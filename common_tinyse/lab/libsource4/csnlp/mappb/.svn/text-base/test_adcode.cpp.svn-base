#include "adcodeMap.h"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	adcodeMap admap;
	const char* path = "./conf";
    	const char*  file = "mappb.conf";  
	admap.init(path, file);
	//admap.loadMapLoc();
	//admap.loadAdcode();

	vector<string> find;
	vector<string> res;
	string::size_type pos = 0;
    string line, pattern = " ";
    ifstream ifile("./data/adcode_input");
    while(getline(ifile, line)) {
		find.clear();
		res.clear();
        pos = line.find(pattern);
		while (pos != -1) {
                	find.push_back(line.substr(0,pos));
                    line = line.substr(pos+1,line.length());
                    pos = line.find(pattern);
        }
        find.push_back(line);
		if ( admap.process(find, res) > 0 ) {
			for(int i = 0; i < res.size(); i++)
			{
				cout << res[i] << "\t";
			}
			cout << "\n";
		}
	}
	ifile.close();
	
	return 0;
}


