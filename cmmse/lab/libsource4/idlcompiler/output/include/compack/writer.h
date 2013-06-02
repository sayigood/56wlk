#ifndef _COMPACK_WRITER_H
#define _COMPACK_WRITER_H


#include <cstring>
#include <bsl/exception/bsl_exception.h>
#include <compack/buffer.h>
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
class BasicObjectWriter;

template<typename T>
class ObjectWriter;

template<typename T>
class ObjectIsoArrayWriter;

template<typename T>
class ArrayWriter;

template<typename T, typename ITEM>
class IsoArrayWriter;

template<typename T>
class Writer {
public:
    /**
     * @brief
     * 获得该pack的打包大小
     * @return  unsigned int
     * @retval
     * @see
     * @note
     **/
    unsigned int size() const {
        if(Writer<T>::_parent == 0) {
            return Writer<T>::_offset;
        } else {
            return Writer<T>::_offset - Writer<T>::_parent->_offset;
        }
    }
    static unsigned putBinary(T* buffer, const void* value, unsigned int _size) {
        if(compack_unlikely(buffer == 0 || value == 0)) {
            throw_mcpack_arg("null arg");
        }
        if(_size < 256) {
            protocol::short_vitem& item = buffer->
                                          template write<protocol::short_vitem>(0);
            item.type = protocol::BINARY | protocol::SHORT_ITEM;
            item.name_len = 0;
            item._content_len = _size;
            buffer->memcpy_from(item.content_offset(), value, _size);
            return sizeof(protocol::short_vitem) + _size;
        } else {
            protocol::long_vitem& item = buffer->template write<protocol::long_vitem>(0);
            item.type = protocol::BINARY;
            item.name_len = 0;
            item._content_len = _size;
            buffer->memcpy_from(item.content_offset(), value, _size);
            return sizeof(protocol::short_vitem) + _size;
        }
    }
    static unsigned putNull(T* buffer) {
        if(compack_unlikely(buffer == 0)) {
            throw_mcpack_arg(__PRETTY_FUNCTION__);
        }
        protocol::short_item& item = buffer->
                                     template write<protocol::short_item>(0);
        item.type = protocol::MCPACK_NULL;
        item.name_len = 0;
        buffer->template write<char>(item.content_offset()) = 0;
        return sizeof(char) + sizeof(protocol::short_item);
    }
    static unsigned int putString(T* buffer, const char* value, unsigned int _size) {
        if(compack_unlikely(buffer == 0 || value == 0)) {
            throw_mcpack_arg("null arg");
        }
        if(_size < 256) {
            protocol::short_vitem& item = buffer->
                                          template write<protocol::short_vitem>(0);
            item.type = protocol::STRING | protocol::SHORT_ITEM;
            item.name_len = 0;
            item._content_len = _size;
            buffer->memcpy_from(item.content_offset(), value, _size);
            return sizeof(protocol::short_vitem) + _size;
        } else {
            protocol::long_vitem& item = buffer->
                                         template write<protocol::long_vitem>(0);
            item.type = protocol::STRING;
            item.name_len = 0;
            item._content_len = _size;
            buffer->memcpy_from(item.content_offset(), value, _size);
            return sizeof(protocol::long_vitem) + _size;
        }
    }
    static unsigned int putString(T* buffer, const char* value) {
        if(compack_unlikely(buffer == 0 || value == 0)) {
            throw_mcpack_arg("null arg");
        }
        return putString(buffer, value, strlen(value) + 1);
    }
    template<typename value_type>
    static unsigned int putNum(T* buffer, value_type value) {
        if(compack_unlikely(buffer == 0)) {
            throw_mcpack_arg("null arg");
        }
        protocol::short_item& item = buffer->
                                     template write<protocol::short_item>(0);
        item.type = protocol::Mcpack_type_id_traits<value_type>::TYPE_ID;
        item.name_len = 0;
        buffer->template write<value_type>(item.content_offset()) = value;
        return sizeof(value_type) + sizeof(protocol::short_item);
    }
    static unsigned int putNum(T* buffer, const char** value) {
        if(compack_unlikely(buffer == NULL || value == NULL || *value == NULL)) {
            throw_mcpack_arg("bad arg");
        }
        int sign = 0;
        unsigned long long unsign = 0;
        long long lsign = 0;
        long double dn;
        const char* end;
        errno = 0;
        dn = strtold(*value, (char**)&end);
        if(compack_likely(end != *value && errno == 0)) {
            *value = end;
            if(sign = (int)dn, sign == dn) {
                return putNum<int>(buffer, sign);
            } else if(lsign = (long long)dn, lsign == dn) {
                return putNum<int64_t>(buffer, lsign);
            } else if(unsign = (unsigned long long)dn, unsign == dn) {
                return putNum<uint64_t>(buffer, unsign);
            } else {
                return putNum<double>(buffer, dn);
            }
        } else {
            throw_mcpack_arg("bad string");
        }
    }
protected:
    Writer<T> * _parent;
    T* _buffer;
    unsigned int _offset;
    friend class ArrayWriter<T>;
    friend class ObjectWriter<T>;
    friend class ObjectIsoArrayWriter<T>;
    friend class BasicObjectWriter<T>;
    template<typename BUFFER, typename ITEM> friend class IsoArrayWriter;
    template<typename BUFFER, typename ITEM> friend class NumWriter;
    template<typename BUFFER> friend class BinaryWriter;
    template<typename BUFFER> friend class StringWriter;
    template<typename BUFFER> friend class NullWriter;
    explicit Writer(T* buffer ) :
        _parent(0), _buffer(buffer), _offset(0) {
        if(compack_unlikely(buffer == 0)) {
            throw_mcpack_arg(__PRETTY_FUNCTION__);
        }
    }
    explicit Writer(Writer<T> * parent) :
        _parent(parent), _buffer(parent->_buffer),
        _offset(parent->_offset) {
    }
private:
    Writer(const Writer<T> &) {
    }
};

