/**
 * @file
 * @brief 提供一组管理非阻塞连接池的方法,支持线程级操作
 */


#ifndef __SOCKPOOL_H_
#define __SOCKPOOL_H_

#include <pthread.h>

#define ERR_SOCKPOOL_EXIST -2
#define ERR_SOCKPOOL_FULL -3
#define ERR_SOCKPOOL_EMPTY -4
#define ERR_SOCKPOOL_OVERTIME -5
#define ERR_SOCKPOOL_SYSERR -6
#define SOCKPOOL_MAXSIZE 4096

typedef struct _socket_pool_t{
	int m_size;
	pthread_cond_t m_empty_cond;
	pthread_mutex_t m_pool_mutex;
	int m_curnum;
	int m_freenum;
	int m_overtime;		//overtime count in seconds
	int m_socks[SOCKPOOL_MAXSIZE];
} Sockpool;

 
/**
 *  初始化套节字池
 *  
 *  @param[in]  sp       指向已分配的套节字池的空间的指针
 *  @param[in]  size     套节字池的大小，最大不能超过4096
 *  @param[in]  overtime 超时时间（秒）
 *  @param[out] 无
 *  @return 操作返回值
 * - 0  成功
 * - -1 失败
 */
int sockpool_init(Sockpool *sp ,const int size,const int overtime);








/**
 *  向套节字池中添加套节字
 *  
 *  @param[in]  sp       套节字池
 *  @param[in]  sock     待加入的套节字
 *  @param[out] 无
 *  @return 操作返回值
 * - 0                  成功
 * - ERR_SOCKPOOL_FULL  已满
 * - ERR_SOCKPOOL_EXIST 已存在
 */
int sockpool_add(Sockpool *sp, int sock);





/**
 *  从套节字池中取一个已准备好的套节字或所有超时的套节字
 *  
 *  @param[in]  sp          套节字池
 *  @param[in]  sock        用于存储已就绪的套节字的引用
 *  @param[in]  ot_socklist 指向存储超时的套节字的空间
 *  @param[in]  otcount     用于存储超时的套节字的个数的引用
 *  @param[out] sock        函数返回0时，存储已就绪的套节字
 *  @param[out] ot_socklist 函数返回ERR_SOCKPOOL_OVERTIME时，存储超时的套节字
 *  @param[out] otcount     函数返回ERR_SOCKPOOL_OVERTIME时，存储超时的套节字的个数
 *  @return 操作返回值
 * - 0                     成功
 * - ERR_SOCKPOOL_EMPTY    套节字池为空
 * - ERR_SOCKPOOL_SYSERR   系统错误
 * - ERR_SOCKPOOL_OVERTIME 超时
 */
int sockpool_pick(Sockpool *sp,int &sock, int *ot_socklist, int &otcount);





/**
 *  将取出的套节字重新放回到套节字池中
 *  
 *  @param[in]  sp          套节字池
 *  @param[in]  sock        套节字，如果为-1，则套节字池中套节字的计数减1
 *  @param[out] 无
 *  @return 操作返回值
 * - 0                     成功
 */
int sockpool_unpick(Sockpool * sp,int sock);



/**
 *  判断套节字池是否已满
 *  
 *  @param[in]  sp          套节字池
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 已满
 * - 0 未满
 */
int sockpool_isfull(Sockpool * sp);

#endif
