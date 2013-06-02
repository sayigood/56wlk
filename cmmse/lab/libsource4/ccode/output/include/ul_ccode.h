#ifndef __CCODE_H__
#define __CCODE_H__
#include "ul_iconv.h"

#define	TRBUF_SIZE	512

#define BIG5_NUM	0x3695	//13973

#define BIG5_1LO	0xa1	//161
#define	BIG5_1HI	0xf9	//249
#define BIG5_1NUM	0x59	//89
#define	BIG5_2LO1	0x40	//64
#define	BIG5_2HI1	0x7e	//126
#define	BIG5_2NUM1	0x3f	//63
#define	BIG5_2LO2	0xa1	//161
#define	BIG5_2HI2	0xfe	//254
#define	BIG5_2NUM2	0x5e	//94
#define	BIG5_2NUM	0x9d	//157

#define	GBK_NUM		0x5e02	//24066

#define GBK_1LO		0x81	//129
#define GBK_1HI		0xfe	//254
#define	GBK_1NUM	0x7e	//126
#define	GBK_2LO		0x40	//64
#define	GBK_2HI		0xfe	//254
#define	GBK_2NUM	0xbf	//191
#define GBK_2INV	0x7f	//127

#define CODETYPE_NONE -1
//mv to langtype
#define CODETYPE_ENG  0
//means gb18030 now
#define CODETYPE_GB   1
#define CODETYPE_BIG5 2
#define CODETYPE_UTF8 3
//rm
#define CODETYPE_JPKR 4

// added by jcode
#define CODETYPE_JIS  5
#define CODETYPE_SJIS 6
#define CODETYPE_EUCJP 7
//rm
#define CODETYPE_UTF8JP 8

//new
#define CODETYPE_ASCII 9
#define CODETYPE_ISO_8859_1 10
#define CODETYPE_EUCKR 11

#define CODETYPE_UCS2 12

//language type
#define LANGTYPE_NONE (-1)
#define LANGTYPE_ENGLISH 0
#define LANGTYPE_SIMP_CHINESE 1
#define LANGTYPE_TRAD_CHINESE 2
#define LANGTYPE_JAPANESE 3

//ÐÂ¾ÉccodeµÄ±êÊ¶£¬µ±LANGTYPE±»ÖÃÎª´ËÖµÊ±ËµÃ÷ÊÇ¾ÉccodeµÄÊ¶±ð½á¹û
#define LANGTYPE_NULL (-3)

#define PY_MAXPAGESIZE 512000
#ifndef u_char
#define u_char		unsigned char
#endif

//============== for ccode_check ccode_check_content exit type =================
//modified to 19 from 23, to avoid redefinition with jcode
#define	MAX_CHECK_EXIT		19
#define	CCODE_CHECK_EXIT_1	1
#define	CCODE_CHECK_EXIT_2	2
#define	CCODE_CHECK_EXIT_3	3
#define	CCODE_CHECK_EXIT_4	4
#define	CCODE_CHECK_EXIT_5	5
#define	CCODE_CHECK_EXIT_6	6
#define	CCODE_CHECK_EXIT_7	7
#define	CCODE_CHECK_EXIT_8	8
#define	CCODE_CHECK_EXIT_9	9
#define	CCODE_CHECK_EXIT_10	10
#define	CCODE_CHECK_EXIT_11	11
#define	CCODE_CHECK_EXIT_12	12
#define	CCODE_CHECK_EXIT_13	13
#define	CCODE_CHECK_EXIT_14	14
#define	CCODE_CHECK_EXIT_15	15
#define	CCODE_CHECK_EXIT_16	16
#define	CCODE_CHECK_EXIT_17	17
#define	CCODE_CHECK_EXIT_18	18
#define	CCODE_CHECK_EXIT_19	19

#define CCODE_CHECK_EXIT_20	20
#define CCODE_CHECK_EXIT_21 21
#define CCODE_CHECK_EXIT_22 22
#define CCODE_CHECK_EXIT_23 23


#define	CCODE_CONT_URLNONE		101
#define	CCODE_CONT_SHORT		102
#define	CCODE_CONT_ENG2GB		103
#define	CCODE_CONT_GBBIG52ENG	104
#define	CCODE_CONT_UTF82NONE	105
#define	CCODE_CONT_NONE2ENG		106
#define	CCODE_CONT_REMAIN		107
#define	CCODE_CONT_UTF82ENG		108

#define CCODE_CONT_NONE2GB		109

//definition for codetype/langtype detection
#define CCODE_BUFFER_SIZE 1024
#define CCODE_MAX_CHARSET_LEN 128
#define CCODE_VALUE_NOT_COMPUTED (-1)
#define CCODE_MAX_SAMPLE_LEN 512
#define CCODE_MAX_DETECT_TRUST 1023
#define CCODE_MIN_DETECT_TRUST 0

//#define CCODE_DETECT_DEBUG

//not used
#define CCODE_BYTE_FREQ_ARRAY_SIZE 94
#define CCODE_BYTE_FREQ_OFFSET (0xa1)
#define CCODE_BYTE_FREQ_END (0xfe)









/** 
 * @brief ±àÂëÊ¶±ð½Ó¿Ú
 * 
 * @param page Ò³Ãæ×Ö·û´®£¬²»¿ÉÎªNULL
 * @param url Ò³Ãæurl£¬²»¿ÉÎªNULL
 * @param charset Ò³Ãæcharset×Ö·û´®£¬ÈçÎªNULLÔò½«ÔÚpageÖÐ²éÕÒ
 * @param content Ò³ÃæÕýÎÄ£¬¿ÉÎªNULL
 * @param[out] codetype ±àÂëÀàÐÍ
 * 
 * @return Ê¶±ð³É¹¦·µ»Ø1£¬³ö´í·µ»Ø-1
*/
int ul_ccode_detect_codetype(const char *page, const char* url, const char* charset, const char* content, int *out_type);





