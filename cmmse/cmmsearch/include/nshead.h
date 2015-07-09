/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file nshead.h
 * @brief 
 *
 * 为了统一多个产品线互联时的交互接口，特定义nshead_t为产品线网络交互统一的
 * 数据包头。本函数库提供一些列支持此规范的网络读写函数。
 *  
 *  加了_ex后缀的接口在性能上有所提高，具体可参看nshead的wiki页面
 **/

#ifndef _NSHEAD_H
#define _NSHEAD_H

#include <unistd.h>
#include <sys/uio.h>
static const unsigned int NSHEAD_MAGICNUM = 0xfb709394;

typedef struct iovec iovec_t;		//发送或接收的多个buf定义

/*struct iovec defined in <sys/uio.h>
struct iovec {
    __ptr_t iov_base;    // Starting address
    size_t iov_len;      // Length in bytes 
};
*/

/// 返回错误码 = NSHEAD_RET_SUCCESS成功, <0失败
typedef enum _NSHEAD_RET_ERROR_T {
    NSHEAD_RET_SUCCESS       =   0, ///<读写OK
    NSHEAD_RET_EPARAM        =  -1, ///<参数有问题
    NSHEAD_RET_EBODYLEN      =  -2, ///<变长数据长度有问题
    NSHEAD_RET_WRITE         =  -3, ///<写的问题
    NSHEAD_RET_READ          =  -4, ///<读消息体失败，具体错误看errno
    NSHEAD_RET_READHEAD      =  -5, ///<读消息头失败, 具体错误看errno
    NSHEAD_RET_WRITEHEAD     =  -6, ///<写消息头失败, 可能是对方将连接关闭了
    NSHEAD_RET_PEARCLOSE     =  -7, ///<对端关闭连接
    NSHEAD_RET_ETIMEDOUT     =  -8, ///<读写超时
    NSHEAD_RET_EMAGICNUM     =  -9, ///<magic_num不匹配
    NSHEAD_RET_UNKNOWN	     =  -10
} NSHEAD_RET_ERROR_T;

/// 读写标志位，所有读写函数默认NSHEAD_CHECK_MAGICNUM
typedef enum _NSHEAD_FLAGS_T {
    NSHEAD_CHECK_NONE        =    0,
    NSHEAD_CHECK_MAGICNUM    = 0x01,    ///<检查magic_num是否正确
    NSHEAD_CHECK_PARAM       = 0x02,    ///<检查参数是否正确
} NSHEAD_FLAGS_T;

/**
 * ns产品线网络交互统一的包头，注释包含为(M)的为必须遵循的规范
 */
typedef struct _nshead_t
{
    unsigned short id;              ///<id
    unsigned short version;         ///<版本号
    ///(M)由apache产生的logid，贯穿一次请求的所有网络交互
    unsigned int   log_id;
    ///(M)客户端标识，建议命名方式：产品名-模块名，比如"sp-ui", "mp3-as"
    char           provider[16];
    ///(M)特殊标识，标识一个包的起始
    unsigned int   magic_num;
    unsigned int   reserved;       ///<保留
    ///(M)head后请求数据的总长度
    unsigned int   body_len;
} nshead_t;

/**
 *@brief 响应消息头
 */
/*
typedef struct _nsreshead_t{
    nshead_t head;		///
    int status;			///<响应状态
}nsreshead_t;
*/

//这里使用的默认参数和重载，纯C程序不能使用
#ifdef __cplusplus
/**
 * @brief 根据nshead系列函数的错误码得到错误的字符串描述
 * @param ret the error number return by nshead_read? or nshead_write?
 * @return the error string;
 */
const char* nshead_error(int ret);


/** 
 * @brief 读数据, 实际读取数据 head->body_len
 * 
 * @param sock socket 句柄
 * @param buf 读缓冲区
 * @param buf_size 读入缓冲区的最大大小
 * @param timeout 超时
 * @param flags 定义如上
 * 
 * @return 
 */