template<typename T>
class BasicObjectWriter : public Writer<T> {
public:

    /**
     * @brief
     * 关闭该pack
     * @return  void
     * @retval
     * @see
     * @note 之后的任何操作都会出core
     **/
    void close() {
        if(Writer<T>::_buffer == 0)
            return;
        if(Writer<T>::_parent == 0) {
            protocol::long_vitem& item = Writer<T>::_buffer->
                                         template write<protocol::long_vitem>(0);
            item._content_len = Writer<T>::_offset
                                - sizeof(protocol::long_vitem);
            Writer<T>::_buffer->template write<protocol::long_pack>
            (0 + item.content_offset()).item_count = _count;
        } else {
            protocol::long_vitem& item = Writer<T>::_buffer->
                                         template write<protocol::long_vitem>
            (Writer<T>::_parent->_offset);
            item._content_len = Writer<T>::_offset
                                - Writer<T>::_parent->_offset
                                - sizeof(protocol::long_vitem) - item.name_len;
            Writer<T>::_buffer->template write<protocol::long_pack>
            (Writer<T>::_parent->_offset + item.content_offset()).item_count
            = _count;
            Writer<T>::_parent->_offset = Writer<T>::_offset;
        }
        Writer<T>::_buffer = 0;
    }
    ~BasicObjectWriter() {
        try {
            close();
        } catch(...) {
        }
    }
protected:
    explicit BasicObjectWriter(T* buffer): Writer<T>(buffer), _count(0) {
    }
    explicit BasicObjectWriter(Writer<T> * parent):
        Writer<T>(parent), _count(0) {
    }
    inline void increase() {
        _count++;
    }
private:
    friend class ArrayWriter<T>;
    friend class ObjectWriter<T>;
    friend class ObjectIsoArrayWriter<T>;
    unsigned int _count;
};