/** 
 * @brief ±àÂëÊ¶±ð½Ó¿Ú
 * 
 * @param page Ò³Ãæ×Ö·û´®£¬²»¿ÉÎªNULL
 * @param url Ò³Ãæurl£¬²»¿ÉÎªNULL
 * @param charset Ò³Ãæcharset×Ö·û´®£¬ÈçÎªNULLÔò½«ÔÚpageÖÐ²éÕÒ
 * @param content Ò³ÃæÕýÎÄ£¬¿ÉÎªNULL
 * @param content  codetype ÒÑ¾­±æÈÏµÄ±àÂëÀàÐÍ
 * @param[out] langtype ÓïÑÔÀàÐÍ
 * @param[out] trustarr  ÓïÑÔÖÃÐÅ¶ÈÊý×é
 * 
 * @return Ê¶±ð³É¹¦·µ»Ø1£¬³ö´í·µ»Ø-1
*/
int ul_ccode_detect_langtype(const char *page, const char* url, const char* charset, const char* content, int *codetype, int* langtype,int * trustarr=NULL);




//=========================================

/* You should specify the `path' to txt's files in which char-map tables are
 * allocated while using these functions.
 *
 * All tables are lying in path/code directory.
 */

/**
 * @brief ccode¼ÓÔØÊ±µÄ¿ØÖÆflag
 * <ul>FLAG_SPECIAL_CHAR ¿ØÖÆccode·±¼ò×ª»»µÄÐÐÎª£¬ÖÃ´ËflagÔò»á¶ÔÒ»Ð©´Ê×é½øÐÐÌØÊâ´¦Àí£¬Èç¡°²tÍû¡±²»»á±»×ªÎª¡°ÁËÍû¡±
 * <ul>FLAG_QUERY_DETECT ¿ØÖÆÊÇ·ñ¼ÓÔØ¶Ì´®±àÂëÊ¶±ðµÄ´Êµä£¬ÈçÐèÒªÓÃµ½¶Ì´®±àÂëÊ¶±ð½Ó¿Úul_ccode_query_detect£¬ÐèÒªÖÃ´ËÏî
 * <ul>FLAG_NOT_SPECIAL_CHAR ¿Õflag£¬Ö»¼ÓÔØÄ¬ÈÏ¹¦ÄÜ
 */
#define FLAG_SPECIAL_CHAR 1
#define FLAG_QUERY_DETECT 2
#define FLAG_NOT_SPECIAL_CHAR 0 

/**
 * @brief ¼ÓÔØccode dataÎÄ¼þ
 *
 * @param [in] path   : char* £dataÎÄ¼þÂ·¾¶ eg: ./code
 * @param [in] flag   : int ÊÇ·ñ½øÐÐÌØÊâ×Ö·ûµÄ×ª»»£¬ÀýÈç¡±²tÍû¡°,Ä¬ÈÏ²»½øÐÐ×ª»»¡£
 * @return  int 
 * @retval  ³É¹¦0 Ê§°Ü-1
 * @see 
**/
int ul_ccode_load(char *path , int flag = FLAG_NOT_SPECIAL_CHAR) ;

/**
 * @brief	ccode_conv½á¹¹È«¾Ö³õÊ¼»¯. 
 *	ÔÚ²»ÐèÒª¼ÓÔØccode´ÊµäÊ±£¬ÓÃÕâ¸ö½Ó¿ÚÌæ´úul_ccode_load()½øÐÐÈ«¾Ö³õÊ¼»¯¡£
 * @return  int 
 * @retval	0:³õÊ¼»¯³É¹¦£»-1:³õÊ¼»¯Ê§°Ü¡£   
 * @see 
**/
int ul_ccode_conv_init();

/*
return codetype
currently: 
it can be 
0: english
1: GBK
2: Big5
3: UTF-8 encoded chinese
-1: Other
*/

int ul_ccode_check_realtext(const char* pstr);
int ul_ccode_check(char *pstr);
int ul_ccode_check_notetype (char *pstr, int *out_type);

/*
check whether the main content of a URL is Chinese and should be indexed
Notice: MUST call ul_trans2gb before sending pTitle & pContent to this function
		old_codetype is the return of ul_ccode_check
return: new codetype
*/
int ul_ccode_check_content
	(const char* pTitle, const char* pContent, const char* pUrl, int old_codetype);
int ul_ccode_check_content_notetype
	(const char* pTitle, const char* pContent, const char* pUrl, int old_codetype, int *out_type);

int ul_ccode_del();

/**
 * @brief ccode_conv½á¹¹È«¾Ö»ØÊÕº¯Êý¡£ 
 *  ÔÚ²»ÐèÒªÓÃul_ccode_load()¼ÓÔØccode´ÊµäÊ±£¬ÓÃÕâ¸ö½Ó¿ÚÌæ´úul_ccode_del()½øÐÐÈ«¾Ö»ØÊÕ¡£
 * @return  void 
**/
void ul_ccode_conv_del();

 /*=============================================================================
  * Function:        ul_is_tradgbk_str       
  * 
  * Description:     judge whether a string is encoded in traditional gbk
  * 
  * Input:           const char* pstr, the string to be judged, it must be a
  * 				 
  * 				 zero ended string                
  * 
  * Return:          1, traditional gbk string; 0. simple gbk string 
  * 
  *============================================================================*/
int ul_is_tradgbk_str(const char* pstr);

