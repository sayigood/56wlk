/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file connectmanager.h
 * @brief 
 *  
 **/


#ifndef  __CONNECTMANAGER_H_
#define  __CONNECTMANAGER_H_

#include "Configure.h"
#include "strategy.h"
#include "connection.h"
#include "connectmgr_utils.h"
#include "bsl/map.h"

namespace comcm{

//以下两个函数类型，用于创建Connection/Server，因为用户可能需要自定义这些类，需要创建的是子类实体
//参数n表示要创建的元素个数，返回一个指针数组，每个元素是一个子类的指针
//这个函数通常是这个形式：
//Server * my_creater()
//{ 
//  return new (std::nothrow) MyClass;
//}
//注意对于分配内存失败的情况，请不要抛出异常,分配失败的情况返回NULL,清理工作请在内部处理
//这里是ConnectManager内部使用，最后会由ConnectManager内部释放，用户不用关心.
//
//
typedef Connection * (* connection_creater_t) ();		  /**< 创建Connection的函数指针*/
typedef Server * (* server_creater_t) ();		          /**< 创建Server的函数指针       */

const int CONN_LIST_NUM = 8;
/**
* @brief 连接管理器
*  
*/
class ConnectManager{
	public:
		/**
		 * @brief 两个函数指针，用于创建Server和Connection
		 * 			因为实际使用的是子类而非基类，所以ConnectManager不知道要创建的实体
		 * 			应该是什么类型的
		 * 			应该在init之前被调用
		 *
		 * @param [in] a   : connection_creater_t
		 * @param [in] b   : server_creater_t
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int setCreater(connection_creater_t a, server_creater_t b);
		//-------------初始化行为，支持从configure直接初始化------------
		/**
		 * @brief 初始化函数，从一个Configure结构体初始化ConnectManager
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
		 * @param [in] svrs   : Server** server指针数组，由外部用户自己管理内存，
		 * 						ConnectManager内部会拷贝一份
		 * @param [in] svrnum   : int server数量
		 * @param [in] shortConnection   : bool 是否短连接
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int init(Server ** svrs, int svrnum, bool shortConnection);

		//---------------设置策略，属于初始化的一部分--------------------
		/**
		 * @brief 设置选择Server的策略
		 * 			初始化时操作，非线程安全
		 *
		 * @param [in] stt   : Strategy*
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int setStrategy(Strategy * stt);
		/**
		 * @brief 设置健康检查策略
		 * 			初始化时操作，非线程安全
		 *
		 * @param [in] hchk   : HealthyChecker*
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int setHealthyChecker(HealthyChecker * hchk);

		//-------reload新的服务器列表，多线程同时执行reload是不安全的------------
		/**
		 * @brief 运行时更新server配置（仅限Server，不更新其它配置）
		 * 			不能两个线程同时reload
		 *
		 * @param [in] conf   : const comcfg::ConfigUnit&
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int reload(const comcfg::ConfigUnit & conf);
		/**
		 * @brief 运行时更新server配置
		 * 			不能两个线程同时reload
		 *
		 * @param [in] svrs   : Server** Server的指针数组
		 * @param [in] svrnum   : int
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int reload(Server ** svrs, int svrnum);


//        virtual int removeServer(Server *svr); //TODO

        /**
         * @brief 增加Server
         *
         * 接口本身线程不安全,需要与init, addServer互斥
         *
         * @param [in] svr   : Server*
         * @return  int 
         * @retval   
        **/
        virtual int addServer(Server *svr);

		/**
		 * @brief 设置日志打印的BasicLogAdapter
		 * 			about BasicLogAdapter : 详见cvs : public/comlog-plugin
		 * 			所有对于BasicLogAdapter的set/get操作都不是线程安全的
		 *
		 * @param [in] logger   : BasicLogAdapter*
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int setLogAdapter(BasicLogAdapter * logger);
		/**
		 * @brief 获取在使用的BasicLogAdapter
		 * 			所有对于BasicLogAdapter的set/get操作都不是线程安全的
		 *
		 * @return  BasicLogAdapter* 
		 * @retval   
		 * @see 
		**/
		virtual BasicLogAdapter * getLogAdapter();
		/**
		 * @brief 获取连接类型
		 *
		 * @return  int 
		 * @retval   
		 * 			LONG_CONN 长连接
		 * 			SHORT_CONN 短连接
		 * @see 
		**/
		virtual int getConnectType();

		//-------------获取或归还socket---------------
		//int fetchSocket();
		//int freeSocket(int sock);


