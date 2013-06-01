 
/**
 * @file
 * @brief 实现逻辑大文件存取的封装
 **/
#ifndef _MULTIFILES_HEAD_FILE_
#define _MULTIFILES_HEAD_FILE_

#define MF_FD_MAXNUM  2048          ///支持最大的硬盘文件: 2048个
#define MF_PER_FILE_MAX_LEN (int)((unsigned int)1024*1024*1024*2-1024*1025)  //单个文件最大长度
#define MF_TRAVERSE_BUFSIZE (1024*1024*10)  ///遍历时 缓冲区的大小

//#define MF_FILES_MAX_SIZE (MF_PER_FILE_MAX_LEN*MF_FD_MAXNUM)
const long long MF_FILES_MAX_SIZE = (unsigned long long)((unsigned long long)MF_PER_FILE_MAX_LEN*MF_FD_MAXNUM);

#define DATAFILE_NAME_FMT "%s.%ddat"                      //数据文件名的格式

///mfiles库 返回值的定义, >=0表示成功, <0表示失败. 失败情况的各种错误码如下:
enum mfiles_error_code_enum{ 
	MF_ERR_OTHER  = -1,      ///普通错误
	MF_ERR_EMPTY   = -2,      ///数据为空, 数据文件中存在空数据项
	MF_ERR_NOEXIST = -3,      ///数据块不存在, 意味超出了最大的块ID
	MF_ERR_DATA    = -4,      ///数据块不完整: 只读出一部分, 或只写一部分, 或数据长度和块长不符合

	MF_ERR_PARAM   = -6,      ///接口传入参数错误
	MF_ERR_FILE    = -7,      ///文件错误: 文件i/o, 文件打开
	MF_ERR_SYS     = -8       ///系统错误: malloc失败, 超出最大ID号
};

typedef struct mfiles_t
{
	int  _rw_fd[MF_FD_MAXNUM];
	long long _cur_logiclen;  //文件组当前的数据逻辑长度. 该长度并不代表数据实际长度，因为有些文件空或少的情况.
	int _cur_logicfno;  //文件组当前最大号

	bool _iswrite_sync;      //是否写同步.
	bool _isreadonly;       //是否只读

	int  _file_maxsize; //单个文件的数据大小
	char _filepath[256]; //文件号基本路径.
	char _module[32];

	struct{ //load时检测到的数据, 只是刚启动时的检测情况，不代表运行后的实时反应.
		long long _cur_datalen; //启动时，检测各个文件的大小，得到的数据实际总长度. 运行时不变
		int  _last_fno, _last_foff;
	}_load;
}mfiles_t;

/**     
 *  初始化结构体. 检查数据是否正确
 *      
 *  @param[in]  pmf          大文件指针
 *  @param[in]  modulename   模块名    
 *  @param[in]  filename     文件名   
 *  @param[in]  filemaxsize  单个文件最大大小
 *  @param[in]  iswritesync  是否磁盘写同步
 *  @param[in]  isreadonly   是否只读
 *  @param[out] pfb          block文件指针，封装文件名等各种属性信息
 *  @return 
 *  - MF_ERR_PARAM  参数错误
 *  - MF_ERR_FILE   文件打开错误
 *  - MF_ERR_SYS    系统错误
 *  - 成功返回0
 */ 
int mfiles_init( mfiles_t* pmf, const char *modulename, const char *filename, 
		int filemaxsize, bool iswritesync, bool isreadonly );

/**     
 *  关闭mfiles. 如果clear_data置位，将清除所有数据文件
 *      
 *  @param[in]  pmf          大文件指针
 *  @param[in]  clear_data   是否清除文件(默认不清除)
 *  @return 
 *  - MF_ERR_PARAM  参数错误
 *  - MF_ERR_FILE   文件打开错误
 *  - 0 成功
 */
int mfiles_fini( mfiles_t* pmf, bool clear_data = false );


/**     
 *  磁盘同步
 *      
 *  @param[in]  pmf       大文件指针
 *  @return 
 *  - MF_ERR_PARAM  参数错误
 *  - 0 成功
 */
int mfiles_sync( mfiles_t* pmf );

typedef int (*mfiles_handler)(void* pbuf, int toreadlen, int readlen, long long offset, 
		bool *stop_flag, void* pex1, void* pex2 );

/**     
 *  重载遍历 (采用File方式，能大大提高遍历的速度 ) 
 *      
 *  @param[in]  pmf          大文件指针
 *  @param[in]  pbuf         为遍历操作准备的缓存
 *  @param[in]  toreadlen    每次读取的长度
 *  @param[in]  handler      遍历函数的句柄,如下定义：typedef int (*mfiles_handler)(void* pbuf, int toreadlen, int readlen, long long offset, bool *stop_flag, void* pex1, void* pex2 );
 *  @param[in]  pex1         遍历函数的参数1
 *  @param[in]  pex2	     遍历函数的参数2
 *  @return 
 *  - MF_ERR_PARAM  参数错误
 *  - MF_ERR_SYS    系统错误
 *  - >=0 成功遍历的块的个数
 */
int mfiles_traverse_read(mfiles_t* pmf, void*pbuf, int toreadlen, mfiles_handler handler,void* pex1, void* pex2);


/**     
 *  检查是否有追加的新数据; 如果有新数据,则将max_curbid更新到文件数据相应的id号.
 *      
 *  @param[in]  pmf           大文件指针
 *  @return 
 *  - >0 表示有更新
 *  - <0 表示无更新
 *  - =0 暂时未定义
 *  @note 主要用于多进程共享文件, 能检查数据添加、更新的机制,处于效率和实际应用的考虑，目前此函数不支持非连续存放的追加检查
 */ 
int mfiles_chk_newdata( mfiles_t* pmf );




/**     
 *  读数据  有可能有些文件没有数据，但是不会读取失败，而是空数据
 *      
 *  @param[in]  pmf       大文件指针
 *  @param[in]  buf       读取数据的buf
 *  @param[in]  count     读取数据的长度
 *  @param[in]  offset    读取数据的起始位置
 *  @return 
 *  - MF_ERR_PARAM  参数错误
 *  - MF_ERR_FILE   文件错误
 *  - >=0 读取到的长度
 *  @note 当读取到逻辑文件末尾时，会返回真正读到的长度，当准备读取的长度与实际读取的长度不匹配时，此时意味着文件结束
 */
int mfiles_pread( mfiles_t* pmf, void *buf, int count, long long offset );


/**     
 *  向逻辑大文件内写数据
 *      
 *  @param[in]  pmf       大文件指针
 *  @param[in]  buf       写入数据的buf
 *  @param[in]  count     写入数据的长度
 *  @param[in]  offset    写入数据的起始位置
 *  @return 
 *  - MF_ERR_PARAM  参数错误
 *  - MF_ERR_FILE   文件错误
 *  - >=0 写入数据的长度
 */
int mfiles_pwrite( mfiles_t* pmf, const void *buf, int count, long long offset );

/**     
 *  追加写数据
 *      
 *  @param[in]  pmf       大文件指针
 *  @param[in]  buf       写入数据的buf
 *  @param[in]  count     写入数据的长度
 *  @return 
 *  - MF_ERR_PARAM  参数错误
 *  - MF_ERR_FILE   文件错误
 *  - >=0 写入数据的长度
 *  @note append操作与系统的append操作不同,并不是线程安全的,因此安全的写操作限制在一个进程内完成
 */
int mfiles_append( mfiles_t* pmf, const void *buf, int count );

#endif