template<typename T>
class ObjectWriter : public BasicObjectWriter<T> {
public:
    /**
     * @brief
     * 创建mcpack object，mcpack打包的起点
     * @param [in] buffer   : T* 用于存放数据的缓冲区, 可以是
     *      mcpack::buffer/insbuffer/autobuffer/writev_buffer等
     * @see
     * @note
     **/
    explicit ObjectWriter(T* buffer): BasicObjectWriter<T>(buffer) {
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(0);
        item.type = protocol::OBJECT;
        item.name_len = 0;
        Writer<T>::_offset += sizeof(protocol::long_vitem)
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
    }
    /**
     * @brief
     * 以parent为父节点，创建子object
     * @param [in] parent   : ObjectWriter<T>* 父节点
     * @param [in] name   : const char* 子object的名称
     * @see
     * @note
     **/
    ObjectWriter(ObjectWriter<T> * parent, const char* name)
        : BasicObjectWriter<T>(parent) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        int name_len = strlen(name) + 1;
        if(compack_unlikely(name_len > 255)) {
            throw_mcpack_arg("name too long");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_parent->_offset);
        item.type = protocol::OBJECT;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }
    /**
     * @brief
     * 以parent为父节点，创建子object
     * @param [in] parent   : ObjectWriter<T>* 父节点
     * @param [in] name   : const char* const char* 子object的名称
     * @param [in] name_len   : unsigned int 名称的长度，包括结尾0
     * @see
     * @note 函数信任传入的长度，并不做任何检查
     **/
    ObjectWriter(ObjectWriter<T> * parent,
                 const char* name,
                 unsigned int name_len)
        : BasicObjectWriter<T>(parent) {
        if(compack_unlikely(name_len > 255)) {
            throw_mcpack_arg("name too long");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_parent->_offset);
        item.type = protocol::OBJECT;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }
    /**
     * @brief
     * 以parent为父节点，创建子array
     * @param [in/out] parent   : ArrayWriter<T>* 父节点
     * @see
     * @note
     **/
    explicit ObjectWriter(ArrayWriter<T> * parent):
        BasicObjectWriter<T>(parent) {
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_parent->_offset);
        item.type = protocol::OBJECT;
        item.name_len = 0;
        Writer<T>::_offset += sizeof(protocol::long_vitem)
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }
    ObjectWriter& putItem(const ObjectIterator& it) {
        char type = it.getType();
        if(type & protocol::DELETED_ITEM) {
            if(type & protocol::FIXED_ITEM) {
                put_short_item(it);
            } else if(type & protocol::SHORT_ITEM) {
                put_item<protocol::short_vitem>(it);
            } else {
                put_item<protocol::long_vitem>(it);
            }
            BasicObjectWriter<T>::increase();
        }
        return *this;
    }
    /**
     * @brief
     * 放入整数, bool也可看作特殊的整数
     * @param [in] name   : char const* 字段名
     * @param [in] value   : value_type 值
     * @return  template<typename value_type>   void
     * @retval
     * @see
     * @note
     **/
    template<typename value_type>
    ObjectWriter& putNum(char const* name, value_type value) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        if(compack_unlikely(strlen(name) > 254)) {
            throw_mcpack_arg("name too long");
        }
        putNum<value_type>(name, strlen(name) + 1, value);
        return *this;
    }
    /**
     * @brief
     * 放入整数, bool也可看作特殊的整数
     * @param [in] name   : char const* 字段名
     * @param [in] name_len   : unsigned char 值
     * @param [in] value   : value_type 值
     * @return  template<typename value_type>   void
     * @retval
     * @see
     * @note
     **/
    template<typename value_type>
    ObjectWriter& putNum(char const* name, unsigned int name_len,
                         value_type value) {
        if(compack_unlikely(name == NULL || name_len > 255)) {
            throw_mcpack_arg("bad name");
        }
        protocol::short_item& item = Writer<T>::_buffer->
                                     template write<protocol::short_item>(Writer<T>::_offset);
        item.type = protocol::Mcpack_type_id_traits<value_type>::TYPE_ID;
        item.name_len = name_len;
        int len = item.content_offset();
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_buffer->template write<value_type>
        (Writer<T>::_offset + len) = value;
        Writer<T>::_offset += sizeof(value_type)
                              + sizeof(protocol::short_item)
                              + name_len;
        BasicObjectWriter<T>::increase();
        return *this;
    }
    const char* putNum(char const* name, unsigned int name_len,
                       const char* value) {
        if(compack_unlikely(name == NULL || name_len > 255 || value == NULL)) {
            throw_mcpack_arg("bad name");
        }
        int sign = 0;
        unsigned long long unsign = 0;
        long long lsign = 0;
        long double dn;
        const char* end;
        errno = 0;
        dn = strtold(value, (char**)&end);
        if(compack_likely(end != value && errno == 0)) {
            if(sign = (int)dn, sign == dn) {
                putNum<int>(name, name_len, sign);
            } else if(lsign = (long long)dn, lsign == dn) {
                putNum<int64_t>(name, name_len, lsign);
            } else if(unsign = (unsigned long long)dn, unsign == dn) {
                putNum<uint64_t>(name, name_len, unsign);
            } else {
                putNum<double>(name, name_len, dn);
            }
            return end;
        } else {
            throw_mcpack_arg("bad string");
        }
    }
    const char* putNum(char const* name, const char* value) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        return putNum(name, strlen(name) + 1, value);
    }
    /**
     * @brief
     * 放入null
     * @param [in] name   : char const*
     * @return  void
     * @retval
     * @see
     * @note
     **/
    ObjectWriter& putNull(char const* name) {
        if(compack_unlikely(name == 0)) {
            throw_mcpack_arg("null arg");
        }
        putNull(name, strlen(name) + 1);
        return *this;
    }
    /**
     * @brief
     * 放入null
     * @param [in] name   : char const*
     * @param [in] name_len   : unsigned char
     * @return  void
     * @retval
     * @see
     * @note
     **/
    ObjectWriter& putNull(char const* name, unsigned int name_len) {
        if(compack_unlikely(name == NULL || name_len > 255)) {
            throw_mcpack_arg("bad name");
        }
        protocol::short_item& item = Writer<T>::_buffer->
                                     template write<protocol::short_item>(Writer<T>::_offset);
        item.type = protocol::MCPACK_NULL;
        item.name_len = name_len;
        int len = item.content_offset();
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_buffer->template write<char>
        (Writer<T>::_offset + len) = 0;
        Writer<T>::_offset += sizeof(char) + sizeof(protocol::short_item) + name_len;
        BasicObjectWriter<T>::increase();
        return *this;
    }

    /**
     * @brief
     * 放入string
     * @param [in] name   : char const* 字段名
     * @param [in] name_len   : unsigned char 字段名长度
     * @param [in] string   : const char* 值
     * @param [in] string_len   : unsigned int 字符串长度，包括结尾0
     * @return  void
     * @retval
     * @see
     * @note 函数信任传入的长度，并不做任何检查
     **/
    ObjectWriter& putString(char const* name,
                            unsigned int name_len,
                            const char* str,
                            unsigned int str_len) {
        if(compack_unlikely(name == NULL || name_len > 255)) {
            throw_mcpack_arg("bad name");
        }
        if(compack_unlikely(str == NULL)) {
            throw_mcpack_arg("bad data");
        }
        putVitem(name, name_len, protocol::STRING, str, str_len);
        return *this;
    }

    /**
     * @brief
     * 放入string，同putString不同的是，如果指定长度的最后不是以\0结尾，会自动
     * 增加\0作为结束符
     * 传入的长度是被信任的
     *
     * @param [in] name   : char const*
     * @param [in] str    : const char*
     * @param [in] str_len: unsigned int 字符串长度
     * @return  ObjectWriter&
     * @retval
     * @see
     **/
    ObjectWriter& putNString(char const* name,
                             const char* str,
                             unsigned int str_len) {
        if(compack_unlikely(name == NULL || str == NULL)) {
            throw_mcpack_arg("bad name");
        }
        if(compack_unlikely(strlen(name) > 254)) {
            throw_mcpack_arg("name too long");
        }
        putVitem(name, strlen(name) + 1,
                 protocol::STRING | protocol::FIXED_ITEM, str, str_len);
        return *this;
    }
    /**
     * @brief
     * 放入string，同putString不同的是，如果指定长度的最后不是以\0结尾，会自动
     * 增加\0作为结束符
     * 传入的长度是被信任的
     *
     * @param [in] name   : char const*
     * @param [in] name_len: unsigned int 字符串长度
     * @param [in] str    : const char*
     * @param [in] str_len: unsigned int 字符串长度
     * @return  ObjectWriter&
     * @retval
     * @see
     **/
    ObjectWriter& putNString(char const* name,
                             unsigned int name_len,
                             const char* str,
                             unsigned int str_len) {
        if(compack_unlikely(name == NULL || name_len > 255)) {
            throw_mcpack_arg("bad name");
        }
        if(compack_unlikely(str == NULL)) {
            throw_mcpack_arg("bad data");
        }
        if (name_len == 0) {
            putVitem("", 1, protocol::STRING | protocol::FIXED_ITEM, str, str_len);
        } else {
            putVitem(name, name_len,
                     protocol::STRING | protocol::FIXED_ITEM, str, str_len);
        }
        return *this;
    }
    /**
     * @brief
     * 放入string
     * @param [in] name   : char const* 字段名
     * @param [in] string   : const char* 值
     * @return  void
     * @retval
     * @see
     * @note 推荐使用带长度的版本
     **/
    ObjectWriter& putString(char const* name, const char* str) {
        if(compack_unlikely(name == NULL || str == NULL)) {
            throw_mcpack_arg("bad name");
        }
        if(compack_unlikely(strlen(name) > 254)) {
            throw_mcpack_arg("name too long");
        }
        putVitem(name, strlen(name) + 1,
                 protocol::STRING, str, strlen(str) + 1);
        return *this;
    }

    /**
     * @brief
     * 放入二进制数据
     * @param [in] name   : char const* 字段名
     * @param [in] name_len   : unsigned char 字段名长度
     * @param [in] value   : void* 值
     * @param [in] size   : unsigned int 值长度
     * @return  void
     * @retval
     * @see
     * @note 函数信任传入的长度，并不做任何检查
     **/
    ObjectWriter& putBinary(char const* name,
                            unsigned int name_len,
                            const void* value,
                            unsigned int item_size) {
        if(compack_unlikely(name == NULL || name_len > 255 || item_size != 0 && value == 0)) {
            throw_mcpack_arg("bad name");
        }
        putVitem(name, name_len, protocol::BINARY, value, item_size);
        return *this;
    }

    /**
     * @brief
     * 放入二进制数据
     * @param [in] name   : char const* 字段名
     * @param [in] value   : void* 值
     * @param [in] size   : unsigned int 值长度
     * @return  void
     * @retval
     * @see
     * @note
     **/
    ObjectWriter& putBinary(char const* name, const void* value,
                            unsigned int item_size) {
        if(compack_unlikely(name == NULL || value == NULL)) {
            throw_mcpack_arg("bad name");
        }
        if(compack_unlikely(strlen(name) > 254)) {
            throw_mcpack_arg("name too long");
        }
        putVitem(name, strlen(name) + 1, protocol::BINARY, value, item_size);
        return *this;
    }