/*
ul_trans2gb
return 0 when success
return -1 when error 
*/
int ul_trans2gb(char *input,char *output, int codetype);


 /*=============================================================================
  * Function:        ul_big5_gbk_str       
  * 
  * Description:     translate big5 encoding string into gbk encoding string
  * 
  * Input:           u_char* psrc, input big5 encoding string 
  *                  int len, the length of the psrc, len<=0, the the lenght 
  *                  will be calculated internally,  but must make sure that 
  *                  psrc ended with '\0';                
  * 
  * Output:          u_char* pDest, buffer to hold the gbk encoding string
  * 
  * Return:          <0, failed; >=0, succeed. the lengh of the input string; 
  * 
  * Others:          the buffer length of pdes must be larger or equal than the length
  * 				 of input string.  ul_ccode_load() must be called before
  * 				 this function.
  *============================================================================*/
int ul_big5_gbk_str(u_char *psrc, u_char *pdes, int len);

int ul_big5_gbi_str(u_char *psrc, u_char *pdes, int len);

 /*=============================================================================
  * Function:        ul_gbk_big5_str       
  * 
  * Description:     translate gbk encoding string into big5 encoding string
  * 
  * Input:           u_char* psrc, input big5 encoding string 
  *                  int len, the length of the psrc, len<=0, the the lenght 
  *                  will be calculated internally,  but must make sure that 
  *                  psrc ended with '\0';                
  * 
  * Output:          u_char* pDest, buffer to hold the gbk encoding string
  * 
  * Return:          <0, failed; >=0, succeed. the lengh of the input string; 
  * 
  * Others:          the buffer length of pdes must be larger or equal than the length
  * 				 of input string.  ul_ccode_load() must be called before
  * 				 this function.
  *============================================================================*/
int ul_gbk_big5_str(u_char *psrc, u_char *pdes, int len);

int ul_gbk_gbi_str(u_char *psrc, u_char *pdes, int len);
int ul_gbi_big5_str(u_char *psrc, u_char *pdes, int len);
int ul_gbi_gbk_str(u_char *psrc, u_char *pdes, int len);


// transfer quanjiao keyboard letter/number/symbol to banjiao 
// this transformation can be made in-place, i.e. szIn == szOut
void ul_trans2bj(const char *szIn, char *szOut);

// transfer some special chinese quanjiao symbols to its corresponding banjiao form
// this transformation can be made in-place, i.e. szIn == szOut
void ul_trans2bj_ext(const char *szIn, char *szOut);

// transfer from uppercase to lower case, for gbk-encoded string
// this transformation can be made in-place, i.e. szIn == szOut
void ul_trans2lower(const char *szIn, char *szOut);

/*
obsolete function, use ul_trans2bj/ul_trans2bj_ext/ul_trans2lower instead
* transfer quanjiao to banjiao and upper to lower

int ul_transfer_ch(char *input,char *output);
*/

/*=============================================================================
 * Function:		ul_gbk_to_utf8       
 *
 * Description:		translate gbk encoding string into utf8 encoding string
 *
 * Input:			const char* pSrc, input gbk encoding string 
 * 					const unsigned nDesCnt, the length of the dest buffer
 *
 * Output:			char* pDest, buffer to hold the utf8 encoding string
 *
 * Return:			<0, failed; >0, succeed. the lengh of the utf8 string; 
 *
 * Others:			nDestCnt must be larger than 3/2*strlen(pSrc);
 *
 *============================================================================*/
int ul_gbk_to_utf8(char* pDest, const unsigned nDesCnt, const char* pSrc);

/*=============================================================================
 * Function:		ul_utf8_to_gbk       
 *
 * Description:		translate utf8 encoding string into gbk encoding string
 *
 * Input:			const char* pSrc, input gbk encoding string 
 * 					const unsigned nDesCnt, the length of the dest buffer
 *
 * Output:			char* pDest, buffer to hold the gbk encoding string
 *
 * Return:			<0, failed; >0, succeed. the lengh of the gbk string; 
 *
 * Others:			nDestCnt must be larger than strlen(pSrc);
 *
 *============================================================================*/
int ul_utf8_to_gbk(char* pDest, const unsigned nDesDnt, const char* pSrc);

inline int is_big5( u_char *upstr )
{
	if ( (upstr[0]>=0xa1) && (upstr[0]<=0xf9) &&
	     ( (upstr[1]>=0x40) && (upstr[1]<=0x7e) ||
               (upstr[1]>=0xa1) && (upstr[1]<=0xfe)
	   ) ) {
		return 1;
	}
	else{
		return 0;
	}
}

inline int is_gbk( u_char *pstr )
{
	if ( (pstr[0]>=GBK_1LO) && (pstr[0]<=GBK_1HI) &&
	     (pstr[1]>=GBK_2LO) && (pstr[1]<=GBK_2HI) &&
             (pstr[1]!=GBK_2INV)
	   ) {
		return 1;
	}
	else{
		return 0;
	}
}

inline int is_euckr(u_char* pstr){
	return (pstr[0] >= 0xA1 && pstr[0] <= 0xFE && pstr[1] >= 0xA1 && pstr[1] <= 0xFE) ? 1 : 0;
}


inline int is_gb(u_char* pstr)
{
	if ( (pstr[0]>=0xB0) && (pstr[0]<=0xF7) &&
	     (pstr[1]>=0xA1) && (pstr[1]<=0xFE) ) 
	{
		return 1;
	}
	else{
		return 0;
	}
}

