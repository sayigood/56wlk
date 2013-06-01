/**
 * @file
 * @brief 检索串解析
 */

/**
 * @brief 对用户输入的检索串进行规范化处理，并解析成标准格式供应用进一步使用。
 */

#ifndef __QUERYPARSER_H__
#define __QUERYPARSER_H__

#define QP_MAX_STRLEN 256
#define QP_MAX_QUERYLEN 76

#define SYNTAX_QUERY_END  '\0'
#define SYNTAX_WORD       ' '
#define SYNTAX_ADD        '+'
#define SYNTAX_MINUS      '-'
#define SYNTAX_OR         '|'
#define SYNTAX_ADJUST     'A'
#define SYNTAX_ESCAPE     'E'
#define SYNTAX_URL        'W'
#define SYNTAX_INURL      'U'
#define SYNTAX_TITLE      'T'
#define SYNTAX_SITE       'S'
#define SYNTAX_ANCHOR     'L'
#define SYNTAX_CONT       'C'
#define SYNTAX_COLON_END  '#'
#define SYNTAX_L_BRACKET  '('
#define SYNTAX_R_BRACKET  ')'
#define SYNTAX_L_BOOKMARK '<'
#define SYNTAX_R_BOOKMARK '>'
#define SYNTAX_QUOTATION  '\"'

#define SYMBOL_L_BOOKMARK "\0\0《"
#define SYMBOL_R_BOOKMARK "\0\0》"

#define SYN_SUG_QUOTATION   1
#define SYN_SUG_BOOKMARK    2

#define MAX_SEG_WORDS	  100
#define MAX_SEG_WORD_LEN	8

#define FILE_TYPE_NONE  -1
#define FILE_TYPE_ALL	0
#define	FILE_TYPE_DOC	1
#define	FILE_TYPE_PDF	2
#define	FILE_TYPE_PPT	3
#define	FILE_TYPE_XLS	4
#define	FILE_TYPE_WPS	5
#define	FILE_TYPE_RTF	6

typedef struct MutiDoc_fType
{
	char len;
	char val;
	char *name;
}mType_t;


/**
 *  支持带双字节的trim
 *  
 *  @param[in]  inStr  输入字符串
 *  @param[in]  outStr trim操作后的字符串存放的buf
 *  @param[out] outStr trim操作后的字符串
 *  @return 输出字符串的长度
 */
int trimString(const char *inStr, char *outStr);


/**
 *  去处seg标记的字符,输出用空格代替
 *  
 *  @param[in]  inStr  输入字符串
 *  @param[in]  outStr 去处seg的字符串存放的buf
 *  @param[out] outStr 去处seg的字符串
 *  @return 0
 */
int removeSeg(char *inStr, char *outStr);	


/**
 *  初始化检索串解析，在主线程开始处调用一次
 *  
 *  @param[in]  无
 *  @param[out] 无
 *  @return 1
 */
int queryInit();


/**
 *  对检索串进行解析
 *  
 *  @param[in]  inStr              待解析的检索串
 *  @param[in]  termStr            存储解析出单词的缓冲区，缓冲区的长度需大于QP_MAX_STRLEN
 *  @param[in]  opStr              存储解析出操作符序列的缓冲区，缓冲区的长度需大于QP_MAX_STRLEN
 *  @param[in]  opt                检索选项，以位表示，0x01: 新闻检索; 0x02:需要粘贴
 *  @param[in]  suggest            存储建议的缓冲区，不少于256个字节，可以为空
 *  @param[in]  site_cluster_flag  存储站内聚类标志的缓冲区，可以为空
 *  @param[in]  haveSyntax         存储语法标志的缓冲区，可以为空
 *  @param[in]  fType              指向字符的指针，用于存储检索文档类型，可以为空
 *  @param[out] termStr            存放解析出的单词
 *  @param[out] opStr              存放解析出的操作符序列   
 *  @param[out] suggest            存放建议信息，前面部分为单词，以'\0'结束，suggest[255]存放提示类型:SYN_SUG_BOOKMARK、SYN_SUG_QUOTATION
 *  @param[out] site_cluster_flag  存有站内聚类标志，如果不需要站内类聚（即检索串中有site语法），则返回0，否则将不改变词变量的内容	
 *  @param[out] haveSyntax         存放语法标志，0－无语法操作符、1－有语法操作符
 *  @param[out] fType              存放检索文档的类型
 *  @return 操作返回值
 * - 0  成功
 * - <0 失败
 *  @note fType类型:
 * - FILE_TYPE_ALL
 * - FILE_TYPE_DOC
 * - FILE_TYPE_PDF
 * - FILE_TYPE_PPT
 * - FILE_TYPE_XLS
 * - FILE_TYPE_WPS
 * - FILE_TYPE_RTF
 *  @note termStr与opStr的说明:
 * opStr与termStr的长度一致，并且每个字节都是对应关系, 例如:\n
 *    termStr: "hello world"\n
 *    opStr:   "     |     "\n
 *    opStr中是5个SYNTAX_WORD、1个SYNTAX_OR、5个SYNTAX_WORD、\n
 *             1个SYNTAX_QUERY_END\n
 * 即: termStr中对应opStr中的SYNTAX_WORD的部分为单词，而\n
 * termStr中对应opStr具体的操作符的部分没有意义。\n
 * opStr中可存放的操作符如下：\n
 *      SYNTAX_QUERY_END        '\0'\n
 *      SYNTAX_WORD             ' '\n
 *      SYNTAX_ADD              '+'\n
 *      SYNTAX_MINUS            '-'\n
 *      SYNTAX_OR               '|'\n
 *      SYNTAX_ADJUST           'A'\n
 *      SYNTAX_ESCAPE           'E'\n
 *      SYNTAX_URL              'W'\n
 *      SYNTAX_INURL            'U'\n
 *      SYNTAX_TITLE            'T'\n
 *      SYNTAX_SITE             'S'\n
 *      SYNTAX_ANCHOR           'L'\n
 *      SYNTAX_CONT             'C'\n
 *      SYNTAX_COLON_END        '#'\n
 *      SYNTAX_L_BRACKET        '('\n
 *      SYNTAX_R_BRACKET        ')'\n
 *      SYNTAX_L_BOOKMARK       '<'\n
 *      SYNTAX_R_BOOKMARK       '>'\n
 *      SYNTAX_QUOTATION        '\"'
 */
int queryParser(char *inStr, char *termStr, char *opStr, int opt, char *suggest, 
		int *site_cluster_flag, int *haveSyntax,char *fType);
		
		
		
/**
 *  结束检索串解析，主线程退出前调用
 *  
 *  @param[in]  无
 *  @param[out] 无
 *  @return 1
 */
int queryFinish();

#endif
