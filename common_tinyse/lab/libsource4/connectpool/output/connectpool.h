/**
 * @file connectpool.h
 * @brief connect pool
 *
 **/
#ifndef _CONNECTPOOL_H_
#define _CONNECTPOOL_H_


#include <pthread.h>

/**
 *   @brief connect pool  
 */ 
class ConnectPool
{
public:
	enum ErrorType {
		ERR_SUCCESS = 0,  //成功返回零
		ERR_FAILED = -1,  //一般错误
		ERR_CONN = -2,    //连接失败
		ERR_MEM = -3,     //内存分配失败
		ERR_RETRY = -4,   //所有重试失败
		ERR_PARAM = -5,   //传入参数有问题
		ERR_SOCK = -6,    //传入sock fd有问题，可能不存在
		ERR_ASSERT = -7   //原来的assert, 严重数据错误
	};

	static const int LOG_LEVEL_DISABLE = 0x7fff;

	/**
	 *   @brief 服务器配置信息，由调用者填写
	 *   
	 *   @param addr             服务器地址
	 *   @param port             服务器端口
	 *   @param ctimeout_ms      服务器连接超时，单位: ms
	 */ 
	struct server_conf_t {
		static const int MAX_ADDR_LEN = 128;
		char addr[MAX_ADDR_LEN];
		int port;
		int ctimeout_ms;//ms
	};

	/**
	 *   @brief  初始化
	 *   
	 *   @param serverConfs             每个服务器的配置
	 *   @param serverCount             服务器数
	 *   @param sockPerServer           每个服务器允许的连接数
	 *   @param connectRetry            连接重试次数
	 *   @param longConnect             是否长连接
	 *   @return  >=0表示成功，否则失败
	 *   
	 *   @see  server_conf_t
	 */ 
	int  Init(const server_conf_t serverConfs[], int serverCount, int sockPerServer, 
			  int connectRetry=1, bool longConnect=true);

	/**
	 *   @brief  设置日志输出，如果不调用此函数设置，不输出任何日志
	 *   
	 *   @param modulename           日志名，随日志头输出：[%s]
	 *   @param warning_level        warning日志对应ul_log级别, LOG_LEVEL_DISABLE为不输出
	 *   @param notice_level         notice日志对应ul_log级别, LOG_LEVEL_DISABLE为不输出
	 *   @param debug_level          debug日志对应ul_log级别, LOG_LEVEL_DISABLE为不输出
	 *   
	 *   @note 如果不调用此函数设置，不输出任何日志
	 */
	void SetLogInfo(const char modulename[], int warning_level, int notice_level, 
					int debug_level = LOG_LEVEL_DISABLE);

	/**
	 *   @brief  设置均衡策略参数
	 *   
	 *   @param visitRecordSize         最多记录条数
	 *   @param visitFailThreshold      判定服务器不可用的出错数
	 *   @param failScale               出错不访问因子，应当>=1.0
	 *   @param minVisitRatio           单个服务器最小访问机率, 单位: 1%
	 *   @param lvtQueueSize            最近访问线程号记录条数, 如果设为零, 则不检测线程号
	 *   @return  >=0表示成功，否则失败
	 *   
	 *   @note 请在Init之前调用
	 *         如果不调用此函数设置，这几个参数将使用以下默认值
	 */
	int SetBalanceInfo(int visitRecordSize = 1000, int visitFailThreshold = 600, 
					   double failScale = 3.0, double minVisitRatio = 2.0, int lvtQueueSize = 2);

	/**
	 *   @brief  取得一个可用的连接
	 *   
	 *   @param balanceCode    平衡参数, <0表示不使用balanceCode
	 *   @return  >=0为socket句柄，否则失败
	 *   
	 *   @note 主要是失败情况是连接服务器失败(ERR_CONN)，
	 *         但是不建议用户对FetchSocket做retry，因为FetchSocket已经做了retry，
	 *         connectRetry参数可以在Init函数设置
	 */ 
	int  FetchSocket(int balanceCode = -1);

	/**
	 *   @brief  释放一个连接   
	 *   
	 *   @param sock              socket句柄
	 *   @param errclose          是否请求失败, 主要指通讯上的失败(需要进行retry的失败), 不是数据逻辑上的失败
	 *   
	 *   @return  >=0表示成功，否则失败
	 */ 
	int  FreeSocket(int sock, bool errclose);

