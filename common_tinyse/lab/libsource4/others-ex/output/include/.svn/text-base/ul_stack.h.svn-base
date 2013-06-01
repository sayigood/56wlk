/**
 * @file
 * @brief 栈是LIFO的数据结构，通过下面一组函数，使用者可以建立和销毁栈，向栈中压入和弹出不同类型数据元素，判断栈的当前状态等。
 */

#ifndef __UL_STACK__
#define __UL_STACK__


typedef enum
{
    T_CHAR,
    T_SHORT,
    T_INT,
    T_LONG,
    T_VOID
}StackItemType;

typedef struct
{
	int size;
	int top;
	void *stack_buff;
}Stack;

/**
 *  初始化栈
 *  
 *  @param[in]  size  栈的大小
 *  @param[out] 无
 *  @return 栈指针
 * - 非NULL   成功
 * - NULL     失败
 */
Stack *ul_stack_init(int size);

/**
 *  释放栈
 *  
 *  @param[in]  stack  栈指针
 *  @param[out] 无
 *  @return 无
 */
void ul_stack_destroy(Stack *stack);



/**
 *  push一个字符
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  字符
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_stack_push_char(Stack *stack, char value);


/**
 *  pop一个字符
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  字符存放的指针
 *  @param[out] value  字符值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_popup_char(Stack *stack, char *value);


/**
 *  从栈顶端提取一个char，栈自身不发生变化
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  字符存放的指针
 *  @param[out] value  字符值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_peek_char(Stack *stack, char *value);


/**
 *  push一个short
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  short值
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_stack_push_short(Stack *stack, short value);


/**
 *  pop一个short
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  short存放的指针
 *  @param[out] value  short值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_popup_short(Stack *stack, short *value);


/**
 *  从栈顶端提取一个short，栈自身不发生变化
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  short存放的指针
 *  @param[out] value  short值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_peek_short(Stack *stack, short *value);


/**
 *  push一个int
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  int值
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_stack_push_int(Stack *stack, int value);


/**
 *  pop一个int
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  int存放的指针
 *  @param[out] value  int值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_popup_int(Stack *stack, int *value);

/**
 *  从栈顶端提取一个int，栈自身不发生变化
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  int存放的指针
 *  @param[out] value  int值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_peek_int(Stack *stack, int *value);



/**
 *  push一个long
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  long值
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_stack_push_long(Stack *stack, long value);


/**
 *  pop一个long
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  long存放的指针
 *  @param[out] value  long值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_popup_long(Stack *stack, long *value);


/**
 *  从栈顶端提取一个long，栈自身不发生变化
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  long存放的指针
 *  @param[out] value  long值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_peek_long(Stack *stack, long *value);


/**
 *  push一个void 指针
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  指针
 *  @param[out] 无
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，空间满
 */
int ul_stack_push_void(Stack *stack, void *value);


/**
 *  pop一个void *
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  存放指针的指针
 *  @param[out] value  指针值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_popup_void(Stack *stack, void **value);


/**
 *  从栈顶端提取一个指针，栈自身不发生变化
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  value  存放指针的指针
 *  @param[out] value  指针值
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_peek_void(Stack *stack, void **value);



/**
 *  取栈顶元素的类型，栈自身不发生变化
 *  
 *  @param[in]  stack  栈指针
 *  @param[in]  type   存放类型的指针
 *  @param[out] type   获得类型
 *  @return 操作返回值
 * - 1 成功
 * - 0 失败，栈空
 */
int ul_stack_peek_type(Stack *stack, StackItemType *type);


/**
 *  获取栈大小
 *  
 *  @param[in]  stack  栈指针
 *  @param[out] 无
 *  @return 大小
 */
int ul_stack_get_size(Stack *stack);


/**
 *  获取栈剩余空间的大小
 *  
 *  @param[in]  stack  栈指针
 *  @param[out] 无
 *  @return 空间大小
 */
int ul_stack_get_space(Stack *stack);




/**
 *  判断栈是否为空
 *  
 *  @param[in]  stack  栈指针
 *  @param[out] 无
 *  @return 是否为空
 * - true  空
 * - false 不空
 */
bool ul_stack_is_empty(Stack *stack);


/**
 *  判断栈是否已满
 *  
 *  @param[in]  stack  栈指针
 *  @param[out] 无
 *  @return 是否为空
 * - true  满
 * - false 不满
 */
bool ul_stack_is_full(Stack *stack);

#endif
