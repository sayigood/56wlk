/* -*- c++ -*- 
    */
#ifndef _ODICT_H_
#define _ODICT_H_

enum {
    ODB_ADD_OK=0, // 添加节点成功
    ODB_ADD_ERROR=1,// 添加节点失败（未知原因）
    ODB_ADD_EXISTS=2,// 已经存在节点导致添加失败
    ODB_ADD_OVERWRITE=3,// 已经存在导致覆盖原来节点
    ODB_ADD_INVALID=4,// 插入节点的签名不可以是{0,0}
    
    ODB_DEL_OK=0,// 删除节点成功
    ODB_DEL_ERROR=1,// 删除节点失败（未知原因）
    ODB_DEL_NOT_EXISTS=2,// 要删除的节点不存在
    
    ODB_SEEK_OK=0,// 成功找到节点
    ODB_SEEK_ERROR=1,// 查找节点失败（未知原因）
    ODB_SEEK_FAIL=2,// 节点不存在
    
    ODB_MOD_OK=0,// 修改节点内容成功
    ODB_MOD_ERROR=1,// 修改节点内容失败（未知原因）
    ODB_MOD_FAIL=2,// 修改节点内容错误（节点不存在）
    
    ODB_RENEW_OK=0,// 清空字典成功
    ODB_RENEW_ERROR=1,// 清空字典失败（未知原因）

    ODB_SAVE_OK=0,// 保存字典成功
    ODB_SAVE_ERROR=1,// 保存字典失败（未知原因）

    ODB_LOAD_NOT_EXISTS=1,// 加载字典时指定的字典不存在
    
    ODB_DESTROY_OK=0,// 释放字典资源成功
};

// node number for block i/o
const int ODB_MID_NODE_NUM=4*1024;
// node number in node block
const int ODB_NODE_BLOCK_NUM=256*1024;
// i/o buffer
const unsigned int ODB_IO_BUFFSIZE=4*1024*1024;
// default hash number
const int ODB_DEFAULT_HASH_NUM=1000000;

typedef struct _sodict_snode_t{
    unsigned int sign1;
    unsigned int sign2;
    unsigned int cuint1;
    unsigned int cuint2;
}sodict_snode_t;

typedef struct _sodict128_snode_t{
    unsigned long long sign1;
    unsigned long long sign2;
    unsigned int cuint1;
    unsigned int cuint2;
}sodict128_snode_t;

typedef struct _sodict_build_t sodict_build_t;
typedef struct _sodict_search_t sodict_search_t;
typedef struct _sodict128_build_t sodict128_build_t;
typedef struct _sodict128_search_t sodict128_search_t;

/**
 *  创建一个新字典,并分配足够使用的hash单元
 *
 *  @param[in]  hash_num  使用hash单元的个数
 *  @param[out] 无
 *  @return 字典的指针,这个指针通常作为参数被其他函数引用,例如::odb_add
 * - 非NULL   成功
 * - NULL     失败
 *  @note 当hash_num>0时,字典将根据hash_num设置hash数组的大小,否则,将按默认值设置hash数组的大小,默认值为1000000
 */
sodict_build_t *odb_creat(int hash_num);

/**
 *  类似::odb_creat,创建一个字典,并且将指定的文件内容load到字典中
 *
 *  @param[in]  path      load文件的路径
 *  @param[in]  filename  load文件的文件名
 *  @param[in]  hash_num  使用hash单元的个数
 *  @param[out] 无
 *  @return 字典的指针,这个指针通常作为参数被其他函数引用,例如::odb_add
 * - ODB_LOAD_NOT_EXISTS   指定载入的文件不存在
 * - NULL                  load文件失败,有可能是创建字典错误,或读文件错误等
 * - 其他返回值            字典的指针
 *  @note 当hash_num>0时,字典将根据hash_num设置hash数组的大小,否则,将按制定文件中当前存储的数据单元个数设置hash单元个数
 */
sodict_build_t *odb_load(char *path, char *filename, int hash_num);