private:
    friend class ArrayWriter<T>;
    template<typename BUFFER, typename ITEM> friend class IsoArrayWriter;
    void putVitem(char const* name,
                  unsigned char name_len,
                  unsigned char type,
                  const void* value,
                  unsigned int item_size) {
        bool need_terminator = false;
        const char terminator = '\0';
        //如果为STRING,那么需要在最后面补0.
        if((type==(protocol::SHORT_ITEM & protocol::STRING)) ||
                ((type & protocol::FIXED_ITEM) && (type & protocol::STRING))) {
            if(item_size==0 || reinterpret_cast<const char*>(value)[item_size-1]!='\0') {
                //增加item size的长度
                //同时设置terminator..
                item_size++;
                need_terminator=true;
            }
        }
        if(item_size < 256) {
            protocol::short_vitem& item = Writer<T>::_buffer->
                                          template write<protocol::short_vitem>(Writer<T>::_offset);
            if (type & protocol::FIXED_ITEM && type & protocol::STRING) {
                //如果是long vitem的话,那么设置成为short item..
                item.type = protocol::STRING | protocol::SHORT_ITEM;
            } else {
                item.type = type | protocol::SHORT_ITEM;
            }
            //为了减容后面的逻辑
            if(need_terminator) {
                item_size--;
            }
            item.name_len = name_len;
            item._content_len = item_size;
            if (need_terminator) {
                item._content_len += 1;
            }
            int len = item.content_offset();
            Writer<T>::_buffer->memcpy_from
            (Writer<T>::_offset + sizeof(item), name, name_len);
            Writer<T>::_buffer->memcpy_from
            (Writer<T>::_offset + len, value, item_size);
            Writer<T>::_offset += sizeof(protocol::short_vitem) +
                                  name_len + item_size;
            if (need_terminator) {
                Writer<T>::_buffer->memcpy_from
                (Writer<T>::_offset, &terminator, 1);
                Writer<T>::_offset += 1;
            }
        } else {
            protocol::long_vitem& item = Writer<T>::_buffer->
                                         template write<protocol::long_vitem>(Writer<T>::_offset);
            if (type & protocol::FIXED_ITEM && type & protocol::STRING) {
                item.type = protocol::STRING;
            } else {
                item.type = type;
            }
            //为了和后面逻辑兼容
            if(need_terminator) {
                item_size--;
            }
            item.name_len = name_len;
            item._content_len = item_size;
            if (need_terminator) {
                item._content_len += 1;
            }
            int len = item.content_offset();
            Writer<T>::_buffer->memcpy_from
            (Writer<T>::_offset + sizeof(item), name, name_len);
            Writer<T>::_buffer->memcpy_from(Writer<T>::_offset
                                            + len, value, item_size);
            Writer<T>::_offset += sizeof(protocol::long_vitem)
                                  + name_len + item_size;
            if (need_terminator) {
                Writer<T>::_buffer->memcpy_from
                (Writer<T>::_offset, &terminator, 1);
                Writer<T>::_offset += 1;
            }
        }
        BasicObjectWriter<T>::increase();
    }
    template<typename ITEM>
    void put_item(const ObjectIterator& it) {
        ITEM& item = Writer<T>::_buffer->template write<ITEM>
        (Writer<T>::_offset);
        item.type = it.getType();
        item.name_len = it.getNameLength() + 1;
        item._content_len = it.length();
        int len = item.content_offset();
        int len1 = item.size();
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item),
         it.getName(), it.getNameLength() + 1);
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + len,
         it.value(), it.length());
        Writer<T>::_offset += len1;
    }
    void put_short_item(const ObjectIterator& it) {
        protocol::short_item& item = Writer<T>::_buffer->
                                     template write<protocol::short_item>(Writer<T>::_offset);
        item.type = it.getType();
        item.name_len = it.getNameLength() + 1;
        int len = item.content_offset();
        int len1 = item.size();
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), it.getName(),
         it.getNameLength() + 1);
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + len,
         it.value(), it.length());
        Writer<T>::_offset += len1;
    }
};
template<typename T>
class ObjectIsoArrayWriter : public BasicObjectWriter<T> {
public:
    /**
     * @brief
     * 创建mcpack object，mcpack打包的起点
     * @param [in] buffer   : T* 用于存放数据的缓冲区, 可以是
     *      mcpack::buffer/insbuffer/autobuffer/writev_buffer等
     * @see
     * @note
     **/
    explicit ObjectIsoArrayWriter(T* buffer):
        BasicObjectWriter<T>(buffer) {
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(0);
        item.type = protocol::OBJECTISOARRAY;
        item.name_len = 0;
        Writer<T>::_offset += sizeof(protocol::long_vitem)
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
    }
    /**
     * @brief
     * 以parent为父节点，创建子object
     * @param [in] parent   : ObjectIsoArray<T>* 父节点
     * @param [in] name   : const char* 子object的名称
     * @see
     * @note
     **/
    ObjectIsoArrayWriter(ObjectWriter<T> * parent, const char* name)
        : BasicObjectWriter<T>(parent) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        int name_len = strlen(name) + 1;
        if(compack_unlikely(name_len > 255)) {
            throw_mcpack_arg("name too long");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_parent->_offset);
        item.type = protocol::OBJECTISOARRAY;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }
    /**
     * @brief
     * 以parent为父节点，创建子object
     * @param [in] parent   : ObjectIsoArray<T>* 父节点
     * @param [in] name   : const char* const char* 子object的名称
     * @param [in] name_len   : unsigned int 名称的长度，包括结尾0
     * @see
     * @note 函数信任传入的长度，并不做任何检查
     **/
    ObjectIsoArrayWriter(ObjectWriter<T> * parent,
                         const char* name,
                         unsigned int name_len)
        : BasicObjectWriter<T>(parent) {
        if(compack_unlikely(name_len > 255)) {
            throw_mcpack_arg("name too long");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_parent->_offset);
        item.type = protocol::OBJECTISOARRAY;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }
    /**
     * @brief
     * 以parent为父节点，创建子array
     * @param [in/out] parent   : ArrayWriter<T>* 父节点
     * @see
     * @note
     **/
    explicit ObjectIsoArrayWriter(ArrayWriter<T> * parent)
        : BasicObjectWriter<T>(parent) {
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_parent->_offset);
        item.type = protocol::OBJECTISOARRAY;
        item.name_len = 0;
        Writer<T>::_offset += sizeof(protocol::long_vitem)
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }

private:
    friend class ArrayWriter<T>;
    template<typename BUFFER, typename ITEM> friend class IsoArrayWriter;
};

template<typename T>
class ArrayWriter : public BasicObjectWriter<T> {
public:
    /**
     * @brief
     * 创建array
     * @param [in] buffer   : T*
     * @see
     * @note 这种方式建立的mcpack顶层是数组
     * 有可能无法被某些代码正确处理
     **/
    explicit ArrayWriter(T* buffer): BasicObjectWriter<T>(buffer) {
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(0);
        item.type = protocol::ARRAY;
        item.name_len = 0;
        Writer<T>::_offset += sizeof(protocol::long_vitem)
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
    }

    /**
     * @brief
     * 在object中创建array
     * @param [in] parent   : ObjectWriter<T>* 父object
     * @param [in] name   : const char* 字段名
     * @param [in] name_len   : unsigned int 字段名长度
     * @see
     * @note
     **/
    ArrayWriter(ObjectWriter<T> * parent,
                const char* name,
                unsigned int name_len)
        : BasicObjectWriter<T>(parent) {
        if(compack_unlikely(name == NULL || name_len > 255)) {
            throw_mcpack_arg("bad name");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_offset);
        item.type = protocol::ARRAY;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }

    /**
     * @brief
     * 在object中创建array
     * @param [in] parent   : ObjectWriter<T>* 父object
     * @param [in] name   : const char*
     * @see
     * @note
     **/
    ArrayWriter(ObjectWriter<T> * parent,
                const char* name)
        : BasicObjectWriter<T>(parent) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        int name_len = strlen(name) + 1;
        if(compack_unlikely(name_len > 255)) {
            throw_mcpack_arg("name too long");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_offset);
        item.type = protocol::ARRAY;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }
    /**
     * @brief
     * 在同构对象数组中创建array
     * @param [in/out] parent   : ObjectIsoArrayWriter<T>*
     * @param [in/out] name   : const char*
     * @param [in/out] name_len   : unsigned int
     * @see
     * @note
     **/
    ArrayWriter(ObjectIsoArrayWriter<T> * parent,
                const char* name,
                unsigned int name_len)
        : BasicObjectWriter<T>(parent) {
        if(compack_unlikely(name == NULL || name_len > 255)) {
            throw_mcpack_arg("bad name");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_offset);
        item.type = protocol::ARRAY;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }

    /**
     * @brief
     * 在同构对象数组中创建array
     * @param [in] parent   : ObjectIsoArrayWriter<T>* 父object
     * @param [in] name   : const char*
     * @see
     * @note
     **/
    ArrayWriter(ObjectIsoArrayWriter<T> * parent, const char* name)
        : BasicObjectWriter<T>(parent) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        int name_len = strlen(name) + 1;
        if(compack_unlikely(name_len > 255)) {
            throw_mcpack_arg("name too long");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_offset);
        item.type = protocol::ARRAY;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }

    /**
     * @brief
     * 在array中创建array
     * @param [in/out] parent   : ArrayWriter<T>* 父array
     * @see
     * @note
     **/
    explicit ArrayWriter(ArrayWriter<T> * parent): BasicObjectWriter<T>(parent) {
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_parent->_offset);
        item.type = protocol::ARRAY;
        item.name_len = 0;
        Writer<T>::_offset += sizeof(protocol::long_vitem)
                              + sizeof(protocol::long_pack);
        Writer<T>::_buffer->check(Writer<T>::_offset);
        parent->increase();
    }

    /**
     * @brief
     * 放入int
     * @param [in] value   : value_type 值
     * @return  template<typename value_type>   inline void
     * @retval
     * @see
     * @note
     **/
    template<typename value_type>
    inline ArrayWriter& putNum(value_type value) {
        protocol::short_item& item = Writer<T>::_buffer->
                                     template write<protocol::short_item>(Writer<T>::_offset);
        item.type = protocol::Mcpack_type_id_traits<value_type>::TYPE_ID;
        item.name_len = 0;
        Writer<T>::_buffer->template write<value_type>
        (Writer<T>::_offset + item.content_offset()) = value;
        Writer<T>::_offset += sizeof(value_type) + sizeof(protocol::short_item);
        BasicObjectWriter<T>::increase();
        return *this;
    }

    const char* putNum(const char* value) {
        if(value == NULL) {
            throw_mcpack_arg("bad value");
        }
        int sign = 0;
        unsigned long long unsign = 0;
        long long lsign = 0;
        long double dn;
        const char* end;
        errno = 0;
        dn = strtold(value, (char**)&end);
        if(compack_likely(end != value && errno == 0)) {
            if(sign = (int)dn, sign == dn) {
                putNum<int>(sign);
            } else if(lsign = (long long)dn, lsign == dn) {
                putNum<int64_t>(lsign);
            } else if(unsign = (unsigned long long)dn, unsign == dn) {
                putNum<uint64_t>(unsign);
            } else {
                putNum<double>(dn);
            }
            return end;
        } else {
            throw_mcpack_arg("bad string");
        }
    }

    /**
     * @brief
     * 放入null
     * @return  void
     * @retval
     * @see
     * @note
     **/
    inline ArrayWriter& putNull() {
        protocol::short_item& item = Writer<T>::_buffer->template write<protocol::short_item>(Writer<T>::_offset);
        item.type = protocol::MCPACK_NULL;
        item.name_len = 0;
        Writer<T>::_buffer->template write<char>(Writer<T>::_offset + item.content_offset()) = 0;
        Writer<T>::_offset += sizeof(char) + sizeof(protocol::short_item);
        BasicObjectWriter<T>::increase();
        return *this;
    }

    /**
     * @brief
     * 放入string
     * @param [in] string   : const char* 值
     * @return  void
     * @retval
     * @see
     * @note 推荐使有有长度的版本
     **/
    inline ArrayWriter& putString(const char* str) {
        if(compack_unlikely(str == 0)) {
            throw_mcpack_arg("null arg");
        }
        putVitem(protocol::STRING, str, strlen(str) + 1);
        return *this;
    }

    /**
     * @brief
     * 放入string
     * @param [in] string   : const char* 值
     * @param [in] size   : unsigned int 长度
     * @return  void
     * @retval
     * @see
     * @note
     **/
    inline ArrayWriter& putString(const char* str, unsigned int item_size) {
        if(compack_unlikely(str == 0)) {
            throw_mcpack_arg("null arg");
        }
        putVitem(protocol::STRING, str, item_size);
        return *this;
    }

    /**
     * @brief
     *
     * @param [in] str     : const char*
     * @param [in] str_len : unsigned int
     * @return  ArrayWriter&
     * @retval
     * @see
     **/
    inline ArrayWriter& putNString(const char* str, unsigned int str_len) {
        if(compack_unlikely(str == 0)) {
            throw_mcpack_arg("null arg");
        }
        putVitem(protocol::STRING | protocol::FIXED_ITEM, str, str_len);
        return *this;
    }
    /**
     * @brief
     * 放入二进制数据
     * @param [in] value   : void*
     * @param [in] size   : unsigned int
     * @return  void
     * @retval
     * @see
     * @note
     **/
    inline ArrayWriter& putBinary(const void* value, unsigned int item_size) {
        if(compack_unlikely(item_size != 0 && value == 0)) {
            throw_mcpack_arg("null arg");
        }
        putVitem(protocol::BINARY, value, item_size);
        return *this;
    }

