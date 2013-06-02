/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file utils.h
 * @brief 
 *  
 **/
#ifndef  __UTILS_H_
#define  __UTILS_H_
#include "bsl/var/IVar.h"
#include "bsl/var/assign.h"
#include "bsl/var/var_traits.h"

namespace bsl{
    namespace var{
        /**
         * @brief 递归打印IVar数据到字符串
         *
         * @param [in] var   : const IVar&              被打印的IVar根结点
         * @param [in] res   : IVar::string_type&       打印到的字符串
         * @param [in] verbose_level   : size_t         递归最大层数
         * @param [in] line_delimiter   : const char*   分行符
         * @param [in] per_indent   : size_t            每次缩进字节数
         * @param [in] total_indent   : size_t          总缩进字节数
         * @return  void 
         * @retval   
         * @see 
        **/
        void dump_to_string(const IVar& var, IVar::string_type& res, size_t verbose_level, const char *line_delimiter, size_t per_indent = 4, size_t total_indent = 0 );

        /**
         * @brief 递归、缩进打印IVar到STDOUT，GDB调试专用
         *
         * @param [in] var   : const IVar&
         * @param [in] verbose_level   : size_t
         * @return  void 
         * @retval   
         * @see 
        **/
        void print( const IVar& var, size_t verbose_level );
    }
}
#endif  //__UTILS_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
