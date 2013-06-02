/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file Null.h
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_NULL_H__
#define  __BSL_VAR_NULL_H__
#include "bsl/var/IVar.h"

namespace bsl{
namespace var{
    class Null: public IVar{
    public:
        typedef IVar::string_type          string_type;
        typedef IVar::field_type           field_type;

        //special methods
        Null(){ }

        Null( const Null& other ):IVar(other){ }

        virtual ~Null(){ } 

        Null& operator = ( const Null& ){
            return *this;
        }

        //methods for all
        /**
         * @brief 清理函数
         *
         * @return  void 
         * @retval   
         * @see 
         **/
        virtual void clear(){
            // pass
        }

        /**
         * @brief 克隆函数
         *
         * 该函数只克隆本身结点，不克隆子结点，对引用类型，克隆指向的结点
         *
         * @param [in] rp   : bsl::ResourcePool&
         * @return  Null& 
         * @retval   
         * @see 
        **/
        virtual Null& clone( bsl::ResourcePool& rp ) const {
            return rp.clone(*this);
        }

        /**
         * @brief 输出调试信息（仅用于调试）可通过verbose_level控制递归层数
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type dump(size_t /*verbose_level*/ = 0) const {
            return "[bsl::var::Null]null";
        }

        /**
         * @brief 转化为字符串
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type to_string() const {
            return "null";
        }

        /**
         * @brief 获取类型字符串（所有var类型都支持，仅用于调试使用，勿用于类型判断）
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type get_type() const {
            return "bsl::var::Null";
        }

        /**
         * @brief 获取类型掩码（所有var类型都支持）
         *
         * @return  IVar::mask_type 
         * @retval   
         * @see 
        **/
        virtual IVar::mask_type get_mask() const {
            return 0;
        }

        virtual Null& operator = ( IVar& var ){
            if ( !var.is_null() ){
                throw bsl::InvalidOperationException()<<BSL_EARG<<"{"<<__PRETTY_FUNCTION__<<"("<<var.dump()<<")}";
            }
            return *this;
        }

        virtual bool is_null() const {
            return true;
        }

        using IVar::operator =;

    public:
        static Null null;
    };

}}   //namespace bsl::var
#endif  //__BSL_VAR_NULL_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
