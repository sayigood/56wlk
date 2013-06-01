#include <fstream>

#include "get_addr.h"

int main()
{
	GetAddr my_ga;
    const char* path = "./conf";
    const char*  file = "mappb.conf"; 
	int ret = my_ga.init(path, file);
    if(ret<0)
    {
        FATAL_LOG_NLP("init failed");
        return -1;
    }

    trie_node *node;
    //foreach_trie_tree(my_ga.getTree());
   
    ifstream fin;
    fin.open("./data/addr_input");
    if( !fin) 
    {
        FATAL_LOG_NLP("cannot open file addr_input");
        return -1; 
    }
    vector<string> v_seg_word;
    string line,word;

    while(getline(fin,line))
    {
        istringstream stream(line);
        while(stream >> word)
        {
            v_seg_word.push_back(word);
        }

        char query[1024];
        if(getline(fin,line))
            snprintf(query,sizeof(query),"%s",line.c_str());
        else
            break;

        vector<string> v_addr;
    
        my_ga.process(v_seg_word, query, v_addr);

        vector<string>::iterator iter;
        for(iter = v_addr.begin(); iter != v_addr.end(); iter++)
        {
            cout<<*iter<<"\t";
        }
        cout<<endl;
        v_seg_word.clear();
    }
    return 1;
}