/**
 *  类似::odb_load,不过本函数创建的是只供查询用的字典
 *
 *  @param[in]  path      load文件的路径
 *  @param[in]  filename  load文件的文件名
 *  @param[out] 无
 *  @return     字典的指针
 * - ODB_LOAD_NOT_EXISTS   文件不存在
 * - NULL                  load文件失败,有可能是创建字典错误,或读文件错误等
 * - 其他返回值            字典的指针
 */
sodict_search_t *odb_load_search(char *path, char *filename);


/**
 * @brief
 * 使用mmap方式打开字典文件，便于共享内存
 * @param [in/out] path   : char*
 * @param [in/out] filename   : char*
 * @return  sodict_search_t*
 * @retval
 * - ODB_LOAD_NOT_EXISTS   文件不存在
 * - NULL                  load文件失败,有可能是创建字典错误,或读文件错误等
 * - 其他返回值            字典的指针
 * @see
**/
sodict_search_t *odb_load_search_mmap(char *path, char *filename);

/**
 * @brief
 * 释放使用odb_load_search_mmap建立的字典
 * @param [in/out] sdb   : sodict_search_t*
 * @return     ODB_DESTROY_OK
 * @see
**/
int odb_destroy_search_mmap(sodict_search_t *sdb);
/**
 *  释放字典资源
 *
 *  @param[in]  sdb      字典指针
 *  @param[out] 无
 *  @return     ODB_DESTROY_OK
 */
int odb_destroy(sodict_build_t *sdb);


/**
 *  释放只读字典资源
 *
 *  @param[in]  sdb      字典指针
 *  @param[out] 无
 *  @return     ODB_DESTROY_OK
 */
int odb_destroy_search(sodict_search_t *sdb);


/**
 *  保存字典到指定的文件中
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  path      文件的路径
 *  @param[in]  filename  文件名
 *  @param[out] 无
 *  @return     保存是否成功
 * - ODB_SAVE_OK      保存成功
 * - ODB_SAVE_ERROR   保存失败
 */
int odb_save(sodict_build_t *sdb, char *path, char *filename);



/**
 *  保存字典到指定的文件中,类似odb_save，只针对只读字典
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  path      文件的路径
 *  @param[in]  filename  文件名
 *  @param[out] 无
 *  @return     保存是否成功
 * - ODB_SAVE_OK      保存成功
 * - ODB_SAVE_ERROR   保存失败
 */
int odb_save_search(sodict_search_t *sdb, char *path, char *filename);




/**
 *  类似::odb_save,保存字典到指定的文件中,不同的是,本函数确保旧文件不被销毁
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  path      文件的路径
 *  @param[in]  filename  文件名
 *  @param[out] 无
 *  @return     保存是否成功
 * - ODB_SAVE_OK      保存成功
 * - ODB_SAVE_ERROR   保存失败
 *  @note 当同名旧文件存在时,旧文件将添加".odb.ss."前缀,更名后保存
 */
int odb_save_safely(sodict_build_t *sdb, char *path, char *filename);


/**
 *  类似::odb_save_search,保存只读字典到指定的文件中,不同的是,本函数确保旧文件不被销毁
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  path      文件的路径
 *  @param[in]  filename  文件名
 *  @param[out] 无
 *  @return     保存是否成功
 * - ODB_SAVE_OK      保存成功
 * - ODB_SAVE_ERROR   保存失败
 *  @note 当同名旧文件存在时,旧文件将添加".odb.ss."前缀,更名后保存
 */

int odb_save_search_safely(sodict_search_t *sdb, char *path, char *filename);
/**
 *  清空当前dict中所有内容,dict恢复到odb_creat调用后时的状态
 *
 *  @param[in]  sdb       字典指针
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_RENEW_OK      成功
 * - ODB_RENEW_ERROR   失败
 *  @note 当返回ODB_RENEW_ERROR时,dict有可能已经被销毁,因此,再次使用sdb指针是非常危险的,请谨慎处理
 */
int odb_renew(sodict_build_t *sdb);