inline int is_big5_str(const char* pstr)
{
	char szContent[1024];
	char* pSrc = (char*) pstr;
	char* pDest = szContent;
	char* pEnd = szContent + 256;
	u_char ch;
	int count = 0, len, i, real_count=0;
	short former_word = 0;

	// Get out the double bite words
	while( (ch = *pSrc) != 0 && pSrc[1] ){
		if ( ch <= 0x7f )
			++pSrc;
		else{
			if(*(short*)pSrc != former_word)
				++real_count;
			*pDest++ = ch;
			*pDest++ = pSrc[1];
			former_word= *(short*)pSrc;
			pSrc += 2;
			++count;
		}
        if( pDest >= pEnd )
		   break;
	}
	*pDest = 0;
	len = pDest - szContent;

	if(count == 0)
		return 0;

	for(i= 0;i<count; i++){
		if(!is_big5((u_char*)&szContent[i]))
			return 0;
		if(!is_gb((u_char*)&szContent[i]))
			return 1;
	}

	// can not judge from code range, make a choice of bigger probability
	return 0;
}

inline int is_gbk_str(const char* pSrc);



/**
 * @brief ³õÊ¼»¯ÖÐÈÕÎÄ±àÂë×ª»»ËùÐèµÄ½á¹¹ccode_conv_t
 * <p>¸Ã½á¹¹²»ÊÇÏß³Ì°²È«µÄ£¬Ã¿¸ö×ÓÏß³ÌÐèÒªµ¥¶ÀÎ¬»¤Ò»¸ö</p>
 *
 * @param conv ´ý³õÊ¼»¯µÄ×ª»»½á¹¹Ìå£¬²»ÄÜÎªNULL
 *
 * @return ³É¹¦·µ»Ø0£¬³ö´í·µ»Ø-1
 */
int ul_ccode_conv_open(ccode_conv_t* conv);

/**
 * @brief	Ïß³ÌÄÚccode³õÊ¼»¯º¯Êý£¬Ã¿¸öÏß³Ìµ÷ÓÃÒ»´Î. 
 * µÈÍ¬ÓÚul_ccode_conv_open(ccode_conv_t* conv)£¬µ«²»ÐèÒªÏÔÊ½´«µÝccode_conv_t½á¹¹.
**/
int ul_ccode_conv_open();

/**
 * @brief ¹Ø±ÕÖÐÈÕÎÄ±àÂë×ª»»ËùÐèµÄ½á¹¹
 * <p>¸Ã²»ÊÇÏß³Ì°²È«µÄ£¬Ã¿¸ö×ÓÏß³ÌÐèÒªµ¥¶ÀÎ¬»¤Ò»¸ö</p>
 *
 * @param conv ´ý¹Ø±ÕµÄ×ª»»½á¹¹Ìå
 *
 * @return ·µ»Ø0
 */
int ul_ccode_conv_close(ccode_conv_t* conv);

/**
 * @brief	Ïß³ÌÄÚccode¹Ø±Õº¯Êý.
 * µÈÍ¬ÓÚul_ccode_conv_close(ccode_conv_t* conv)£¬µ«²»ÐèÒªÏÔÊ½´«µÝccode_conv_t½á¹¹.
**/
int ul_ccode_conv_close();

/** 
 * @brief ½«ÊäÈë×Ö·û´®×ª»»³ÉGBI±àÂë
 * <p>Ä¿Ç°´¦ÀíGB18030(GBK) BIG5 UTF8µ½GBIµÄ×ª»»¡£ÆäËüÊäÈë±àÂëÀàÐÍ½«Ö±½Ó¿½±´</p>
 * 
 * @param input ÊäÈë×Ö·û´®
 * @param output Êä³öbuffer£¬×¢Òâ£ºÈç¹ûÊäÈë±àÂëÀàÐÍÊÇUTF8£¬ÔòinputºÍoutput²»¿ÉÊÇÍ¬Ò»buffer£¬output´óÐ¡×îºÃÊÇinputµÄ2±¶
 * @param outlen Êä³öbuffer´óÐ¡£¬µ±Êä³öbufferÐ´ÂúÊ±½«²»ÔÙ¼ÌÐø×ª»»
 * @param codetype ÊäÈëµÄ±àÂëÀàÐÍ
 * @param conv ÓÃÓÚ×ö±àÂë×ª»»µÄ½á¹¹Ìå£¬ÓÃul_ccode_conv_openºÍul_ccode_conv_close´ò¿ªºÍ¹Ø±Õ¡£¸Ã½á¹¹²»ÊÇÏß³Ì°²È«µÄ£¬¸÷×ÓÏß³ÌÐèÒªÎ¬»¤µ¥¶ÀµÄ½á¹¹
 * 
 * @return ×ª»»³É¹¦·µ»Ø0£¬³ö´í·µ»Ø-1
 */
int ul_trans2gbi(char *input, char *output, int outlen, int codetype, ccode_conv_t* conv);

int ul_trans2gbi(char *input, char *output, int outlen, int codetype);

