/**
 * @file
 * @brief 字符缓冲区
 */

/**
 * @brief 缓冲区主要是为向某一空间中频繁的追加信息提供简化的操作，同时可以防止数据越界，缓冲区以两种方式提供操作\n
 * 空间，一种是用户自己分配空间，然后通过ul_buff_bind()函数将空间与缓冲区绑定，此种方式不能对空间进行自动\n
 * 扩展，与之对应的要通过ul_buff_unbind()解除缓冲区与空间的绑定，空间需要用户自己释放；另一种方式是通过\n
 * ul_buff_alloc()自动为缓冲区分配空间，缺省不进行空间的自动扩展，如果需要空间可以自动扩展，可以通过\n
 * ul_buff_set_realloc_size()设置每次扩展的大小，与之相对应的，需通过ul_buff_free()释放空间。
 */



#ifndef __UL_BUFF__
#define __UL_BUFF__


#define F_REALLOC		0x01
#define F_BINDED		0x02

typedef struct {
	int  pos;		// current position
	int  len;		// buffer's length
	int  ralen;		// realloc size of increament
	char flag;		// internal flag
        char *buffer;		// start position
}Buff;


/**
 *  初始化缓冲区结构
 *  
 *  @param[in]   buff  Buff的指针
 *  @param[out]  buff  Buff初始化后的结果
 *  @return 操作结果
 * - 0      成功
 * - -1     失败
 */
int ul_buff_init(Buff *buff);


/**
 *  为缓冲区绑定空间
 *  
 *  @param[in]  buff    Buff的指针
 *  @param[in]  buffer  代绑定的buf
 *  @param[in]  len     buf长度
 *  @param[out] buff    绑定操作后的结构
 *  @return 操作结果
 * - 0      成功
 * - -1     失败
 *  @note 绑定模式下,当buf满时,不支持自动分配空间,需要函数应用着注意这方面的判断
 */
int ul_buff_bind(Buff *buff, char *buffer, int len);



/**
 *  取消缓冲区的空间绑定
 *  
 *  @param[in]  buff    Buff的指针
 *  @param[out] 无
 *  @return 空间指针
 * - 非NULL      成功
 * - NULL        失败
 *  @note 此函数与::ul_buff_bind函数配对使用
 */
char *ul_buff_unbind(Buff *buff);




/**
 *  为缓冲区分配空间
 *  
 *  @param[in]  buff    Buff的指针
 *  @param[in]  len     缓冲区大小
 *  @param[out] 无
 *  @return 操作结果
 * - 0      成功
 * - -1     失败
 *  @note 此函数不可用于绑定的模式
 */
int ul_buff_alloc(Buff *buff, int len);


/**
 *  释放缓冲区空间
 *  
 *  @param[in]  buff    Buff的指针
 *  @param[out] 无
 *  @return 操作结果
 * - 0      成功
 * - -1     失败
 *  @note 此函数不可用于绑定的模式
 */
int ul_buff_free(Buff *buff);



/**
 *  设置空间扩展的增量大小
 *  
 *  @param[in]  buff    Buff的指针
 *  @param[in]  size    增量大小
 *  @param[out] 无
 *  @return 操作结果
 * - 0      成功
 * - -1     失败
 *  @note 绑定的空间不能进行扩展,即被绑定的空间不能重新分配
 */
int ul_buff_set_realloc_size(Buff *buff, int size);


/**
 *  输出一个字符到缓冲区
 *  
 *  @param[in]  buff    Buff的指针
 *  @param[in]  ch      输出的字符
 *  @param[out] 无
 *  @return 操作结果
 * - 1      成功
 * - 0      空间不够
 * - -1     失败
 */
int ul_buff_putc(Buff *buff, char ch);

/**
 *  输出一个字符串到缓冲区
 *  
 *  @param[in]  buff    Buff的指针
 *  @param[in]  str     输出的字符
 *  @param[out] 无
 *  @return 操作结果
 * - 1      成功
 * - 0      空间不够
 * - -1     失败
 */
int ul_buff_puts(Buff *buff, char *str);


/**
 *  输出一个N个字节到缓冲区
 *  
 *  @param[in]  buff    Buff的指针
 *  @param[in]  src     指向待输出的数据
 *  @param[in]  len     待输出数据的长度
 *  @param[out] 无
 *  @return 操作结果
 * - >0     成功,实际输出的字节数
 * - 0      空间不够
 * - -1     失败
 */
int ul_buff_copy(Buff *buff, void *src, int len);



/**
 *  格式化输出到缓冲区
 *  
 *  @param[in]  buff     Buff的指针
 *  @param[in]  fmt, ... 请参见C函数printf
 *  @param[out] 无
 *  @return 操作结果
 * - >0     成功,实际输出的字节数
 * - 0      空间不够
 * - -1     失败
 */
int ul_buff_printf(Buff *buff, char *fmt, ...);



/**
 *  获取缓冲区当前空间的大小
 *  
 *  @param[in]  buff     Buff的指针
 *  @param[out] 无
 *  @return 操作结果
 * - >0     成功,缓冲区空间的大小
 * - -1     空间未被绑定或未分配
 */
int ul_buff_get_buffer_size(Buff *buff);


/**
 *  获取缓冲区当前数据长度
 *  
 *  @param[in]  buff     Buff的指针
 *  @param[out] 无
 *  @return 操作结果
 * - >0     成功,缓冲区空间的数据长度
 * - -1     空间未被绑定或未分配
 */
int ul_buff_get_data_len(Buff *buff);


/**
 *  获取缓冲区的空间地址
 *  
 *  @param[in]  buff     Buff的指针
 *  @param[out] 无
 *  @return 指向空间的指针
 * - 非NULL      成功
 * - NULL        失败,空间未被绑定或未分配
 */
char *ul_buff_get_buffer(Buff *buff);

#endif
