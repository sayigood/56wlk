/**
 * @file iterator.h
 * @brief
 *
 **/

#ifndef  _COMPACK_ITERATOR_H_
#define  _COMPACK_ITERATOR_H_

#include <cstdlib>
#include <cerrno>
#include <bsl/check_cast.h>
#include <compack/protocol.h>
#include <compack/exception.h>


#undef compack_likely
#undef compack_unlikely
#if 0
#define compack_unlikely(x) x
#define compack_likely(x) x
#else
#define compack_unlikely(x) __builtin_expect((x),0)
#define compack_likely(x) __builtin_expect((x),1)
#endif

namespace compack {

template<typename T>
class Reader;

class ObjectIterator;

class Iterator {
public:
    Iterator() {
    }
    explicit Iterator(const ObjectIterator& it);
    /**
     * @brief
     * 返会字段值所在的内存
     * @return  const void*
     * @retval
     * @see
     * @note
    **/
    inline const void* value() const {
        return _value;
    }
    /**
     * @brief
     * 返回字段类型
     * @return  unsigned char
     * @retval
     * @see
     * @note
    **/
    inline unsigned char getType() const {
        return _type;
    }
    /**
     * @brief
     * 是否是数
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    inline bool isNumber() const {
        switch(_type) {
            case protocol::INT_8:
            case protocol::INT_16:
            case protocol::INT_32:
            case protocol::INT_64:
            case protocol::UINT_8:
            case protocol::UINT_16:
            case protocol::UINT_32:
            case protocol::UINT_64:
            case protocol::FLOAT:
            case protocol::DOUBLE:
                return true;
        }
        return false;
    }
    /**
     * @brief
     * 是否是字符串
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    inline bool isString() const {
        switch(_type) {
            case protocol::STRING | protocol::SHORT_ITEM:
            case protocol::STRING:
                return true;
        }
        return false;
    }
    /**
     * @brief
     * 是否是二进制数据
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    inline bool isBinary() const {
        switch(_type) {
            case protocol::BINARY | protocol::SHORT_ITEM:
            case protocol::BINARY:
                return true;
        }
        return false;
    }
    /**
     * @brief
     * 是否是object
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    inline bool isObject() const {
        if(_type == protocol::OBJECT) {
            return true;
        }
        return false;
    }
    /**
     * @brief
     * 是否是同构对象数组
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    inline bool isObjectIsoArray() const {
        if(_type == protocol::OBJECTISOARRAY) {
            return true;
        }
        return false;
    }
    /**
     * @brief
     * 是否是array
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    inline bool isArray() const {
        if(_type == protocol::ARRAY || _type == protocol::ISOARRAY) {
            return true;
        }
        return false;
    }
    template<typename T>
    inline bool isIsoArray() const {
        if(compack_unlikely(_value==0x0)) {
            throw_mcpack_data("bad data");
        }

        if(_type == protocol::ISOARRAY && _value_length >= 1 && *(const char*)_value == protocol::Mcpack_type_id_traits<T>::TYPE_ID) {
            return true;
        }
        return false;
    }
    template<typename T>
    inline const T* getAsIsoArray() const {
        if(compack_unlikely(_value==0x0)) {
            throw_mcpack_data("bad data");
        }

        if(compack_unlikely(_type != protocol::ISOARRAY || _value_length == 0 || *(const char*)_value != protocol::Mcpack_type_id_traits<T>::TYPE_ID)) {
            throw_mcpack_type("not a isoarray");
        }
        return reinterpret_cast<const T*>((const char*)_value + 1);
    }
    inline unsigned int getIsoArrayLength() const {
        if(compack_unlikely(_value==0x0)) {
            throw_mcpack_data("bad data");
        }

        if(compack_unlikely(_type != protocol::ISOARRAY || _value_length == 0)) {
            throw_mcpack_type("not a isoarray");
        }
        return (_value_length - 1) / (*(const char*)_value & protocol::FIXED_ITEM);
    }
    /**
     * @brief
     * 是否为null
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    inline bool isNull() const {
        if(_type == protocol::MCPACK_NULL) {
            return true;
        }
        return false;
    }
    /**
     * @brief
     * 是否是boolean值
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    inline bool isBool() const {
        if(_type == protocol::BOOL) {
            return true;
        }
        return false;
    }
    /**
     * @brief
     * 值的长度(字节)
     * @return  unsigned int
     * @retval
     * @see
     * @note
    **/
    inline unsigned int length() const {
        return _value_length;
    }
    /**
     * @brief
     * 按字符串获取
     * @return  const char*
     * @retval  类型不匹配时抛异常
     * @see
     * @note
    **/
    inline const char* getAsString() const {
        if(compack_unlikely(_value==0x0)) {
            throw_mcpack_data("bad data");
        }

        if(_type == protocol::STRING || _type == (protocol::STRING | protocol::SHORT_ITEM)) {
            if(compack_unlikely(reinterpret_cast<const char*>(_value)[_value_length - 1] != 0)) {
                throw_mcpack_data("bad data");
            }
            return reinterpret_cast<const char*>(_value);
        } else {
            throw_mcpack_type("not a string");
            //__builtin_unreachable ();
            return 0;  //this should never reach
        }
    }
    inline const void* getAsBinary() const {
        if(compack_unlikely(_value==0x0)) {
            throw_mcpack_data("bad data");
        }

        if(compack_unlikely(_type == protocol::BINARY || _type == (protocol::BINARY | protocol::SHORT_ITEM))) {
            return reinterpret_cast<const void*>(_value);
        } else {
            throw_mcpack_type("not a binary");
            //__builtin_unreachable ();
            return 0;  //this should never reach
        }
    }
    /**
     * @brief
     * 按bool获取
     * @return  bool
     * @retval  类型不匹配时抛异常
     * @see
     * @note
    **/
    inline bool getAsBool() const {
        if(compack_unlikely(_value==0x0)) {
            throw_mcpack_data("bad data");
        }

        if(compack_unlikely(_type == protocol::BOOL)) {
            return *reinterpret_cast<const bool*>(_value) == true;
        } else {
            throw_mcpack_type("not a number");
        }
        //__builtin_unreachable ();
        return false;  //this should never reach
    }
    template <typename T>
    inline T getAsNum() const {
        if(compack_unlikely(_value==0x0)) {
            throw_mcpack_data("bad data");
        }

        if(_type == protocol::Mcpack_type_id_traits<T>::TYPE_ID) {
            return *(T*)_value;
        }
        switch(_type) {
            case protocol::INT_8:
                return bsl::check_cast<T, protocol::mc_int8_t>(
                           *reinterpret_cast<const protocol::mc_int8_t*>(_value));
            case protocol::INT_16:
                return bsl::check_cast<T, protocol::mc_int16_t>(
                           *reinterpret_cast<const protocol::mc_int16_t*>(_value));
            case protocol::INT_32:
                return bsl::check_cast<T, protocol::mc_int32_t>(
                           *reinterpret_cast<const protocol::mc_int32_t*>(_value));
            case protocol::INT_64:
                return bsl::check_cast<T, protocol::mc_int64_t>(
                           *reinterpret_cast<const protocol::mc_int64_t*>(_value));
            case protocol::UINT_8:
                return bsl::check_cast<T, protocol::mc_uint8_t>(
                           *reinterpret_cast<const protocol::mc_uint8_t*>(_value));
            case protocol::UINT_16:
                return bsl::check_cast<T, protocol::mc_uint16_t>(
                           *reinterpret_cast<const protocol::mc_uint16_t*>(_value));
            case protocol::UINT_32:
                return bsl::check_cast<T, protocol::mc_uint32_t>(
                           *reinterpret_cast<const protocol::mc_uint32_t*>(_value));
            case protocol::UINT_64:
                return bsl::check_cast<T, protocol::mc_uint64_t>(
                           *reinterpret_cast<const protocol::mc_uint64_t*>(_value));
            case protocol::FLOAT:
                return bsl::check_cast<T, double>(*reinterpret_cast<const float*>(_value));
            case protocol::DOUBLE:
                return bsl::check_cast<T, double>(*reinterpret_cast<const double*>(_value));
            case protocol::STRING:
            case protocol::STRING | protocol::SHORT_ITEM:
                return bsl::check_cast<T, const char*>(
                           reinterpret_cast<const char*>(_value));
        }
        throw_mcpack_type("not a number");
        //__builtin_unreachable ();
        return T(); //this shoud never reach
    }