/** 
 * @brief ½«ÊäÈë×Ö·û´®×ª»»³ÉGB18030±àÂë
 * <p>Ä¿Ç°´¦ÀíBIG5 UTF8 JIS SHIFT_JIS EUCJPµ½GB18030µÄ×ª»»£¬ÆäËüÊäÈë±àÂëÀàÐÍ½«Ö±½Ó¿½±´</p>
 * 
 * @param input ÊäÈë×Ö·û´®
 * @param output Êä³öbuffer£¬×¢Òâ£ºÈç¹ûÊäÈë±àÂëÀàÐÍÊÇUTF8 JIS SHIFT_JIS EUCJP£¬ÔòinputºÍoutput²»¿ÉÊÇÍ¬Ò»buffer£¬outputµÄ´óÐ¡×îºÃÊÇinputµÄ2±¶
 * @param outlen Êä³öbuffer´óÐ¡£¬µ±Êä³öbufferÐ´ÂúÊ±½«²»ÔÙ¼ÌÐø×ª»»
 * @param codetype ÊäÈëµÄ±àÂëÀàÐÍ
 * @param cconv ÓÃÓÚ×ö±àÂë×ª»»µÄ½á¹¹Ìå£¬ÓÃul_ccode_conv_openºÍul_ccode_conv_close´ò¿ªºÍ¹Ø±Õ¡£¸Ã½á¹¹²»ÊÇÏß³Ì°²È«µÄ£¬¸÷×ÓÏß³ÌÐèÒªÎ¬»¤µ¥¶ÀµÄ½á¹¹
 * 
 * @return ×ª»»³É¹¦·µ»Ø0£¬³ö´í·µ»Ø-1
 */
int ul_trans2gb18030(char* input, char* output, int outlen, int codetype, ccode_conv_t* conv);

int ul_trans2gb18030(char* input, char* output, int outlen, int codetype);

/**
 * @brief ÅÐ¶Ï×Ö·û´®Ç°4¸ö×Ö½ÚÊÇ·ñÔÚgb18030ÂëÓòÄÚ
 *
 * @param s ÊäÈë×Ö·û´®£¬¿ÉÒÔÎªNULL
 *
 * @return ÊÇÔò·µ»Øtrue£¬NULL»ò²»ÔÚÔò·µ»Øfalse
 */
inline bool ul_is_gb18030_4bytes(const char* s){
	return (s != NULL)
		&& IN_RANGE(s[0], 0x81, 0xFE)
		&& IN_RANGE(s[1], 0x30, 0x39)
		&& IN_RANGE(s[2], 0x81, 0xFE)
		&& IN_RANGE(s[3], 0x30, 0x39);
}

/**
 * @brief ÅÐ¶Ï×Ö·û´®Ç°4¸ö×Ö½ÚÊÇ·ñÊÇgb18030-2005µÄËÄ×Ö½Úºº×Ö
 *
 * @param s ÊäÈë×Ö·û´®£¬¿ÉÒÔÎªNULL
 *
 * @return ÊÇÔò·µ»Øtrue£¬NULL»ò²»ÊÇÔò·µ»Øfalse
 */
inline bool ul_is_gb18030_4bytes_hanzi(const char* s){
	return (s != NULL)
		&& (IN_RANGE(s[0], 0x81, 0x82) || IN_RANGE(s[0], 0x95, 0x98)) //2 sections
		&& IN_RANGE(s[1], 0x30, 0x39)
		&& IN_RANGE(s[2], 0x81, 0xFE)
		&& IN_RANGE(s[3], 0x30, 0x39);
}

/** 
 * @brief ÅÐ¶Ï×Ö·û´®¿ªÍ·ÊÇ·ñÊÇºÏ·¨µÄGB18030×Ö·û£¬¸ø³ö×Ö·ûµÄ×Ö½ÚÊý
 * 
 * @param s ÊäÈë×Ö·û´®£¬¼Ù¶¨ÊÇgb18030±àÂë
 * 
 * @return ×Ö·ûµÄ×Ö½ÚÊý
 * 0: Èç¹ûÊäÈëÎªNULL»ò¿Õ´®
 * 1: Èç¹ûÊäÈëÊ××Ö½ÚÎªASCII×Ö·û
 * 2: Èç¹ûÊäÈëÊ×2×Ö½ÚÎªGBK×Ö·û£¬¼´GB18030µÄË«×Ö½Ú²¿·Ö
 * 4: Èç¹ûÊäÈëÊ×4×Ö½ÚÎªGB18030µÄËÄ×Ö½Ú²¿·Ö
 * -1: Èç¹ûÊäÈë²»ÊÇºÏ·¨GB18030×Ö·û
 */
int ul_next_gb18030(const char* s);

/** 
 * @brief ÅÐ¶Ï×Ö·û´®º¬GB18030×Ö·û¸öÊý£¬°üÀ¨ascii×Ö·û¡¢GB18030Ë«×Ö½Ú¡¢GB18030ËÄ×Ö½Ú¡£²»°üÀ¨·Ç·¨×Ö·û
 * 
 * @param s ÊäÈë×Ö·û´®£¬¼Ù¶¨ÊÇGB18030±àÂë
 * 
 * @return GB18030×Ö·û¸öÊý
 */
int ul_gb18030_len(const char* s);

/** 
 * @brief ÅÐ¶Ï×Ö·û´®Ê×Á½×Ö½ÚÊÇ·ñÊÇGBÈ«½Ç±êµã
 * <p>°üÀ¨£º ¡¢ ¡£ ¡¤ ¡¥ ¡¦ ¡§ ¡¨ ¡ª ¡« ¡¬ ¡­ ¡® ¡¯ ¡° ¡± ¡² ¡³¡´ ¡µ ¡¶ ¡· ¡¸ ¡¹ ¡º ¡» ¡¼ ¡½¡¾ ¡¿
 *  £¡ £¢ ££ £¤ £¥ £¦ £§ £¨ £©£ª £« £¬ £­ £® £¯£º £» £¼ £½ £¾ £¿ £Û £Ü£Ý £Þ £ß £à£û £ü £ý £þ</p>
 * 
 * @param s ÊäÈë×Ö·û´®£¬¼Ù¶¨ÊÇGB18030±àÂë
 * 
 * @return ÊÇ·µ»Øtrue£¬·ñÔò·µ»Øfalse
 */
