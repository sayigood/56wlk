/**
 * @file
 * @brief 实现逻辑大文件按块存取的封装
 **/
#ifndef _FILEBLOCK_HEAD_FILE_
#define _FILEBLOCK_HEAD_FILE_
#include "mfiles.h"

#define FB_FD_MAXNUM  2048          ///支持最大的硬盘文件: 2048个
#define FB_BLOCK_MAXID  (int)(((unsigned int)1024*1024*1024*2)-1)      //2G-1, 20亿-1
#define FB_BLOCK_MAXSIZE (1024*1024*10)        ///一块最大为10M

///fileblock库 返回值的定义, >=0表示成功, <0表示失败. 失败情况的各种错误码如下:
enum fileblock_error_code_enum{ 
	FB_ERR_OTHER  = -1,      ///普通错误
	FB_ERR_EMPTY   = -2,      ///数据为空, 数据文件中存在空数据项
	FB_ERR_NOEXIST = -3,      ///数据块不存在, 意味超出了最大的块ID
	FB_ERR_DATA    = -4,      ///数据块不完整: 只读出一部分, 或只写一部分, 或数据长度和块长不符合

	FB_ERR_PARAM   = -6,      ///接口传入参数错误
	FB_ERR_FILE    = -7,      ///文件错误: 文件i/o, 文件打开
	FB_ERR_SYS     = -8       ///系统错误: malloc失败, 超出最大ID号
};

typedef struct fblock_t
{
	mfiles_t  mfiles; ///多文件的逻辑文件管理
	int  max_curbid;  //当前块号的最大号, reload时检测各个文件长度计算最大号. 添加数据时,会更新.

	int  blocksize;   //块大小.
	char module[32];
}fblock_t;

/**     
 *  初始化结构体. 检查数据是否正确
 *      
 *  @param[in]  pfb          block文件指针
 *  @param[in]  modulename   模块名    
 *  @param[in]  filename     文件名   
 *  @param[in]  blocksize    设定的块大小
 *  @param[in]  iswritesync  是否磁盘写同步
 *  @param[in]  isreadonly   是否只读
 *  @param[out] pfb          block文件指针，封装文件名等各种属性信息
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_FILE   文件打开错误
 *  - FB_ERR_DATA   数据错误
 *  - FB_ERR_SYS    block系统错误
 *  - 成功返回0
 */  
int fblock_init( fblock_t* pfb, const char *modulename, const char *filename, 
		int blocksize, bool iswritesync, bool isreadonly );

/**     
 *  关闭fblock. 如果clear_data置位，将清除所有数据文件
 *      
 *  @param[in]  pfb          block文件指针
 *  @param[in]  clear_data   是否清除文件(默认不清除)
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_FILE   文件打开错误
 *  - FB_ERR_DATA   数据错误
 *  - FB_ERR_SYS    block系统错误
 *  - 0 成功
 */
int fblock_fini( fblock_t* pfb, bool clear_data = false );

/**     
 *  根据block id 读取数据
 *      
 *  @param[in]  pfb       block文件指针
 *  @param[in]  blockid   读取的块id
 *  @param[in]  preadbuf  读取的buf地址
 *  @param[in]  bufsize   取出的长度
 *  @param[out] preadbuf  取出的数据
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_NOEXIST id不存在
 *  - FB_ERR_EMPTY   返回数据空
 *  - FB_ERR_DATA   数据错误
 *  - FB_ERR_FILE   文件打开错误
 *  - 0 成功
 */
int fblock_read( fblock_t* pfb,  int blockid, void* preadbuf, int bufsize );

/**     
 *  根据block id读取数据中的一部分，但不可跨块读取
 *      
 *  @param[in]  pfb       block文件指针
 *  @param[in]  blockid   读取的块id
 *  @param[in]  preadbuf  读取的buf地址
 *  @param[in]  bufsize   取出的长度
 *  @param[in]  off       block内部偏移量
 *  @param[out] preadbuf  取出的数据
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_NOEXIST id不存在
 *  - FB_ERR_EMPTY   返回数据空
 *  - FB_ERR_FILE   文件打开错误
 *  - 0 成功
 */
int fblock_read_partial( fblock_t* pfb,  int blockid, void* preadbuf, int bufsize, int off );


/**     
 *  根据blockid，将数据块写到相应位置
 *      
 *  @param[in]  pfb       block文件指针
 *  @param[in]  blockid   准备写入的块id
 *  @param[in]  data      写入的数据
 *  @param[in]  bufsize   写入的长度
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_DATA   数据错误
 *  - 0 成功
 */
