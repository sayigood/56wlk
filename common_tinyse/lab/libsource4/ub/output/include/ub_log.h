/////////////////////////////////////////////////////////////////////////////

#ifndef __UB_LOG_H__
#define __UB_LOG_H__

////////////////////////////////////////////////////////////////
//////// Module:  ub_log, log related module of general framework
///////  Comment: standlization to log format will benefit us all
///////
////////////////////////////////////////////////////////////////

#include <pthread.h>
#include "ul_log.h"


/**********************************************************************/
/***************general framework notice function suggestment**********/
//通用框架规定了notice日志中的一级格式：
//[logid:834798723][proctime:23ms][reqip:127.0.32.123][reqserver:nstrans]
//[cmdno:3(update)][svrname:di][errno:-3(readiheadfaild)][系统一级预留信息]
//[userinfo:{push的信息}][用户一级信息]
/**********************************************************************/
/**********************************************************************/


//最大的userinfo长度
#define NOTICE_INFO_MAXLEN LOG_BUFF_SIZE_EX

//特殊不可见的分隔符
#define UB_SPACE  '\10'

//第一级信息的类型
typedef enum ub_notice_type{
    UB_LOG_ZERO = 0,		/**< this one is not used, and should be the first*/
    UB_LOG_LOGID,		/**< 设置logid对应的命令*/
    UB_LOG_PROCTIME,	/**< 设置处理时间对应的命令*/
    UB_LOG_REQIP,		/**< 设置请求IP对应的命令*/
    UB_LOG_REQSVR,		/**< 设置请求服务器名称对应的命令*/
    UB_LOG_SVRNAME,		/**< 设置当前服务名称*/
    UB_LOG_CMDNO,		/**< 设置请求命令对应的命令*/
    UB_LOG_ERRNO,		/**< 设置处理结果对应的命令*/
    UB_LOG_END		    /**<this one should be the last*/
};


/**
 * @brief 初始化函数
 *     应该在打LOG之前运行,和ul_log有没有open无关,可以在程序最开始进行
 *
 * @param[in] log_file 日志文件的文件名
 *
 * @param[in] log_path 日志所在的目录(可以是相对目录)
 *
 * @param[in] max_size 日志文件的最大长度
 *
 * @param[in] log_level日志的等级，(分为0,1,2,4,8,16几个等级)
 *
 * @param[in] to_syslog  interesting events to send to syslog
 *
 * @param[in] assorted switches
 *
 * @return
 *       0表示成功
 *		-1设置日志失败
**/
int ub_log_init(const char* log_path = "./log", const char* log_file = "ub_log.",
	int max_size = 1000, int log_level = 16, int to_syslog = 0, int spec = 0);


/**
 * @brief 线程中初始化函数时调用，用于线程打印日志
 *       进入线程后调用
 *
 * @param[in] thread_name 线程的信息
 *
 * @return
 *		0表示成功
 *		-1设置日志失败
 */
int ub_log_initthread(const char* thread_name);


/**
 * @brief 程序结束时调用，用来关闭日志功能
 */
void ub_log_close();


/**
 * @brief 程序结束时调用，用来关闭日志功能
 */
void ub_log_closethread();


/**
 * @brief 设置线程notice日志栈信息, 最大长度为 NOTICE_INFO_MAXLEN
 * 按照通用框架规范，这一部分日志为RD所使用，不为PE统计使用
 *
 * @param[in] key key info of key:value
 *
 * @param[in] valuefmt format of info
 *
 * @return
 *	  length of truely added info
 */
unsigned int ub_log_pushnotice(const char* key, const char* valuefmt, ...);


/**
 * @brief 设置基本notice 信息，这些项是使用ub_log建议打出的必填项目
 *
 * @param[in] type basic infomation type
 *
 * @param[in] valuefmt format of info
 *
 * @return	number of chars been added
 */
unsigned int ub_log_setbasic(ub_notice_type type, const char* valuefmt, ...);


/**
 * @brief 取得基本notice的信息
 *
 * @param type serven type basic infomation type
 *
 * @return
 *		string info
 *
 * @comment for ub_log_use
 */
char* ub_log_getbasic(ub_notice_type type);


/**
 * @brief set logid
 *
 * @param logid logid of network
 *
 * @return
 *		logid that set
 */
unsigned int ub_log_setlogid(unsigned int logid);


/**
 * @brief get logid
 *
 * @param logid logid of network
 *
 * @return
 *		logid that get
 */
unsigned int ub_log_getlogid();


/**
 * @brief clear logid
 *
 * @param logid logid of network
 *
 * @return
 *		logid that clear
 */
unsigned int ub_log_clearlogid();


/**
 * @brief 弹出push进入的信息
 *
 * @return
 *		string of user puted
 *
 * @comment for ub_log_use
 */
char* ub_log_popnotice();


