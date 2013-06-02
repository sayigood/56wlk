#ifndef _AF_UTILS_H_20110621
#define _AF_UTILS_H_20110621
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
using namespace std;

namespace af{
    namespace common{

template<int buf_size>
class static_string{
public:
    static_string()
    {
        _inner_buf[0] = '\0';
        _length = 0;
    }

    static_string(const char * p )
    {
        _length = strlen(p);
        if (buf_size <= _length) {
            _length = buf_size - 1;
        }   
        memcpy(_inner_buf, p, _length);
        _inner_buf[_length] = '\0';
    }

    static_string(const static_string & s)
    {
        _length = s.length();
        if (buf_size <= _length) {
            _length = buf_size - 1;
        }   
        memcpy(_inner_buf, s.c_str(), _length);
        _inner_buf[_length] = '\0';
    }

    bool empty() const
    {
        return ( 0 == _length);
    }

    static_string & operator=(const static_string & s)
    {
        _length = s.length();
        if (buf_size <= _length) {
            _length = buf_size - 1;
        }
        memcpy(_inner_buf, s.c_str(), _length);
        _inner_buf[_length] = '\0';
        return *this;
    }

    const char * c_str() const
    {
        return _inner_buf;
    }

    const uint32_t length() const
    {
        return _length;
    }

private:
    char _inner_buf[buf_size];
    uint32_t _length;
};
template<class T>
class static_vector{

public:
    static_vector(){ _cur_size = 0; }
    int resize(int item_num)
    {
        _max_size = item_num;
        _cur_size = _max_size;
        _inner_array = new (std::nothrow)T[item_num];
        if( NULL == _inner_array )
        {
            return -1;
        }

        return 0;

    }

    int reserve(int item_num)
    {
        _max_size = item_num;
        _cur_size =  0 ;
        _inner_array = new(std::nothrow) T[item_num];
        if( NULL == _inner_array )
        {
            return -1;
        }

        return 0;


    }

    int push_back(const T & item )
    {
        if( _cur_size >= _max_size )
        {
            return -1;
        }
        _inner_array[_cur_size] = item;
        ++_cur_size;
        return 0;

    }

    size_t size() const
    {
        return _cur_size;
    }

    int clear()
    {
        _cur_size = 0 ;
        return 0;
    }

    T & operator[](int idx)
    {

        return _inner_array[idx]  ;
    }
     const T & operator[](int idx) const
    {

        return _inner_array[idx]  ;
    }

private:
    T * _inner_array;
    int _cur_size ;
    int _max_size ;
};



}}//namespace

#endif