private:
    void putVitem(unsigned char type, const void* value, unsigned int item_size) {
        bool need_terminator = false;
        const char terminator = 0;
        //如果为STRING,那么需要在最后面补0.
        if((type==(protocol::SHORT_ITEM & protocol::STRING)) ||
                ((type & protocol::FIXED_ITEM) && (type & protocol::STRING))) {
            if(item_size==0 || reinterpret_cast<const char*>(value)[item_size-1]!='\0') {
                //增加item size的长度
                //同时设置terminator..
                item_size++;
                need_terminator=true;
            }
        }
        if(item_size < 256) {
            protocol::short_vitem& item = Writer<T>::_buffer->
                                          template write<protocol::short_vitem>(Writer<T>::_offset);
            if (type & protocol::FIXED_ITEM && type & protocol::STRING) {
                //如果是long vitem的话,那么设置成为short item..
                item.type = protocol::STRING | protocol::SHORT_ITEM;
            } else {
                item.type = type | protocol::SHORT_ITEM;
            }
            //为了兼容后面的行为..
            if(need_terminator) {
                item_size--;
            }
            item.name_len = 0;
            item._content_len = item_size;
            if (need_terminator) {
                item._content_len += 1;
            }
            Writer<T>::_buffer->memcpy_from
            (Writer<T>::_offset + item.content_offset(), value, item_size);
            Writer<T>::_offset += sizeof(protocol::short_vitem) + item_size;

            if (need_terminator) {
                Writer<T>::_buffer->memcpy_from
                (Writer<T>::_offset, &terminator, 1);
                Writer<T>::_offset += 1;
            }
        } else {
            protocol::long_vitem& item = Writer<T>::_buffer->
                                         template write<protocol::long_vitem>(Writer<T>::_offset);
            if (type & protocol::FIXED_ITEM && type & protocol::STRING) {
                item.type = protocol::STRING;
            } else {
                item.type = type;
            }
            //为了兼容后面行为
            if(need_terminator) {
                item_size--;
            }
            item.name_len = 0;
            item._content_len = item_size;
            if (need_terminator) {
                item._content_len += 1;
            }
            Writer<T>::_buffer->memcpy_from
            (Writer<T>::_offset + item.content_offset(), value, item_size);
            Writer<T>::_offset += sizeof(protocol::long_vitem) + item_size;
            if (need_terminator) {
                Writer<T>::_buffer->memcpy_from
                (Writer<T>::_offset, &terminator, 1);
                Writer<T>::_offset += 1;
            }
        }
        BasicObjectWriter<T>::increase();
    }
    friend class ObjectWriter<T>;
    template<typename BUFFER, typename ITEM> friend class IsoArrayWriter;
};