	/**  @brief call back for query server
	 *
	 *   @return FAILED_AND_RETRY for retry, 
	 *           others will return to caller by funcRet of QueryWithRetry
	 */
	typedef int query_func_t(int sock, void* arg0, void *arg1, void* arg2);
	static const int FAILED_AND_RETRY = -0xffff;

	/**
	 *   @brief 带重试的请求函数
	 *   
	 *   @param func                    请求使用的回调函数，由调用者定义
	 *   @param arg0,arg1,arg2          回调函数参数
	 *   @param funcRet                 回调函数返回值, 如果不需要, 可以设为NULL
	 *   @param retryCount              请求重试次数
	 *   @param balanceCode             平衡参数, <0表示不使用balanceCode
	 *   
	 *   @return >=0表示成功，否则失败, 
	 *           特别地，如果所有重试都失败返回ERR_RETRY，如果连接失败返回ERR_CONN
	 */ 
	int  QueryWithRetry(query_func_t func, void* arg0, void *arg1, void* arg2, int* funcRet,
						int retryCount, int balanceCode = -1);
	
	ConnectPool();
	~ConnectPool();	

	int  GetCurSockCount() { return m_serverCount * m_sockPerServer; }
	bool GetLongConnect()  { return m_longConnect;  }	

protected:
	enum Status{
		CPS_BUSY=0,
		CPS_READY=1,
		CPS_INVALID=2		
	};
	
	struct sock_info_t {
		int sock;
		int status;
	};

	struct server_info_t {
		static const int MAX_ADDR_LEN = 128;
		char addr[MAX_ADDR_LEN];
		int port;
		int ctimeout_ms;

		sock_info_t* socks;
		int cur_pos;
		int free_count;

		//VISIT RECORD INFO
		struct visit_record_t {
			char       flag;
		};
		visit_record_t* record_queue;
		int queue_size;
		int queue_pos;
		int fail_count;

		void queue_insert(char flag)
			{
				if(record_queue[queue_pos].flag != flag) {
					if(flag>0) {
						fail_count++;
					} else {
						fail_count--;
					}
				}
				record_queue[queue_pos].flag = flag;
				queue_pos = (queue_pos+1) % queue_size;
			}
		
		//LAST VISIT THREAD INFO
		static const int MAX_LVT_QUEUE_SIZE = 10;
		pthread_t lvt_queue[MAX_LVT_QUEUE_SIZE];
		int lvt_queue_size;
		int lvt_queue_pos;
		void lvt_insert(pthread_t tid) 
			{
				if(lvt_queue_size > 0) {
					lvt_queue[lvt_queue_pos] = tid;
					lvt_queue_pos = (lvt_queue_pos+1) % lvt_queue_size;
				}
			}

		bool lvt_find(pthread_t tid) const
			{	
				for(int i=0; i<lvt_queue_size; ++i) {
					if(lvt_queue[i] == tid) {
						return true;
					}
				}
				return false;
			}
		bool is_last_visit_failed() const
			{	
				return lvt_find(pthread_self());
			}
	};

	struct log_info_t {
		log_info_t() 
			: warning_level(LOG_LEVEL_DISABLE), notice_level(LOG_LEVEL_DISABLE), debug_level(LOG_LEVEL_DISABLE) 
			{
				modulename[0] = '\0';
			}
		int warning_level;
		int notice_level;
		int debug_level;
		static const int LOG_MODULENAME_LEN = 64;
		char modulename[LOG_MODULENAME_LEN];
	};

	static const int ratio_precision = 10000;//概率计算，万分之一的精度

	int ConnectServer(int idx);
	int __check_connection(int sock);
	int __FetchSocket(int balanceCode, int& oldStatus);
	int __FreeSocket(int sockIdx, bool errclose);
	int __find_free(sock_info_t* array, int size);
	int __find_free(server_info_t* array, int size, int* cur_pos, int find_alive_only);

	int select_server(int balanceCode);
	bool is_use_select_server(int index);

protected:
	int  m_serverCount;	
	int  m_sockPerServer;

	int  m_freeSockCount;
	int  m_nextServerIdx;
	int  m_nextAliveServerIdx;
	server_info_t* m_serverArr;
	sock_info_t* m_sockArr;
    
	pthread_mutex_t m_mutex;
	pthread_cond_t m_condition;	

	bool m_longConnect;
	int  m_retryCount;

	double  m_minVisitRatio;//percent
	int  m_visitRecordSize;
	int  m_visitFailThreshold;
	double  m_failScale;
	int  m_lvtQueueSize;

	log_info_t m_log;
};

#endif


