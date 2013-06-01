/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file Number.h
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_NUMBER_H__
#define  __BSL_VAR_NUMBER_H__

#include <cstdlib>
#include <climits>
#include "bsl/var/IVar.h"
#include "bsl/check_cast.h"
#include "bsl/var/var_traits.h"

namespace bsl{
namespace var{
    template<typename T>
    class Number: public IVar{
    public:
        typedef IVar::string_type   string_type;
        typedef IVar::field_type    field_type;

    public:
        //special methods
        Number( T __value = 0 ) :_value(__value){}

        Number( const Number& other )
            :IVar(other), _value( other._value ) {}

        Number& operator = ( const Number& other ){
            _value = other._value;
            return *this;
        }

        //methods for all
        virtual Number& operator = ( IVar& var ) {
            try{
                if ( var.is_double() ){
                    _value = check_cast<T,double>( var.to_double() );
                }else if ( var.is_uint64() ){
                    _value = check_cast<T,unsigned long long int>( var.to_uint64() );
                }else{
                    _value = check_cast<T,long long int>( var.to_int64() );
                }
                
            }catch(bsl::Exception& e){
                e<<"{"<<__PRETTY_FUNCTION__<<"("<<var.dump()<<")}";
                throw;
            }
            return *this;
        }

        /**
         * @brief 清理函数
         *
         * @return  void 
         * @retval   
         * @see 
         **/
        virtual void clear(){
            _value = 0;
        }

        /**
         * @brief 克隆函数
         *
         * 该函数只克隆本身结点，不克隆子结点，对引用类型，克隆指向的结点
         *
         * @param [in] rp   : bsl::ResourcePool&
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& clone( bsl::ResourcePool& rp ) const {
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
            return  string_type("[bsl::var::Number<").append(typeid(T).name()).append(">]").append(check_cast<string_type>(_value));
        }

        /**
         * @brief 转化为字符串
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type to_string() const {
            return  check_cast<string_type>(_value);
        }

        /**
         * @brief 获取类型字符串（所有var类型都支持，仅用于调试使用，勿用于类型判断）
         *
         * @return  string_type 
         * @retval   
         * @see 
        **/
        virtual string_type get_type() const {
            return "bsl::var::Number";
        }

        /**
         * @brief 获取类型掩码（所有var类型都支持）
         *
         * @return  IVar::mask_type 
         * @retval   
         * @see 
        **/
        virtual IVar::mask_type get_mask() const {
            return var_traits<T>::MASK;
        }

        /**
         * @brief 转化为布尔类型
         *
         * @return  bool 
         * @retval   
         * @see 
        **/
        virtual bool to_bool() const {
            return _value != 0;
        }

        /**
         * @brief 转化为8位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed char 
         * @retval   
         * @see 
        **/
        virtual signed char to_int8() const {
            return check_cast<signed char,T>(_value);
        }

        /**
         * @brief 转化为8位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned char 
         * @retval   
         * @see 
        **/
        virtual unsigned char to_uint8() const {
            return check_cast<unsigned char, T>(_value);
        }

        /**
         * @brief 转化为16位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed short 
         * @retval   
         * @see 
        **/
        virtual signed short to_int16() const {
            return check_cast<signed short, T>(_value);
        }

        /**
         * @brief 转化为16位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned short 
         * @retval   
         * @see 
        **/
        virtual unsigned short to_uint16() const {
            return check_cast<unsigned short,T>(_value);
        }

        /**
         * @brief 转化为32位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed int 
         * @retval   
         * @see 
        **/
        virtual signed int to_int32() const {
            return check_cast<signed int, T>(_value);
        }

        /**
         * @brief 转化为32位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned int 
         * @retval   
         * @see 
        **/
        virtual unsigned int to_uint32() const {
            return check_cast<unsigned int, T>(_value);
        }

        /**
         * @brief 转化为64位有符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  signed long long 
         * @retval   
         * @see 
        **/
        virtual signed long long to_int64() const {
            return check_cast<signed long long,T>(_value);
        }

        /**
         * @brief 转化为64位无符号整数类型（所有is_number()返回真的类型都支持）
         *
         * @return  unsigned long long 
         * @retval   
         * @see 
        **/
        virtual unsigned long long to_uint64() const {
            return check_cast<unsigned long long,T>(_value);
        }

