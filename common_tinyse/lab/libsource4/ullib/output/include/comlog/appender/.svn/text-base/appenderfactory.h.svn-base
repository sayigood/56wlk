/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file appenderfactory.h
 * @brief 
 *  
 **/


#ifndef  __APPENDERFACTORY_H_
#define  __APPENDERFACTORY_H_

#include "comlog/appender/appender.h"
#include "comlog/comlog.h"
#include "comlog/xutils.h"
namespace comspace{

class AppenderFactory{
public:
	//Actually, MAX_NAME_LEN is 15. A name with length >= 16 is invalid.
	static const unsigned int MAX_NAME_LEN = 16;		  /**<        */
	//typedef Appender* (*func_type)(com_device_t&);

	/**
	 * @brief 
	 *
	 * @param [in/out] dev   : com_device_t&
	 * @return  Appender* 
	 * @retval   
	**/
	static Appender* getAppender(com_device_t &dev);
	/**
	 * @brief 
	 *
	 * @param [in/out] dev   : com_device_t&
	 * @return  Appender* 
	 * @retval   
	**/
	static Appender* tryAppender(com_device_t &dev);
	/**
	 * @brief 
	 *
	 * @param [in/out] appenderName   : const char*
	 * @param [in/out] getAppender   : func_type
	 * @param [in/out] tryAppender   : func_type
	 * @return  int 
	 * @retval   
	**/
	static int registAppender(const char *appenderName, func_type getAppender, func_type tryAppender);
	/**
	 * @brief 
	 *
	 * @param [in/out] appenderName   : const char*
	 * @return  bool 
	 * @retval   
	**/
	static bool isAppender(const char *appenderName);
	/**
	 * @brief 
	 *
	**/
	AppenderFactory();
	/**
	 * @brief 
	 *
	**/
	~AppenderFactory();
	struct AppenderList{
		char name[MAX_NAME_LEN];		  /**<        */
		func_type tryAppender;		  /**<        */
		func_type getAppender;		  /**<        */
		AppenderList* next;		  /**<        */
	};
private:

//	static AppenderList* appList;
	/**
	 * @brief 
	 *
	 * @return  AppenderList* 
	 * @retval   
	**/
	static AppenderList* get_static_appList();
	/**
	 * @brief 
	 *
	 * @param [in/out] al   : AppenderList*
	 * @return  void 
	**/
	static void set_static_appList(AppenderList* al);
//	static AppenderList* appTail;
	/**
	 * @brief 
	 *
	 * @return  AppenderList* 
	**/
	static AppenderList* get_static_appTail();
	/**
	 * @brief 
	 *
	 * @param [in/out] al   : AppenderList*
	 * @return  void 
	**/
	static void set_static_appTail(AppenderList* al);
//	static pthread_mutex_t getLock;
//	static pthread_mutex_t tryLock;

	/**
	 * @brief 
	 *
	 * @param [in/out] appenderName   : const char*
	 * @return  AppenderList* 
	**/
	static AppenderList* searchAppender(const char* appenderName);
	/**
	 * @brief 
	 *
	 * @param [in/out] node   : AppenderList*
	 * @return  void 
	**/
	static void insertAppender(AppenderList* node);

};


};


#endif  //__APPENDERFACTORY_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
