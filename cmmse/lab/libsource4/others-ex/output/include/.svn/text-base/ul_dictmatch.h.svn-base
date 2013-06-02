/**
 * @file
 * @brief 带属性的特定子字符串查找
 */


/**
 * @brief 在一个字符串中查找特定的子字符串，首先将所有需要查找的子字符串插入到子字符串树中；或存储在文件中，\n
 * 通过批量导入的方式加入到子字符串树中。\n
 * 通过批量导入的方式将子字符串加入到词典中需要预先将子字符串和其属性存放在文本文件中。文件的格式如下：\n
 *                子字符串         属性\n                  
 *                子字符串         属性\n                    
 * 每行一条记录，前面为子字符串，后面为属性，中间用空格或制表符分开。属性以字符串的形式表示，而在加入字典时\n
 * 需要的时整型的属性值，所以要由应用程序提供将字符串形式的属性映射成整数形式的属性的函数，整数形式的属性值\n
 * 必须是2的倍数，即只能占据整型值中的某一位。\n
 */
#ifndef _PTDEF_H
#define _PTDEF_H
/*********************************************************************************
 * Filename	: dictmatch.h
 * Descript	: search for a set of words in a long string. those set of words can 
 * 			: construct a dict; search in a string from beginning to the end, 
 * 			: save every result that can be found in dict.
 ********************************************************************************/


//	{{{	macros defined here
#define u_int unsigned int
#define DM_DENTRY_NULL          0xFFFFFFFF
#define DM_DENTRY_FIRST         0xFFFFFFFE
#define DM_SUFENTRY_NULL        0xFFFFFFFF
#define DM_LEMMA_NULL           0xFFFFFFFF
#define DM_COMMON_NULL          0xFFFFFFFF


//match operation type
#define DM_OUT_ALL	0
#define DM_OUT_FMM	1
//	}}}


//	{{{ data type defined here
//lemma structure for ppack
typedef struct _DM_LEMMA
{
	u_int len;
	u_int prop;
	char * pstr;
	u_int bpos;
}dm_lemma_t;

//lemma structure for dict
typedef struct _DM_INLEMMA
{
	u_int len;
	u_int prop;
	u_int bpos;
}dm_inlemma_t;

typedef struct _DM_ENTRY
{
	u_int value;
	u_int lemma_pos;
	u_int suffix_pos;
}dm_entry_t;

typedef struct _DM_DICT
{
	char*		strbuf;   // buffer for store word  result;
	u_int		sbsize;
	u_int		sbpos; // the next position in word buf

	dm_entry_t* dentry; // the dict entry list
	u_int		desize;
	u_int		depos; // the first unused pos in de list

	u_int* 		seinfo; // the suffix entry list
	u_int 		seisize;
	u_int		seipos;

	dm_lemma_t* lmlist; // the lemma list
	u_int 		lmsize;
	u_int		lmpos; // the first unused pos in lemma list

	u_int 		entrance;
}dm_dict_t;

typedef struct _DM_PACK{
	u_int			maxterm;	// 切词结果所能容纳最大基本词数
	dm_lemma_t**	ppseg;
	u_int*			poff;
	u_int			ppseg_cnt;
}dm_pack_t;
//	}}}

//	{{{	module interface declared here

extern int (*dm_prop_str2int_p)(char *);
extern void (*dm_prop_int2str_p)(int, char *);




/**
 *  创建查找词典
 *  
 *  @param[in]  lemma_num  子字符串的数量
 *  @param[out] 无
 *  @return 词典的指针
 * - 非NULL   成功
 * - NULL     失败
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
dm_dict_t* dm_dict_create(int lemma_num);



/**
 *  load二进制文件存储的词典
 *  
 *  @param[in]  fullname  存储的带路径的文件名
 *  @param[out] 无
 *  @return 词典的指针
 * - 非NULL   成功
 * - NULL     失败
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
dm_dict_t* dm_binarydict_load(const char* fullname);


/**
 *  二进制方式将词典存储到文件中
 *  
 *  @param[in]  pdict  存储的带路径的文件名
 *  @param[in]  fullname  存储的带路径的文件名
 *  @param[out] 无
 *  @return 词典的指针
 * - 1   成功
 * - -1  文件创建失败
 * - -2  写数据失败
 * - -3  调整dict失败
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
int dm_binarydict_save(dm_dict_t * pdict,const char* fullname);



/**
 *  创建用于存储子字符串的词典并将文件中子字符串加载词典中
 *  
 *  @param[in]  fullpath   词典文件名
 *  @param[in]  lemma_num  子字符串的数量
 *  @param[out] 无
 *  @return 词典的指针
 * - 非NULL   成功
 * - NULL     失败
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
dm_dict_t* dm_dict_load(const char* fullpath, int lemma_num);


/**
 *  删除查找词典
 *  
 *  @param[in]  pdict   词典指针
 *  @param[out] 无
 *  @return 无
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
void dm_dict_del(dm_dict_t* pdict);


/**
 *  创建查找结果存储结构
 *  
 *  @param[in]  max_wnum   最多可存储的结果数
 *  @param[out] 无
 *  @return   查找结果存储结构
 * - 非NULL   成功
 * - NULL     失败
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
dm_pack_t* dm_pack_create(int max_wnum);

/**
 *  删除查找结果存储结构
 *  
 *  @param[in]  ppack   查找结果存储结构
 *  @param[out] 无
 *  @return 无
 *  @note 为了适应32位词典在64位下能正常加载，将词典内部词条的结构和切词结构独立开了（以前是一个结构），分别是dm_inlemma_t和dm_lemma_t。这样注意dm_pack_t* dm_pack_create(int max_wnum);中max_wnum如果设的很大的话，内存会有明显增加。
 */
