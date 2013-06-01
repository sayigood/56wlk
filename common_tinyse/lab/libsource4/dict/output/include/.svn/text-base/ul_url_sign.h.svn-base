#ifndef _UL_URL_SIGN_H_
#define _UL_URL_SIGN_H_


/**
 * @brief calc 64bit site sign & 64bit url sign for a url string
 *
 * @param [in] urlstr   : const char*  null terminated url string
 * @param [out] site_sign   : unsigned long long& 
 * @param [out] url_sign   : unsigned long long&
 * @return  int  0 on sucess -1 on failure
 **/
//对urlstr进行归一化,然后使用murmurhash来计算site和url的签名.
//这里site_sign[0]和url_sign[0]为高32位,site_sign[1]和url_sign[1]为低32位.
int create_url_sign(const char *urlstr,unsigned int site_sign[2],unsigned int url_sign[2]);
int create_url_sign(const char* urlstr, unsigned long long& site_sign, unsigned long long& url_sign);

#endif
