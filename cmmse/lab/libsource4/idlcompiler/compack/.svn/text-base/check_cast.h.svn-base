/**
 * @file check_cast.h
 * @version $Revision: 1.3 $
 * @brief
 *
 **/
#ifndef  __CHECK_CAST_H_
#define  __CHECK_CAST_H_

#include <stdexcept>

namespace compack {
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
inline DestType check_cast( DestType value ) {
    return value;
}
//from c-style string
template<>
inline char check_cast<char,const char*>( const char* s ) {
    if ( !s ) {
        throw std::invalid_argument("null pointer");
    }
    return s[0];
}

template<>
inline unsigned char check_cast<unsigned char,const char*>( const char* s ) {
    if ( !s ) {
        throw std::invalid_argument("null pointer");
    }
    return s[0];
}

template<>
long check_cast<long, const char*> ( const char* s );

template<>
inline short check_cast<short, const char*> ( const char* s ) {
    return check_cast<short>( check_cast<long>( s ) );
}

template<>
inline int check_cast<int, const char*> ( const char* s ) {
    return check_cast<int>( check_cast<long>( s ) );
}


template<>
long long check_cast<long long, const char*>( const char* s );

template<>
unsigned long check_cast<unsigned long, const char*>( const char* s );

template<>
inline unsigned short check_cast<unsigned short, const char*>( const char* s ) {
    return check_cast<unsigned short>( check_cast<unsigned long>( s ) );
}

template<>
inline unsigned int check_cast<unsigned int, const char*>( const char* s ) {
    return check_cast<unsigned int>( check_cast<unsigned long>( s ) );
}


template<>
unsigned long long check_cast<unsigned long long, const char*>( const char* s );

template<>
float check_cast<float, const char*>( const char* s );

template<>
double check_cast<double, const char*>( const char* s );


template<>
long double check_cast<long double, const char*>( const char* s );


template<>
inline char check_cast<char,char*>( char* s ) {
    if ( !s ) {
        throw std::invalid_argument("null pointer");
    }
    return s[0];
}

template<>
inline unsigned char check_cast<unsigned char,char*>( char* s ) {
    if ( !s ) {
        throw std::invalid_argument("null pointer");
    }
    return s[0];
}

template<>
inline long check_cast<long, char*> ( char* s ) {
    return check_cast<long, const char*>(s);
}

template<>
inline short check_cast<short, char*> ( char* s ) {
    return check_cast<short>( check_cast<long>( s ) );
}

template<>
inline int check_cast<int, char*> ( char* s ) {
    return check_cast<int>( check_cast<long>( s ) );
}


template<>
inline long long check_cast<long long, char*>( char* s ) {
    return check_cast<long long, const char*>(s);
}

template<>
inline unsigned long check_cast<unsigned long, char*>( char* s ) {
    return check_cast<unsigned long, const char*>(s);
}

template<>
inline unsigned short check_cast<unsigned short, char*>( char* s ) {
    return check_cast<unsigned short>( check_cast<unsigned long>( s ) );
}

template<>
inline unsigned int check_cast<unsigned int, char*>( char* s ) {
    return check_cast<unsigned int>( check_cast<unsigned long>( s ) );
}


template<>
inline unsigned long long check_cast<unsigned long long, char*>( char* s ) {
    return check_cast<unsigned long long, const char*>(s);
}

template<>
inline float check_cast<float, char*>( char* s ) {
    return check_cast<float, const char*>(s);
}

template<>
inline double check_cast<double, char*>( char* s ) {
    return check_cast<double, const char*>(s);
}

template<>
inline long double check_cast<long double, char*>( char* s ) {
    return check_cast<long double, const char*>(s);
}
template<>
inline short check_cast<short, short>( short value ) {


    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, short>( short value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, short>( short value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, short>( short value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, short>( short value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, short>( short value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, short>( short value ) {

    if ( value < -128 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, short>( short value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, long double>( long double value ) {

    if ( value < -32768.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, long double>( long double value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, long double>( long double value ) {

    if ( value < -2147483648.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, long double>( long double value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, long double>( long double value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, long double>( long double value ) {

    if ( value < -2147483648.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, long double>( long double value ) {

    if ( value < -128.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, long double>( long double value ) {

    if ( value < -9223372036854775808.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, int>( int value ) {

    if ( value < -32768 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, int>( int value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, int>( int value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, int>( int value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, int>( int value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, int>( int value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, int>( int value ) {

    if ( value < -128 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, int>( int value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, double>( double value ) {

    if ( value < -32768.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, double>( double value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, double>( double value ) {

    if ( value < -2147483648.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, double>( double value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, double>( double value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, double>( double value ) {

    if ( value < -2147483648.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, double>( double value ) {

    if ( value < -128.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, double>( double value ) {

    if ( value < -9223372036854775808.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned char>( unsigned char value ) {


    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned char>( unsigned char value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned char>( unsigned char value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned char>( unsigned char value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned char>( unsigned char value ) {


    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned char>( unsigned char value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned char>( unsigned char value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned char>( unsigned char value ) {


    if ( value > 127 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned char>( unsigned char value ) {


    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned char>( unsigned char value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned char>( unsigned char value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned char>( unsigned char value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned char>( unsigned char value ) {


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, float>( float value ) {

    if ( value < -32768.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, float>( float value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, float>( float value ) {

    if ( value < -2147483648.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, float>( float value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, float>( float value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, float>( float value ) {

    if ( value < -2147483648.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, float>( float value ) {

    if ( value < -128.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, float>( float value ) {

    if ( value < -9223372036854775808.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, long>( long value ) {

    if ( value < -32768L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, long>( long value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, long>( long value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, long>( long value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, long>( long value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, long>( long value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, long>( long value ) {

    if ( value < -128L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, long>( long value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, char>( char value ) {


    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, char>( char value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, char>( char value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, char>( char value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, char>( char value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, char>( char value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, char>( char value ) {


    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, char>( char value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned short>( unsigned short value ) {


    if ( value > 32767 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned short>( unsigned short value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned short>( unsigned short value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned short>( unsigned short value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned short>( unsigned short value ) {


    if ( value > 255 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned short>( unsigned short value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned short>( unsigned short value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned short>( unsigned short value ) {


    if ( value > 127 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned short>( unsigned short value ) {


    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned short>( unsigned short value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned short>( unsigned short value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned short>( unsigned short value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned short>( unsigned short value ) {


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned long>( unsigned long value ) {


    if ( value > 32767UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned long>( unsigned long value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned long>( unsigned long value ) {


    if ( value > 2147483647UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned long>( unsigned long value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned long>( unsigned long value ) {


    if ( value > 255UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned long>( unsigned long value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned long>( unsigned long value ) {


    if ( value > 2147483647UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned long>( unsigned long value ) {


    if ( value > 127UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned long>( unsigned long value ) {


    if ( value > 65535UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned long>( unsigned long value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned long>( unsigned long value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned long>( unsigned long value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned long>( unsigned long value ) {


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, long long>( long long value ) {

    if ( value < -32768LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, long long>( long long value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, long long>( long long value ) {

    if ( value < -2147483648LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, long long>( long long value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, long long>( long long value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, long long>( long long value ) {

    if ( value < -2147483648LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, long long>( long long value ) {

    if ( value < -128LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, long long>( long long value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned long long>( unsigned long long value ) {


    if ( value > 32767ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned long long>( unsigned long long value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned long long>( unsigned long long value ) {


    if ( value > 2147483647ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned long long>( unsigned long long value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned long long>( unsigned long long value ) {


    if ( value > 255ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned long long>( unsigned long long value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned long long>( unsigned long long value ) {


    if ( value > 2147483647ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned long long>( unsigned long long value ) {


    if ( value > 127ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned long long>( unsigned long long value ) {


    if ( value > 65535ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned long long>( unsigned long long value ) {


    if ( value > 4294967295ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned long long>( unsigned long long value ) {


    if ( value > 9223372036854775807ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned long long>( unsigned long long value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned long long>( unsigned long long value ) {


    if ( value > 4294967295ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned int>( unsigned int value ) {


    if ( value > 32767U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned int>( unsigned int value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned int>( unsigned int value ) {


    if ( value > 2147483647U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned int>( unsigned int value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned int>( unsigned int value ) {


    if ( value > 255U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned int>( unsigned int value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned int>( unsigned int value ) {


    if ( value > 2147483647U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned int>( unsigned int value ) {


    if ( value > 127U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned int>( unsigned int value ) {


    if ( value > 65535U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned int>( unsigned int value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned int>( unsigned int value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned int>( unsigned int value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned int>( unsigned int value ) {


    return static_cast<unsigned int>(value);
}

#else

template<>
inline short check_cast<short, short>( short value ) {


    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, short>( short value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, short>( short value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, short>( short value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, short>( short value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, short>( short value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, short>( short value ) {

    if ( value < -128 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, short>( short value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, short>( short value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, long double>( long double value ) {

    if ( value < -32768.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, long double>( long double value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, long double>( long double value ) {

    if ( value < -2147483648.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, long double>( long double value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, long double>( long double value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, long double>( long double value ) {

    if ( value < -9223372036854775808.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, long double>( long double value ) {

    if ( value < -128.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, long double>( long double value ) {

    if ( value < -9223372036854775808.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, long double>( long double value ) {

    if ( value < 0.0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, int>( int value ) {

    if ( value < -32768 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, int>( int value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, int>( int value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, int>( int value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, int>( int value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, int>( int value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, int>( int value ) {

    if ( value < -128 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, int>( int value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, int>( int value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, double>( double value ) {

    if ( value < -32768.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, double>( double value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, double>( double value ) {

    if ( value < -2147483648.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, double>( double value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, double>( double value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, double>( double value ) {

    if ( value < -9223372036854775808.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, double>( double value ) {

    if ( value < -128.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, double>( double value ) {

    if ( value < -9223372036854775808.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, double>( double value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned char>( unsigned char value ) {


    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned char>( unsigned char value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned char>( unsigned char value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned char>( unsigned char value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned char>( unsigned char value ) {


    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned char>( unsigned char value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned char>( unsigned char value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned char>( unsigned char value ) {


    if ( value > 127 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned char>( unsigned char value ) {


    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned char>( unsigned char value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned char>( unsigned char value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned char>( unsigned char value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned char>( unsigned char value ) {


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, float>( float value ) {

    if ( value < -32768.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, float>( float value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, float>( float value ) {

    if ( value < -2147483648.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, float>( float value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, float>( float value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, float>( float value ) {

    if ( value < -9223372036854775808.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, float>( float value ) {

    if ( value < -128.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, float>( float value ) {

    if ( value < -9223372036854775808.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 9223372036854775807.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 18446744073709551615.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, float>( float value ) {

    if ( value < 0.0 ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295.0 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, long>( long value ) {

    if ( value < -32768L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, long>( long value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, long>( long value ) {

    if ( value < -2147483648L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, long>( long value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, long>( long value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, long>( long value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, long>( long value ) {

    if ( value < -128L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, long>( long value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, long>( long value ) {

    if ( value < 0L ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295L ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, char>( char value ) {


    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, char>( char value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, char>( char value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, char>( char value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, char>( char value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, char>( char value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, char>( char value ) {


    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, char>( char value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, char>( char value ) {

    if ( value < 0 ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned short>( unsigned short value ) {


    if ( value > 32767 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned short>( unsigned short value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned short>( unsigned short value ) {


    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned short>( unsigned short value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned short>( unsigned short value ) {


    if ( value > 255 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned short>( unsigned short value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned short>( unsigned short value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned short>( unsigned short value ) {


    if ( value > 127 ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned short>( unsigned short value ) {


    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned short>( unsigned short value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned short>( unsigned short value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned short>( unsigned short value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned short>( unsigned short value ) {


    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned long>( unsigned long value ) {


    if ( value > 32767UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned long>( unsigned long value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned long>( unsigned long value ) {


    if ( value > 2147483647UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned long>( unsigned long value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned long>( unsigned long value ) {


    if ( value > 255UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned long>( unsigned long value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned long>( unsigned long value ) {


    if ( value > 9223372036854775807UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned long>( unsigned long value ) {


    if ( value > 127UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned long>( unsigned long value ) {


    if ( value > 65535UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned long>( unsigned long value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned long>( unsigned long value ) {


    if ( value > 9223372036854775807UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned long>( unsigned long value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned long>( unsigned long value ) {


    if ( value > 4294967295UL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, long long>( long long value ) {

    if ( value < -32768LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 32767LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, long long>( long long value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, long long>( long long value ) {

    if ( value < -2147483648LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 2147483647LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, long long>( long long value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 255LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, long long>( long long value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, long long>( long long value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, long long>( long long value ) {

    if ( value < -128LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 127LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 65535LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, long long>( long long value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, long long>( long long value ) {

    if ( value < 0LL ) {
        throw std::underflow_error("out of range");
    }


    if ( value > 4294967295LL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned long long>( unsigned long long value ) {


    if ( value > 32767ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned long long>( unsigned long long value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned long long>( unsigned long long value ) {


    if ( value > 2147483647ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned long long>( unsigned long long value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned long long>( unsigned long long value ) {


    if ( value > 255ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned long long>( unsigned long long value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned long long>( unsigned long long value ) {


    if ( value > 9223372036854775807ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned long long>( unsigned long long value ) {


    if ( value > 127ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned long long>( unsigned long long value ) {


    if ( value > 65535ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned long long>( unsigned long long value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned long long>( unsigned long long value ) {


    if ( value > 9223372036854775807ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned long long>( unsigned long long value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned long long>( unsigned long long value ) {


    if ( value > 4294967295ULL ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned int>(value);
}


template<>
inline short check_cast<short, unsigned int>( unsigned int value ) {


    if ( value > 32767U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<short>(value);
}


template<>
inline long double check_cast<long double, unsigned int>( unsigned int value ) {


    return static_cast<long double>(value);
}


template<>
inline int check_cast<int, unsigned int>( unsigned int value ) {


    if ( value > 2147483647U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<int>(value);
}


template<>
inline double check_cast<double, unsigned int>( unsigned int value ) {


    return static_cast<double>(value);
}


template<>
inline unsigned char check_cast<unsigned char, unsigned int>( unsigned int value ) {


    if ( value > 255U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned char>(value);
}


template<>
inline float check_cast<float, unsigned int>( unsigned int value ) {


    return static_cast<float>(value);
}


template<>
inline long check_cast<long, unsigned int>( unsigned int value ) {


    return static_cast<long>(value);
}


template<>
inline char check_cast<char, unsigned int>( unsigned int value ) {


    if ( value > 127U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<char>(value);
}


template<>
inline unsigned short check_cast<unsigned short, unsigned int>( unsigned int value ) {


    if ( value > 65535U ) {
        throw std::underflow_error("out of range");
    }

    return static_cast<unsigned short>(value);
}


template<>
inline unsigned long check_cast<unsigned long, unsigned int>( unsigned int value ) {


    return static_cast<unsigned long>(value);
}


template<>
inline long long check_cast<long long, unsigned int>( unsigned int value ) {


    return static_cast<long long>(value);
}


template<>
inline unsigned long long check_cast<unsigned long long, unsigned int>( unsigned int value ) {


    return static_cast<unsigned long long>(value);
}


template<>
inline unsigned int check_cast<unsigned int, unsigned int>( unsigned int value ) {


    return static_cast<unsigned int>(value);
}

}
//#include "check_cast_bsl_string.h"

#endif  //__CHECK_CAST_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
