/********************************************************************
 ********************************************************************/

#ifndef WP_FELT_H__
#define WP_FELT_H__

// {{{ Header Files
#include <scwdef.h>
#include <postagger.h>

#include "feltrule.h"
#include "wp_proper_noun.h"
#include "replace_index.h"
#include "identify_index.h"

#include "wp_nomeans.h"
#include "wp_place_parent.h"
#include "ul_thr.h"
//#include "mid.h"
 // }}}

// {{{ Macro Definition
/*通用前后缀*/
#define FELT_SET_APPFIX(property,value)		((property) | value)
#define FELT_GET_APPFIX(property)		((property) & 0x00000003)

/*普通前后缀*/
#define FELT_SET_COMFIX(property,value)		((property) | ((value) << 2))
#define FELT_GET_COMFIX(property)		(((property) & 0x0000001C) >> 2)

/*地域信息*/
#define FELT_SET_AREA(property,value)		((property) | (value << 5))
#define FELT_GET_AREA(property)			(((property) & 0x000000E0) >> 5)

/*重要性*/
#define FELT_SET_IMPT(property,value)		((property) | ((value) << 8))
#define FELT_GET_IMPT(property)			(((property) & 0x00000700) >> 8)

/*是否高频--通用diff索引*/
#define FELT_SET_DIFFLAG(property)		((property) | 0x00000800)
#define FELT_GET_DIFFLAG(property)		((property) & 0x00000800)

/*dropword property*/
#define FELT_SET_DROP(property,value)		((property) | (value << 12))
#define FELT_GET_DROP(property)			(((property) & 0x00003000) >> 12)

/*splitword property*/
#define FELT_SET_SPLIT(property,value)		((property) | (value << 14))
#define FELT_GET_SPLIT(property)		(((property) & 0x0000C000) >> 14)

/*diff merge reweight*/
#define FELT_SET_REQ_DIFF(property, value)	((property) |( value) << 16)
#define FELT_GET_REQ_DIFF(property)		(((property) & 0x00010000) >> 16)

#define SEARCH_IN_AT 0
#define SEARCH_IN_CT 1

#define MAX_QUERY_LEN 80 

/*
 * Term 的类型
 */
#define WEIGHT_OFFSET_EXACT		0	//绝对要求Offset限制与权重
#define WEIGHT_OFFSET_NORMAL	1	//从Diff Term或者Bitmap Term中提升上来的
#define WEIGHT_OFFSET_DEFAULT	2	//Offset限制可选, 权重要求  默认值
#define WEIGHT_ONLY   3				//只要求权重
#define OFFSET_ONLY   4				//只要求Offset, 目前未使用
#define APPEAR_ONLY   5				//只命中
#define NO_USE        6				//可丢弃的词

/*
 * Offset 限制的类型
 */
#define OFFSET_LOOSE		0	//相邻Term不紧密
#define OFFSET_NORMAL		1	//在只有两个Term && OFFSET_LOOSE, 由 OFFSET_LOOSE 升为此值 
								//默认值
#define OFFSET_TIGHT_HALF	2	//相邻Term之间关系半紧密 由OFFSET_TIGHT_WHOLE降为此值
#define OFFSET_TIGHT_WHOLE  3	//相邻Term完全紧密
// }}} 

//策略编号
#define OMIT_JUNK_INDEX 0		//省略垃圾词策略生效	长Query二期
// }}}

// }}} 

// {{{ phraseoffreq
#define WHOLEOFFREQ 1
#define HALFOFFREQ 2
// }}}

#define FELT_MAX_BUF_SIZE 65536
#define FELT_MIN_BUF_SIZE 4
// }}}

/* {{{ constant defined here */
const char FELT_PREFIX[] = "W_";
const int FELT_PREFIX_LEN = 2;
const int FELT_QR_FREQ_LB = 100;
const int FELT_QU_FREQ_LB = 10;
const int FELT_QS_FREQ_LB = 0;
/* }}} */

/* {{{ data structure defined here */
typedef struct WP_FELT_T
{
	char* m_buf;
	int m_size;
	int m_curpos;
	int m_termcount;
	int* m_termoffsets;
	int* m_termpos;
}
wp_felt_t;

typedef struct WP_SCOPE_T
{
	int m_valid;
	int m_ftype;
	int m_ptype;
	int m_felted;
	int m_difflag;
	int m_impt;
	int m_area;
	int m_comfix;
	int m_appfix;
	int m_drop;
    int m_phraselen;
}
wp_scope_t;

/*
 * temporaray backup store for scw_out_t structure
 * 只能备份scw_out_t 五个BUFFER之一
 */
typedef struct WP_PackBuf
{
	int termcount;
	char buf[MAX_QUERY_LEN * 2 + 40];
	int pos[MAX_QUERY_LEN * 2 + 40];
	int offsets[MAX_QUERY_LEN * 2 + 40];
}
wp_packbuf_t;

/*
 * Toolkit Class
 * Query data with its length
 * Move From IndSearch
 * @auther
 */