template<typename T, typename ITEM>
class IsoArrayWriter : public Writer<T> {
public:
    /**
     * @brief
     * 创建同构数组
     * @param [in] parent   : ObjectWriter<T>*
     * @param [in] name   : const char*
     * @param [in] name_len   : unsigned int
     * @see
     * @note
     **/
    IsoArrayWriter(ObjectWriter<T> * parent,
                   const char* name,
                   unsigned int name_len)
        : Writer<T>(parent) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        init(name, name_len);
        parent->increase();
    }

    /**
     * @brief
     * 创建同构数组
     * @param [in/out] parent   : ObjectWriter<T>*
     * @param [in/out] name   : const char*
     * @see
     * @note
     **/
    IsoArrayWriter(ObjectWriter<T> * parent, const char* name): Writer<T>(parent) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        int name_len = strlen(name) + 1;
        init(name, name_len);
        parent->increase();
    }

    /**
     * @brief
     * 创建同构数组
     * @param [in] parent   : ObjectWriter<T>*
     * @param [in] name   : const char*
     * @param [in] name_len   : unsigned int
     * @see
     * @note
     **/
    IsoArrayWriter(ObjectIsoArrayWriter<T> * parent,
                   const char* name,
                   unsigned int name_len)
        : Writer<T>(parent) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        init(name, name_len);
        parent->increase();
    }

    /**
     * @brief
     * 创建同构数组
     * @param [in/out] parent   : ObjectWriter<T>*
     * @param [in/out] name   : const char*
     * @see
     * @note
     **/
    IsoArrayWriter(ObjectIsoArrayWriter<T> * parent, const char* name)
        : Writer<T>(parent) {
        if(compack_unlikely(name == NULL)) {
            throw_mcpack_arg("bad name");
        }
        int name_len = strlen(name) + 1;
        init(name, name_len);
        parent->increase();
    }

    /**
     * @brief
     * 创建同构数组
     * @param [in/out] parent   : ArrayWriter<T>*
     * @see
     * @note
     **/
    explicit IsoArrayWriter(ArrayWriter<T> * parent): Writer<T>(parent) {
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_offset);
        item.type = protocol::ISOARRAY;
        item.name_len = 0;
        Writer<T>::_buffer->template write<protocol::isoarray>
        (Writer<T>::_offset + sizeof(protocol::long_vitem)).type
        = protocol::Mcpack_type_id_traits<ITEM>::TYPE_ID;
        Writer<T>::_offset += sizeof(protocol::long_vitem)
                              + sizeof(protocol::isoarray);
        parent->increase();
    }

    /**
     * @brief
     * 放入元素
     * @param [in/out] value   : ITEM
     * @return  void
     * @retval
     * @see
     * @note
     **/
    IsoArrayWriter& putNum(ITEM value) {
        Writer<T>::_buffer->template write<ITEM>(Writer<T>::_offset) = value;
        Writer<T>::_offset += sizeof(ITEM);
        return *this;
    }

    /**
     * @brief
     * 批量放入元素
     * @param [in/out] value   : ITEM* 数组指针
     * @param [in/out] length   : unsigned int 数组长度
     * @return  void
     * @retval
     * @see
     * @note
     **/
    IsoArrayWriter& putNum(const ITEM* value, unsigned int length) {
        if(length == 0) {
            return *this;
        }
        if(compack_unlikely(value == 0)) {
            throw_mcpack_arg("null arg");
        }
        Writer<T>::_buffer->memcpy_from(Writer<T>::_offset, value, sizeof(*value) * length);
        Writer<T>::_offset += sizeof(ITEM) * length;
        return *this;
    }

    /**
     * @brief
     * 关闭数组
     * @return  void
     * @retval
     * @see
     * @note 关闭后任何操作都会出core
     **/
    void close() {
        if(Writer<T>::_buffer == 0)
            return;
        if(Writer<T>::_parent == 0) {
            protocol::long_vitem& item = Writer<T>::_buffer->
                                         template write<protocol::long_vitem>(0);
            item._content_len = Writer<T>::_offset
                                - sizeof(protocol::long_vitem);
        } else {
            protocol::long_vitem& item = Writer<T>::_buffer->
                                         template write<protocol::long_vitem>(Writer<T>::_parent->_offset);
            item._content_len = Writer<T>::_offset
                                - Writer<T>::_parent->_offset
                                - sizeof(protocol::long_vitem) - item.name_len;
            Writer<T>::_parent->_offset = Writer<T>::_offset;
        }
        Writer<T>::_buffer = 0;
    }
    ~IsoArrayWriter() {
        try {
            close();
        } catch(...) {
        }
    }
private:
    inline void init(const char* name, unsigned int name_len) {
        if(compack_unlikely(name_len > 255)) {
            throw_mcpack_arg("name too long");
        }
        protocol::long_vitem& item = Writer<T>::_buffer->
                                     template write<protocol::long_vitem>(Writer<T>::_offset);
        item.type = protocol::ISOARRAY;
        item.name_len = name_len;
        Writer<T>::_buffer->memcpy_from
        (Writer<T>::_offset + sizeof(item), name, name_len);
        Writer<T>::_buffer->template write<protocol::isoarray>
        (Writer<T>::_offset + sizeof(protocol::long_vitem) + name_len).type
        = protocol::Mcpack_type_id_traits<ITEM>::TYPE_ID;
        Writer<T>::_offset += sizeof(protocol::long_vitem) + name_len
                              + sizeof(protocol::isoarray);
    }
};
}


#endif //_WRITER_H