/**
 *  与::odb_renew相同,为了向下兼容而保留
 *
 *  @param[in]  sdb       字典指针
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_RENEW_OK      成功
 * - ODB_RENEW_ERROR   失败
 *  @note 当返回ODB_RENEW_ERROR时,dict有可能已经被销毁,因此,再次使用sdb指针是非常危险的,请谨慎处理
 */
int odb_renew_ex(sodict_build_t *sdb);


/**
 *  向dict添加一个节点
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     添加的节点内容
 *  @param[in]  overwrite_if_exists       覆盖标志,如果overwrite_if_exists!=0,当dict中有相同节点时,将会覆盖原有值,并返回ODB_ADD_OVERWRITE,否则返回ODB_ADD_EXISTS
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_ADD_OK      dict中不存在该节点,并且插入成功
 * - ODB_ADD_ERROR   插入操作失败
 * - ODB_ADD_OVERWRITE   dict中已经有相同签名的数据,并且覆盖标志overwrite_if_exists!=0,原数据节点被覆盖
 * - ODB_ADD_EXISTS   dict中已经有相同签名的数据,并且覆盖标志overwrite_if_exists==0,原数据节点保留,不做插入操作
 * - ODB_ADD_INVALID  插入数据不合法，snode的签名不可以是{0,0}
 */
int odb_add(sodict_build_t *sdb, sodict_snode_t *snode, int overwrite_if_exists);


/**
 *  如果指定节点存在,在dict中删除指定节点
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     要删除的节点
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_DEL_OK      dict中存在该节点,并且删除成功
 * - ODB_DEL_NOT_EXISTS   dict中无此数据
 * - ODB_DEL_ERROR 删除失败snode的sign1和sign2为0
 */
int odb_del(sodict_build_t *sdb, sodict_snode_t *snode);


/**
 *  在dict中查找指定节点的内容
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     要查找的节点
 *  @param[out] snode     如果找到,将节点内容存储此处
 *  @return     状态信息
 * - ODB_SEEK_OK      在dict中找到指定节点,并将节点内容输出到snode中
 * - ODB_SEEK_FAIL    dict中无此数据
 */
int odb_seek(sodict_build_t *sdb, sodict_snode_t *snode);


/**
 *  在只读dict中查找指定节点的内容
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     要查找的节点
 *  @param[out] snode     如果找到,将节点内容存储此处
 *  @return     状态信息
 * - ODB_SEEK_OK      在dict中找到指定节点,并将节点内容输出到snode中
 * - ODB_SEEK_FAIL    dict中无此数据
 */
int odb_seek_search(sodict_search_t *sdb, sodict_snode_t *snode);



/**
 *  在dict中修改指定节点的信息
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     要查找的节点
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_MOD_OK       在dict中找到指定节点,并修改了节点中的内容
 * - ODB_MOD_FAIL     在dict中未找到指定节点
 */
int odb_mod(sodict_build_t *sdb, sodict_snode_t *snode);


/**
 *  遍历字典中所有单元
 *
 *  @param[in]  sdb                 字典指针
 *  @param[in]  include_deleted     是否遍历已删除节点
 *  @param[in]  traverse_handler    遍历过程进行具体操作的函数句柄
 *  @param[out] 无
 *  @return     无
 *  @note
 *  traverse_handler函数根据遍历需求自行设计,其中参数snode为遍历过程的一个节点,stop_now用来向遍历函数发送停止遍历的命令
 */
void odb_traverse(sodict_build_t *sdb, int include_deleted,
                  void (*traverse_handler)(sodict_snode_t *snode, int *stop_now));



/**
 *  遍历字典中所有单元(可在遍历操作函数中增加传入参数)
 *
 *  @param[in]  sdb                 字典指针
 *  @param[in]  include_deleted     是否遍历已删除节点
 *  @param[in]  traverse_handler    遍历过程进行具体操作的函数句柄
 *  @param[in]  ex_arg              传入具体遍历函数的参数
 *  @param[out] 无
 *  @return     无
 *  @note traverse_handler函数根据遍历需求自行设计,其中参数snode为遍历过程的一个节点,stop_now用来向遍历函数发送停止遍历的命令
 */
