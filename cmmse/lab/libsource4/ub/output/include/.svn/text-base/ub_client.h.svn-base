/////////////////////////////////////////////////////////////////////////////

#ifndef __UB_CLIENT_H__
#define __UB_CLIENT_H__

/////////////////////////////////////////////////////////
/// Module:  ub_client, client part of c/s struct of general framework
//  Comment: client is a connection pool based module
//
///////////////////////////////////////////////////////////

#include "ul_def.h"
#include "ub_conf.h"
#include "ub_log.h"
#include "nshead.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

enum {
  UB_CLIENT_LINGER_OPEN = 1
};


typedef struct _ub_client_t			ub_client_t;


typedef struct _ub_client_attr_t	ub_client_attr_t;

//保留接口
typedef struct _ub_client_mod_attr_t{
	u_int					nomagiccheck;		/**<是否行验证码检查(和shead时设置为1)*/
}ub_client_mod_attr_t;

typedef struct _ub_client_mod_attr_t ub_client_mod_attr_t;


//回调数据合法性检查函数,检查通过返回0
typedef int (* ub_callback_check)(void *res);


//并行发送的数据结构
typedef struct _ub_parallel_t{
	char				name[WORD_SIZE/16];	/**<服务器名称*/
	//用来选择适当的服务器，该值在调用中可能会被改变。
	u_int				hash_key;			/**<服务器选择键值*/
	nshead_t			*reqhead;			/**<发送buffer缓冲*/
	//如果传入reqbuff为空，则只使用reqhead
	void				*reqbuff;			/**<发送buffer缓冲*/
	nshead_t			*reshead;			/**<接收buffer缓冲*/
	//如果传入resbuff为空，则只使用reshead
	void				*resbuff;			/**<接收buffer缓冲*/
	u_int				maxreslen;			/**<接收的最大长度*/
	bool				successed;			/**<网络是否交互成功*/
	ub_callback_check	checkcallback;		/**<回调函数*/
	u_int				reserv_int[8];		/**<对用户的保留字段*/
	u_int				ub_client_reserved;	/**<系统保留字段*/
}ub_parallel_t;


/**
 * @brief			init	ub_client
 *
 * @param[in]		attr	if null, using default attribute
 *
 * @return
 *		NULL for error
 *		otherwise the ub_client_t core struct
 */
extern ub_client_t*
ub_client_init(ub_client_attr_t *attr = NULL);


/**
 * @brief			close all resource allocated by ub_client
 *
 * @param[in][out]	client  ub_client core struct
 *
 * @param[in]		attr	if null, using default attribute
 *
 * @return
 *		0 for success;
 *		otherwise error,
 */
extern int
ub_client_close(ub_client_t *client);


/**
 * @brief			add a moudle info with attribute config(非线程安全函数!!!)
 *					DO NOT call ub_client_add in thread, call it in main thread init
 *					proscess.
 *
 * @param[in]		client	ub_client core struct
 *
 * @param[in]		name	server type name
 *
 * @param[in]		svr		config info read from ub_conf
 *
 * @param[in]		attr	attribute for module, if null, using default attribute
 *
 * @return
 *		0 for success;
 *		otherwise error
 */
extern int
ub_client_add(ub_client_t *client, const ub_request_svr_t *svr,
		const char *name, const ub_client_mod_attr_t *attr=NULL);


/**
 * @brief		invite server module to process request, to finish interest job,
 *				talk is the oldest way of processing c/s request,
 *				summon ,command and demmand is a litter bit rude,
 *				invite is the exactlly way of c/s work together to change internet.
 *				method just list nshead interface
 *
 * @param[in]	client			ub_client core struct
 *
 * @param[in]	name			module name
 *
 * @param[in]	hash_key		-1 for random select
 *
 * @param[in]	reqhead			request buffer head
 *
 * @param[in]	reqbuffer		request buffers
 *
 * @param[in]	reshead			response buffer head
 *
 * @param[in]	resbuffer		response information store buffers
 *
 * @param[in]	maxreslen		max length of resbuffer(including nshead)
 *
 * @param[in]	checkcallback	call back check function of return info
 *
 * @return
 *		0 for success;
 *		otherwise error
 */
extern int
	ub_client_orginvite(ub_client_t *client, const char *name, u_int hash_key,
		        nshead_t *reqhead, void *reqbuff, nshead_t *reshead, void *resbuff,
		        u_int maxreslen, ub_callback_check checkcallback = NULL);

/**
 * @brief		invite server module to process request, to finish interest job,
 *				talk is the oldest way of processing c/s request,
 *				summon ,command and demmand is a litter bit rude,
 *				invite is the exactlly way of c/s work together to change internet.
 *				method just list nshead interface
 *
 * @param[in]	client			ub_client core struct
 *
 * @param[in]	name			module name
 *
 * @param[in]	hash_key		-1 for random select
 *
 * @param[in]	reqhead			request buffer head
 *
 * @param[in]	reshead			response buffer head
 *
 * @param[in]	maxreslen		max length of resbuffer(including nshead)
 *
 * @param[in]	checkcallback	call back check function of return info
 *
 * @return
 *		0 for success;
 *		otherwise error
 */
extern int
ub_client_invite(ub_client_t *client, const char *name,  u_int hash_key,
		nshead_t *reqhead, nshead_t *reshead,
		u_int maxreslen, ub_callback_check checkcallback = NULL);


/**
 * @brief		invite with servers parallelly
 *
 * @param[in]	client      ub_client core struct
 *
 * @param[in]	parallel	list of informations needed for parallelly send
 *
 * @param[in]	count       length of parallel
 *
 * @param[in]	ms_timeout  timeout of total process,
 *							ub_client will user largest read+write timeout settings.
 *
 * @return
 *		0 if success
 *		-1 if failed
 */
extern int
ub_client_basicgroupinvite(ub_client_t *client, ub_parallel_t *parallel, u_int count, u_int ms_timeout = 0);


/**
 * @brief		invite with servers parallelly(以parallel->name为结束判断标志)
 *
 * @param[in]	client      ub_client core struct
 *
 * @param[in]	parallel	list of informations needed for parallelly send, end with parallel->name is NULL
 *							plus a empty ub_parallel_t structure to the end.
 *
 * @param[in]	ms_timeout  timeout of total process,
 *							ub_client will user largest read+write timeout settings.
 *
 * @return
 *		0 if success
 *		-1 if failed
 */
extern int
ub_client_groupinvite(ub_client_t *client, ub_parallel_t *parallel, u_int ms_timeout = 0);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__UB_CLIENT_H__

/* vim: set ts=4 sw=4 tw=0 noet: */
