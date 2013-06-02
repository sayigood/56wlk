/*
 * @brief ip认证系统 支持统配符格式
 * ip识别格式如下:
 * 		127.*.*.*
 * 		127.0.0.1
 * 		127.0.0.0/24
 * 		10.0.10.1-10.1.11.1
 * 		127.123.42.1/13-128.111.1.1/13
 *
 * 在调用ub_auth_compile后支持二分查找和ip区间合并,适合认证ip数比较多的情况
 * 没有调用ub_auth_complie采用线性查找方式
 * @reversion auth1.0.0.0
 */

#ifndef _UB_AUTH_H
#define _UB_AUTH_H

typedef struct _ub_auth_t ub_auth_t;

/*
 * @brief 创建ip认证系统句柄
 * @param[in] unsigned int sup_size 表示默认的支持ip数，可以动态扩展，建议用默认值
 * @return 返回创建的ip认证句柄，失败返回NULL
 */
ub_auth_t * ub_auth_create(unsigned int sup_size=64);

/*
 * @brief 销毁ip认证系统
 * @param[in] ub_auth_t * ip认证句柄
 */
void ub_auth_destroy(ub_auth_t *);

/*
 * @breif 	从文件中载入ip信息
 * 			文件个数如下
 * 				#每个区间一行，开头的#表示注释行
 * 				127.*.*.*
 * 				127.0.0.1
 * 				127.0.0.0/24
 * 				10.0.10.1-10.1.11.1
 * 				127.123.42.1/13-128.111.1.1/13
 * @param[in]	ub_auth_t *auth 认证系统句柄
 * @param[in]	const char *file	ip文件路径
 * @return		成功返回0，其他失败
 */
int ub_auth_load_ip(ub_auth_t *auth, const char *file);

/*
 * @breif		手动将ip段添加到认证系统中
 * @param[in]	ub_auth_t *auth 认证系统句柄
 * @param[in]	const char *ip ip段的字符串描述
 * @return		成功返回0，其他失败
 */
int ub_auth_push_ip(ub_auth_t *auth, const char *ip);

/*
 * @brief		编译导入的ip信息，合并重复区间，构建二分查找区间数
 * @param[in]	ub_auth_t *auth 认证系统句柄
 * return		返回0成功，其他失败
 */
int ub_auth_compile(ub_auth_t *auth);

/*
 * @brief		判断ip是否在认证系统中
 * @param[in]	ub_auth_t *auth 认证系统句柄
 * @param[in]	ip	判断的ip *.*.*.* 格式
 * @return		1 表示存在， 0表示不存在
 */
int ub_auth_ip_str(ub_auth_t *auth, const char *ip);

/*
 * @brief		判断ip是否在认证系统中
 * @param[in]	ub_auth_t *auth 认证系统句柄
 * @param[in]	ip	网络交互中获取的32位int行ip
 * @return		1 表示存在， 0表示不存在
 */
int ub_auth_ip_int(ub_auth_t *auth, const unsigned int ip);

/*
 * @brief		判断ip是否在认证系统中
 * @param[in]	ub_auth_t *auth 认证系统句柄
 * @param[in]	fd	网络交互accept到的fd句柄，根据句柄解析ip地址
 * @return		1 表示存在， 0表示不存在
 */
int ub_auth_ip_fd(ub_auth_t *auth, const int fd);

#endif