typedef struct _in_info
{
	char*	inbuf;
	int		slen;
//	int	omitBasicCount;		//被省略的基本词的个数 用以照顾省略移入FELT
//	int	omitBasicLen;		//被省略的基本词的总长度 用以照顾省略移入FELT
}
info_in_t;

/*
 * Toolkit Class
 * All input data for IndSearch and Felt
 * @auther
 */
typedef struct SEARCH_SEG_DATA
{
	info_in_t m_in;
	
	scw_out_t* m_out;
	
	wp_felt_t m_felt;
	wp_felt_t m_felt_merge;
	
	char* m_outbuf[8];				//前六个是切词buffer,后一个是粘接buffer
	unsigned int* m_poss[8];
	unsigned int* m_offsets[8];
	unsigned int* m_count[8];
	
	char* m_tmpbuf;
	
	pos_pack_t* m_pos_pack;
	scw_item_t m_pitem;
	dm_pack_t* m_pDmPack;
	
	char m_tags[MAX_QUERY_LEN];
	char m_term_type[MAX_QUERY_LEN];
	char m_off_type[MAX_QUERY_LEN];
	char m_ispronoun[MAX_QUERY_LEN];
	char m_isorgname[MAX_QUERY_LEN];
	bool m_hasproper;
}
search_seg_data_t;


/*
 * 词典包
 * 包含粘接模块使用的各种词典
 */
typedef struct
{
	scw_worddict_t* pScwDict;		//指向分词词典
	pos_dict_t*   pPosDict;			//指向词性标注词典
	Sdict_search* pFeltDict; 		//指向粘接词典,由索引对象负责初始化
	Sdict_search* pAtFeltDict;		//指向AT粘接词典
	Sdict_search* pDropDict;		//指向重查省略词词典,在进行重查粘接时使用
	wp_ruledict_t* pRuleDict;		//单字前后缀粘接规则词典
	Sdict_search* pRequire;			//需求词词典
//	dm_dict_t* pEmbracedict;		//上下位关系词典，暂时取消

	Sdict_search* pPlaceMap;		//地名上下级关系映射词典 
	char* placeParentBuffer;		//地名上级缓冲区 中国#中国-北京#中国-河北-石家庄# (WJY)
	Sdict_search* pOmitDict;		//省略词词典	(WJY)
	Sdict_search* phrasedict;		//拆分词词典
	dm_dict_t* pProperNounDict;		//挂载专名词典
	dm_dict_t* pOrgnameDict;		//挂载机构名词典
	dm_dict_t* pSemanticDict;		//归一化词典

	Sdict_search* pDiffDict;		//diff term 词典
	Sdict_search* pHiLiIdfDict;		//高亮词IDF词典
	Sdict_search* pStopWordDict;		//停用词表
	Sdict_search* pOffTypeDict;		//Term距离属性词典
	Sdict_search* yibian1_dict;     	//易变索引页主域词典
	Sdict_search* yibian2_dict;
	Sdict_search*vd_nomeans_dict;
	Sdict_search* m_pSitefix;		//寻址类后缀词典
	trans_dict_t * m_err_transdict;		//纠错词典
	trans_dict_t * m_syn_transdict;		//模板同义替换词典
	ident_dict_t * m_site_identdict;	//机构名识别词典
	
	bool policySwitch[10];			//策略生效开关数组	不合理 日后修改掉
	unsigned short sema_num_map[65536]; //数词转换表
}
wp_dict_pack_t;
/* }}} */

/**************************************************
 * type		  : public 
 * function	  : init the segData
 * in-argu	  : bufsize	 : the segData's buf size (by byte) 
 * out-argu	  : psegData : the data pir
 *			  : phraseoffreq : atFelt use
 * return	  : >=0 if success
 *			  : <0  if failed
 * other	  : psegData和phraseoffreq只需要在外部声明，不需要
 *			  : 分配空间，bufsize最小为4，最大为65536,如果函数
 *			  : 执行过程中失败，会打印失败信息并释放已经申请的
 *			  : 内存，然后返回-1
 * *************************************************/
int init_segData(search_seg_data_t *&psegData, char *&phraseoffreq, int bufsize);

/****************************************************
 * type		  : public
 * function	  : init the dict pack
 * in-argu	  : worddict : the worddict dir path
 *            : bsdict   : the bsdict dir path
 * out-argu	  : pdictPack: the dict pack
 *			  : atFelt : atFelt flag
 * return	  : >=0 if sucess
 *			  : <0  if failed 
 * other	  : worddict和bsdict为文件夹路径，内部存放的词典见
 *			  : 相关文档，atFelt是ps用户at建库的，ns产品用不到
 *			  : 使用默认值即可
 * **************************************************/
int init_dictPack(char* worddict,char* bsdict, wp_dict_pack_t *&pdictPack, bool atFelt = false);