inline bool ul_is_gb_punct(const char* s){
	return (s != NULL) && (((u_char)s[0] == 0xA1 && IN_RANGE((u_char)s[1], 0xA2, 0xBF) && (u_char)s[1] != 0xA9) //punctuation in A1
			|| (IS_GB_SMAEK((u_char*)s) && !IS_GB_1234((u_char*)s) && !IS_GB_ABCD((u_char*)s)));//punctuation in A3
}

/**
 * @brief ³õÊ¼»¯gb18030µ½utf8¼äµÄ×ª»»ËùÐèµÄiconv_t½á¹¹
 * <p>iconv²»ÊÇÏß³Ì°²È«µÄ£¬Ã¿¸ö×ÓÏß³ÌÐèÒªµ¥¶ÀÎ¬»¤Ò»¸öiconv_t½á¹¹</p>
 *
 * @param gb18030_utf8 gb18030µ½utf8µÄ×ª»»½á¹¹£¬ÈçÎªNULLÔò²»³õÊ¼»¯
 * @param utf8_gb18030 utf8µ½gb18030µÄ×ª»»½á¹¹£¬ÈçÎªNULLÔò²»³õÊ¼»¯
 *
 * @return ³É¹¦·µ»Ø0£¬³ö´í·µ»Ø-1
 */
int ul_gb18030_utf8_open(iconv_t* gb18030_utf8, iconv_t* utf8_gb18030);

/**
 * @brief ¹Ø±Õgb18030µ½utf8¼äµÄ×ª»»ËùÐèµÄiconv_t½á¹¹
 * <p>iconv²»ÊÇÏß³Ì°²È«µÄ£¬Ã¿¸ö×ÓÏß³ÌÐèÒªµ¥¶ÀÎ¬»¤Ò»¸öiconv_t½á¹¹</p>
 *
 * @param gb18030_utf8 gb18030µ½utf8µÄ×ª»»½á¹¹£¬ÈçÎªNULLÔò²»´¦Àí
 * @param utf8_gb18030 utf8µ½gb18030µÄ×ª»»½á¹¹£¬ÈçÎªNULLÔò²»´¦Àí
 *
 * @return ·µ»Ø0
 */
int ul_gb18030_utf8_close(iconv_t* gb18030_utf8, iconv_t* utf8_gb18030);

/** 
 * @brief ½«ÊäÈëµÄgb18030±àÂëµÄ×Ö·û´®×ª»»Îªbig5±àÂë
 * <p>¶ÔÓÚbig5Ã»ÓÐµÄgb18030×Ö·û£¬ÒÔÁ½¸ö¿Õ¸ñ´úÌæ¡£</p>
 * 
 * @param psrc ÊäÈë×Ö·û´®
 * @param pdes Êä³ö×Ö·û´®£¬Á½Õß¿ÉÒÔÊÇÍ¬Ò»¸öbuffer£¬·ñÔòÇë±£Ö¤Êä³öbuffer´óÐ¡ÖÁÉÙ¸úÊäÈëÏàÍ¬
 * @param slen ÊäÈë×Ö·û´®³¤¶È£¬º¯ÊýÖÁ¶à×ª»»slen¸ö×Ö½Ú£¬Èçslen<=0£¬½«×ª»»Õû¸öÊäÈë
 * 
 * @return ×ª»»³É¹¦·µ»Ø1£¬³ö´í·µ»Ø-1
 */
int ul_gb18030_big5_str(u_char* psrc, u_char* pdes, int slen);

/** 
 * @brief ½«ÊäÈëµÄbig5±àÂëµÄ×Ö·û´®×ª»»³Égb18030±àÂë
 * <p>ÓÉÓÚbig5×Ö·ûÔÚgbkÖÐ¶¼ÓÐÓ³Éä£¬¶øgbkÊÇgb18030µÄ×Ó¼¯£¬ËùÒÔ±¾º¯ÊýÊµ¼Ê×öµÄÊÇbig5->gbk×ª»»</p>
 * 
 * @param psrc ÊäÈë×Ö·û´®
 * @param pdes Êä³ö×Ö·û´®£¬Á½Õß¿ÉÒÔÊÇÍ¬Ò»¸öbuffer£¬·ñÔòÇë±£Ö¤Êä³öbuffer´óÐ¡ÖÁÉÙ¸úÊäÈëÏàÍ¬
 * @param slen ÊäÈë×Ö·û´®³¤¶È£¬º¯ÊýÖÁ¶à×ª»»slen¸ö×Ö½Ú£¬Èçslen<=0£¬½«×ª»»Õû¸öÊäÈë
 * 
 * @return ×ª»»³É¹¦·µ»Ø1£¬³ö´í·µ»Ø-1
 */
int ul_big5_gb18030_str(u_char* psrc, u_char* pdes, int slen);

/** 
 * @brief ½«ÊäÈëµÄgb18030±àÂëµÄ×Ö·û´®×ª»»Îª¼òÌå
 * 
 * @param psrc ÊäÈë×Ö·û´®
 * @param pdes Êä³ö×Ö·û´®£¬Á½Õß¿ÉÒÔÊÇÍ¬Ò»¸öbuffer£¬·ñÔòÇë±£Ö¤Êä³öbuffer´óÐ¡ÖÁÉÙ¸úÊäÈëÏàÍ¬
 * @param slen ÊäÈë×Ö·û´®³¤¶È£¬º¯ÊýÖÁ¶à×ª»»slen¸ö×Ö½Ú£¬Èçslen<=0£¬½«×ª»»Õû¸öÊäÈë
 * 
 * @return ×ª»»³É¹¦·µ»Ø1£¬³ö´í·µ»Ø-1
 */
