/** $Id: stopword.cpp,v 1.2 2008/08/22 10:55:53 yanghy2 Exp $ */

#include "stopword.h"

#include "utils.h"

#include "ul_sign.h"
#include "ul_log.h"

void trim_string( char *pstr )
{
    char *ptmp = pstr;
    while( (*ptmp==' ' || *ptmp=='\t' || *ptmp=='\r' || *ptmp=='\n' ||
           *ptmp=='\v' || *ptmp=='\b') && '\0'!=*ptmp )
        ptmp++;
    memmove( pstr, ptmp, strlen(ptmp)+1 );
    if( strlen(pstr)<=0 )
        return;
    char *pend = pstr+strlen(pstr)-1;
    while( (*pend==' ' || *pend=='\t' || *pend=='\r' || *pend=='\n' ||
           *pend=='\v' || *pend=='\b') && '\0'!=*pend )
        *pend--=0;
}
//////////////////////////////

const int SMEM_BASE = 1024*128 + 1;
inline unsigned int _shash_fun(const sign64 & key)
{
    return (key.sign1 + key.sign2) % SMEM_BASE;
}

inline int _shash_cmp_fun(const sign64 & k1, const sign64 & k2)
{
    return ((k1.sign1 == k2.sign1) && (k1.sign2 == k2.sign2)) ? 0 : 1;
}
/** 指定文件是否修改  */
bool file_ismodify( const char* strfile, time_t cur_mtime, time_t* pret_mtime )
{    
	struct stat status;
	memset( &status, 0, sizeof(status) );
	if( stat( strfile, &status )!=0 ) {
		return false;
	}
	if( status.st_mtime == cur_mtime ) {
		return false; 
	}
	*pret_mtime = status.st_mtime;
	return true;
} 


int stop_word_t::init(const char* filename)
{
	dict_ = new hash_map < sign64, int>();
	if(dict_ == NULL) {
		exit(1);
	}

    int opret = dict_->create(SMEM_BASE, _shash_fun,_shash_cmp_fun);
	if (opret<0)
		exit(1);
	
	if(strlen(filename) >= sizeof(filename_)) {
		exit(1);
	}
	strncpy(filename_, filename, sizeof(filename_));

	time_ = 0;
	if( !file_ismodify( filename, 0, &time_) ) {//file suppose to exist, and we get file modify time here
		exit(1);
	}

	if(__load(dict_)) {
		exit(1);
	}
	return 0;
}


int stop_word_t::__load(hash_map < sign64, int> * pdict)
{
	FILE* pfile = fopen( filename_, "r" );
	if(pfile == NULL) {
		return -1;
	}
	
	char linebuf[256];
	int file_line = 0;
	while( fgets(linebuf, sizeof(linebuf), pfile ) ) {
		file_line++;

		trim_string( linebuf );
		if( strlen(linebuf)==0 ) {
			continue;
		}
		sign64 s;
		creat_sign_fs64 (linebuf, strlen(linebuf), &s.sign1, &s.sign2);
		
		int val = 1;
		int opret = pdict->set( s, val,1 );
		if (0 != opret)
		{
			return -1;
	//		continue;
		}
		ul_writelog(UL_LOG_TRACE,"stopword : %s",linebuf);

	}
	fclose(pfile);

	return 0;
}

int stop_word_t::reload(int sleep_secs)
{
	time_t new_mtime;
	if( !file_ismodify( filename_, time_ , &new_mtime) ) {
		return 0;
	}

	hash_map < sign64, int> *pdict = new hash_map < sign64, int>();
	if(pdict == NULL) {
		return -1;
	}

    int opret = pdict->create(SMEM_BASE, _shash_fun,_shash_cmp_fun);
	if (opret<0)
		return -1;

	if(__load(pdict)) {
		delete pdict;
		return -1;
	}

	time_ = new_mtime;

	hash_map < sign64, int> *pold = dict_;
	dict_ = pdict;
	sleep(sleep_secs);
	if(pold) {
		delete  pold;
	}
	return 0;
}

int stop_word_t::find(u_int sign1, u_int sign2)
{

	sign64 s = {sign1,sign2};
	int val = 0;
	int ret = dict_->get(s,&val);
	if (0 != ret)
	{
		return -1;
	}
	return 0;
}
	
int stop_word_t::find(const char* word)
{
	u_int sign1=0, sign2=0;
	creat_sign_fs64 (const_cast<char*>(word), strlen(word), &sign1, &sign2);
	return find(sign1, sign2);
}