		//---------------获取或归还Connection-----------------
		/**
		 * @brief 获取一个连接
		 *
		 * @param [in] key   : int  选取server所用的参考值
		 * @param [in] waittime   : int  如果没有可用连接，则等待多少毫秒后再返回失败
         * @param [in] err : int * 如果fetch失败得到错误信息,包括
         *
         *          comcm::REQ_OK 
         *          comcm::REQ_SVR_FULL,
         *          comcm::REQ_CONN_FAILED,
         *          comcm::REQ_SVR_DISABLE
         *
		 * @return  Connection* 
		 * @retval   
		 * @see 
		**/
        virtual Connection * fetchConnection(int key, int waittime = 0);
		virtual Connection * fetchConnection(int key, int waittime, int *err);
		/**
		 * @brief 归还一个连接
		 *
		 * @param [in/out] c   : Connection*
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int freeConnection(Connection * c);

		/**
		 * @brief 获取Server数量（包括disable和非healthy的server）
		 *
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int getServerSize() ;

        virtual int getRealServerSize();

        virtual int getHealthyServerSize();
		/**
		 * @brief 获取第i台server的信息
		 *
		 * @param [in] id   : int
		 * @return  Server* 
		 * @retval   
		 * @see 
		**/
		virtual Server * getServerInfo(int id) ;

		/**
		 * @brief 获取总的tag个数
		 *
		 * @return  int 
		 * @retval   
		 * @see 
		**/
		virtual int getTagSize();

		/**
		 * @brief 获取第i个tag
		 *
		 * @param [in] id   : int
		 * @return  const char* 
		 * @retval   
		 * @see 
		**/
		virtual const char * getTag(int id);

		/**
		 * @brief 根据tag获取对应的server列表
         * 请注意：该接口返回内部对象的指针，reload时内部对象会被析构，所以在使用reload
         * 功能时建议使用getServerByTagDeep，或者是不保存返回值，每次使用时都重新调用
         * getServerByTag
		 *
		 * @param [in] tag   : const char* tag号
		 * @param [out] siz   : int* 打了此tag的server的数量
		 * @return  const int* 所有打了此tag的server的列表
		 * @retval   
		 * @see 
		**/
		virtual const int * getServerByTag(const char * tag, int *siz);
		
        /**
         * @brief 返回内部对象的拷贝，使用reload功能时是安全的，但是效率低于getServerByTag
         *
         * @param [in/out] tag   : const char*
         * @param [in/out] server_list   : std::vector<int>&
         * @return  int 
         * @retval   
         * @see 
        **/
        virtual int getServerByTagDeep(const char * tag, std::vector<int> &server_list);
		/**
		 * @brief 上一次成功reload的时间
		 *
		 * @return  time_t 
		 * @retval   
		 * @see 
		**/
		virtual time_t lastReload(); //上一次成功reload的时间


        /** 
         * @brief 设置重试次数
         * @param [in] retry_time   : 重试次数
         *                                                    **/
        virtual int setRetryTime(int retry_time);
        /** 
         * @brief 获取重试次数
         * @return  当前的重试次数 
         **/
        virtual int getRetryTime();

		/**
		 * @brief 构造函数
		**/
		ConnectManager();

		/**
		 * @brief 析构函数
		**/
		virtual ~ConnectManager();

		/**
		 * @brief 获取默认的BasicLogAdapter
		 * 			对该BasicLogAdapter的操作全都是非线程安全的
		 *
		 * @return  BasicLogAdapter* 
		 * @retval   
		 * @see 
		**/
		static BasicLogAdapter * defaultLogAdapter();

		Connection * borrowConnection();
		void returnConnection(Connection *);
	protected:
		friend class Server;
		friend class Connection;
		friend class Strategy;
		friend class HealthyChecker;

		Server ** readConfigData(const comcfg::ConfigUnit & conf, int * err);

		void close();//退出，在析构时调用

		connection_creater_t conn_creater;
		server_creater_t svr_creater;

		static BasicLogAdapter _defaultLogAdapter;

		time_t _lastReloadTime;

		int _conn_type;
		int _defaultRetryTime;//最大重试次数，防止策略不关心重试时进入死循环

		// 被friend class Server 使用，Server :: load
		int _defaultCapacityForServer;
		int _defaultCtimeoutForServer;
		int _defaultPortForServer;

		RWLock _rwlock;

		Server * _svr[MAX_SVR_NUM];
		int _svrnum;
		Strategy * _stt;//有默认值
		HealthyChecker * _hchk;

		typedef std::vector<int> Array;
		int initTag(Server ** svrs, int svrnum);
        //由外部加锁，保证线程安全
		int reloadTag(Server ** svrs, int svrnum);
		std::vector <str_t> _taglist;
		std::map <str_t, int> _tagmap;
		std::vector < Array * > _tagvec;

		std::list < Connection * > _conn_list[CONN_LIST_NUM];
		pthread_mutex_t _list_lock[CONN_LIST_NUM];
		//Connection * _live_cnn[];//正在使用的Connection列表
		//int _live_cnn_num;
		//bsl::hashmap <int, Connection *> _live_sock; //sock表，归还时需要查表

		BasicLogAdapter * _log;
};

}

#endif  //__CONNECTMANAGER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
