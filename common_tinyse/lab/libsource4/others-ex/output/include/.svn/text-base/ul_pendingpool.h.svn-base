/**
 * @file
 * @brief PendingPool以C++类的方式提供，是一个TCP连接池，相当于为TCP连接提供一个缓冲区，以下为PendingPool提供\n
 * 的一组方法，可以通过在work_reset_item时设置bKeepAlive设置长短连接
 */


#ifndef __HEAD_PENDINGPOOL_
#define __HEAD_PENDINGPOOL_

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

class PendingPool {
  public:
	PendingPool();
	
/**
 *  从已就绪的队列中中取一个已建立好的连接
 *  
 *  @param[in]  handle  用于存储连接句柄的引用
 *  @param[in]  sock    用于存储连接socket的引用
 *  @param[out] handle  连接句柄
 *  @param[out] sock    连接socket
 *  @return 操作返回值
 * - true   成功
 * - false  失败
 */
	bool work_fetch_item(int &handle, int &sock);
	
	
	
	/**
	 * @brief 关闭连接句柄 
	 *
	 * @param [in] handle   : 连接句柄
	 * @param [in] bKeepAlive   : false短连接句柄关闭，true长连接句柄不马上关闭
	 * @return  void 
	**/
	void work_reset_item(int handle, bool bKeepAlive); 
	int mask(fd_set * pfs);
	
/**
 *  向PendingPool加入一个连接
 *  
 *  @param[in]  sock_work       已建立连接的套节字
 *  @param[out] 无
 *  @return 连接在PendingPool中的位置
 * - >=0 连接在PendingPool中的位置
 * - -1  失败
 */
	int insert_item(int sock_work); 
	void check_item(fd_set * pfs);


/**
 *  将offset位置的连接加入到已就绪的队列中
 *  
 *  @param[in]  offset       连接在PendingPool中的位置(insert_item()的返回值)
 *  @param[out] 无
 *  @return 连接在PendingPool中的位置
 * - 0  成功
 * - -1 失败
 */
	int queue_in(int offset);
	void set_timeout(int sec);
	
/**
 *  设置已就绪队列的长度，注意：此函数和set_socknum必须在其他函数之前进行调用，必须在线程\n
 *  访问pool前设置并且不能动态调整
 *  
 *  @param[in]  len       待设置的长度，注：队列中实际可以存储的数量为len-1
 *  @param[out] 无
 *  @return 无
 */
	void set_queuelen(int len);
	
/**
 *  设置可存储socket的数量，注意：此函数和set_queuelen必须在其他函数之前进行调用，必须在线程\n
 *  访问pool前设置并且不能动态调整
 *  
 *  @param[in]  num       待设置的数量
 *  @param[out] 无
 *  @return 无
 */
	void set_socknum(int num);
	
/**
 *  获取等待获取连接的线程数
 *  
 *  @param[in]  无
 *  @param[out] 无
 *  @return 等待获取连接的线程数
 */
	int get_freethread();
	
/**
 *  获取已就绪队列的长度，返回的长度比set_queuelen()设置的长度少一个
 *  
 *  @param[in]  无
 *  @param[out] 无
 *  @return 已就绪队列的长度
 */
	int get_queuelen();
	
	enum { MAX_SOCK = 500 };
	enum { QUEUE_LEN = 100 };
	enum { DEFAULT_TIMEOUT_SEC = 60 };
	typedef enum { NOT_USED = 0, READY, BUSY } eumStatus;
  private:
	struct SItem {
		int nSock;
		int nLastActive;
		int status;
	};

	int   m_socketNum;
	SItem *m_aySocket;

	int m_queueLen;
	int *m_ayReady;
	int m_nGet;
	int m_nPut;

	int m_nFreeThread;
	int m_nTimeOut;				// connection timeout value, in seconds

	pthread_mutex_t m_mutex;
	pthread_cond_t m_condition;

	pthread_mutex_t m_mutexCPU;
	pthread_cond_t m_condCPU;
	int m_nMaxSock;
};
#endif
