/**
 * @file
 * @brief 队列是FIFO的数据结构，通过下面一组函数，使用者可以建立和销毁队列，向队列中加入和从队列中取出不同类型数据元素，判断队列的当前状态等。进程级操作，线程使用需加锁
 */

#ifndef __UL_QUEUE__
#define __UL_QUEUE__


typedef enum
{
    T_CHAR,
    T_SHORT,
    T_INT,
    T_LONG,
    T_VOID
}QueueItemType;

typedef struct
{
	int size;
	int head;
	int tail;
	void *queue_buff;
}Queue;



/**
 *  初始化队列
 *  
 *  @param[in]  size  队列大小
 *  @param[out] 无
 *  @return 队列指针
 * - 非NULL   成功
 * - NULL     失败
 */
Queue *ul_queue_init(int size);

/**
 *  销毁队列
 *  
 *  @param[in]  queue  队列指针
 *  @param[out] 无
 *  @return 无
 */
void ul_queue_destroy(Queue *queue);

/**
 *  字符入队列
 *  
 *  @param[in]  queue  队列指针
 *  @param[in]  value  字符值
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_queue_in_char(Queue *queue, char value);


/**
 *  字符型出队列
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  字符型存放的指针
 *  @param[out] value  字符值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，队列空
 */
int ul_queue_out_char(Queue *queue, char *value);



/**
 *  从队列中取一个字符，队列自身不发生变化
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  字符存放的指针
 *  @param[out] value  字符值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_queue_peek_char(Queue *queue, char *value);


/**
 *  short型入队列
 *  
 *  @param[in]  queue  队列指针
 *  @param[in]  value  short型值
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_queue_in_short(Queue *queue, short value);

/**
 *  short型出队列
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  short型存放的指针
 *  @param[out] value  short型值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，队列空
 */
int ul_queue_out_short(Queue *queue, short *value);


/**
 *  从队列中取一个short型，队列自身不发生变化
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  short型存放的指针
 *  @param[out] value  short型值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_queue_peek_short(Queue *queue, short *value);


/**
 *  int型入队列
 *  
 *  @param[in]  queue  队列指针
 *  @param[in]  value  int型值
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_queue_in_int(Queue *queue, int value);


/**
 *  int型出队列
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  int型存放的指针
 *  @param[out] value  int型值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，队列空
 */
int ul_queue_out_int(Queue *queue, int *value);


/**
 *  从队列中取一个int型，队列自身不发生变化
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  int型存放的指针
 *  @param[out] value  int型值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_queue_peek_int(Queue *queue, int *value);

/**
 *  long型入队列
 *  
 *  @param[in]  queue  队列指针
 *  @param[in]  value  long型值
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_queue_in_long(Queue *queue, long value);


/**
 *  long型出队列
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  long型存放的指针
 *  @param[out] value  long型值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，队列空
 */
int ul_queue_out_long(Queue *queue, long *value);


/**
 *  从队列中取一个long型，队列自身不发生变化
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  long型存放的指针
 *  @param[out] value  long型值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_queue_peek_long(Queue *queue, long *value);


/**
 *  void *入队列
 *  
 *  @param[in]  queue  队列指针
 *  @param[in]  value  指针值
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_queue_in_void(Queue *queue, void *value);


/**
 *  void *型出队列
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  指针型存放的指针
 *  @param[out] value  指针型值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，队列空
 */
int ul_queue_out_void(Queue *queue, void **value);


/**
 *  从队列中取一个指针型，队列自身不发生变化
 *  
 *  @param[in]  stack  队列指针
 *  @param[in]  value  指针型存放的指针
 *  @param[out] value  指针型值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_queue_peek_void(Queue *queue, void **value);


/**
 *  取队列头元素的类型，队列自身的状态不变
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  type   存放类型的指针
 *  @param[out] type   获得类型
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_queue_peek_type(Queue *queue, QueueItemType *type);



/**
 *  获取队列大小
 *  
 *  @param[in]  stack  队列结构指针
 *  @param[out] 无
 *  @return 大小
 */
int ul_queue_get_size(Queue *queue);



/**
 *  获取队列剩余空间的大小
 *  
 *  @param[in]  stack  队列结构指针
 *  @param[out] 无
 *  @return 空间大小
 */
int ul_queue_get_space(Queue *queue);


/**
 *  判断队列是否为空
 *  
 *  @param[in]  stack  队列结构指针
 *  @param[out] 无
 *  @return 是否为空
 * - true  空
 * - false 不空
 */
bool ul_queue_is_empty(Queue *queue);


/**
 *  判断队列是否已满
 *  
 *  @param[in]  stack  队列结构指针
 *  @param[out] 无
 *  @return 是否为空
 * - true  满
 * - false 不满
 */
bool ul_queue_is_full(Queue *queue);

#endif
