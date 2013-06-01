/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file cc_trans.h
 * @brief 
 *  
 **/


#ifndef  __CC_TRANS_H_
#define  __CC_TRANS_H_

#include "cc_struct.h"

namespace comcfg{
	class Trans{
		public:
#if 0
			static const unsigned long long max_uint64 = 0xFFFFFFFFFFFFFFFF;
			static const unsigned long long min_uint64 = 0x0;
			static const unsigned long long max_int64 = 0x7FFFFFFFFFFFFFFF;
			static const unsigned long long min_int64 = 0x8000000000000000;
			static const unsigned long long max_uint32 = 0xFFFFFFFF;
			static const unsigned long long min_uint32 = 0x0;
			static const unsigned long long max_int32 = 0x7FFFFFFF;
			static const unsigned long long min_int32 = 0x80000000;
			static const unsigned long long max_uint16 = 0xFFFF;
			static const unsigned long long min_uint16 = 0x0;
			static const unsigned long long max_int16 = 0x7FFF;
			static const unsigned long long min_int16 = 0x8000;
#endif
		public:
			/**
			 * @brief 判断一个字符串是否是整数格式
			 *
			 * @param [in] str   : const str_t&
			 * @return  bool 
			 * @retval   
			 * @see 
			**/
			static bool isInteger(const str_t & str);
			/**
			 * @brief 判断一个字符串是否是浮点数格式（不检查精度范围）
			 *
			 * @param [in] str   : const str_t&
			 * @return  bool 
			 * @retval   
			 * @see 
			**/
			static bool isFloat(const str_t & str);
			/**
			 * @brief 把一个字符串转换成一个int64
			 *
			 * @param [in] __str   : const str_t& bsl::string类型的字符串
			 * @param [out] buf   : int64*  转换结果
			 * @return  int 0成功，失败：OUTOFRANGE, NULLVALUE, FORMATERROR
			 * @retval   
			 * @see 
			**/
			static int str2int64(const str_t & __str, long long * buf);
			/**
			 * @brief 把一个字符串转换成一个uint64
			 *
			 * @param [in] __str   : const str_t& bsl::string类型的字符串
			 * @param [out] buf   : uint64* 转换结果
			 * @return  int 0成功，失败：OUTOFRANGE, NULLVALUE, FORMATERROR
			 * @retval   
			 * @see 
			**/
			static int str2uint64(const str_t & __str, unsigned long long * buf);
			/**
			 * @brief 把一个带有转义字符的字符串进行解析（把\xFF, \022, \n之类的文本转换成实际的字符）
			 *         这个串必须是双引号开头、双引号结尾。
			 *
			 *         【configure内部使用，如不熟悉内部行为，不推荐使用】
			 *
			 * @param [in] __str   : const str_t& bsl::string类型的字符串
			 * @param [out] buf   : uint64* 转换结果
			 * @return  int 0成功，失败：FORMATERROR
			 * @retval   
			 * @see 
			**/
			static int str2str(const str_t & str, str_t * buf);
			/**
			 * @brief 把一个字符串转换成一个double
			 *
			 * @param [in] __str   : const str_t& bsl::string类型的字符串
			 * @param [out] buf   : double* 转换结果
			 * @return  int 0成功，失败：OUTOFRANGE, NULLVALUE, FORMATERROR
			 * @retval   
			 * @see 
			**/
			static int str2double(const str_t & str, double * buf);

			//仅限configure使用
			static str_t cutComment(const str_t & str);
	};
}
















#endif  //__CC_TRANS_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
