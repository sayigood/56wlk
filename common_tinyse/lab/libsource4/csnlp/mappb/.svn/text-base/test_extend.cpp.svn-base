#include "extendMap.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	extendMap etmap;
	const char *path = "./conf";
	const char *file = "mappb.conf";	
	etmap.init(path, file);

	std::vector<string> find;
	string::size_type pos = 0;
        string line, pattern = "\t";
	std::ifstream ifile("./data/extend_input");
        while(getline(ifile, line)) {
		find.clear();
                pos = line.find(pattern);
		while (pos != -1) {
                	find.push_back(line.substr(0,pos));
                        line = line.substr(pos+1,line.length());
                        pos = line.find(pattern);
                }
                find.push_back(line);
		if ( etmap.process(find) > 0 ) {
			for(int i = 0; i < find.size(); i++)
			{
				cout << find[i] << "\t";
			}
			cout << "\n";
		}
	}
	ifile.close();
	
	return 0;
}


