#ifndef _COMPACK_READER_H
#define _COMPACK_READER_H

#include <compack/buffer.h>
#include <compack/iterator.h>

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
class Reader {
public:
    Reader(): _buffer(0), _count(0), _type(0), _isotype(0) {
    }
    /**
     * @brief
     * 字段总数
     * @return  unsigned int
     * @retval
     * @see
     * @note
    **/
    unsigned int count() const {
        return _count;
    }
    static Iterator getIterator(const T& buffer) {
        Iterator it;
        char type = buffer.template read<unsigned char>(0);
        if(type & protocol::DELETED_ITEM) {
            if(type & protocol::FIXED_ITEM) {
                const protocol::short_item& item = buffer.template read<protocol::short_item>(0);
                it._type = item.type;
                it._offset = 0;
                it._value_length = item.content_len();
                it._value = buffer.ptr(item.content_offset(), item.content_len());
            } else if(type & protocol::SHORT_ITEM) {
                const protocol::short_vitem& item = buffer.template read<protocol::short_vitem>(0);
                it._type = item.type;
                it._offset = 0;
                it._value_length = item.content_len();
                it._value = buffer.ptr(item.content_offset(), item.content_len());
            } else {
                const protocol::long_vitem& item = buffer.template read<protocol::long_vitem>(0);
                it._type = item.type;
                it._offset = 0;
                it._value_length = item.content_len();
                it._value = buffer.ptr(item.content_offset(), item.content_len());
            }
            return it;
        } else {
            throw_mcpack_data("bad data");
        }
        return it;
    }
    /**
     * @brief
     * 从buffer中读取
     * @param [in] buffer   : const T&
     * @see
     * @note
    **/
    Reader(const T& buffer) : _buffer(&buffer), _offset(0), _isotype(0),_size(0) {
        init();
    }
    Reader(const T& buffer, const Iterator& it) : _buffer(&buffer), _offset(it._offset), _isotype(0),_size(0) {
        init();
    }
    /**
     * @brief
     * 打开字节点，可以是Object，Array, IsoArray
     * @param [in] parent   : const Reader<T>&
     * @param [in] it   : const Iterator&
     * @see
     * @note
    **/
    Reader(const Reader<T> &parent, const Iterator& it): _buffer(parent._buffer), _offset(it._offset), _isotype(0),_size(0) {
        init();
    }
    /**
     * @brief
     * 返回类型，可以是Object，Array, IsoArray
     * @return  unsigned char
     * @retval
     * @see
     * @note
    **/
    unsigned char getType() const {
        return _type;
    }

    /**
     * @brief 返回当前对象的size.
     * @return unsigned int
     */
    unsigned int size() const {
        return _size;
    }

    /**
     * @brief
     * 下一个元素
     * @param [in/out] it   : Iterator&
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    bool next(Iterator& it) {
        return next_imp(it);
    }
    /**
     * @brief
     * 下一个元素
     * @param [in/out] it   : ObjectIterator&
     * @return  bool
     * @retval
     * @see
     * @note
    **/
    bool next(ObjectIterator& it) {
        return next_imp(it);
    }
    void reset() {
        _offset = _start;
    }

