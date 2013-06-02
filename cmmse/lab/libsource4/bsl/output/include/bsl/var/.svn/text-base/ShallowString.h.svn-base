/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file ShallowString.h
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_SHALLOW_STRING_H__
#define  __BSL_VAR_SHALLOW_STRING_H__
#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/check_cast.h"
#include "bsl/ShallowCopyString.h"

namespace bsl{
namespace var{
    class ShallowString: public IVar{
    public:
        typedef IVar::string_type   string_type;
        typedef IVar::field_type    field_type;

    public:
        //special methods
        ShallowString( )
            : _value() {}

        ShallowString( const ShallowString& other )
            : IVar(other), _value(other._value) { }

        ShallowString( const char * cstr )
            : IVar(), _value(cstr) { }

        ShallowString( const char * cstr, size_t len )
            : IVar(), _value(cstr, len) { }

        ShallowString( const bsl::ShallowCopyString& value_)
            : IVar(), _value( value_.c_str(), value_.size() ) { }


        ShallowString( const string_type& value_ )
            : IVar(), _value(value_.c_str(), value_.size()){}

        ShallowString& operator = ( const ShallowString& other ){
            _value = other._value;
            return *this;
        }

        virtual ~ShallowString(){
            //pass
        }

        //methods for all
        virtual ShallowString& operator = ( IVar& var ){
            if ( var.is_string() ){
                _value = var.c_str();
                return *this;
            }else{
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<_value.c_str()<<"] cast from["<<var.dump(0)<<"]";
            }
        }

        /**
         * @brief 输出调试信息（仅用于调试）可通过verbose_level控制递归层数
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type dump(size_t /*verbose_level*/=0) const {
            return  string_type("[bsl::var::ShallowString]").append(_value.c_str(), _value.size());
        }

        /**
         * @brief 转化为字符串
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type to_string() const {
            return _value.c_str();
        }

        /**
         * @brief 获取类型字符串（所有var类型都支持，仅用于调试使用，勿用于类型判断）
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type get_type() const {
            return "bsl::var::ShallowString";
        }

        /**
         * @brief 获取类型掩码（所有var类型都支持）
         *
         * @return  IVar::mask_type 
         * @retval   
         * @see 
        **/
        virtual IVar::mask_type get_mask() const {
            return IVar::IS_STRING;
        }

        /**
         * @brief 清理函数
         *
         * @return  void 
         * @retval   
         * @see 
         **/
        virtual void clear(){
            _value.clear();
        }

        /**
         * @brief 克隆函数
         *
         * 该函数只克隆本身结点，不克隆子结点，对引用类型，克隆指向的结点
         *
         * @param [in] rp   : bsl::ResourcePool&
         * @return  ShallowString& 
         * @retval   
         * @see 
        **/
        virtual ShallowString& clone( bsl::ResourcePool& rp ) const {
            return rp.clone(*this);
        }

        //testers
        /**
         * @brief 判断是否是字符串类型
         *
         * @return  bool 
         * @retval   
         * @see 
        **/
        virtual bool is_string() const {
            return true;
        }

        //methods for value
        /**
         * @brief 转化为布尔类型
         *
         * @return  bool 
         * @retval   
         * @see 
        **/
        virtual bool to_bool() const {
            return _value.c_str()[0] != '\0';
        }

        /**
         * @brief 转化为8位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed char 
         * @retval   
         * @see 
        **/
        virtual signed char to_int8() const {
            return check_cast<signed char>(_value.c_str());
        }

        /**
         * @brief 转化为8位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned char 
         * @retval   
         * @see 
        **/
        virtual unsigned char to_uint8() const {
            return check_cast<unsigned char>(_value.c_str());
        }

        /**
         * @brief 转化为16位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed short 
         * @retval   
         * @see 
        **/
        virtual signed short to_int16() const {
            return check_cast<signed short>(_value.c_str());
        }

        /**
         * @brief 转化为16位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned short 
         * @retval   
         * @see 
        **/
        virtual unsigned short to_uint16() const {
            return check_cast<unsigned short>(_value.c_str());
        }

        /**
         * @brief 转化为32位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed int 
         * @retval   
         * @see 
        **/
        virtual signed int to_int32() const {
            return check_cast<signed int>(_value.c_str());
        }

        /**
         * @brief 转化为32位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned int 
         * @retval   
         * @see 
        **/
        virtual unsigned int to_uint32() const {
            return check_cast<unsigned int>(_value.c_str());
        }

        /**
         * @brief 转化为64位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed long long 
         * @retval   
         * @see 
        **/
        virtual signed long long to_int64() const {
            return check_cast<signed long long>(_value.c_str());
        }

        /**
         * @brief 转化为64位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned long long 
         * @retval   
         * @see 
        **/
        virtual unsigned long long to_uint64() const {
            return check_cast<unsigned long long>(_value.c_str());
        }

        /**
         * @brief 转化为单精度浮点数类型（所有is_number()返回真的类型都支持）
         *
         * @return  float 
         * @retval   
         * @see 
        **/
        virtual float to_float() const {
            return check_cast<float>(_value.c_str());
        }

        /**
         * @brief 转化为双精度浮点数类型（所有is_number()返回真的类型都支持）
         *
         * @return  double 
         * @retval   
         * @see 
        **/
        virtual double to_double() const {
            return check_cast<double>(_value.c_str());
        }

        /**
         * @brief 使用const char *类型赋值
         *
         * @param [in] val   : const char *
         * @return  ShallowString& 
         * @retval   
         * @see 
        **/
        virtual ShallowString& operator = ( const char * cstr ){
            _value = cstr;
            return *this;
        }

        /**
         * @brief 使用字符串类型赋值
         *
         * @param [in] val   : const string_type&
         * @return  ShallowString& 
         * @retval   
         * @see 
        **/
        virtual ShallowString& operator = ( const string_type& str ){
            _value = str;
            return *this;
        }

        //use default version for bool、raw、number
        using IVar::operator =;

        /**
         * @brief 返回C风格字符串表示
         *
         * 所有is_string()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * @return  const char* 
         * @retval   
         * @see 
        **/
        virtual const char * c_str() const {
            return _value.c_str();
        }

        /**
         * @brief 返回C风格字符串表示的长度，不包括末尾的'\0'
         *
         * 所有is_string()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * @return  size_t 
         * @retval   
         * @see 
        **/
        virtual size_t c_str_len() const {
            return _value.size();
        }

    private:
        bsl::ShallowCopyString _value;
    };

}}//namespace bsl::var

#endif  //__BSL_VAR_SHALLOW_STRING_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
