/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file AutoBuffer.h
 * @brief 
 *  
 **/
#ifndef  __BSL_AUTO_BUFFER_H_
#define  __BSL_AUTO_BUFFER_H_
#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "bsl/utils/bsl_memcpy.h"

namespace bsl{
    /**
    * @brief 类似于auto_ptr但更加安全的字符串缓冲区类
    *  
    *  该类的一大特点是不会抛出异常。在内存不足的时候，该类会截断字符串并置"被截断位"。
    *  AutoBuffer对象自行管理一片用于表示字符串的缓冲区，并提供方法追加各种类型对象。
    *  若内存不足时，内存容量将番倍，若申请新内存失败，将填满旧内存，而且使truncated()方法返回true。
    */
    class AutoBuffer{
    public:
        /**
         * @brief 构造函数
         *
         * 可传入__capacity参数指定预分配的内存空间。如__capacity==0没有动态内存分配。
         *
         * 注：如内存分配失败不会抛出异常。其结果相当于以0为参数构造。
         *
         * @param [in] __capacity   : size_t
         * @see 
        **/
        explicit AutoBuffer( size_t __capacity = DEFAULT_CAPACITY )
            :_size(0), _capacity(__capacity), _truncated(false), _str(NULL){
            if ( __capacity != 0 ){
                _str = static_cast<char *>(malloc(_capacity+1));
                if ( _str ){
                    _str[0] = 0;
                }else{
                    _capacity = 0;
                }
            }
        }
        

        /**
         * @brief 把另一AutoBuffer的内存转移到本AutoBuffer（另一AutoBuffer会被清空），O(1)复杂度
         *
         * @param [in] other   : AutoBuffer&
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& transfer_from ( AutoBuffer& other ){
            if ( &other != this ){
                if ( _str ){
                    free( _str );
                }
                _size       = other._size;      other._size     = 0;
                _capacity   = other._capacity;  other._capacity = 0;
                _truncated  = other._truncated; other._truncated= false;
                _str        = other._str;       other._str      = NULL;
            }
            return *this;
        }

        /**
         * @brief 交换两AutoBuffer内容，O(1)复杂度
         *
         * @param [in] other   : AutoBuffer&
         * @return  void 
         * @retval   
         * @see 
        **/
        void swap( AutoBuffer& other ){
            std::swap( _str,        other._str );
            std::swap( _size,       other._size );
            std::swap( _capacity,   other._capacity );
            std::swap( _truncated,  other._truncated );
        }

        /**
         * @brief 析构函数
         *
         * @see 
        **/
        ~AutoBuffer( ){
            if ( _str ){
                free( _str );
            }
        }

        /**
         * @brief AutoBuffer长度。不包括最后的'\0'
         *
         * @return  size_t 
         * @retval   
         * @see 
        **/
        size_t size() const{
            return _size;
        }
        
        /**
         * @brief AutoBuffer当前容量。保证容量>=长度。当容量不足时，容量会自动增长。
         *
         * @return  size_t 
         * @retval   
         * @see 
        **/
        size_t capacity() const {
            return _capacity;
        }

        /**
         * @brief 返回AutoBuffer是否为空
         *
         * @return  bool 
         * @retval   
         * @see 
        **/
        bool empty() const {
            return _size == 0;
        }

        /**
         * @brief 返回AutoBuffer是否为满
         *
         * @return  bool 
         * @retval   
         * @see 
        **/
        bool full() const {
            return _size == _capacity;
        }

        /**
         * @brief 返回AutoBuffer是否发生了截断
         *
         * @return  bool 
         * @retval   
         * @see 
        **/
        bool truncated() const {
            return _truncated;
        }

        /**
         * @brief 返回AutoBuffer内容的C风格字符串表示。O(1)复杂度
         *
         * @return  const char* 
         * @retval   
         * @see 
        **/
        const char * c_str() const {
            if ( _str ){
                return _str;
            }else{
                return "";
            }
        }

        /**
         * @brief 清空内容
         *
         * @return  void 
         * @retval   
         * @see 
        **/
        void clear() {
            if ( _str && _capacity ){
                _str[0] = 0;
            }
            _size = 0;
        }

        /**
         * @brief 手动扩大内存容量
         *
         * @param [in] __capacity   : size_t
         * @return  bool 
         * @retval   
         * @see 
        **/
        bool reserve( size_t __capacity ) {
            if ( __capacity > _capacity  ){
                if ( __capacity < _capacity * 2 ){ 
                    __capacity = _capacity * 2;
                }

                char * _new = static_cast<char*>(malloc(__capacity + 1));
                if ( !_new ){
                    return false;
                }

                if ( _str ){
                    xmemcpy( _new, _str, _size + 1 );
                    free( _str );
                }

                _str        = _new;
                _capacity   = __capacity;
            }

            return true;
        }

