/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file socketpool.h
 * @brief 
 *  
 **/


#ifndef  __SOCKETPOOL_H_
#define  __SOCKETPOOL_H_

#include "connectmgr.h"
#include "bsl/map.h"
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "connectpool.h"

class SocketDefaultStrategy;
class SocketDefaultChecker;
class SocketConnection;

class SocketPool{
	protected:
		BasicLogAdapter *_log;
		comcm::ConnectManager _manager;
		comcm::ConnectManager * _mgr;
		comcm::Strategy * _stt;
		comcm::HealthyChecker * _checker;
		SocketDefaultStrategy * _def_stt;
		SocketDefaultChecker * _def_checker;
		bsl::phashmap < int, comcm::Connection * > _sockmap;
        int _retry_time;        /**< 重试次数       */
	public:
		/**
		 * @brief 初始化函数，从一个Configure结构体初始化SocketPool
		 * 			不能重复init
		 *
		 * @param [in] conf   : const comcfg::ConfigUnit&
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int init(const comcfg::ConfigUnit & conf);
		/**
		 * @brief 初始化函数，用server列表初始化ConnectManager
		 * 			不能重复init
		 *
		 * @param [in] svrs   : Server** server指针数组，用户自己管理内存，
		 * 						ConnectManager内部会拷贝一份
		 * @param [in] svrnum   : int server数量
		 * @param [in] shortConnection   : bool 是否短连接
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int init(comcm::Server ** svrs, int svrnum, bool shortConnection);

        virtual int init(const ConnectPool::server_conf_t serverConfs[], int serverCount,
                         int sockPerServer, int connectRetry=1, bool longConnect=true);

		/**
		 * @brief 设置选择Server的策略
		 * 			初始化时操作，非线程安全
		 *
		 * @param [in] stt   : Strategy*
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		int setStrategy(comcm::Strategy * stt);
		/**
		 * @brief 设置健康检查策略
		 * 			初始化时操作，非线程安全
		 *
		 * @param [in] hchk   : HealthyChecker*
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		int setHealthyChecker(comcm::HealthyChecker * hchk);

		/**
		 * @brief 运行时更新server配置（仅限Server，不更新其它配置）
		 * 			不能两个线程同时reload
		 *
		 * @param [in] conf   : const comcfg::ConfigUnit&
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		int reload(const comcfg::ConfigUnit & conf);
		/**
		 * @brief 运行时更新server配置
		 * 			不能两个线程同时reload
		 *
		 * @param [in] svrs   : Server 指针数组
		 * @param [in] svrnum   : 数组个数
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		int reload(comcm::Server ** svrs, int svrnum);

		/**
		 * @brief 获取一个可用的连接
		 *
		 * @param [in] key   : int 参考的选取参数
		 * @param [in] waittime   : int 如果没有可用连接，则等待多少毫秒
         * @param [in] *err : int * 如果FetchSocket失败返回 错误号,包括
         *          comcm::REQ_SVR_FULL,
         *          comcm::REQ_CONN_FAILED,
         *          comcm::REQ_SVR_DISABLE
		 * @return  成功返回的sock 
         * -1 失败 >0成功建立连接的sock
		 * @retval   
		 * @see 
		**/
		int FetchSocket(int key = -1, int waittime = 0, int *err = NULL);
		/**
		 * @brief 归还一个连接
		 *
		 * @param [in] sock   : int
		 * @param [in] errclose   : bool 是否出错了
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		int FreeSocket(int sock, bool errclose);

		/**
		 * @brief 根据sock获取对应的Connection
		 * 			便于对Connection的直接操作
		 *
		 * @param [in] sock   : int
		 * @return  Connection* 
		 * @retval   
		 * @see 
		**/
		comcm::Connection * getConnectionBySock(int sock);

		/**
		 * @brief 设置BasicLogAdapter
		 *
		 * @param [in/out] logger   : BasicLogAdapter*
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		int setLogAdapter(BasicLogAdapter * logger);
		/**
		 * @brief 获取BasicLogAdapter
		 *
		 * @return  BasicLogAdapter* 
		 * @retval   
		 * @see 
		**/
		BasicLogAdapter * getLogAdapter();

        /**
         * @brief 设置失败的重试次数
         * 主要针对connectmgr中的 defaultRetryTime控制最大重试次数
         *
         * 一般情况下不需要使用，用户只要在selectServer参数中判断req->nthRetry
         *
         * @return  int 
        **/
        int setRetryTime(int retry_time);


        /**
         * @brief 获取当前设置的重试次数
         *
         * @return  int 
        **/
        int getRetryTime();

		/**
		 * @brief 获取底层的ConnectManager
		 * 			SocketPool是对ConnectManager的包装
		 * 			此接口便于用户的直接操作
		 *
		 * @return  comcm::ConnectManager* 
		 * @retval   
		 * @see 
		**/
		comcm::ConnectManager * getManager();
		SocketPool();
		virtual ~SocketPool();
};
		
class SocketConnection : public comcm::Connection {
	protected:
		int _sock;
	public:
        SocketConnection();
		virtual void * getHandle();
		virtual int connect(comcm::Server * );
		virtual int disconnect();
		virtual int ping();
};

const int DEFAULT_WEIGHT = 1024;
class SocketDefaultStrategy : public comcm::Strategy{
		int _svr_status[comcm::MAX_SVR_NUM];
		int _avail_num;
		int _avail_list[comcm::MAX_SVR_NUM];
		time_t _last_reload;
		comcm::RWLock _lock;
	public:
		SocketDefaultStrategy();
		virtual int selectServer(const comcm::ConnectionRequest * req);
		virtual int notify(comcm::Connection * conn, int eve);
		virtual int update();
};

class SocketDefaultChecker : public comcm::HealthyChecker{
	public:
		SocketDefaultChecker();
		virtual int healthyCheck();
};

comcm::Server * socket_server_creater();
comcm::Connection * socket_conn_creater();











#endif  //__SOCKETPOOL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