void dm_pack_del(dm_pack_t* ppack);



/**
 *  向词典中加入一个子字符串
 *  
 *  @param[in]  pdict   子字符串查找词典
 *  @param[in]  plm     指向子字符串结构的指针
 *  @param[out] 无
 *  @return   操作返回值
 * - 1     成功
 * - 0     子字符串已存在
 * - -1    失败
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
int dm_add_lemma(dm_dict_t* pdict, dm_lemma_t* plm);


/**
 *  查找字符串含有的匹配字符串
 *  
 *  @param[in]  pdict     子字符串查找词典
 *  @param[in]  ppack     存储查找结果的缓冲区
 *  @param[in]  inbuf     待查找的字符串
 *  @param[in]  inlen     字符串的长度
 *  @param[in]  opertype  默认设置参数opertype为DM_OUT_FMM，也就是词语的正向最大匹配，对应的DM_OUT_ALL是词语的全匹配
 *  @param[out] ppack     存有查找结果
 *  @return   操作返回值
 * - 1     成功
 * - -1    失败
 *  @note 如果返回成功，通过ppack->ppseg_cnt判断查找到的词条数（可能为0）
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
int dm_search(dm_dict_t* pdict, dm_pack_t* ppack, const char* inbuf, int inlen,int opertype = DM_OUT_FMM);

//notice : the function matches words in sentence by "opertype" match method,
//the return value is the OR(|) of all the property of the matched words,
//the type num user defined should less than 32
//1 type should occupy 1 bit of an int;
/**
 *  the function matches words in sentence by "opertype" match method
 *  
 *  @param[in]  pdict     子字符串查找词典
 *  @param[in]  inbuf     待查找的字符串
 *  @param[in]  inlen     字符串的长度
 *  @param[in]  opertype  默认设置参数opertype为DM_OUT_FMM，也就是词语的正向最大匹配，对应的DM_OUT_ALL是词语的全匹配
 *  @return   类型
 *  @note 这是个新接口，其返回结果是输入参数中inbuf这个句子中包含的词语包含的所有类型，默认参数opertype设为正向最大匹配。需要注意的是词典中的词语最多只能有32种类型，每个类型的值应该占据u_int值的1位。
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
u_int dm_search_prop(dm_dict_t* pdict,const char* inbuf, int inlen,int opertype = DM_OUT_FMM);


/**
 *  查看字符串中是否含有特定属性的子串
 *  
 *  @param[in]  pdict    子字符串查找词典
 *  @param[in]  type     待查找的类型
 *  @param[in]  inbuf    待查找的字符串
 *  @param[in]  inlen    字符串的长度
 *  @param[out] 无
 *  @return   操作返回值
 * - 1     存在特定类型的子串，如果type是多个属性值（'|'在一起），任意一个属性符合条件即返回1
 * - 0     不存在特定类型的子串
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
int dm_has_word(dm_dict_t* pdict, int type, const char* inbuf, int inlen,int opertype = DM_OUT_FMM);

/**
 *  查看查找结果中是否含有特定类型的子串，即查找某一特定属性值的子串
 *  
 *  @param[in]  ppack    指向查找结果
 *  @param[in]  type     待查找的类型，即待查找的子串的属性值
 *  @param[out] 无
 *  @return   操作返回值
 * - 1     存在特定类型的子串，如果type是多个属性值（'|'在一起），任意一个属性符合条件即返回1
 * - 0     不存在特定类型的子串
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
int dmpack_has_type(dm_pack_t* ppack, int type);


/**
 *  在词典中查找是否有term对应的数据
 *  
 *  @param[in]  pdict 子字符串查找词典
 *  @param[in]  term  查找的字符串
 *  @param[in]  len   字符串的长度 
 *  @param[out] 无
 *  @return   在list中的位置
 * - >0                找到，返回位置
 * - DM_LEMMA_NULL     未找到
 *  @note 内部使用,接口不需关注 
 *  @see ::dm_dict_create  ::dm_binarydict_load  ::dm_binarydict_save  ::dm_dict_load  ::dm_dict_del  ::dm_pack_create  ::dm_pack_del  ::dm_add_lemma  ::dm_search  ::dm_has_word  ::dmpack_has_type  ::dm_seek_lemma
 */
u_int dm_seek_lemma(dm_dict_t * pdict,const char* term,int len);//for test;

//	}}}

#endif