    /**
     * @brief 返回当前读取的进度.
     * @return unsigned int
     */
    unsigned int offset() const {
        return _offset;
    }

protected:
    unsigned int _value_length;
    unsigned char _type;
    const void* _value;
    unsigned int _offset;
    template<typename BUFFER> friend class Reader;

};

class ObjectIterator : public Iterator {
public:
    /**
     * @brief
     * 返回字段的名字
     * @return  const char*
     * @retval
     * @see
     * @note
    **/
    inline const char* getName() const {
        if(compack_unlikely(_name==0x0)) {
            throw_mcpack_data("bad data");
        }

        if(compack_unlikely(reinterpret_cast<const char*>(_name)[_name_length - 1] != 0)) {
            throw_mcpack_data("bad data");
        }
        return _name;
    }
    /**
     * @brief
     * 返回字段名的长度
     * @return  unsigned int
     * @retval
     * @see
     * @note
    **/
    inline unsigned int getNameLength() const {
        return _name_length - 1;
    }
protected:
    const char* _name;
    unsigned char _name_length;
    template<typename BUFFER> friend class Reader;
};

inline Iterator::Iterator(const ObjectIterator& it) :
    _value_length(it._value_length),
    _type(it._type),
    _value(it._value),
    _offset(it._offset) {
    //对于_value不能够在这个进行守护..
    //因为Reader是友元,能够改写_value字段...
    //Reader里面没有读取_value字段的操操作,只有填充_value操作.
    //所以所有的操作都能够在accessor的方法里面进行守护..
}

}












#endif  //__ITERATOR_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
