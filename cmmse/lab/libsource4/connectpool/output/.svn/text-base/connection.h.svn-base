/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file connection.h
 * @brief 
 *  
 **/


#ifndef  __CONNECTION_H_
#define  __CONNECTION_H_

#include <time.h>
#include <Configure.h>
#include <basiclogadapter.h>

#include "connectmgr_utils.h"

namespace comcm{
	//--------------------------------------------------------------------
	class ConnectManager;
	class Server;
	const int MAX_SVR_NUM = 4096;
	const int DEFAULT_RETRY_TIME = 10;
	const int DEFAULT_CAPACITY = 20;
	const int DEFAULT_CTIMEOUT = 400;//ms
	const int SHORT_CONN = 1;
	const int LONG_CONN = 0;
	enum { 
		//--- kernel status of Connection ---
		CONN_NOT_USED = 0,
		CONN_IS_USING = 1,
		CONN_IS_CONNECTED = 2,
		CONN_BROKEN = 3		  /**<  连接已断掉或有故障      */
	};
	typedef void (* data_destructor_t) (void *);

	//---Connection是无锁的，只能被一个线程操作----
	class Connection{
		protected:
			BasicLogAdapter * _log;
			int _kernelStatus;
			int _userStatus;
			void * _handle;
			void * _self_data;
			data_destructor_t _self_cleaner;
			int _err;
		public: //---public是为了让扩展的Server可以使用---
			int _serverID;
			time_t _lastActive;
			int _id_in_svr;//在server中的id
			int _id_in_cm;//在ConnectManager中的id
			friend class Server;
			friend class ConnectManager;
			void clear();
		public:
			void setKernelStatus(int st);
			int getKernelStatus();//获取内部状态（已连接、已断开等）
			Connection();
			virtual ~Connection();
			/**
			 * @brief 获取裸露的句柄，使用void*是为了兼容各种类型，如：
			 * 			如果是socket类型，则int sock = *(int *)getHandle();
			 *
			 * @return  void* 
			 * @retval   
			 * @see 
			**/
			virtual void * getHandle();//获取裸露句柄
			/**
			 * @brief 设置用户态的连接状态，0表示初始值，其它均未定义
			 * 			值的定义完全由用户决定，ConnectManager不会碰用户态的数据
			 * 			在初始化的时候，这个用户态值被置为0.
			 *
			 * @param [in] st   : int
			 * @return  void 
			 * @retval   
			 * @see 
			**/
			void setStatus(int st);//设置连接状态，用户自定义值，如有些长连接可能需要设置当前的状态信息
			/**
			 * @brief 获取用户态的状态值
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int getStatus();//获取用户设置的连接状态

			/**
			 * @brief 设置错误信息，0表示无错误
			 * 			此错误号可以被Strategy获取。具体定义由Strategy决定
			 *
			 * @param [in] err   : int
			 * @return  void 
			 * @retval   
			 * @see 
			**/
			void setErrno(int err);//设置错误信息
			/**
			 * @brief 获取错误号
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int getErrno();

			/**
			 * @brief 设置一些自定义的数据，供Strategy之类的使用
			 * 		Connection会在恰当的时候，调用destructor释放这些数据
			 * 		
			 *
			 * @param [in/out] d   : void* 数据内容
			 * @param [in/out] _dest   : data_destructor_t 析构数据的函数指针
			 * @return  void 
			 * @retval   
			 * @see 
			**/
			void setSelfData(void * d, data_destructor_t _dest = NULL);//设置一些自定义数据
			/**
			 * @brief 获取自定义的数据
			 *
			 * @return  void* 
			 * @retval   
			 * @see 
			**/
			void * getSelfData();

			/**
			 * @brief 当前server连接到的是哪台server
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int serverID();//连接的是哪个server

			/**
			 * @brief 最近一次活动时间（最近一次修改kernelStatus的时间）
			 *
			 * @return  time_t 
			 * @retval   
			 * @see 
			**/
			time_t lastActive();