        /**
         * @brief 转化为单精度浮点数类型（所有is_number()返回真的类型都支持）
         *
         * @return  float 
         * @retval   
         * @see 
        **/
        virtual float to_float() const {
            return check_cast<float,T>(_value);
        }

        /**
         * @brief 转化为双精度浮点数类型（所有is_number()返回真的类型都支持）
         *
         * @return  double 
         * @retval   
         * @see 
        **/
        virtual double to_double() const {
            return check_cast<double,T>(_value);
        }

        virtual Number& operator = ( bool b ){
            _value = check_cast<T>(b);
            return *this;
        }

        /**
         * @brief 使用signed char类型赋值
         *
         * @param [in] val   : signed char
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( signed char i ){
            _value = check_cast<T, signed char>(i);
            return *this;
        }

        /**
         * @brief 使用unsigned char类型赋值
         *
         * @param [in] val   : unsigned char
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( unsigned char i ){
            _value = check_cast<T, unsigned char>(i);
            return *this;
        }

        /**
         * @brief 使用signed short类型赋值
         *
         * @param [in] val   : signed short
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( signed short i ){
            _value = check_cast<T, signed short>(i);
            return *this;
        }

        /**
         * @brief 使用unsigned short类型赋值
         *
         * @param [in] val   : unsigned short
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( unsigned short i ){
            _value = check_cast<T, unsigned short>(i);
            return *this;
        }

        /**
         * @brief 使用signed int类型赋值
         *
         * @param [in] val   : signed int
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( signed int i ){
            _value = check_cast<T, signed int>(i);
            return *this;
        }

        /**
         * @brief 使用unsigned int类型赋值
         *
         * @param [in] val   : unsigned int
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( unsigned int i ){
            _value = check_cast<T, unsigned int>(i);
            return *this;
        }

        /**
         * @brief 使用signed long long类型赋值
         *
         * @param [in] val   : signed long long
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( signed long long i ){
            _value = check_cast<T, signed long long>(i);
            return *this;
        }

        /**
         * @brief 使用unsigned long long类型赋值
         *
         * @param [in] val   : unsigned long long
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( unsigned long long i ){
            _value = check_cast<T, unsigned long long>(i);
            return *this;
        }

        /**
         * @brief 使用float类型赋值
         *
         * @param [in] val   : float
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( float f ){
            _value = check_cast<T, float>(f);
            return *this;
        }

        /**
         * @brief 使用double类型赋值
         *
         * @param [in] val   : double
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( double d ){
            _value = check_cast<T,double>(d);
            return *this;
        }

        /**
         * @brief 使用const char *类型赋值
         *
         * @param [in] val   : const char *
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( const char * cstr ){
            _value = check_cast<T>(cstr);
            return *this;
        }

        /**
         * @brief 使用字符串类型赋值
         *
         * @param [in] val   : const string_type&
         * @return  Number& 
         * @retval   
         * @see 
        **/
        virtual Number& operator = ( const string_type& str ){
            _value = check_cast<T>(str.c_str());
            return *this;
        }

        //use default version for raw
        using IVar::operator =;

        //testers
        virtual bool is_number() const {
            return true;
        }

        virtual bool is_int8() const {
            return typeid(T) == typeid(signed char);
        }

        virtual bool is_uint8() const {
            return typeid(T) == typeid(unsigned char);
        }

        virtual bool is_int16() const {
            return typeid(T) == typeid(signed short);
        }

        virtual bool is_uint16() const {
            return typeid(T) == typeid(unsigned short);
        }

        virtual bool is_int32() const {
            return typeid(T) == typeid(signed int);
        }

        virtual bool is_uint32() const {
            return typeid(T) == typeid(unsigned int);
        }

        virtual bool is_int64() const {
            return typeid(T) == typeid(signed long long);
        }

        virtual bool is_uint64() const {
            return typeid(T) == typeid(unsigned long long);
        }

        virtual bool is_float() const {
            return typeid(T) == typeid(float);
        }

        virtual bool is_double() const {
            return typeid(T) == typeid(double);
        }

    private:
        T _value;
    };

}}   //namespace bsl::var

#endif  //__BSL_VAR_NUMBER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