/**
 * @brief 清空线程日志相关数据:在每次打notice后，库会自己调用
 *
 * @return
 *        0表示成功
 *	  其它表示失败
 *
 * @comment for ub_log_use
 **/
int ub_log_clearnotice();


/**
 * @brief 得到线程的处理时间, 用于在UB_LOG_DEBUG中打印距离上次打印的时间
 *
 * @return
 *	返回自从上次调用以来执行的时间（微秒)
 *
 * @comment for ub_log_use
 */
unsigned int ub_log_getussecond();


/**
 * @brief 得到用户保存的信息, 打印notice时ub_log调用
 *
 * @return
 *        point to the string in the stack
 *
 * @comment for ub_log_use
 */
//char* ub_log_getnoticeadd();


#define UB_LOG_MONITOR(fmt, arg...)  \
    ul_writelog(UL_LOG_WARNING, "---LOG_MONITOR--- [ %clogid:%s %c][ %creqip:%s %c][%s:%d]" fmt,\
	    UB_SPACE, ub_log_getbasic(UB_LOG_LOGID), UB_SPACE, UB_SPACE, ub_log_getbasic(UB_LOG_REQIP), UB_SPACE,\
			__FILE__, __LINE__, ## arg)

#define UB_LOG_FATAL(fmt, arg...) do { \
    ul_writelog(UL_LOG_WARNING, "---LOG_MONITOR--- [ %clogid:%s %c][ %creqip:%s %c][%s:%d]" fmt,\
	    UB_SPACE, ub_log_getbasic(UB_LOG_LOGID), UB_SPACE, UB_SPACE, ub_log_getbasic(UB_LOG_REQIP), UB_SPACE,\
			__FILE__, __LINE__, ## arg); \
    ul_writelog(UL_LOG_FATAL, "[ %clogid:%s %c][ %creqip:%s %c][%s:%d]" fmt,\
	    UB_SPACE, ub_log_getbasic(UB_LOG_LOGID), UB_SPACE, UB_SPACE, ub_log_getbasic(UB_LOG_REQIP), UB_SPACE,\
			__FILE__, __LINE__, ## arg); \
} while (0)

#define UB_LOG_WARNING(fmt, arg...)  \
    ul_writelog(UL_LOG_WARNING, "[ %clogid:%s %c][ %creqip:%s %c][%s:%d]" fmt,\
	    UB_SPACE, ub_log_getbasic(UB_LOG_LOGID), UB_SPACE, UB_SPACE, ub_log_getbasic(UB_LOG_REQIP), UB_SPACE,\
			__FILE__, __LINE__, ## arg)

#define UB_LOG_NOTICE(fmt, arg...) do { \
    ul_writelog(UL_LOG_NOTICE, "[ %clogid:%s %c]" "[ %cproctime:%s %c]" "[ %creqip:%s %c]" "[ %creqsvr:%s %c]"\
	    "[ %ccmdno:%s %c]" "[ %csvrname:%s %c]" "[ %cerrno:%s %c]" "[ %c%s %c]" "[ %c%s %c]" "[ %c" fmt " %c]", \
	    UB_SPACE, ub_log_getbasic(UB_LOG_LOGID), UB_SPACE, UB_SPACE, ub_log_getbasic(UB_LOG_PROCTIME), UB_SPACE, \
	    UB_SPACE, ub_log_getbasic(UB_LOG_REQIP), UB_SPACE, UB_SPACE, ub_log_getbasic(UB_LOG_REQSVR), UB_SPACE, \
	    UB_SPACE, ub_log_getbasic(UB_LOG_CMDNO), UB_SPACE, UB_SPACE, ub_log_getbasic(UB_LOG_SVRNAME), UB_SPACE, \
		UB_SPACE, ub_log_getbasic(UB_LOG_ERRNO), UB_SPACE, UB_SPACE, "", UB_SPACE, \
		UB_SPACE, ub_log_popnotice(), UB_SPACE, UB_SPACE, ## arg, UB_SPACE);\
	    ub_log_clearnotice(); \
} while (0)

#define UB_LOG_TRACE(fmt, arg...)  \
    ul_writelog(UL_LOG_TRACE, "[ %clogid:%s %c]" fmt,\
	   UB_SPACE, ub_log_getbasic(UB_LOG_LOGID), UB_SPACE, ## arg)

#ifndef NO_UB_DEBUG
#define UB_LOG_DEBUG(fmt, arg...) \
    ul_writelog(UL_LOG_DEBUG, "[ %clogid:%s %c][%s:%d][time:%uus]" fmt,\
	    UB_SPACE, ub_log_getbasic(UB_LOG_LOGID), UB_SPACE, __FILE__, __LINE__,\
	    ub_log_getussecond(), ## arg)
#else
#define UB_LOG_DEBUG(fmt, arg...) \
    do {} while(0)
#endif


#endif //__UB_LOG_H__

/* vim: set ts=4 sw=4 tw=0 noet: */