int fblock_write( fblock_t* pfb, int blockid, const void* data, int datasize );

/**     
 *  根据blockid，修改块内部分数据
 *      
 *  @param[in]  pfb       block文件指针
 *  @param[in]  blockid   准备写入的块id
 *  @param[in]  data      写入的数据
 *  @param[in]  bufsize   写入的长度
 *  @param[in]  off       写入的块内偏移
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_DATA   数据错误
 *  - 0 成功
 */
int fblock_write_partial( fblock_t* pfb, int blockid, const void* data, int datasize, int off );


/**     
 *  磁盘同步
 *      
 *  @param[in]  pfb       block文件指针
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - 0 成功
 */
int fblock_sync( fblock_t* pfb );

typedef int (*fileblock_handler)(unsigned int id, void* pitem,void* pex);

/**     
 *  重载遍历 (采用File方式，能大大提高遍历的速度 ) 
 *      
 *  @param[in]  pfb          block文件指针
 *  @param[in]  pitembuf     为遍历操作准备的缓存
 *  @param[in]  itembufsize  每次读取的长度
 *  @param[in]  handler      遍历函数的句柄,如下定义：typedef int (*fileblock_handler)(unsigned int id, void* pitem,void* pex);
 *  @param[in]  pex          遍历函数的参数指针
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_SYS    系统错误
 *  - >=0 成功遍历的块的个数
 */
int fblock_traverse_load(fblock_t* pfb,void*pitembuf, int itembufsize, fileblock_handler handler,void* pex);


typedef int (*fileblock_handler_ex)(unsigned int id, bool *stop_flag, void* pitem,void* pex);

/**     
 *  重载遍历 (采用File方式，能大大提高遍历的速度 ) 
 *      
 *  @param[in]  pfb          block文件指针
 *  @param[in]  pitembuf     为遍历操作准备的缓存
 *  @param[in]  itembufsize  每次读取的长度
 *  @param[in]  handler      遍历函数的句柄,如下定义：typedef int (*fileblock_handler_ex)(unsigned int id, bool *stop_flag, void* pitem,void* pex); *stop_flag 设为true的时候可以使fblock_traverse_load在中途退出，不继续遍历
 *  @param[in]  pex          遍历函数的参数指针
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_SYS    系统错误
 *  - >=0 成功遍历的块的个数
 */
int fblock_traverse_load_ex(fblock_t* pfb,void*pitembuf, int itembufsize, fileblock_handler_ex handler,void* pex);


/**     
 *  次写入多个块
 *      
 *  @param[in]  pfb           block文件指针
 *  @param[in]  start_bid     写入的起始块的id
 *  @param[in]  block_num     写入的块的个数
 *  @param[in]  data          写入的数据
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_EMPTY  写入数据长度0
 *  - FB_ERR_DATA   数据错误
 *  - FB_ERR_FILE   文件错误
 *  - >=0 写成功的块数
 */
int fblock_multiwrite( fblock_t* pfb, int start_bid, int block_num, const void* data );

 
/**     
 *  一次写入多个, 添加到文件末尾
 *      
 *  @param[in]  pfb           block文件指针
 *  @param[in]  block_num     写入的块的个数
 *  @param[in]  data          写入的数据
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_EMPTY  写入数据长度0
 *  - FB_ERR_DATA   数据错误
 *  - FB_ERR_FILE   文件错误
 *  - >=0 写成功的块数
 */
int fblock_multiappend( fblock_t* pfb, int block_num, const void* data );

 
/**     
 *  一次读入多个
 *      
 *  @param[in]  pfb           block文件指针
 *  @param[in]  start_bid     读取的起始块的id
 *  @param[in]  block_num     读取的块的个数
 *  @param[in]  pbuf          读取的数据指针
 *  @return 
 *  - FB_ERR_PARAM  参数错误
 *  - FB_ERR_EMPTY  读取数据长度0
 *  - FB_ERR_DATA   数据错误
 *  - FB_ERR_FILE   文件错误
 *  - >=0 读成功的块数
 */
int fblock_multiread( fblock_t* pfb, int start_bid, int block_num, void* pbuf );

 
/**     
 *  检查是否有追加的新数据; 如果有新数据,则将max_curbid更新到文件数据相应的id号.
 *      
 *  @param[in]  pfb           block文件指针
 *  @return 
 *  - >0 表示有更新
 *  - <0 表示无更新
 *  - =0 暂时未定义
 *  @note 主要用于多进程共享文件, 能检查数据添加、更新的机制,目前不支持非连续块添加的追加检查
 */ 
int fblock_chk_newdata( fblock_t* pfb );


#endif