int ul_gb18030_gbi_str(u_char* psrc, u_char* pdes, int slen);

/** 
 * @brief ½«ÊäÈëµÄgbi±àÂëµÄ×Ö·û´®×ª»»³Égb18030±àÂë
 * 
 * @param psrc ÊäÈë×Ö·û´®
 * @param pdes Êä³ö×Ö·û´®£¬Á½Õß¿ÉÒÔÊÇÍ¬Ò»¸öbuffer£¬·ñÔòÇë±£Ö¤Êä³öbuffer´óÐ¡ÖÁÉÙ¸úÊäÈëÏàÍ¬
 * @param slen ÊäÈë×Ö·û´®³¤¶È£¬º¯ÊýÖÁ¶à×ª»»slen¸ö×Ö½Ú£¬Èçslen<=0£¬½«×ª»»Õû¸öÊäÈë
 * 
 * @return ×ª»»³É¹¦·µ»Ø1£¬³ö´í·µ»Ø-1
 */
int ul_gbi_gb18030_str(u_char* psrc, u_char* pdes, int slen);

/** 
 * @brief ½«gb18030±àÂëµÄ×Ö·û´®×ª»»Îªutf8±àÂë
 * <p>Óöµ½×ª»»Ê§°ÜµÄ×Ö·ûÓÃÒ»¸ö¿Õ¸ñ´úÌæ</p>
 * 
 * @param psrc ÊäÈë×Ö·û´®
 * @param pdes Êä³ö×Ö·û´®£¬²»¿ÉÓëÊäÈë×Ö·û´®ÏàÍ¬£¬Æä´óÐ¡²»Ó¦Ð¡ÓÚÊäÈëµÄ3/2
 * @param destlen Êä³öbuffer´óÐ¡£¬µ±bufferÐ´ÂúÊ±×ª»»½«Í£Ö¹
 * @param gb18030_utf8 ¶ÔÓ¦µÄiconv_t½á¹¹£¬ÓÃul_gb18030_utf8_open´ò¿ª£¬ÓÃul_gb18030_utf8_close¹Ø±Õ£¬·ÇÏß³Ì°²È«
 * 
 * @return ³É¹¦·µ»Ø×ª»»Ê§°ÜµÄ×Ö·ûÊý£¬³ö´í·µ»Ø-1
 */
int ul_gb18030_utf8_str(char* psrc, char* pdes, int destlen, iconv_t* gb18030_utf8);

int ul_gb18030_utf8_str(char* psrc, char* pdes, int destlen);

/** 
 * @brief ½«utf8±àÂëµÄ×Ö·û´®×ª»»Îªgb18030±àÂë
 * <p>Óöµ½×ª»»Ê§°ÜµÄ×Ö·ûÊ±£¬Èç¹ûÊÇµ¥×Ö½ÚÓÃÒ»¸ö¿Õ¸ñ´úÌæ£¬·ñÔòÓÃÁ½¸ö¿Õ¸ñ´úÌæ</p>
 * 
 * @param psrc ÊäÈë×Ö·û´®
 * @param pdes Êä³ö×Ö·û´®£¬²»¿ÉÓëÊäÈë×Ö·û´®ÏàÍ¬£¬Æä´óÐ¡²»Ó¦Ð¡ÓÚÊäÈëµÄ2±¶
 * @param destlen Êä³öbuffer´óÐ¡£¬µ±bufferÐ´ÂúÊ±×ª»»½«Í£Ö¹
 * @param utf8_gb18030 ¶ÔÓ¦µÄiconv_t½á¹¹£¬ÓÃul_gb18030_utf8_open´ò¿ª£¬ÓÃul_gb18030_utf8_close¹Ø±Õ£¬·ÇÏß³Ì°²È«
 * 
 * @return ³É¹¦·µ»Ø×ª»»Ê§°ÜµÄ×Ö·ûÊý£¬³ö´í·µ»Ø-1
 */
int ul_utf8_gb18030_str(char* psrc, char* pdes, int destlen, iconv_t* utf8_gb18030);

int ul_utf8_gb18030_str(char* psrc, char* pdes, int destlen);



/** 
 * @brief ±àÂëºÍÓïÑÔÊ¶±ð½Ó¿Ú
 * 
 * @param page Ò³Ãæ×Ö·û´®£¬²»¿ÉÎªNULL
 * @param url Ò³Ãæurl£¬²»¿ÉÎªNULL
 * @param charset Ò³Ãæcharset×Ö·û´®£¬ÈçÎªNULLÔò½«ÔÚpageÖÐ²éÕÒ
 * @param content Ò³ÃæÕýÎÄ£¬¿ÉÎªNULL
 * @param[out] codetype ±àÂëÀàÐÍ
 * @param[out] langtype ÓïÑÔÀàÐÍ
 * 
 * @return Ê¶±ð³É¹¦·µ»Ø1£¬³ö´í·µ»Ø-1
 */
int ul_ccode_detect(const char* page, const char* url, const char* charset, const char* content, int* codetype, int* langtype,int * trustarr=NULL);

/**
 * @brief ÈÕÎÄ±àÂëºÍÓïÑÔÊ¶±ð½Ó¿Ú£¬Ö±½Ó·â×°ul_ccode_detect
 *
 * @param page Ò³Ãæ×Ö·û´®£¬²»¿ÉÎªNULL
 * @param url Ò³Ãæurl£¬²»¿ÉÎªNULL
 * @param charset Ò³Ãæcharset×Ö·û´®£¬ÈçÎªNULLÔò½«ÔÚpageÖÐ²éÕÒ
 * @param content Ò³ÃæÕýÎÄ£¬¿ÉÎªNULL
 * @param[out] out_type ±àÂëÀàÐÍ
 *
 * @return Ê¶±ð³É¹¦·µ»Ø1£¬³ö´í·µ»Ø-1
 */