    bool find(const char* name, ObjectIterator& it) const {
        if(compack_unlikely(name == 0)) {
            throw_mcpack_arg("bad arg");
        }
        if(_type == protocol::OBJECT || _type == protocol::OBJECTISOARRAY) {
            Reader<T> _reader = *this;
            _reader.reset();
            while(_reader.next(it)) {
                if(strcmp(name, it.getName()) == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    ObjectIterator operator [] (const char* name) {
        if(compack_unlikely(name == 0)) {
            throw_mcpack_arg("bad arg");
        }
        if(_type == protocol::OBJECT || _type == protocol::OBJECTISOARRAY) {
            ObjectIterator it;
            Reader<T> _reader = *this;
            _reader.reset();
            while(_reader.next(it)) {
                if(strcmp(name, it.getName()) == 0)
                    return it;
            }
        }
        throw_mcpack_data("bad data");
    }

    Iterator operator [] (unsigned int index) {
        if(compack_likely(_type == protocol::ARRAY || _type == protocol::ISOARRAY)) {
            Iterator it;
            Reader<T> _reader = *this;
            _reader.reset();
            while(_reader.next(it)) {
                if(index == 0)
                    return it;
                else
                    index--;
            }
        }
        throw_mcpack_data("bad data");
    }

    inline Reader reader(const char* name) {
        return Reader<T>(*this, this->operator [](name));
    }
    inline Reader reader(unsigned int it) {
        return Reader<T>(*this, this->operator [](it));
    }

private:
    void init() {
        _type = _buffer->template read<unsigned char>(_offset);
        if(_type == protocol::ARRAY || _type == protocol::OBJECT || _type == protocol::OBJECTISOARRAY) {
            const protocol::long_vitem& item = _buffer->template read<protocol::long_vitem>(_offset);
            _count = _buffer->template read<protocol::long_pack>(_offset + sizeof(protocol::long_vitem) + item.name_len).item_count;;
            _limit = _offset + sizeof(protocol::long_vitem) + item.name_len + item.content_len();
            _offset += sizeof(protocol::long_vitem) + item.name_len + sizeof(protocol::long_pack);

            //对象大小.
            _size=sizeof(protocol::long_vitem) + item.name_len + item.content_len();

        } else if(_type == protocol::ISOARRAY) {
            const protocol::long_vitem& item = _buffer->template read<protocol::long_vitem>(_offset);
            _isotype = _buffer->template read<protocol::isoarray>(_offset + sizeof(protocol::long_vitem) + item.name_len).type;
            if(compack_unlikely((_isotype & protocol::FIXED_ITEM) == 0 || (_isotype & protocol::FIXED_ITEM) == _isotype)) {
                throw_mcpack_data("bad data");
            }
            _limit = _offset + sizeof(protocol::long_vitem) + item.name_len + item.content_len();
            _offset += sizeof(protocol::long_vitem) + item.name_len + sizeof(protocol::isoarray);
            _count = (_limit - _offset)/(_isotype & protocol::FIXED_ITEM);

            //对象大小.
            _size=sizeof(protocol::long_vitem) + item.name_len + item.content_len();

        } else {
            throw_mcpack_data("bad data");
        }
        _start = _offset;
    }
    const T* _buffer;
    unsigned int _start;
    unsigned int _offset;
    unsigned int _limit;
    unsigned int _count;
    unsigned char _type;
    unsigned char _isotype;
    unsigned int _size;

    template<typename ITEM>
    void read_item(ObjectIterator& it) {
        const ITEM& item = _buffer->template read<ITEM>(_offset);
        it._type = item.type;
        it._name_length = item.name_len;
        if(it._name_length > 0) {
            it._name = reinterpret_cast<const char*>(_buffer->ptr(_offset + item.name_offset(), item.name_len));
        } else {
            it._name = 0;
        }
        it._offset = _offset;
        it._value_length = item.content_len();
        it._value = _buffer->ptr(_offset + item.content_offset(), item.content_len());
        _offset += item.size();
    }
    template<typename ITEM>
    void read_item(Iterator& it) {
        const ITEM& item = _buffer->template read<ITEM>(_offset);
        it._type = item.type;
        it._offset = _offset;
        it._value_length = item.content_len();
        it._value = _buffer->ptr(_offset + item.content_offset(), item.content_len());
        _offset += item.size();
    }
    template<typename ITERATOR>
    bool next_imp(ITERATOR& it) {
        if(_offset >= _limit) {
            return false;
        }
        if(_type == protocol::ISOARRAY) {
            it._type = _isotype;
            it._offset = _offset;
            it._value_length = _isotype & protocol::FIXED_ITEM;
            it._value = _buffer->ptr(_offset, _isotype & protocol::FIXED_ITEM);
            _offset += _isotype & protocol::FIXED_ITEM;
        } else {
            unsigned char type;
            do {
                type = _buffer->template read<unsigned char>(_offset);
                if(!(type & protocol::DELETED_ITEM)) {
                    if(type & protocol::FIXED_ITEM) {
                        const protocol::short_item& item = _buffer->template read<protocol::short_item>(_offset);
                        _offset += item.size();
                    } else if(type & protocol::SHORT_ITEM) {
                        const protocol::short_vitem& item = _buffer->template read<protocol::short_vitem>(_offset);
                        _offset += item.size();
                    } else {
                        const protocol::long_vitem& item = _buffer->template read<protocol::long_vitem>(_offset);
                        _offset += item.size();
                    }
                    if(_offset >= _limit) {
                        return false;
                    }
                } else {
                    break;
                }
            } while(true);
            if(type & protocol::FIXED_ITEM) {
                read_item<protocol::short_item>(it);
            } else if(type & protocol::SHORT_ITEM) {
                read_item<protocol::short_vitem>(it);
            } else {
                read_item<protocol::long_vitem>(it);
            }
        }
        return true;
    }

};

}

#endif