        /**
         * @brief 追加另一个AutoBuffer
         *
         * @param [in] other   : const AutoBuffer&
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (const AutoBuffer& other){
            return push( other._str, other._size );
        }

        /**
         * @brief 追加布尔值
         *
         * @param [in] b   : bool
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (bool b){
            if ( b ){
                return push( TRUE_LITERAL, TRUE_LITERAL_LEN );
            }else{
                return push( FALSE_LITERAL, FALSE_LITERAL_LEN );
            }
        }

        /**
         * @brief 追加字符，忽略'\0'
         *
         * @param [in] c   : char
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (char c){
            if ( c == '\0' ){
                return *this;
            }
            if ( _size == _capacity ){    //full
                reserve( _size + 1 ); //may fail
            }
            if ( _size < _capacity ){
                _str[_size]    = c;
                _str[++_size]  = '\0';
                _truncated = false;
            }else{
                _truncated = true;
            }
            return *this;
       }

        /**
         * @brief 追加有符号8位整数
         *
         * @param [in] uc   : signed char
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (signed char sc){
            return pushf( "%hhd", sc );
        }

        /**
         * @brief 追加无符号8位整数
         *
         * @param [in] uc   : unsigned char
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (unsigned char uc){
            return pushf( "%hhu", uc );
        }

        /**
         * @brief 追加宽字符
         *
         * @param [in] wc   : wchar_t
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (wchar_t wc){
            if ( wc == 0 ){
                return *this;
            }
#if __GNUC__ <= 2
            wchar_t ws[] = { wc, 0 };
            return pushf( "%ls", ws );
#else
            return pushf( "%lc", wc );
#endif
        }

        /**
         * @brief 追加宽字符串
         *
         * @param [in] ws   : const wchar_t*
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (const wchar_t *ws){
            if ( ws != NULL ){
                pushf( "%ls", ws );
            }else{
                _truncated = false;
            }
            return *this;
        }

        /**
         * @brief 追加int型整数
         *
         * @param [in] i   : int
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (int i){
            return pushf( "%d", i );
        }

        /**
         * @brief 追加unsigned int型整数
         *
         * @param [in] i   : unsigned int
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (unsigned int i){
            return pushf( "%u", i );
        }

        /**
         * @brief 追加long int型整数
         *
         * @param [in] i   : long int
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (long int i){
            return pushf( "%ld", i );
        }

        /**
         * @brief 追加unsigned long int型整数
         *
         * @param [in] i   : unsigned long int
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (unsigned long int i){
            return pushf( "%lu", i );
        }

        /**
         * @brief 追加字符串
         *
         * @param [in] cstr   : const char*
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (const char* cstr ){
            if ( cstr != NULL ){
                push( cstr, strlen(cstr) );
            }else{
                _truncated = false;
            }
            return *this;
        }

        /**
         * @brief 追加long long型整数
         *
         * @param [in] ll   : long long
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (long long ll){
            return pushf( "%lld", ll );
        }

        /**
         * @brief 追加unsigned long long型整数
         *
         * @param [in] ll   : unsigned long long
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (unsigned long long ll){
            return pushf( "%llu", ll );
        }

        /**
         * @brief 追加double型浮点数
         *
         * @param [in] n   : double
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (double n){
            return pushf( "%lg", n );
        }

        /**
         * @brief 追加long double型浮点数
         *
         * @param [in] n   : long double
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (long double n){
            return pushf( "%Lg", n );
        }

        /**
         * @brief 追加指针字面值
         *
         * @param [in] p   : void*
         * @return  AutoBuffer&  operator 
         * @retval   
         * @see 
        **/
        AutoBuffer& operator << (void *p){
            return pushf( "%p", p );
        }