void odb_traverse_ex(sodict_build_t *sdb, int include_deleted,
                     void (*traverse_handler)(sodict_snode_t *snode, int *stop_now, void *arg), void *ex_arg);



/**
 *  遍历只读字典中所有单元
 *
 *  @param[in]  sdb                 字典指针
 *  @param[in]  traverse_handler    遍历过程进行具体操作的函数句柄
 *  @param[out] 无
 *  @return     无
 *  @note traverse_handler函数根据遍历需求自行设计,其中参数snode为遍历过程的一个节点,stop_now用来向遍历函数发送停止遍历的命令
 */
void odb_traverse_search(sodict_search_t *sdb, void (*traverse_handler)(sodict_snode_t *snode, int *stop_now));

/**
 *  遍历只读字典中所有单元(可在遍历操作函数中增加传入参数)
 *
 *  @param[in]  sdb                 字典指针
 *  @param[in]  traverse_handler    遍历过程进行具体操作的函数句柄
 *  @param[out] 无
 *  @return     无
 *  @note traverse_handler函数根据遍历需求自行设计,其中参数snode为遍历过程的一个节点,stop_now用来向遍历函数发送停止遍历的命令
 */
void odb_traverse_search_ex(sodict_search_t *sdb, void (*traverse_handler)(sodict_snode_t *snode, int *stop_now,void *arg),
                            void *ex_arg);
/**
 *  输出字典当前状态(debug时使用)
 *
 *  @param[in]  sdb                 字典指针
 *  @param[out] 无
 *  @return     无
 */
void odb_build_report(sodict_build_t *sdb);


/**
 *  输出只读字典当前状态(debug时使用)
 *
 *  @param[in]  sdb                 字典指针
 *  @param[out] 无
 *  @return     无
 */
void odb_search_report(sodict_search_t *sdb);

/**
 * 得到字典实际使用的节点数
 *
 * @param[in] sdb   字典指针
 * @return	    实际节点数
 */
int odb_get_nodenum(sodict_build_t *sdb);

int odb_search_get_nodenum(sodict_search_t *sdb);

/**
 * 得到hash的大小
 * @param[in] sdb   字典指针
 * @return          实际hash大小
 */
int odb_get_hashnum(sodict_build_t *sdb);

int odb_search_get_hashnum(sodict_search_t *sdb);

/**
 * 根据当前字典中节点个数调整字典hash个数，以提高查询性能
 *
 * @param[in] sdb   字典指针
 * @return          0:成功，-1:失败
 */
int odb_adjust(sodict_build_t *sdb);

/**
 * 对于只读字典，比较sdb1与sdb2是否完全相同，两个字典完全相同不仅要求字典的内容完全相同，每个
 * hash桶的元素顺序也要相同．
 *
 * @param[in] sdb1 字典指针
 * @param[in] sdb2 字典指针
 * @return  1 两字典完全相同, 0 两字典不相同
 */

int odb_is_equal_search(sodict_search_t *sdb1, sodict_search_t *sdb2);


/**
 *  创建一个新字典,并分配足够使用的hash单元
 *
 *  @param[in]  hash_num  使用hash单元的个数
 *  @param[out] 无
 *  @return 字典的指针,这个指针通常作为参数被其他函数引用,例如::odb_add
 * - 非NULL   成功
 * - NULL     失败
 *  @note 当hash_num>0时,字典将根据hash_num设置hash数组的大小,否则,将按默认值设置hash数组的大小,默认值为1000000
 */
sodict128_build_t *odb128_creat(int hash_num);




