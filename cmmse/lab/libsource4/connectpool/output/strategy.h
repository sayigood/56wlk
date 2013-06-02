/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file strategy.h
 * @brief 
 *  
 **/


#ifndef  __STRATEGY_H_
#define  __STRATEGY_H_

#include "connection.h"

namespace comcm{
	enum{
		//--- errer code for ConnectionRequest.err ---
		REQ_OK = 0,
		REQ_SVR_FULL,
		REQ_CONN_FAILED,
		REQ_SVR_DISABLE,
        REQ_SVR_SELECT_FAIL
	};

	enum{
		//--- error code for Connetion::setErrno / getErrno
		ERR_OK = 0,
		ERR_TALK_FAIL, //交互失败
		ERR_DEAD_LIKE, //假死，能连接，不能发送、接收
		ERR_OTHER
	};

	enum{
		CONN_FREE = 1
	};


	class ConnectionRequest{
		public:
		int key;		  /**< 请求的hash参考值       */
		int nthRetry;		  /**< 第几次重试，0表示第一次选择       */
		int serverID;		  /**< 上一次选择的serverID       */
		int err;		  /**< 上一次连接的错误信息       */
		Connection * conn;		  /**< 上一次连接所得到的Connection(可能为NULL)       */
		const char * tag;		  /**< 指定tag号       */
		ConnectionRequest(){};
		virtual ~ConnectionRequest(){};
	};


	//--------------------------------------------------------------------
	class Strategy{
		protected:
			ConnectManager * _mgr;
			BasicLogAdapter * _log;
			time_t _last_reload;
		public:
			//从ConnectManager中选取一台Server
			/**
			 * @brief 虚函数：从ConnectManager中选取一台server
			 * 		具体行为由子类定义
			 * 		此操作由ConnectManager调用，并且在调用时已加对ConnectManager的读锁
			 * 		不能启用、停用server，但可以对单台server进行一些写操作
			 * 		
			 *
			 * @param [in/out] req   : const ConnectionRequest*
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int selectServer(const ConnectionRequest * req);
			//通知一个事件
			/**
			 * @brief 通知一个事件
			 * 			仅仅起到通知的作用，Strategy自行决定如何处理通知
			 *			比如：eve = CONN_FREE;表示一个Connection被归还
			 *			此接口的作用只是告知而已，strategy可以不理会此通知
			 *
			 * @param [in] conn   : Connection*
			 * @param [in] eve   : int
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int notify(Connection * conn, int eve);

            /**
             * @brief 更新策略状态 
             *
             * 一些情况下用户需要控制策略的状态，比如配置的更新
             * 对于策略的影响
             *
             * @return  0为成功 非0失败
            **/
            virtual int update();
			/**
			 * @brief 设置自己的Manager
			 * 			一般不需要用户调用此接口
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setManager(ConnectManager * ); //设置Manager对象
			/**
			 * @brief 获取Manager
			 *
			 * @return  const ConnectManager* 
			 * @retval   
			 * @see 
			**/
			const ConnectManager * getManager();

			Strategy();
			virtual ~Strategy();
	};
	//--------------------------------------------------------------------
	class HealthyChecker{
		protected:
			ConnectManager * _mgr;
			BasicLogAdapter * _log;
			pthread_t _tid;
			bool _single_thread;
			friend void * thread_worker(void * pd);
			struct AsyncWorker{
				int seconds;
				HealthyChecker * checker;
				bool run;
			}_worker;
		public:
			//从ConnectManager中选取一台Server
			/**
			 * @brief 检查自己所绑定的Manager，看看其上server是否是健康
			 * 		行为由子类决定
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int healthyCheck();
			//启动一个专门的线程来运行healthyCheck
			/**
			 * @brief 启动一个新的线程，并用它来执行this->healthyCheck();
			 *
			 * @param [in] seconds   : int 间隔时间 (seconds)
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int startSingleThread(int seconds = 5);
			/**
			 * @brief 终止自己启动的线程
			 *
			 * @return  void 
			 * @retval   
			 * @see 
			**/
			void joinThread();
			/**
			 * @brief 设置我的Manager
			 * 			一般用户不需要调用此接口
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setManager(ConnectManager *);
			/**
			 * @brief 获取Manager
			 *
			 * @return  const ConnectManager* 
			 * @retval   
			 * @see 
			**/
			const ConnectManager * getManager();
			HealthyChecker();
			virtual ~HealthyChecker();
	};

}//namespace comcm













#endif  //__STRATEGY_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
