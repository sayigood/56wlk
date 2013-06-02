/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file msocketpool.h
 * @brief 
 *  
 **/




#ifndef  __MSOCKETPOOL_H_
#define  __MSOCKETPOOL_H_



#include <stdio.h>
#include <ul_net.h>
#include "connection.h"
#include "socketpool.h"

namespace comcm
{

/**
* @brief 支持双网卡的Server
*  
*/
class MServer : public comcm::Server
{
    protected:
        char _ip2[16];
        unsigned int _ip_int2;
        bool _vaild_list[2];
        int initMBasicValue(const comcfg::ConfigUnit & conf);
    public:
        /**
        * @brief 获取第二个IP
        *  
        */
        virtual const char *getIP2();
        /**
        * @brief 设置第二个IP
        *  
        */
        virtual int setIP2(const char *p);
        /**
        * @brief 按照配置文件初始化
        *  
        */
        virtual int init(const comcfg::ConfigUnit & conf);

        /**
        * @brief 比较两个Server是否相等
        *  
        */
        virtual bool equal(const comcm::Server & other);
        /**
        * @brief clone一个同类型的Server
        *  
        */
        virtual int clone(const comcm::Server * oth);


        /**
         * @brief 设置有效ip 
         *
         * @param [in] ip   : 0 或者 1, 分别表示第一，二个ip
         *
         * 连接的时候会优先选择
         * @return  int 
        **/
        inline int setValidIp(int id)
        {
            if (id >= 0 && id <= 1) {
                _vaild_list[id] = true;
                return 0;
            }
            return -1;
        }

        /**
         * @brief 设置ip为无效ip,在连接的时候对于无效ip不会尝试进行连接
         * 
         * 如果两个ip都为无效ip那么都不会尝试连接
         *
         * @param [in] id   : 0 或者 1， 分别表示第一，二个ip
         * @return  int 
        **/
        inline int setInValidIp(int id)
        {
            if (id >= 0 && id <= 1) {
                _vaild_list[id] = false;
                return 0;
            }
            return -1;
        }

        /**
         * @brief 判断ip是否有效
         *
         * @param [in/out] id   : int
         * @return  bool 
        **/
        inline bool isValid(int id)
        {
            if (id >= 0 && id <= 1) {
                return _vaild_list[id];
            }
            return false;
        }
};



//

/**
* @brief 支持双网络的连接
*  
*/
class MSocketConnection : public SocketConnection
{
    protected:
        int _last_connect;
    public:
        MSocketConnection();
    /**
    * @brief 连接一个Server
    *  
    */
    virtual int connect(comcm::Server *);

    /**
     * @brief 返回当前使用的ip,第一个还是第二个ip
     *
     * 一般使用的使用，在msocketpool中时候使用getConnectionBySock获取到
     * connection, 然后dynamic_cast转化为子类，进行调用
     * comcm::MSocketConnection * conn = dynamic_cast<comcm::MSocketConnection*>
     *             (skpool.getConnectionBySock(sock));
     * conn->getCurrentIP();
     *
     * @return 0 表示使用ip1, 1表示使用ip2
     * -1表示错误，没有建立连接
     *
    **/
    virtual int getCurrentIP();
};

}
/**
* @brief 创建一个支持双网卡的Server
*  
*  给MSocketPool 使用
*/
comcm::Server * msocket_server_creater();

/**
* @brief 创建一个支持双网卡的Connection
* 给 MSocketPool 使用 
*/
comcm::Connection * msocket_conn_creater();


/**
* @brief 支持双网卡的SocketPool, MSocketPool
* 这里重写了初始化接口
* 其他接口参看 SocketPool的 接口
*  
*/
class MSocketPool : public SocketPool
{
    public:
    virtual int init(const comcfg::ConfigUnit & conf);
    virtual int init(comcm::Server ** svrs, int svrnum, bool shortConnection);
    virtual ~MSocketPool();
    
};



#endif  //__MSOCKETPOOL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