/**
 *  类似::odb128_creat,创建一个字典,并且将指定的文件内容load到字典中
 *
 *  @param[in]  path      load文件的路径
 *  @param[in]  filename  load文件的文件名
 *  @param[in]  hash_num  使用hash单元的个数
 *  @param[out] 无
 *  @return 字典的指针,这个指针通常作为参数被其他函数引用,例如::odb_add
 * - ODB_LOAD_NOT_EXISTS   指定载入的文件不存在
 * - NULL                  load文件失败,有可能是创建字典错误,或读文件错误等
 * - 其他返回值            字典的指针
 *  @note 当hash_num>0时,字典将根据hash_num设置hash数组的大小,否则,将按制定文件中当前存储的数据单元个数设置hash单元个数
 */
sodict128_build_t *odb128_load(char *path, char *filename, int hash_num);



/**
 *  类似::odb128_load,不过本函数创建的是只供查询用的字典
 *
 *  @param[in]  path      load文件的路径
 *  @param[in]  filename  load文件的文件名
 *  @param[out] 无
 *  @return     字典的指针
 * - ODB_LOAD_NOT_EXISTS   文件不存在
 * - NULL                  load文件失败,有可能是创建字典错误,或读文件错误等
 * - 其他返回值            字典的指针
 */
sodict128_search_t *odb128_load_search(char *path, char *filename);


/**
 * @brief
 * 使用mmap方式打开字典文件，便于共享内存
 * @param [in/out] path   : char*
 * @param [in/out] filename   : char*
 * @return  sodict128_search_t*
 * @retval
 * - ODB_LOAD_NOT_EXISTS   文件不存在
 * - NULL                  load文件失败,有可能是创建字典错误,或读文件错误等
 * - 其他返回值            字典的指针
 * @see
**/
sodict128_search_t *odb128_load_search_mmap(char *path, char *filename);

/**
 * @brief
 * 释放使用odb_load_search_mmap建立的字典
 * @param [in/out] sdb   : sodict128_search_t*
 * @return     ODB_DESTROY_OK
 * @see
**/
int odb_destroy_search_mmap(sodict128_search_t *sdb);
/**
 *  释放字典资源
 *
 *  @param[in]  sdb      字典指针
 *  @param[out] 无
 *  @return     ODB_DESTROY_OK
 */
int odb_destroy(sodict128_build_t *sdb);


/**
 *  释放只读字典资源
 *
 *  @param[in]  sdb      字典指针
 *  @param[out] 无
 *  @return     ODB_DESTROY_OK
 */
int odb_destroy_search(sodict128_search_t *sdb);


/**
 *  保存字典到指定的文件中
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  path      文件的路径
 *  @param[in]  filename  文件名
 *  @param[out] 无
 *  @return     保存是否成功
 * - ODB_SAVE_OK      保存成功
 * - ODB_SAVE_ERROR   保存失败
 */
int odb_save(sodict128_build_t *sdb, char *path, char *filename);



/**
 *  保存字典到指定的文件中,类似odb_save，只针对只读字典
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  path      文件的路径
 *  @param[in]  filename  文件名
 *  @param[out] 无
 *  @return     保存是否成功
 * - ODB_SAVE_OK      保存成功
 * - ODB_SAVE_ERROR   保存失败
 */
int odb_save_search(sodict128_search_t *sdb, char *path, char *filename);




/**
 *  类似::odb_save,保存字典到指定的文件中,不同的是,本函数确保旧文件不被销毁
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  path      文件的路径
 *  @param[in]  filename  文件名
 *  @param[out] 无
 *  @return     保存是否成功
 * - ODB_SAVE_OK      保存成功
 * - ODB_SAVE_ERROR   保存失败
 *  @note 当同名旧文件存在时,旧文件将添加".odb.ss."前缀,更名后保存
 */
int odb_save_safely(sodict128_build_t *sdb, char *path, char *filename);


/**
 *  类似::odb_save_search,保存只读字典到指定的文件中,不同的是,本函数确保旧文件不被销毁
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  path      文件的路径
 *  @param[in]  filename  文件名
 *  @param[out] 无
 *  @return     保存是否成功
 * - ODB_SAVE_OK      保存成功
 * - ODB_SAVE_ERROR   保存失败
 *  @note 当同名旧文件存在时,旧文件将添加".odb.ss."前缀,更名后保存
 */