int nshead_read(int sock, void *buf, size_t buf_size, int timeout,
	        unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 读数据，实际读取数据head->body_len
 * @param sock socket句柄
 * @param buf 读缓冲区
 * @param buf_size 读缓冲区的最大大小，body_len>buf_size时返回错误NSHEAD_RET_BODYLEN
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_read(int sock, nshead_t *head, void *buf,
		size_t buf_size, int timeout,
		unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 读数据，实际读取数据head->body_len
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_read(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, size_t buf_size, int timeout,
		unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 *@brief 多段读，先读取一个nshead，然后读取多段数据，填入iovs的iov_base所指空间，长度由iov_len指定
 *@param iovs 读取时要填写的结构，iov_base指定各buf地址，iov_len指定各buf长度
 *@param iov_count iovs数组大小
 *@notes 实际读取的长度由head中body_len,除非iovs空间不够或者读取错误，iovs中每个iov_len必须填写。
 *       目前的超时控制不精确，可以理解为每次能否读取的响应超时
 *@return 成功返回0，异常返回对应的错误码
 */
int nshead_readv(int sock, 
	nshead_t *head, 
	iovec_t *iovs, int iov_count, 
	int timeout, unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 写数据, 一次写完所有数据。发送总长度为sizeof(nshead_t)+head->body_len;
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_write(int sock, nshead_t *head, int timeout,
	unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 写数据, 数据分2部分写。第2部分的数据长度 = body_len
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_write(int sock, nshead_t *head, void *buf, int timeout,
	unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 写数据, 数据分3部分写，第三部分数据长度 = body_len - req_size
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_write(int sock, nshead_t *head, void *req, size_t req_size,
	void *buf, int timeout, unsigned flags = NSHEAD_CHECK_MAGICNUM);


/**
 *@brief 多段写，先写入一个head，然后写入多段数据，iovs的iov_base指向要发送的空间，长度由iov_len指定
 *@param iovs 发送时要填写的结构，iov_base指定各buf地址，iov_len指定各buf长度
 *@param iov_count iovs数组大小
 *@notes 实际发送的长度由head中body_len指定,iovs中每个iov_len必须填写。
 *       目前的超时控制不精确，可以理解为每次能否写入的响应超时
 *@return 成功返回0，异常返回对应的错误码
 */
int nshead_writev(int sock, 
	nshead_t *head, 
	iovec_t *iovs, int iov_count, 
	int timeout, unsigned flags = NSHEAD_CHECK_MAGICNUM);

// 下面的_ex版本使用了ullib中新的读写超时函数，提升了一定的性能

/** 
 * @brief 读数据, 实际读取数据 head->body_len
 * 
 * @param sock socket 句柄
 * @param buf 读缓冲区
 * @param buf_size 读入缓冲区的最大大小
 * @param timeout 超时
 * @param flags 定义如上
 * 
 * @return 
 */
int nshead_read_ex(int sock, void *buf, size_t buf_size, int timeout,
	        unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 读数据，实际读取数据head->body_len
 * @param sock socket句柄
 * @param buf 读缓冲区
 * @param buf_size 读缓冲区的最大大小，body_len>buf_size时返回错误NSHEAD_RET_BODYLEN
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_read_ex(int sock, nshead_t *head, void *buf,
		size_t buf_size, int timeout,
		unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 读数据，实际读取数据head->body_len
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_read_ex(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, size_t buf_size, int timeout,
		unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 *@brief 多段读，先读取一个nshead，然后读取多段数据，填入iovs的iov_base所指空间，长度由iov_len指定
 *@param iovs 读取时要填写的结构，iov_base指定各buf地址，iov_len指定各buf长度
 *@param iov_count iovs数组大小
 *@notes 实际读取的长度由head中body_len,除非iovs空间不够或者读取错误，iovs中每个iov_len必须填写。
 *       目前的超时控制不精确，可以理解为每次能否读取的响应超时
 *@return 成功返回0，异常返回对应的错误码
 */
int nshead_readv_ex(int sock, 
	nshead_t *head, 
	iovec_t *iovs, int iov_count, 
	int timeout, unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 写数据, 一次写完所有数据。发送总长度为sizeof(nshead_t)+head->body_len;
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_write_ex(int sock, nshead_t *head, int timeout,
	unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 写数据, 数据分2部分写。第2部分的数据长度 = body_len
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_write_ex(int sock, nshead_t *head, void *buf, int timeout,
	unsigned flags = NSHEAD_CHECK_MAGICNUM);

/**
 * 写数据, 数据分3部分写，第三部分数据长度 = body_len - req_size
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_write_ex(int sock, nshead_t *head, void *req, size_t req_size,
	void *buf, int timeout, unsigned flags = NSHEAD_CHECK_MAGICNUM);


/**
 *@brief 多段写，先写入一个head，然后写入多段数据，iovs的iov_base指向要发送的空间，长度由iov_len指定
 *@param iovs 发送时要填写的结构，iov_base指定各buf地址，iov_len指定各buf长度
 *@param iov_count iovs数组大小
 *@notes 实际发送的长度由head中body_len指定,iovs中每个iov_len必须填写。
 *       目前的超时控制不精确，可以理解为每次能否写入的响应超时
 *@return 成功返回0，异常返回对应的错误码
 */
int nshead_writev_ex(int sock, 
	nshead_t *head, 
	iovec_t *iovs, int iov_count, 
	int timeout, unsigned flags = NSHEAD_CHECK_MAGICNUM);



#endif


//为纯C程序提供的接口,去除了重载和默认参数
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief 根据nshead系列函数的错误码得到错误的字符串描述
 * @param ret the error number return by nshead_read? or nshead_write?
 * @return the error string;
 */
const char* nshead_error_c(int ret);



/**
 * 读数据，实际读取数据head->body_len
 * 参数意义与nshead_read中参数相同的接口一样
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_read_c(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, size_t buf_size, int timeout,
		unsigned flags);

/**
 *@brief 多段读，先读取一个nshead，然后读取多段数据，填入iovs的iov_base所指空间，长度由iov_len指定
 *@param iovs 读取时要填写的结构，iov_base指定各buf地址，iov_len指定各buf长度
 *@param iov_count iovs数组大小
 *@notes 实际读取的长度由head中body_len,除非iovs空间不够或者读取错误，iovs中每个iov_len必须填写。
 *       目前的超时控制不精确，可以理解为每次能否读取的响应超时
 *@return 成功返回0，异常返回对应的错误码
 */
int nshead_readv_c(int sock, 
	nshead_t *head, 
	iovec_t *iovs, int iov_count, 
	int timeout, unsigned flags);


/**
 * 写数据, 数据分3部分写，第三部分数据长度 = body_len - req_size
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_write_c(int sock, nshead_t *head, void *req, size_t req_size,
	void *buf, int timeout, unsigned flags);


/**
 *@brief 多段写，先写入一个head，然后写入多段数据，iovs的iov_base指向要发送的空间，长度由iov_len指定
 *@param iovs 发送时要填写的结构，iov_base指定各buf地址，iov_len指定各buf长度
 *@param iov_count iovs数组大小
 *@notes 实际发送的长度由head中body_len指定,iovs中每个iov_len必须填写。
 *       目前的超时控制不精确，可以理解为每次能否写入的响应超时
 *@return 成功返回0，异常返回对应的错误码
 */
int nshead_writev_c(int sock, 
	nshead_t *head, 
	iovec_t *iovs, int iov_count, 
	int timeout, unsigned flags);

//下面加_ex的接口的性能相对同名未加_ex的接口有所提高，具体可参看nshead的wiki页面

/**
 * 读数据，实际读取数据head->body_len
 * 参数意义与nshead_read中参数相同的接口一样
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_read_c(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, size_t buf_size, int timeout,
		unsigned flags);

/**
 *@brief 多段读，先读取一个nshead，然后读取多段数据，填入iovs的iov_base所指空间，长度由iov_len指定
 *@param iovs 读取时要填写的结构，iov_base指定各buf地址，iov_len指定各buf长度
 *@param iov_count iovs数组大小
 *@notes 实际读取的长度由head中body_len,除非iovs空间不够或者读取错误，iovs中每个iov_len必须填写。
 *       目前的超时控制不精确，可以理解为每次能否读取的响应超时
 *@return 成功返回0，异常返回对应的错误码
 */
int nshead_readv_c(int sock, 
	nshead_t *head, 
	iovec_t *iovs, int iov_count, 
	int timeout, unsigned flags);


/**
 * 写数据, 数据分3部分写，第三部分数据长度 = body_len - req_size
 * @return 成功返回0，异常返回对应的错误码
 */
int nshead_write_c(int sock, nshead_t *head, void *req, size_t req_size,
	void *buf, int timeout, unsigned flags);


/**
 *@brief 多段写，先写入一个head，然后写入多段数据，iovs的iov_base指向要发送的空间，长度由iov_len指定
 *@param iovs 发送时要填写的结构，iov_base指定各buf地址，iov_len指定各buf长度
 *@param iov_count iovs数组大小
 *@notes 实际发送的长度由head中body_len指定,iovs中每个iov_len必须填写。
 *       目前的超时控制不精确，可以理解为每次能否写入的响应超时
 *@return 成功返回0，异常返回对应的错误码
 */
int nshead_writev_c(int sock, 
	nshead_t *head, 
	iovec_t *iovs, int iov_count, 
	int timeout, unsigned flags);


#ifdef __cplusplus
};
#endif

/**
 *@brief 读取响应消息头和body 
 *  head里指定长度
 *@return 成功返回0，异常返回对应的错误码
 */
/*
int nsreshead_read (int sock,nsreshead_t *head,void *buf,unsigned int len,
	        int timeout,unsigned flags = NSHEAD_CHECK_MAGICNUM);
*/
/**
 *@brief 写以响应消息头为开头的数据, 数据分2部分写，第2部分数据长度 head里指定长度
 *@return 成功返回0，异常返回对应的错误码
 */
/*
int nsreshead_write (int sock,nsreshead_t *head,void *buf,
	int timeout,unsigned flags = NSHEAD_CHECK_MAGICNUM);
*/
/**
 *@brief 完成一次基于nshead_t请求和nsreshead_t响应的调用
 */
/*
int nshead_invoke (int sock,const nshead_t *req_head,void *req_body,
	nsreshead_t *res_head,void *res_body,unsigned int res_len,
	int timeout,unsigned flags = NSHEAD_CHECK_MAGICNUM);

void nshead_set (nshead_t *head,
	unsigned short id,unsigned short version,
	unsigned int log_id,char *provider,
	unsigned int body_len);
*/


#endif

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
