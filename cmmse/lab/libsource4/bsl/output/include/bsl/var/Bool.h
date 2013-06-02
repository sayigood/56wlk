/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file Bool.h
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_BOOL_H__
#define  __BSL_VAR_BOOL_H__
#include "bsl/var/IVar.h"

namespace bsl{
namespace var{
    class Bool: public IVar{
    public:
        typedef IVar::string_type           string_type;
        typedef IVar::field_type            field_type;

        //special methods
        Bool()
            :_value(false){}

        Bool( const Bool& other )
            :IVar(other), _value(other._value){ }

        Bool( bool value_ )
            :IVar(), _value(value_){}

        virtual ~Bool(){ } 

        Bool& operator = ( const Bool& other ){
            _value = other._value;
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
            _value = false;
        }

        /**
         * @brief 克隆函数
         *
         * 该函数只克隆本身结点，不克隆子结点，对引用类型，克隆指向的结点
         *
         * @param [in] rp   : bsl::ResourcePool&
         * @return  Bool& 
         * @retval   
         * @see 
        **/
        virtual Bool& clone( bsl::ResourcePool& rp ) const {
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
            if ( _value ){
                return "[bsl::var::Bool]true";
            }else{
                return "[bsl::var::Bool]false";
            }
        }

        /**
         * @brief 转化为字符串
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type to_string() const {
            if ( _value ){
                return "true";
            }else{
                return "false";
            }
        }

        /**
         * @brief 获取类型字符串（所有var类型都支持，仅用于调试使用，勿用于类型判断）
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type get_type() const {
            return "bsl::var::Bool";
        }

        /**
         * @brief 获取类型掩码（所有var类型都支持）
         *
         * @return  IVar::mask_type 
         * @retval   
         * @see 
        **/
        virtual IVar::mask_type get_mask() const {
            return IVar::IS_BOOL;
        }

        virtual Bool& operator = ( IVar& other ){
            _value = other.to_bool();
            return *this;
        }

        virtual bool is_bool() const {
            return true;
        }

        //all other is_xxx() return false;

        //methods for bool
        virtual Bool& operator = ( bool val ){
            _value = val;
            return *this;
        }

        virtual Bool& operator = ( int val ){
            _value = val;
            return *this;
        }

        virtual Bool& operator = ( long long val ){
            _value = val;
            return *this;
        }

        /**
         * @brief 使用float类型赋值
         *
         * @param [in] val   : float
         * @return  Bool& 
         * @retval   
         * @see 
        **/
        virtual Bool& operator = ( float f ){
            _value = f;
            return *this;
        }

        /**
         * @brief 使用double类型赋值
         *
         * @param [in] val   : double
         * @return  Bool& 
         * @retval   
         * @see 
        **/
        virtual Bool& operator = ( double val ){
            _value = val;
            return *this;
        }

        /**
         * @brief 使用const char *类型赋值
         *
         * @param [in] val   : const char *
         * @return  Bool& 
         * @retval   
         * @see 
        **/
        virtual Bool& operator = ( const char * val ){
            _value = (NULL != val && '\0' != val[0]);
            return *this;
        }

        /**
         * @brief 使用字符串类型赋值
         *
         * @param [in] val   : const string_type&
         * @return  Bool& 
         * @retval   
         * @see 
        **/
        virtual Bool& operator = ( const string_type& val ){
            _value = (val.c_str()[0] != '\0') ;
            return *this;
        }

        /**
         * @brief 转化为布尔类型
         *
         * @return  bool 
         * @retval   
         * @see 
        **/
        virtual bool to_bool() const {
            return _value;
        }

        /**
         * @brief 转化为8位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed char 
         * @retval   
         * @see 
        **/
        virtual signed char to_int8() const {
            return static_cast<signed char>(_value);
        }

        /**
         * @brief 转化为8位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned char 
         * @retval   
         * @see 
        **/
        virtual unsigned char to_uint8() const {
            return static_cast<unsigned char>(_value);
        }

        /**
         * @brief 转化为16位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed short 
         * @retval   
         * @see 
        **/
        virtual signed short to_int16() const {
            return static_cast<signed short>(_value);
        }

        /**
         * @brief 转化为16位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned short 
         * @retval   
         * @see 
        **/
        virtual unsigned short to_uint16() const {
            return static_cast<unsigned short>(_value);
        }

        /**
         * @brief 转化为32位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed int 
         * @retval   
         * @see 
        **/
        virtual signed int to_int32() const {
            return static_cast<signed int>(_value);
        }

        /**
         * @brief 转化为32位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned int 
         * @retval   
         * @see 
        **/
        virtual unsigned int to_uint32() const {
            return static_cast<unsigned int>(_value);
        }

        /**
         * @brief 转化为64位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed long long 
         * @retval   
         * @see 
        **/
        virtual signed long long to_int64() const {
            return static_cast<signed long long>(_value);
        }

        /**
         * @brief 转化为64位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned long long 
         * @retval   
         * @see 
        **/
        virtual unsigned long long to_uint64() const {
            return static_cast<unsigned long long>(_value);
        }

        /**
         * @brief 转化为单精度浮点数类型（所有is_number()返回真的类型都支持）
         *
         * @return  float 
         * @retval   
         * @see 
        **/
        virtual float to_float() const {
            return static_cast<float>(_value);
        }

        /**
         * @brief 转化为双精度浮点数类型（所有is_number()返回真的类型都支持）
         *
         * @return  double 
         * @retval   
         * @see 
        **/
        virtual double to_double() const {
            return static_cast<double>(_value);
        }

        //using default version for raw
        using IVar::operator =;
    private:
        bool _value;
    };

}}   //namespace bsl::var
#endif  //__BSL_VAR_BOOL_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