int odb_save_search_safely(sodict128_search_t *sdb, char *path, char *filename);
/**
 *  清空当前dict中所有内容,dict恢复到odb_creat调用后时的状态
 *
 *  @param[in]  sdb       字典指针
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_RENEW_OK      成功
 * - ODB_RENEW_ERROR   失败
 *  @note 当返回ODB_RENEW_ERROR时,dict有可能已经被销毁,因此,再次使用sdb指针是非常危险的,请谨慎处理
 */
int odb_renew(sodict128_build_t *sdb);



/**
 *  与::odb_renew相同,为了向下兼容而保留
 *
 *  @param[in]  sdb       字典指针
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_RENEW_OK      成功
 * - ODB_RENEW_ERROR   失败
 *  @note 当返回ODB_RENEW_ERROR时,dict有可能已经被销毁,因此,再次使用sdb指针是非常危险的,请谨慎处理
 */
int odb_renew_ex(sodict128_build_t *sdb);


/**
 *  向dict添加一个节点
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     添加的节点内容
 *  @param[in]  overwrite_if_exists       覆盖标志,如果overwrite_if_exists!=0,当dict中有相同节点时,将会覆盖原有值,并返回ODB_ADD_OVERWRITE,否则返回ODB_ADD_EXISTS
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_ADD_OK      dict中不存在该节点,并且插入成功
 * - ODB_ADD_ERROR   插入操作失败
 * - ODB_ADD_OVERWRITE   dict中已经有相同签名的数据,并且覆盖标志overwrite_if_exists!=0,原数据节点被覆盖
 * - ODB_ADD_EXISTS   dict中已经有相同签名的数据,并且覆盖标志overwrite_if_exists==0,原数据节点保留,不做插入操作
 * - ODB_ADD_INVALID  插入数据不合法，snode的签名不可以是{0,0}
 */
int odb_add(sodict128_build_t *sdb, sodict128_snode_t *snode, int overwrite_if_exists);


/**
 *  如果指定节点存在,在dict中删除指定节点
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     要删除的节点
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_DEL_OK      dict中存在该节点,并且删除成功
 * - ODB_DEL_NOT_EXISTS   dict中无此数据
 * - ODB_DEL_ERROR 删除失败snode的sign1和sign2为0
 */
int odb_del(sodict128_build_t *sdb, sodict128_snode_t *snode);


/**
 *  在dict中查找指定节点的内容
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     要查找的节点
 *  @param[out] snode     如果找到,将节点内容存储此处
 *  @return     状态信息
 * - ODB_SEEK_OK      在dict中找到指定节点,并将节点内容输出到snode中
 * - ODB_SEEK_FAIL    dict中无此数据
 */
int odb_seek(sodict128_build_t *sdb, sodict128_snode_t *snode);


/**
 *  在只读dict中查找指定节点的内容
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     要查找的节点
 *  @param[out] snode     如果找到,将节点内容存储此处
 *  @return     状态信息
 * - ODB_SEEK_OK      在dict中找到指定节点,并将节点内容输出到snode中
 * - ODB_SEEK_FAIL    dict中无此数据
 */
int odb_seek_search(sodict128_search_t *sdb, sodict128_snode_t *snode);



/**
 *  在dict中修改指定节点的信息
 *
 *  @param[in]  sdb       字典指针
 *  @param[in]  snode     要查找的节点
 *  @param[out] 无
 *  @return     状态信息
 * - ODB_MOD_OK       在dict中找到指定节点,并修改了节点中的内容
 * - ODB_MOD_FAIL     在dict中未找到指定节点
 */
int odb_mod(sodict128_build_t *sdb, sodict128_snode_t *snode);


/**
 *  遍历字典中所有单元
 *
 *  @param[in]  sdb                 字典指针
 *  @param[in]  include_deleted     是否遍历已删除节点
 *  @param[in]  traverse_handler    遍历过程进行具体操作的函数句柄
 *  @param[out] 无
 *  @return     无
 *  @note
 *  traverse_handler函数根据遍历需求自行设计,其中参数snode为遍历过程的一个节点,stop_now用来向遍历函数发送停止遍历的命令
 */