        /**
         * @brief 追加另一个AutoBuffer
         *
         * @param [in] other   : const AutoBuffer&
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(const AutoBuffer& other){
            return push( other._str, other._size );
        }

        /**
         * @brief 追加布尔值
         *
         * @param [in] b   : bool
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(bool b){
            if ( b ){
                return push( TRUE_LITERAL, TRUE_LITERAL_LEN );
            }else{
                return push( FALSE_LITERAL, FALSE_LITERAL_LEN );
            }
        }

        /**
         * @brief 追加字符，忽略'\0'
         *
         * @param [in] c   : char
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(char c){
            if ( c == '\0' ){
                return *this;
            }
            if ( _size == _capacity ){    //full
                reserve( _size + 1 ); //may fail
            }
            if ( _size < _capacity ){
                _str[_size]    = c;
                _str[++_size]  = '\0';
                _truncated = false;
            }else{
                _truncated = true;
            }
            return *this;
        }

        /**
         * @brief 追加有符号8位整数
         *
         * @param [in] uc   : signed char
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(signed char sc){
            return pushf("%hhd", sc);
        }

        /**
         * @brief 追加无符号8位整数
         *
         * @param [in] uc   : unsigned char
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(unsigned char uc){
            return pushf("%hhu", uc);
        }

        /**
         * @brief 追加多个字符，忽略'\0'
         *
         * @param [in] count   : int
         * @param [in] c   : char
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push( size_t count, char c){
            if ( c != '\0' ){
                if ( _size + count > _capacity ){    //full
                    if( !reserve( _size + count ) ){
                        //reserve fail
                        count = _capacity - _size;
                        _truncated = true;
                    }else{
                        _truncated = false;
                    }
                }
                if ( count ){
                    //str != NULL
                    memset( _str + _size, c, count );
                    _str[ _size+=count ] = '\0';
                }
            }
            return *this;
        }

        /**
         * @brief 追加宽字符，忽略'\0'
         *
         * @param [in] wc   : wchar_t
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(wchar_t wc){
            if ( wc == 0 ){
                return *this;
            }
#if __GNUC__ <= 2
            wchar_t ws[] = { wc, 0 };
            return pushf( "%ls", ws );
#else
            return pushf( "%lc", wc );
#endif
        }

        /**
         * @brief 追加宽字符串，忽略'\0'
         *
         * @param [in] ws   : const wchar_t*
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(const wchar_t * ws){
            if ( ws != NULL ){
                pushf( "%ls", ws );
            }else{
                _truncated = false;
            }
            return *this;
        }

        /**
         * @brief 追加int型整数
         *
         * @param [in] i   : int
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(int i){
            return pushf( "%d", i );
        }

        /**
         * @brief 追加unsigned int型整数
         *
         * @param [in] i   : unsigned int
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(unsigned int i){
            return pushf( "%u", i );
        }

        /**
         * @brief 追加long int型整数
         *
         * @param [in] i   : long int
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(long int i){
            return pushf( "%ld", i );
        }

        /**
         * @brief 追加unsigned long int型整数
         *
         * @param [in] i   : unsigned long int
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(unsigned long int i){
            return pushf( "%lu", i );
        }

        /**
         * @brief 追加字符串
         *
         * @param [in] cstr   : const char*
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(const char* cstr ){
            if ( cstr != NULL ){
                push( cstr, strlen(cstr) );
            }else{
                _truncated = false;
            }
            return *this;
        }

        /**
         * @brief 追加字符串子串
         *
         * 调用者必须保证strlen(cstr) >= sub_str_len，否则行为未定义
         *
         * @param [in] cstr   : const char*
         * @param [in] sub_str_len   : size_t
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(const char* cstr, size_t sub_str_len );
        
        /**
         * @brief 追加long long型整数
         *
         * @param [in] ll   : long long
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(long long ll ){
            return pushf( "%lld", ll );
        }

        /**
         * @brief 追加unsigned long long型整数
         *
         * @param [in] ll   : unsigned long long
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push(unsigned long long ll ){
            return pushf( "%llu", ll );
        }

        /**
         * @brief 追加double型浮点数
         *
         * @param [in] n   : double
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push( double n ){
            return pushf( "%lg", n );
        }

        /**
         * @brief 追加long double型浮点数
         *
         * @param [in] n   : long double
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& push( long double n ){
            return pushf( "%Lg", n );
        }

        /**
         * @brief 追加void *字符串
         *
         * @param [in] p   : void*
         * @return  AutoBuffer&
         * @retval   
         * @see 
        **/
        AutoBuffer& push( void *p ){
            return pushf( "%p", p );
        }
        //attrbute can only be put at function declarations until g++ 3
        /**
         * @brief 以类似printf()语法追加字符串
         *
         * @param [in] format   : const char*
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& pushf( const char *format, ... ) __attribute__ ((format (printf, 2, 3) ));

        /**
         * @brief 以类似vprintf()语法追加字符串
         *
         * @param [in] format   : const char*
         * @param [in] ap   : va_list
         * @return  AutoBuffer& 
         * @retval   
         * @see 
        **/
        AutoBuffer& vpushf( const char *format, va_list ap );


    public:

        static const int DEFAULT_CAPACITY = 64;
        static const char * const TRUE_LITERAL; 
        static const char * const FALSE_LITERAL; 

    private:
        AutoBuffer( const AutoBuffer& other );
        AutoBuffer& operator = ( const AutoBuffer& );

        size_t  _size;
        size_t  _capacity;
        bool    _truncated;
        char *  _str;

        static const size_t TRUE_LITERAL_LEN; 
        static const size_t FALSE_LITERAL_LEN; 
    };

}   //namespace bsl;


#endif  //__BSL_AUTO_BUFFER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
