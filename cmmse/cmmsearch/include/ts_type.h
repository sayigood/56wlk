/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: ts_type.h,v 1.2 2009/11/18 20:27:50 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file ts_type.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/07/24 13:41:35
 * @version $Revision: 1.2 $ 
 * @brief 自定义类型
 *  
 **/


#ifndef  __TS_TYPE_H_
#define  __TS_TYPE_H_


//#include <stdio.h>
//#include <stdlib.h>
#include <vector>
using namespace std;

#pragma pack (4)    ///保持四字节对齐
/** @brief 64位签名 */
typedef struct sign64_t
{
    uint32 sign1;
    uint32 sign2;
} sign64_t;

/** @brief 指针类型 */
typedef void *pointer_t;

/** @brief 初始化函数类型 */
typedef int (*ts_init_func_t) ();

/** @brief 硬盘索引文件的term类型 */
typedef struct ts_disk_term_t
{
    union
    {
        sign64_t term;
        struct
        {
            uint32 sign1;
            uint32 sign2;
        };
    };
    long long offset;
    int dlen;

} ts_disk_term_t;

/** @brief 索引结构 */
typedef struct ts_index_t
{
    uint32 id;                /**< 内部id       */
	uint32 sweight;           /**< 静态权重       */

} ts_index_t;




/** @brief term信息 */
typedef struct _ts_terminfo_t
{
    sign64_t term;
    union
    {
        uint32 weight;
        uint32 ind_num;
    };
} ts_terminfo_t;

/** 存储从req中获得的结果数信息，以及得到的结果情况*/
typedef struct _basic_req_info_t
{
    char cmd_str[TS_CMDSTR_MAXLEN];        /**< 命令号     */
    uint32 err_no;                         /**< 错误号     */

    uint32 page_no;                        /**< 页号      */
    uint32 num_per_page;                   /**< 每页显示条数       */

    uint32 ret_num;                        /**< 返回个数      */
    uint32 disp_num;                       /**< 总共查询到的个数      */
} basic_req_info;


/** 道排拉链中用的信息*/
typedef struct _brief_t
{
    uint32 pubtime;
    uint32 msgsign1;
    uint32 msgsign2;
    
    /*
     * 31 30 ... 5 4 3 2 1 0
     * 0~2标志来源：000--采集 001--微信 010--www发布
     * */
    uint32 mark;
    
}brief_t;
/**
* @brief buffer类
*   用于存储一些小型的buffer
 */
class ts_buffer_t
{

	public:
		const uint32 _size;
		uint32 _used;
		char *_buf;
		bool no_alloc;
	public:
		ts_buffer_t(char *buf, uint32 size):_size(size)
		{
			_buf = buf;
			no_alloc = true;
		};

		ts_buffer_t(uint32 size):_size(size)
		{
			no_alloc = false;
			_used = 0;
			_buf = (char *)malloc(_size);   ///小数据buffer，不考虑分配不出的情况
		};
		~ts_buffer_t()
		{
			if(!no_alloc && _buf)
				free(_buf);
			_used = 0;
		}
		void reset()
		{
			_used = 0;
		}
	private:
		ts_buffer_t();
};
#pragma pack()










#endif //__TS_TYPE_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
