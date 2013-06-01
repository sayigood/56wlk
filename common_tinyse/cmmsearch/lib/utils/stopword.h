/** $Id: stopword.h,v 1.1 2008/08/22 04:02:42 yanghy2 Exp $ */
#ifndef __STOP_WORD__
#define __STOP_WORD__

#include "unistd.h"
#include "hashmap.h"
using namespace ansx;
typedef struct
{
	unsigned int sign1;
	unsigned int sign2;
} sign64;
class stop_word_t
{
public:
	int init( const char* filename);
	int reload(int sleep_secs);

	//query, return 0 if found, otherwise return -1 
	int find(u_int sign1, u_int sign2);
	int find(const char* word);

private:
	int __load(hash_map<sign64,int> *dict_);
	hash_map < sign64, int> *dict_;
	size_t size_;

	time_t time_;
	char   filename_[256];
};

#endif