			/**
			 * @brief 虚函数：连接服务器
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int connect(Server * );
			/**
			 * @brief 虚函数：断开连接
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int disconnect();
			/**
			 * @brief 虚函数：检查连接是否存活（长连接使用,ConnectManager内部使用）
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int ping();//检查连接是否存活
	};
	//--------------------------------------------------------------------
	//-----Server是有较多锁保护的，除非有特殊说明，否则该接口就是线程安全的----
	class Server{
		protected:
			char _ip[16];
			unsigned int _ip_int;
			int _port;
			int _id; //在ConnectManager中所分配到的id
			int _capacity;//最多可以连接多少个
			int _ctimeout;//连接超时ms
			int _liveConn;//活动的连接数
			int _pendingLongConn;//未断开且无人使用的长连接数

			RWLock _svrlock;
			bool _isHealthy;
			bool _enable;
			ConnectManager * _mgr;
			BasicLogAdapter * _log;
			std::vector <str_t> _tag;
			std::vector <Connection *> _conns;
			int initBasicValue(const comcfg::ConfigUnit & conf);
		public:
			Server();
			virtual ~Server();
			//用一个configure初始化Server
			/**
			 * @brief 用一个Configure初始化Server
			 * 			非线程安全
			 *
			 * @param [in] conf   : const comcfg::ConfigUnit&
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int init(const comcfg::ConfigUnit & conf);

			//判断两个Server是否等同
			/**
			 * @brief 判两个server是否逻辑上等同
			 * 			内部加锁的情况：对this加锁读，对other不加锁读
			 *
			 * @param [in] other   : const Server&
			 * @return  bool 
			 * @retval   
			 * @see 
			**/
			virtual bool equal(const Server & other);
			/**
			 * @brief 克隆一个server
			 * 			相当于拷贝构造：*this = *oth
			 * 			内部加锁的情况：对this加锁写，对other不加锁读
			 *
			 * @param [in] oth   : const Server*
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int clone(const Server * oth);//相当于拷贝构造 *this = *oth
			//在这个Server上获取一个Connection
			/**
			 * @brief 在当前的Server上获取一个连接
			 *
			 * @param [out] err   : int* 错误号，如果非0表示出错
			 * @return  Connection* 
			 * @retval   
			 * @see 
			**/
			virtual Connection * fetchConnection(int * err);
			/**
			 * @brief 归还一个连接
			 *
			 * @param [in] c   : Connection*
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int freeConnection(Connection * c);

			/**
			 * @brief 关闭多余的连接
			 * 			当管理的活动连接数超过n个，管理多余的、并且无人使用的长连接
			 *
			 * @param [in] n   : int
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			virtual int closeExtraConnection(int n);

			//目前活动的连接数
			/**
			 * @brief 活动的连接数（包括使用中、未使用但保持连接）
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int liveConnectionNum();
			/**
			 * @brief 获取第i个连接的信息
			 *
			 * @param [in] id   : int
			 * @return  Connection* 
			 * @retval   
			 * @see 
			**/
			Connection * getConnectionInfo(int id);

			//获取基本信息
			/**
			 * @brief 获取ip
			 *
			 * @return  const char* 
			 * @retval   
			 * @see 
			**/
			const char * getIP();
			/**
			 * @brief 获取端口号
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int getPort();

            /**
             * @brief 获取连接上但未被fetch取得的连接数 
             *
             * @return  int 
             * @retval   
             * @see 
             * @note 
            **/
            int getPendingLongConn();
			/**
			 * @brief 获取该server最大连接数
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int getCapacity();
			/**
			 * @brief 获取连接超时 ms
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int getTimeout();

			//设置基本信息
			/**
			 * @brief 设置端口号
			 * 			此端口只能在初始化时使用
			 * 			设置完之后，运行时态不应该修改端口号和IP
			 *
			 * @param [in] port   : int
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setPort(int port);
			/**
			 * @brief 设置ip地址
			 * 			此接口只能在初始化时使用
			 * 			设置完之后，运行时态不应该修改端口号和IP
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setIP(const char *);
			/**
			 * @brief 设置最大连接数
			 *
			 * @param [in/out] cap   : int
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setCapacity(int cap);
			/**
			 * @brief 设置连接超时
			 *
			 * @param [in/out] tmout   : int
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setTimeout(int tmout);
#if 1
			/**
			 * @brief 添加一个tag
			 * 		此接口只能在初始化时使用
			 * 		设置完后，运行时态不应修改tag
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setTag(const char *); //NTS! not thread safe
			/**
			 * @brief 删除一个tag
			 * 		此接口只能在初始化时使用
			 * 		设置完后，运行时态不应修改tag
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int removeTag(const char *);
			/**
			 * @brief 是否存在该tag
			 *
			 * @return  bool 
			 * @retval   
			 * @see 
			**/
			bool hasTag(const char *);
			/**
			 * @brief 有多少个tag
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int getTagSize();//有多少个tag
			/**
			 * @brief 获取第i个tag
			 *
			 * @param [in/out] i   : int
			 * @return  const char* 
			 * @retval   
			 * @see 
			**/
			const char * getTag(int i);//获取第i个tag
#endif
			/**
			 * @brief 是否为健康状态
			 *
			 * @return  bool 
			 * @retval   
			 * @see 
			**/
			bool isHealthy();//标记是否为健康状态
			/**
			 * @brief 更新健康状态
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setHealthy(bool );

			/**
			 * @brief 停用该server
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int disable();
			/**
			 * @brief 启用该server
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int enable();
			/**
			 * @brief 是否启用
			 *
			 * @return  bool 
			 * @retval   
			 * @see 
			**/
			bool isEnable();

			/**
			 * @brief 设置serverid
			 * 		此接口只能在初始化时使用，由ConnectManager调用
			 * 		设置完后，运行时态不应修改id
			 *
			 * @param [in/out] id   : int
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setID(int id);//设置server ID
			/**
			 * @brief 获取serverid
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int getID();

			/**
			 * @brief 设置我属于哪一个ConnectManager
			 *
			 * @return  int 
			 * @retval   
			 * @see 
			**/
			int setManager(ConnectManager *);

			//添加和删除Server的接口。
			////这个功能其实可以通过reload实现。因为需求不强烈，暂时搁置。
			//int removeServer();
			//int addServer();
	};

}














#endif  //__CONNECTION_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
