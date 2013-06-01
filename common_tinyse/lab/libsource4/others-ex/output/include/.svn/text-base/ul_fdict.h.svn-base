/**
 * @file
 * @brief 是一个256叉查找树的数组实现，将字符串和一个整型值
 */
#ifndef __UL_FDICT_H__
#define __UL_FDICT_H__

#define FDICT_SIZE 65536
#define DF_INVALID_DATA -2147483647

typedef struct _fnode_t {
	int data;
	unsigned short index[256];
} fnode_t;

typedef struct _fdict_t {
	int size;
	int num;
	fnode_t *nodes;
} fdict_t;



/**
 *  创建一个dict
 *  
 *  @param[in]  无 
 *  @param[out] 无
 *  @return 词典的指针
 * - 非NULL   成功
 * - NULL     失败
 */
extern fdict_t *df_create();

/**
 *  向字典中添加一个str - data 对
 *  
 *  @param[in]  pd      字典指针
 *  @param[in]  in      字符串
 *  @param[in]  data    字符串对应的data
 *  @param[out] 无
 *  @return data值
 * - 非NULL   成功
 * - NULL     失败
 */
extern int df_add(fdict_t *pd, char *in, int data);


/**
 *  根据字符串在字典中查找对应的data
 *  
 *  @param[in]  pd      字典指针
 *  @param[in]  in      字符串
 *  @param[out] 无
 *  @return data值
 * - data值          查找成功
 * - DF_INVALID_DATA 查找失败
 */
extern int df_seek(fdict_t *pd, char *in);


/**
 *  调整字典,根据size重新申请需要的节点内存
 *  
 *  @param[in]  pd      字典指针
 *  @param[out] 无
 *  @return 操作返回值
 * - 0    成功
 * - -1   失败
 */
extern int df_adjust(fdict_t *pd);


/**
 *  删除字典
 *  
 *  @param[in]  pd   字典指针
 *  @param[out] 无
 *  @return 0
 */
extern int df_delete(fdict_t *pd);


/**
 *  copy字典
 *  
 *  @param[in]  pd   字典指针
 *  @param[out] 无
 *  @return 新字典指针
 * - 非NULL   成功
 * - NULL     失败
 */
extern fdict_t *df_copy(fdict_t *pd);

#endif
