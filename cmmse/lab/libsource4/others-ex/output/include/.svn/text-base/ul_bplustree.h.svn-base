/**
 * @file
 * @brief 特定子字符串查找
 */


/**
 * @brief 在一个字符串中查找特定的子字符串，首先将所有需要查找的子字符串插入到子字符串树中；或存储在文件中，\n
 * 通过批量导入的方式加入到子字符串树中。
 */
#ifndef __BPLUSTREE_H__
#define __BPLUSTREE_H__

#include "stdlib.h"
#include "stdio.h"
#include "ul_log.h"
#include "string.h"
#include "ul_ccode.h"
//#include "asfilter.h"

#define LEAF_BIT 2
#define VALID_BIT 1
#define MAX_ITEM_NUM 100
#define MAX_LENGTH 256
#define IS_LEAF(node) (node->status & LEAF_BIT)
#define IS_VALID(node) (node->status & VALID_BIT)

#define MAX_SNAME_LEN 256

typedef struct _TreeNode TreeNode;

struct _TreeNode
{
   TreeNode **child;		//children node
   unsigned char key;		//node value 
   unsigned char status;	//0bit: whether is a valid word
							//1bit: whether is a leaf node
   unsigned char modkey;	//value to distinguish leaf values         
};
          

typedef struct _SubString 
{
	int begin; //the offset of string start in the original input word
	int end;   //the offset of string end in the original input word
	char name[MAX_SNAME_LEN];
} SubString;




/**
 *  利用传递的key生成一个叶子节点
 *  
 *  @param[in]  key   key值
 *  @param[out] 无
 *  @return 存放节点的地址
 * - 非NULL   成功
 * - NULL     失败
 *  @note 内部函数,接口不必关注
 */
extern TreeNode *InitTreeNode(unsigned char key);



/**
 *  将用key生成的节点作为当前节点的子节点插入到树中
 *  
 *  @param[in]  node   当前节点
 *  @param[in]  key    key值
 *  @param[out] 无
 *  @return 存放下一key值的地址
 * - 非NULL   成功
 * - NULL     失败
 *  @note 内部函数,接口不必关注
 */
extern TreeNode *InsertToChild(TreeNode* node, unsigned char key);

/**
 *  释放节点
 *  
 *  @param[in]  node  节点的指针
 *  @param[out] 无
 *  @return 无
 *  @note 内部函数,接口不必关注
 */
extern void FreeNode(TreeNode *node);
/**
 *  查找字典中匹配的最长的子字符串
 *  
 *  @param[in]  root   树的根节点
 *  @param[in]  str    查找的字符串
 *  @param[out] 无
 *  @return 最大长度
 * - >0   成功
 * - 0    失败
 *  @note 内部函数,接口不必关注
 */
extern int SearchString(TreeNode *root, char *str);

/**
 *  创建子字符串树
 *  
 *  @param[in]  无
 *  @param[out] 无
 *  @return 指向子字符串树的指针
 * - 非NULL   成功
 * - NULL     失败
 */
extern TreeNode *InitTree();




/**
 *  将字符串插入到tree中
 *  
 *  @param[in]  root   树的根节点
 *  @param[in]  str    插入的字符串
 *  @param[out] 无
 *  @return 操作结果
 * - 0   成功
 * - -1  失败
 */
extern int InsertString(TreeNode *root, char *str);



/**
 *  查找字符串中所含有的子字符串，输出到一个线性缓冲区中
 *  
 *  @param[in]  root   树状字典的根
 *  @param[in]  query  待查找的字符串
 *  @param[in]  len    字符串长度
 *  @param[in]  result 于存储查找到的子字符串的缓冲区
 *  @param[in]  reslen 返回buf的长度
 *  @param[out] result 返回所有查找结果,每个子字符串以‘0’结尾，连续存储在result中
 *  @return 查找到item的个数
 *  @note query可能会被修改，所以不能使用常量
 */
extern int seek_string(TreeNode *root, char* query, int len, char* result, int reslen);




/**
 *  查找字符串中所含有的子字符串，输出到一个结构化的缓冲区中
 *  
 *  @param[in]  root    树状字典的根
 *  @param[in]  query   待查找的字符串
 *  @param[in]  len     字符串长度
 *  @param[in]  tgbuf   用于存储查找到的子字符串的缓冲区
 *  @param[in]  tgbsize 返回buf的长度
 *  @param[out] tgbuf   返回所有查找结果,每个子字符串以‘0’结尾，连续存储在result中
 *  @return 存有查找到的子字符串
 *  @note query可能会被修改，所以不能使用常量
 */
extern int seek_p(TreeNode *root, char* query, int len, SubString* tgbuf, int tgbsize);





/**
 *  创建子字符串树并将文件中的子字符串加入到树中
 *  
 *  @param[in]  filepath    文件所在路径
 *  @param[in]  filename    文件名
 *  @param[out] 无
 *  @return 指向子字符串树的指针
 * - 非NULL   成功
 * - NULL     失败
 *  @note 文件格式为每行一个子字符串
 */
extern TreeNode *CreateTreeFromFile(char* filepath, char* filename);



/**
 *  释放子字符串树
 *  
 *  @param[in]  root   指向树的根的指针
 *  @param[out] 无
 *  @return 无
 */
extern void FreeTree(TreeNode *root);




#endif

