/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file check_cast.h
 * @brief 
 *  
 **/
#ifndef  __CHECK_CAST_H_
#define  __CHECK_CAST_H_

#include "bsl/exception/bsl_exception.h"

namespace bsl{
    /**
     * @brief check_cast<>模板函数
     *
     * 提供对C/C++各原生类型提供带上下溢出检查功能模板函数。使用方式与static_cast<>完全相同。
     *
     * 目前支持所有有符号数值类型、无符号数值类型、浮点数类型中任意两类型的双向转换以及C风格字符串（const char *）到以上各类型的单向转换。不考虑浮点数转换成浮点数类型的数值溢出问题。
     *
     * @param [in] value   : SrcType
     * @return  DestType 
     * @retval   
     * @see 
    **/
    template<typename DestType, typename SrcType>
        DestType check_cast( SrcType value );


    /**
     * @brief check_cast<>模板函数在DestType == SrcType时的偏特化版本
     *
     * @param [in] value   : DestType
     * @return  DestType 
     * @retval   
     * @see 
    **/
    template<typename DestType>
        inline DestType check_cast( DestType value ){
            return value;
        }

}
#include "bsl/check_cast/check_cast_generated.h"   //generated code
#include "bsl/check_cast/check_cast_cstring.h"
#include "bsl/check_cast/check_cast_bsl_string.h"

#endif  //__CHECK_CAST_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