void odb_traverse(sodict128_build_t *sdb, int include_deleted,
                  void (*traverse_handler)(sodict128_snode_t *snode, int *stop_now));



/**
 *  遍历字典中所有单元(可在遍历操作函数中增加传入参数)
 *
 *  @param[in]  sdb                 字典指针
 *  @param[in]  include_deleted     是否遍历已删除节点
 *  @param[in]  traverse_handler    遍历过程进行具体操作的函数句柄
 *  @param[in]  ex_arg              传入具体遍历函数的参数
 *  @param[out] 无
 *  @return     无
 *  @note traverse_handler函数根据遍历需求自行设计,其中参数snode为遍历过程的一个节点,stop_now用来向遍历函数发送停止遍历的命令
 */
void odb_traverse_ex(sodict128_build_t *sdb, int include_deleted,
                     void (*traverse_handler)(sodict128_snode_t *snode, int *stop_now, void *arg), void *ex_arg);



/**
 *  遍历只读字典中所有单元
 *
 *  @param[in]  sdb                 字典指针
 *  @param[in]  traverse_handler    遍历过程进行具体操作的函数句柄
 *  @param[out] 无
 *  @return     无
 *  @note traverse_handler函数根据遍历需求自行设计,其中参数snode为遍历过程的一个节点,stop_now用来向遍历函数发送停止遍历的命令
 */
void odb_traverse_search(sodict128_search_t *sdb, void (*traverse_handler)(sodict128_snode_t *snode, int *stop_now));

/**
 *  遍历只读字典中所有单元(可在遍历操作函数中增加传入参数)
 *
 *  @param[in]  sdb                 字典指针
 *  @param[in]  traverse_handler    遍历过程进行具体操作的函数句柄
 *  @param[out] 无
 *  @return     无
 *  @note traverse_handler函数根据遍历需求自行设计,其中参数snode为遍历过程的一个节点,stop_now用来向遍历函数发送停止遍历的命令
 */
void odb_traverse_search_ex(sodict128_search_t *sdb, void (*traverse_handler)(sodict128_snode_t *snode, int *stop_now,void *arg),
                            void *ex_arg);
/**
 *  输出字典当前状态(debug时使用)
 *
 *  @param[in]  sdb                 字典指针
 *  @param[out] 无
 *  @return     无
 */
void odb_build_report(sodict128_build_t *sdb);


/**
 *  输出只读字典当前状态(debug时使用)
 *
 *  @param[in]  sdb                 字典指针
 *  @param[out] 无
 *  @return     无
 */
void odb_search_report(sodict128_search_t *sdb);

/**
 * 得到字典实际使用的节点数
 *
 * @param[in] sdb   字典指针
 * @return	    实际节点数
 */
int odb_get_nodenum(sodict128_build_t *sdb);

int odb_search_get_nodenum(sodict128_search_t *sdb);

/**
 * 得到hash的大小
 * @param[in] sdb   字典指针
 * @return          实际hash大小
 */
int odb_get_hashnum(sodict128_build_t *sdb);

int odb_search_get_hashnum(sodict128_search_t *sdb);

/**
 * 根据当前字典中节点个数调整字典hash个数，以提高查询性能
 *
 * @param[in] sdb   字典指针
 * @return          0:成功，-1:失败
 */
int odb_adjust(sodict128_build_t *sdb);

/**
 * 对于只读字典，比较sdb1与sdb2是否完全相同，两个字典完全相同不仅要求字典的内容完全相同，每个
 * hash桶的元素顺序也要相同．
 *
 * @param[in] sdb1 字典指针
 * @param[in] sdb2 字典指针
 * @return  1 两字典完全相同, 0 两字典不相同
 */
int odb_is_equal_search(sodict128_search_t *sdb1, sodict128_search_t *sdb2);

#endif