int ul_jcode_detect(char *page, const char* url, const char* charset, const char* content, int *out_type);

/** 
 * @brief ¼ì²â¶ÌÎÄ±¾µÄ±àÂëÀàÐÍ£¬¿ÉÒÔÊÇGBK UTF8 »òUCS2±àÂë. ÈçÊäÈëÎªUCS2£¬½«±»×ª»»ÎªGBK
 * 
 * @param query ´ý¼ì²éµÄ¶ÌÎÄ±¾£¬³¤¶ÈÓ¦¸ÃÔÚ(1, 102400)·¶Î§ÄÚ
 * 
 * @return ¶ÌÎÄ±¾±àÂëÀàÐÍ£¬CODETYPE_GB CODETYPE_UTF8 CODETYPE_NONEÖ®Ò»£¬³ö´íÒ²·µ»Ø-2
 */
int ul_ccode_query_detect(char* query);

/** 
 * @brief ½«ÊäÈë×Ö·û´®ÓÉUCS2±àÂë×ª»»ÎªGBK±àÂë
 * 
 * @param src ÊäÈë×Ö·û´®£¬²»ÄÜÎª¿Õ
 * @param[out] dst Êä³öbuffer
 * @param dstlen Êä³öbuffer´óÐ¡£¬²»ÄÜÐ¡ÓÚ1
 * 
 * @return Êä³ö×Ö·û´®³¤¶È£¬³ö´í·µ»Ø-1
 */
int ul_ucs2_gbk_str(char* src, char* dst, int dstlen);
	
/**
 * @brief	»ñÈ¡Ïß³ÌÄÚÈ«¾ÖË½ÓÐµÄccode_conv½á¹¹ 
**/
ccode_conv_t *get_ccode_conv();
/**
 * @brief  Ê¹ÓÃhtmltree À´Ìá¸ß±àÂë¾«È·¶È±àÂëÊ¶±ðºÍÓïÑÔÊ¶±ðÔö¼ÓÁ½¸ö½Ó¿Ú£¬
      Ö÷ÒªÊÇ°Ñ´«Èë info½á¹¹£¬¸ÄÎªÁËÓëul_ccode_detect½Ó¿ÚÀàËÆ¡£ 
      °Ñinfo ½á¹¹¸ÄÎªÁËÏß³ÌÄÚ²¿Êý¾Ý½á¹¹£¬ºÍccode_conv_t Ò»Ñù£¬ÎªÁË±£³Ö½Ó¿Ú²»±ä£¬
      ³õÊ¼»¯ºÍÏú»ÙµÄº¯Êý·Åµ½ÁËul_ccode_conv_open()ºÍul_ccode_conv_close()ÖÐ£¬²»ÐèÒªinit¡£ 
      ÓïÑÔÊ¶±ðÔö¼ÓÁË  trustarr Êý×é ÓÃÓÚ´æ·Å¸÷¸öÓïÑÔÖÃÐÅ¶È ¡£
      µ«Ö»¶Ô±àÂëÅÐ¶¨Îªutf-8ºÍ gb18030Ê± ÓÐÓÃ¡£ 
      ¶ÔÃ¿¸öÍøÒ³£¬Îñ±ØÏÈµ÷ÓÃccode_detect_codetype¡£È»ºóµ÷ÓÃccode_detect_langtype¡£
      Ô­ÓÐ½Ó¿ÚÓëÏÖ½Ó¿Ú²»Òª»ìÓÃ¡£ul_ccode_detect¿ÉÒÔÓÃ£¬Ã»ÎÊÌâ¡£
      ¿¼ÂÇµ½Ð§ÂÊ£¬µ÷ÓÃccode_detect_codetypeºóÔÙµ÷ÓÃccode_detect_langtype£¬
      Èç¹û²ÎÊýurl²»±äµÄ»°²»»áÔÙÖØÐÂ¼ÆËãÒ»´Îinfo¡£
      Èç¹ûÒªÖØÐÂ¼ÆËã£¬¿ÉÒÔ´«Èë¿Õ×Ö·û´®""¡£×¢Òâ²»¿É´«ÈëNULL¡£»áÖ±½Ó·µ»Ø¡£
 *
 * @param page Ò³Ãæ×Ö·û´®£¬²»¿ÉÎªNULL
    @main_content ÌáÈ¡µÄÖ÷ÒªÄÚÈÝ£¬
    @page_content ÌáÈ¡µÄÄÚÈÝ£¬
 * @param url Ò³Ãæurl£¬²»¿ÉÎªNULL
 * @param charset Ò³Ãæcharset×Ö·û´®£¬ÈçÎªNULLÔò½«ÔÚpageÖÐ²éÕÒ
 * @param content Ò³ÃæÕýÎÄ£¬¿ÉÎªNULL
 * @param[out] codetype ±àÂëÀàÐÍ
 * @param[out] langtype ÓïÑÔÀàÐÍ
 * @param[out] trustarr    ×îºóÒ»¸öÔªËØÊÇ¼ì²â×Ö·û±àÂëµÄ»º³åÇø³¤¶È
*  

 */
typedef struct _page_info_t{
		char *  page;
		char *  main_content;
		char * page_content;
		char * title;
		int len_page;
		int len_main_content;
		int len_page_content;
		int len_title;
}page_info_t;

int ul_ccode_page_detect(page_info_t* page_info,const char* url, const char* charset, const char* content, int* codetype, int* langtype,int * trustarr);

#endif
