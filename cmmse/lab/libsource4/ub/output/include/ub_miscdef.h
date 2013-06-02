#ifndef __UB_MISCDEF_H__
#define __UB_MISCDEF_H__

#include "ul_def.h"

/**!!一些不知道放在哪比较合适的定义,这个文件是个杂烩，以后可能会慢慢移到其它文件中**/


/*************************************
 *
 * 一些宏定义
 *
 ************************************/

#define UB_MONITOR_PROVIDER "__MONITOR__"

///一些错误消息的前缀,有利于在错误日志分析时进行分类(grep相应的关键字)
//使用的sample  UB_LOG_WARNING(EMSG_NET"nshead_write error [ret:%d]", ret);
#define EMSG_NET    "__E_NET__  "       ///网络连接读写等错误
#define EMSG_IO     "__E_IO__  "        ///文件IO错误
#define EMSG_MEM    "__E_MEM__  "       ///内存分配错误
#define EMSG_AUTH   "__E_AUTH__  "      ///权限控制错误
#define EMSG_DATA   "__E_DATA__  "      ///外部提供数据出错


///STATIC定义,在_TEST模式下,所有函数都不为static,有利于进行基于函数级的单元测试
#ifndef _TEST
#define STATIC  static
#else
#define STATIC
#endif


///比较安全的关闭
#define UB_CLOSE(fd)  do { if (fd>0) { close(fd); fd=-1;} } while(0)
#define UB_FCLOSE(fd)  do { if (fd) { fclose(fd); fd=NULL;} } while(0)




/***********************************
 *
 * 一些结构定义
 *
 **********************************/


///简单的buffer结构
typedef struct _ub_buff_t {
	char    *buf;                   /**< 缓冲区的实际位置      */
	size_t  size;                   /**< 缓冲区的长度          */
	size_t  used;                   /**< 已使用的长度          */
} ub_buff_t;







#endif