/******************************************************************
 * type     :   pulblic
 * function :   ns_get_seg_pos
 * in-argu  :   instring 
 *          :   segData : 切词词性标注粘接结构体
 *          :   dictPack: 字典包
 *          :   level : 粘接粒度，一般为0
 * out-argu :
 * return   :   >= 0 if succeed
 *          :   <0   if failed
 * other    :   函数包括了切词词性标注粘接以及他们之间的一些数
 *			:	据类型转换。instring是原始串的首指针，
 *			:	instring_len是需要处理的长度。ns_get_seg_pos不
 *			:	会改变原始串。由于切词模块的一个缺陷，如果
 *			:	instring中连续的某个term的长度超过了
 *			:	instring_len，那么切词函数将无视instring_len而
 *			:	处理全部instring，所以建议在外部程序中检查一下
 * ****************************************************************/
int ns_get_seg_pos(
         char* instring, int instring_len,  search_seg_data_t *&segData, wp_dict_pack_t *&dictPack);

extern int g_phraseseplen;

/* {{{ wp_felt_hfword_p */
/********************************************************************
 * type        : public
 * function    : felt the high frequency into two-word.This function
 *             : will be used in parser. 
 * in-argu     : pack_outbuf * pout : the out buffer of word segment
 *             : Sdict_search * pdict : the hfword dict
 *             :     if a term in dict, other==0 indicate the term is
 *             :     a stopword, other==1 indicate the term is a hfword and
 *             :     needs to felt
 * out-argu    : wp_felt_t * pres : the output result for felt hfword
 * return      : 1 if success
 *             : <0 if failed
 * notice      : 这个函数用于建库粘接，对高频词进行粘接，具体策略参考相应文档
 *			   : 能够支持的最大输入串长度取决于分配的buf的大小
 ********************************************************************/
int wp_felt_hfword_p (wp_felt_t* pres, scw_out_t* pout,
					  Sdict_search* pdict);
/* }}} */

/* {{{ wp_get_query */
/****************************************************************************
 * 粘接模块最重要的入口函数
 * 根据分词结果和粘接级别, 返回粘接结果
 * 之所以有两个粘接结构指针, 完全是为存储空间节约考虑
 * 本质: 根据level, 分别调用不同的粘接策略, 获取粘接结果
 *
 * type		   : public
 * in-argu	   : pDictPack : 辞典包，封装了原有的切词辞典、粘接辞典，
 *			   :             增加了许多其他用于控制粘接合理性的词典。
 *			   :			 这些词典的加载方式可以参考新版testfelt。
 *			   : search_seg_data_t : 封装了切词、词性标注、粘接结果的
 *			   :			 所有buffer
 *			   : level :     查询层次，支持-1(精确匹配),0(普通查询),1,12,2,3,4(拆分粒度)
 *			   : searchPos : AT查询or CT查询
 *			   : phraseoffreq : at phrase拆分的限制标记，这个用于保存
 *			   :			 输出数据，最终指导offset计算，可以忽略其输出。
 *			   : query_is_org_name : 用来标记query是否为寻址类query，这个用
 *			   :			 于限制粘接粒度。如果多数为问答式query，很少寻址
 *			   :			 类query，则直接传递0进去即可。若希望可以传递正
 *			   :			 确的参数可以参考新版testfelt中这个参数的计算方式。
 *
 * ***************************************************************************/
int wp_get_query(
		const wp_dict_pack_t* pDictPack,	//词典包
		search_seg_data_t* pSegData,	//输出数据包
		int level,						//输入参数, 指定粘接策略
		int searchPos,						//查AT / CT
		char phraseoffreq[],					//查at_phrase offset限制标记
		int query_is_orig_name,				//query类型，是否为寻址类
		bool omit_junk_word = false,		//是否对粘接结果去词
		bool omit_pc_word = false);			//是否对连词介词去词
/* }}} */

/* {{{ wp_init_nomeans_dict  */
/****************************************************************************
 * function  : load no-means phrase configure file and init the internal dict
 * argu-in   : fname: the path and filename of no-means configure file, usually worddict/no_means.txt
 * return    : number of phrases loaded, or -1 if there is any error
 ***************************************************************************/
//int wp_init_nomeans_dict (const char* fname);
/* }}} */

int wp_backup_segdata(search_seg_data_t *des, const search_seg_data_t *src);

int wp_append_buf (wp_felt_t * pFelt, const char *buf, int len, int offset);

int get_subphrase_ind (const scw_out_t* pOut, int boff, int eoff);

/*************************************************                                              
 * Function    : clear_segData()
 * Description : 销毁切词和粘接词典,释放输入输出结构所占内存                                    
 * Input  : psegData		 切词词性标注粘接包指针                                             
 *        : phraseoffreq   记录at phrase拆分的buffer指针                                      
 * Output :         
 * Return : 
 * Others : 该函数是非线程安全的，需要独立使用                                                  
 **************************************************/                                            
void clear_segData(search_seg_data_t *psegData, char* phraseoffreq);
/*************************************************                                              
 * Function    : clear_dictPack()
 * Description : 销毁词典，释放词典所占内存
 * Input  : pdictPack                                      
 * Output :         
 * Return : 
 * Others : 该函数是非线程安全的，需要独立使用                                                  
 **************************************************/                                            
void clear_dictPack(wp_dict_pack_t *pdictPack);

#endif
