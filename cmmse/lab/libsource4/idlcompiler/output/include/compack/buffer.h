#ifndef _COMPACK_BUFFER_H
#define _COMPACK_BUFFER_H

#include <sys/uio.h>
#include <cstring>
#include <stdexcept>
#include <vector>
#include "bsl/check_cast.h"
#include "bsl/utils/bsl_memcpy.h"
#include "compack/exception.h"

#undef compack_likely
#undef compack_unlikely
#define compack_unlikely(x) __builtin_expect((x),0)
#define compack_likely(x) __builtin_expect((x),1)

namespace compack {

class Buffer {
    char* _memory;
    unsigned int _size;
private:
    inline Buffer(const Buffer&) {
    }
public:
    inline Buffer(void* memory, unsigned int __size) {
        _memory = (char*)memory;
        _size = __size;
    }
    inline unsigned int capacity() const {
        return _size;
    }
    inline void memcpy_from(int offset, const void* memory, unsigned int __size) {
        if(compack_unlikely((long long)offset + __size > _size))
            throw_ioexception("Buffer overflow");
        bsl::xmemcpy(_memory + offset, memory, __size);
    }
    inline void memcpy_to(int offset, unsigned int __size, void* memory) const {
        if(compack_unlikely((long long)offset + __size > _size))
            throw_ioexception("Buffer overflow");
        bsl::xmemcpy(memory, _memory + offset, __size);
    }
    template<typename T>
    inline const T& read(unsigned int offset) const {
        if(compack_unlikely((long long)sizeof(T) + offset > _size))
            throw_ioexception("Buffer overflow");
        return *(T*)(_memory + offset);
    }
    template<typename T>
    inline T& write(unsigned int offset) {
        if(compack_unlikely((long long)sizeof(T) + offset > _size))
            throw_ioexception("Buffer overflow");
        return *(T*)(_memory + offset);
    }
    inline const void* ptr(unsigned int offset, unsigned int __size) const {
        if(compack_unlikely((long long)__size + offset > _size))
            throw_ioexception("Buffer overflow");
        return (void*)(_memory + offset);
    }
    inline void* ptr(unsigned int offset, unsigned int __size) {
        if(compack_unlikely((long long)__size + offset > _size))
            throw_ioexception("Buffer overflow");
        return (void*)(_memory + offset);
    }
    inline void check(unsigned int offset) {
        if(compack_unlikely(offset > _size))
            throw_ioexception("Buffer overflow");
    }
};

class AutoBuffer {
    char* _memory;
    unsigned int _capacity;
    void reserve(unsigned long long newsize) {
        if(newsize <= _capacity)
            return;
        else {
            char* newmemory = (char*) malloc(newsize + _capacity);
            bsl::xmemcpy(newmemory, _memory, _capacity);
            free(_memory);
            _memory = newmemory;
            _capacity += newsize;
        }
    }
private:
    AutoBuffer(const AutoBuffer&) {
    }
public:
    inline explicit AutoBuffer(unsigned int __size = 4096) {
        _capacity = __size;
        _memory = (char*)malloc(__size);
    }
    inline unsigned int capacity() const {
        return _capacity;
    }
    inline void memcpy_from(int offset, const void* memory, unsigned int __size) {
        reserve((long long)offset + __size);
        bsl::xmemcpy(_memory + offset, memory, __size);
    }
    inline void memcpy_to(int offset, unsigned int __size, void* memory) const {
        if(compack_unlikely((long long)offset + __size > _capacity))
            throw_ioexception("Buffer overflow");
        bsl::xmemcpy(memory, _memory + offset, __size);
    }
    template<typename T>
    inline const T& read(unsigned int offset) const {
        if(compack_unlikely((long long)sizeof(T) + offset > _capacity))
            throw_ioexception("Buffer overflow");
        return *(T*)(_memory + offset);
    }
    template<typename T>
    inline T& write(unsigned int offset) {
        reserve((long long)offset + sizeof(T));
        return *(T*)(&_memory[offset]);
    }
    inline const void* ptr(unsigned int offset, unsigned int __size) const {
        if(compack_unlikely((long long)__size + offset > _capacity))
            throw_ioexception("Buffer overflow");
        return (void*)(_memory + offset);
    }
    inline void* ptr(unsigned int offset, unsigned int __size) {
        reserve((long long)offset + __size);
        return (void*)(_memory + offset);
    }
    inline void check(unsigned int offset) {
        reserve(offset);
    }
    inline ~AutoBuffer() {
        free(_memory);
    }
    inline void* release() {
        void* p = _memory;
        _memory = 0;
        _capacity = 0;
        return p;
    }

    inline void* buffer() {
        return _memory;
    }
};

template<typename BUFFER>
class WritevBuffer {
    BUFFER* _buffer;
    struct record {
        unsigned int offset;
        unsigned int length;
        const void* Buffer;
    };
    std::vector<record> _iovec;
    WritevBuffer(const WritevBuffer&) {
    }
public:
    inline WritevBuffer(BUFFER* buffer) : _buffer(buffer), _iovec() {
        _iovec.reserve(10);
    }
    inline unsigned int capacity() const {
        return _buffer->capacity();
    }
    inline void memcpy_from(int offset, const void* memory, unsigned int _size) {
        if(_size >= 1024) {
            _buffer->ptr(offset, _size);
            record rec;
            rec.offset = offset;
            rec.length = _size;
            rec.Buffer = memory;
            _iovec.push_back(rec);
        } else {
            _buffer->memcpy_from(offset, memory, _size);
        }
    }
    std::vector<iovec> get_iovec(unsigned int _size) {
        std::vector<iovec> vecs;
        vecs.reserve(_iovec.size() * 2 + 1);
        if(_iovec.size() > 0) {
            iovec vec;
            vec.iov_len = _iovec[0].offset;
            vec.iov_base = const_cast<void*>(_buffer->ptr(0, vec.iov_len));
            vecs.push_back(vec);
            vec.iov_len = _iovec[0].length;
            vec.iov_base = const_cast<void*>(_iovec[0].Buffer);
            vecs.push_back(vec);
            for(unsigned int i = 1; i<_iovec.size(); i++) {
                vec.iov_len = _iovec[i].offset - _iovec[i - 1].offset - _iovec[i - 1].length;
                vec.iov_base = const_cast<void*>(_buffer->ptr(_iovec[i - 1].offset + _iovec[i - 1].length, vec.iov_len));
                vecs.push_back(vec);
                vec.iov_len = _iovec[i].length;
                vec.iov_base = const_cast<void*>(_iovec[i].Buffer);
                vecs.push_back(vec);
            }
            vec.iov_len = _size - _iovec.back().offset - _iovec.back().length;
            vec.iov_base = const_cast<void*>(_buffer->ptr(_size - vec.iov_len, vec.iov_len));
            vecs.push_back(vec);
        } else {
            iovec vec;
            vec.iov_len = _size;
            vec.iov_base = const_cast<void*>(_buffer->ptr(0, _size));
            vecs.push_back(vec);
        }
        return vecs;
    }
    template<typename T>
    inline T& write(unsigned int offset) {
        return _buffer->template write<T>(offset);
    }
    inline void check(unsigned int offset) {
        return _buffer->check(offset);
    }
};

} //namespace compack

#endif // _COMPACK_BUFFER_H
