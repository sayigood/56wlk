 
 
 
/**
 * @file socketpoolext.h
 * @brief 
 *  
 **/



#ifndef  __SOCKETPOOLEXT_H_
#define  __SOCKETPOOLEXT_H_

#include "socketpool.h"

/**
* @brief SocketExtStrategy
*   ´ø½¡¿µ¼ì²éµÄÀ©Õ¹²ßÂÔ
*  
*/
class SocketExtStrategy : public comcm::Strategy {

    public:
        /**
         * @brief 
         *
         * @see 
        **/
        SocketExtStrategy() : comcm::Strategy(){
            srand(time(0) ^ pthread_self());
        }

        /**
         * @brief 
         *
         * @see 
        **/
        virtual ~SocketExtStrategy() {}

        /**
         * @brief 
         *
         * @param [in/out] req   : const comcm::ConnectionRequest*
         * @return  int 
         * @retval   
         * @see 
        **/
        virtual int selectServer(const comcm::ConnectionRequest *req);

};









#endif  //__SOCKETPOOLEXT_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
