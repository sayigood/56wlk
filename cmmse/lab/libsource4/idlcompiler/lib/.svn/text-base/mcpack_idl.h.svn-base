/**
 * @file   runtime.h
 * @brief
 *
 */

#ifndef COM_IDLCOMPILER_RUNTIME_H_
#define COM_IDLCOMPILER_RUNTIME_H_

#include <stdint.h>
#include <new>
#include <string>
#include <vector>

#include "ul_string.h"
#include "mc_pack.h" //for mcpack.
#include "compack/compack.h" //for compack.
#include "bsl/exception.h"
#include "bsl/pool.h"
#include "bsl/containers/hash/bsl_readmap.h"
//#include "bsl/var/implement.h" //for var.

namespace idl {

//==============================Exception==============================
//大部分异常都直接来自于bsl::Eception的异常体系.
//mcpack类型不匹配的异常.
class McpackWrongTypeException :
            public bsl::BasicException<McpackWrongTypeException, bsl::Exception> {
};
// deprecated.
typedef McpackWrongTypeException WrongTypeException;

//mcpack包错误的异常.
class McpackException :
            public bsl::BasicException<McpackException, bsl::Exception> {
  public:
    explicit inline McpackException(int err) {
        _msg << mc_pack_perror(err);
    }
    explicit inline McpackException(const void* err) {
        _msg << mc_pack_perror((int)(long)err);
    }
};

//约束检查失败的异常.
class BreakConstraintException :
            public bsl::BasicException<BreakConstraintException, bsl::Exception> {
};
// deprecated.
typedef BreakConstraintException BreakConstrainException;

//==============================Binary==============================
class binary {
    bsl::mempool* _pool;
    void* _data;
    size_t _size;
    bool _detached;
    inline void _free() {
        if(_detached) {
            _pool->free(_data,_size);
        }
    }
    inline void _assign(const binary& value) {
        if(value._detached) {
            _data=_pool->malloc(value._size);
            if(!_data) {
                throw bsl::BadAllocException() << BSL_EARG;
            }
            _size=value._size;
            bsl::xmemcpy(_data,value._data,_size);
            _detached=true;
        } else {
            _data=value._data;
            _size=value._size;
            _detached=false;
        }
    }
  public:
    explicit inline binary(bsl::mempool* pool):
            _pool(pool),
            _data(0),
            _size(0),
            _detached(false) {
    }
    inline binary(bsl::mempool* pool,const char* s):
            _pool(pool),
            _detached(true) {
        _size=strlen(s);
        _data=_pool->malloc(_size);
        if(!_data) {
            throw bsl::BadAllocException() << BSL_EARG;
        }
        bsl::xmemcpy(_data,s,_size);
    }
    inline binary(const binary& value):
            _pool(value._pool),
            _detached(false) {
        _assign(value);
    }
    inline ~binary() {
        clear();
    }
    inline binary& operator = (const binary& value) {
        _free();
        _assign(value);
        return *this;
    }
    //============================================================
    //accessor.
    inline void set(const void* xdata,size_t xsize) {
        _free();
        _data = _pool->malloc(xsize);
        if(!_data) {
            throw bsl::BadAllocException() << BSL_EARG;
        }
        _size=xsize;
        bsl::xmemcpy(_data,xdata,_size);
        _detached = true;
    }
    inline void set_ptr(const void* xdata,size_t xsize) {
        _free();
        _data=const_cast<void*>(xdata);
        _size=xsize;
        _detached=false;
    }
    inline const void* get(unsigned int* xsize=0) const {
        if(xsize) {
            *xsize=(unsigned int)_size;
        }
        return _data;
    }
    inline const void* data() const {
        return _data;
    }
    inline size_t length() const {
        return _size;
    }
    //============================================================
    //other.
    inline void clear() {
        _free();
        _data=0;
        _size=0;
        _detached=false;
    }
    inline void detach() {
        // 必须做一个判断
        if(!_detached && _data!=0) {
            void* xdata=_pool->malloc(_size);
            if(!xdata) {
                throw bsl::BadAllocException() << BSL_EARG;
            }
            bsl::xmemcpy(xdata,_data,_size);
            _data=xdata;
            _detached = true;
        }
    }
};

//==============================String==============================
class string {
    bsl::mempool* _pool;
    size_t _size; //not included '\0'
    char* _data;
    bool _detached;
    static const char zero;
    inline void _free() {
        if(_detached) {
            _pool->free(_data,_size+1);
        }
    }
    inline void _assign(const string& value) {
        if(value._detached) {
            char* data=static_cast<char*>(_pool->malloc(value._size+1));
            if(!data) {
                throw bsl::BadAllocException() << BSL_EARG;
            }
            _size=value._size;
            _data=data;
            bsl::xmemcpy(_data,value._data,_size+1);
            _detached=true;
        } else {
            _data=value._data;
            _size=value._size;
            _detached=false;
        }
    }
  public:
    explicit inline string(bsl::mempool* pool):
            _pool(pool),
            _size(0),
            _data(const_cast<char*>(&zero)),
            _detached(false){
    }
    inline string(bsl::mempool* pool,const char* s):
            _pool(pool),
            _detached(true){
        _size=strlen(s);
        _data=static_cast<char*>(_pool->malloc(_size+1));
        if(!_data) {
            throw bsl::BadAllocException() << BSL_EARG;
        }
        bsl::xmemcpy(_data,s,_size+1);
    }
    inline string(const string& value):
            _pool(value._pool),
            _detached(false) {
        _assign(value);
    }
    inline ~string() {
        clear();
    }
    inline string& operator= (const string& value) {
        _free();
        _assign(value);
        return *this;
    }
    //============================================================
    //accessor.
    inline const char* get(unsigned int* size=0) const {
        if(size) {
            *size=static_cast<unsigned int>(_size);
        }
        return _data;
    }
    inline const char* c_str() const {
        return _data;
    }
    inline size_t length() const {
        return _size;
    }
    inline void set(const char* s,ssize_t size=-1) {
        _free();
        if(size==-1){
            _size=strlen(s);
        }else{
            _size=static_cast<size_t>(size);
        }
        _data=static_cast<char*>(_pool->malloc(_size+1));
        if(!_data) {
            throw bsl::BadAllocException() << BSL_EARG;
        }
        ul_strlcpy(_data,s,_size+1);
        _detached=true;
    }
    inline void set_ptr(const char* s,ssize_t size=-1) {
        _free();
        _data=const_cast<char*>(s);
        if(size==-1){
            _size=strlen(s);
        }else{
            _size=static_cast<size_t>(size);
        }
        _detached=false;
    }
    //============================================================
    //other.
    inline void clear() {
        if(_detached) {
            _pool->free(_data,_size+1);
        }
        _data=const_cast<char*>(&zero);
        _size=0;
        _detached=false;
    }
    inline void detach() {
        // 必须做一个判断
        if(!_detached && _data!=&zero) {
            char* data=static_cast<char*>(_pool->malloc(_size+1));
            if(!data) {
                throw bsl::BadAllocException() << BSL_EARG;
            }
            ul_strlcpy(data,_data,_size+1);
            _data=data;
            _detached=true;
        }
    }
    inline bool operator!=(const char* s) const {
        return strcmp(_data,s)!=0;
    }
    inline bool operator <(const char* s) const {
        return strcmp(_data,s)<0;
    }
    inline bool operator >(const char* s) const {
        return strcmp(_data,s)>0;
    }
    inline bool operator==(const char* s) const {
        return strcmp(_data,s)==0;
    }
};

//TODO(zhangyan04):we need more powerful hash function
//and more efficient strcmp and memcpy.
struct string_hash {
    inline size_t operator() (const char* key) const {
        size_t hashkey;
        for (hashkey = 0; *key != 0; key++) {
            hashkey *= 16777619;
            hashkey ^= *key;
        }
        return hashkey;
    }
};
struct string_equal {
    inline bool operator() (const char* str, const char* str2) const {
        return strcmp(str, str2)==0;
    }
};
struct string_case_hash {
    inline size_t operator() (const char* key) const {
        size_t hashkey;
        for (hashkey = 0; *key != 0; key++) {
            hashkey *= 16777619;
            if(*key >= 'A' && *key <= 'Z') {
                hashkey ^= *key + ('a' - 'A');
            } else {
                hashkey ^= *key;
            }
        }
        return hashkey;
    }
};
struct string_case_equal {
    inline size_t operator() (const char* str, const char* str2) const {
        return strcasecmp(str, str2)==0;
    }
};

//=============================vector==============================
template <typename T>
class vector {
    bsl::mempool* _pool;
    T** _storage;
    size_t _capacity;
    size_t _size;
  public:
    explicit inline vector(bsl::mempool* pool,
                           size_t capacity=32):
            _pool(pool) {
        _init(capacity);
    }
    inline vector(const vector<T> & value):
            _pool(value._pool) {
        _assign(value);
    }
    inline vector<T>& operator=(const vector<T> &value) {
        _free();
        _assign(value);
        return *this;
    }
    ~vector() {
        _free();
    }
    //============================================================
    //accessor.
    inline const T& get(size_t index) const {
        if(index>=_size) {
            throw bsl::OutOfBoundException() << BSL_EARG;
        }
        return *(_storage[index]);
    }
    inline const T& operator [](size_t index) const {
        if(index>=_size) {
            throw bsl::OutOfBoundException() << BSL_EARG;
        }
        return *(_storage[index]);
    }
    inline T& operator[](size_t index) {
        if(index>=_size){
            _resize(index+1);
        }
        return *(_storage[index]);
    }
    inline T* mutable_get(size_t index) {
        if(index>=_size){
            _resize(index+1);
        }
        return _storage[index];
    }
    inline const T& GetWithoutCheck(size_t index) const {
        return *(_storage[index]);
    }
    inline T* MutableGetWithoutCheck(size_t index) {
        return _storage[index];
    }
    inline T* push_back() {
        _resize(_size+1);
        return _storage[_size-1];
    }
    inline void pop_back() {
        _resize(_size-1);
    }
    inline void resize(size_t index) {
        _resize(index);
    }
    inline size_t size() const {
        return _size;
    }
    inline void clear() {
        _clear();
    }
    inline void detach() {
        for(size_t i=0; i<_size; i++) {
            _storage[i]->detach();
        }
    }
    //============================================================
    //iterator.
    //TODO(zhangyan04):
    template<typename Ref,
             typename Ptr>
    class normal_iterator {
        T** _storage;
        size_t _iter;
      public:
        normal_iterator():
                _storage(NULL),
                _iter(0){
        }
        normal_iterator(T** storage,
                        size_t iter):
                _storage(storage),
                _iter(iter) {
        }
        normal_iterator(const normal_iterator<Ref,Ptr>& value):
                _storage(value._storage),
                _iter(value._iter) {
        }        
        Ref operator*() const {
            return *_storage[_iter];
        }
        Ptr operator->() const {
            return _storage[_iter];
        }
        //operator.
        normal_iterator<Ref,Ptr> & operator++() {
            _iter++;
            return *this;
        }
        normal_iterator<Ref,Ptr> operator++(int) {
            normal_iterator<Ref,Ptr> tmp(*this);
            _iter++;
            return tmp;
        }
        normal_iterator & operator=(const normal_iterator<Ref,Ptr>& value){
            _storage=value._storage;
            _iter=value._iter;
            return *this;
        }
        bool operator==(const normal_iterator<Ref,Ptr>& value) const {
            return _iter==value._iter;
        }
        bool operator!=(const normal_iterator<Ref,Ptr>& value) const {
            return _iter!=value._iter;
        }
    };
    typedef normal_iterator<T&,T*> iterator;
    typedef normal_iterator<const T&,const T*> const_iterator;
    iterator begin() {
        return iterator(_storage,0);
    }
    iterator end() {
        return iterator(_storage,_size);
    }
    const_iterator begin() const {
        return const_iterator(_storage,0);
    }
    const_iterator end() const {
        return const_iterator(_storage,_size);
    }
    //============================================================
  private:
    inline void _init(size_t capacity) {
        _capacity=capacity;
        _size=0;
        _storage=(T**)(_pool->malloc(sizeof(T*)*_capacity));
        if(!_storage) {
            throw bsl::BadAllocException() << BSL_EARG;
        }
    }
    inline void _clear() {
        for(size_t i=0; i<_size; i++) {
            T* item=_storage[i];
            item->~T();
            _pool->free(item,sizeof(*item));
        }
        _size=0;
    }
    inline void _free() {
        _clear();
        _pool->free(_storage,sizeof(T*)*_capacity);
        _capacity=0;
    }
    inline void _assign(const vector<T> &value) {
        _capacity=value._capacity;
        _size=value._size;
        _storage=(T**)(_pool->malloc(sizeof(T*)*_capacity));        
        if(!_storage) {
            throw bsl::BadAllocException() << BSL_EARG;
        }
        T** storage=value._storage;
        for(size_t i=0; i<_size; i++) {
            T* item=(T*)(_pool->malloc(sizeof(T)));
            if(!item) {
                throw bsl::BadAllocException() << BSL_EARG;
            }
            //NOTICE(zhangyan04):
            //创建这个对象
            //同时需要copy原来内容..
            new(item)T(_pool);
            *item=*(storage[i]);
            _storage[i]=item;
        }
    }
    inline void _resize(size_t xsize) {
        if(xsize<_size) { //shrink down.
            //delete [xsize,_size)
            //_capacity doesn't change.
            for(size_t i=xsize; i<_size; i++) {
                T* item=_storage[i];
                item->~T();
                _pool->free(item,sizeof(*item));
            }
            _size=xsize;
        } else if(xsize>_size) { //scale up.
            if(xsize>_capacity) {
                //_capacity larger.
                size_t capacity=_capacity;
                while(capacity<xsize) {
                    capacity*=2;
                }
                T** storage=(T**)(_pool->malloc(sizeof(T*)*capacity));
                if(!storage) {
                    throw bsl::BadAllocException() << BSL_EARG;
                }
                memcpy(storage,_storage,sizeof(T*)*_size);
                _pool->free(_storage,sizeof(T*)*_capacity);
                _storage=storage;
                _capacity=capacity;
            }
            //add [_size,xsize)
            for(size_t i=_size; i<xsize; i++) {
                T* item=(T*)(_pool->malloc(sizeof(T)));
                if(!item) {
                    throw bsl::BadAllocException() << BSL_EARG;
                }
                new(item)T(_pool);
                _storage[i]=item;
            }
            _size=xsize;
        } else {
            //pass.
        }
    }
    //============================================================
    //deprecated.
#ifndef IDL_NOT_USE_DEPRECATED_IF
  public:
    inline T* m_get(size_t index) {
        if(index>=_size){
            _resize(index+1);
        }
        return _storage[index];
    }
    inline T& unsafe_get(size_t index) {
        return *(_storage[index]);
    }
#endif
};

//==============================vector_scalar==============================
template <typename T>
class vector_scalar {
    typedef bsl::pool_allocator<T> Allocator;
    typedef std::vector<T, Allocator > storage_type;
    bsl::mempool* _pool;
    storage_type _storage;
  public:
    typedef typename storage_type::iterator iterator;
    typedef typename storage_type::const_iterator const_iterator;
    explicit inline vector_scalar(bsl::mempool* pool):
            _pool(pool),
            _storage(Allocator(pool)) {
    }
    inline vector_scalar(const vector_scalar<T> & value):
            _pool(value._pool),
            _storage(Allocator(value._pool)) {
        _storage=value._storage;
    }
    inline vector_scalar<T>& operator=(const vector_scalar<T>& value) {
        _storage=value._storage;
        return *this;
    }
    //============================================================
    //accessor.
    inline const T get(size_t index) const {
        if(index>=_storage.size()) {
            throw bsl::OutOfBoundException() << BSL_EARG;
        }
        return _storage[index];
    }
    inline const T operator [](size_t index) const {
        if(index>=_storage.size()) {
            throw bsl::OutOfBoundException() << BSL_EARG;
        }
        return _storage[index];
    }
    inline T* mutable_get(size_t index) {
        if(index>=_storage.size()) {
            _storage.resize(index+1);
        }
        return &(_storage[index]);
    }
    inline T& operator[](size_t index) {
        if(index>=_storage.size()) {
            _storage.resize(index+1);
        }
        return _storage[index];
    }
    inline const T GetWithoutCheck(size_t index) const {
        return _storage[index];
    }
    inline T* MutableGetWithoutCheck(size_t index) {
        return &(_storage[index]);
    }
    //============================================================
    //other.
    inline void resize(size_t index) {
        _storage.resize(index);
    }
    inline size_t size() const {
        return _storage.size();
    }
    inline void push_back(T value) {
        _storage.push_back(value);
    }
    inline void push_back(const T* addr,size_t xsize) {
        _storage.insert(_storage.end(),addr,addr+xsize);
    }
    inline void pop_back() {
        _storage.pop_back();
    }
    inline void clear() {
        _storage.clear();
    }
    inline void detach() {
        //pass.
    }
    inline const T* Address() const {
        return &_storage[0];
    }
    //============================================================
    //iterator.
    iterator begin() {
        return _storage.begin();
    }
    const_iterator begin() const {
        return _storage.begin();
    }
    iterator end() {
        return _storage.end();
    }
    const_iterator end() const {
        return _storage.end();
    }
    //============================================================
#ifndef IDL_NOT_USE_DEPRECATED_IF
    //deprecated.
    inline T* m_get(size_t index) {
        if(index>=_storage.size()) {
            _storage.resize(index+1);
        }
        return &(_storage[index]);
    }
    inline T unsafe_get(size_t index) const {
        return _storage[index];
    }
    inline T& unsafe_get(size_t index) {
        return _storage[index];
    }
    inline const T* array() const{
        return &_storage[0];
    }
#endif
};

//============================================================
//专门为bool提供的话,是因为使用默认的stl迭代器.
//返回类型是std::bit_reference.
template <>
class vector<bool> {
    bsl::mempool* _pool;
    bool* _storage;
    size_t _capacity;
    size_t _size;
  public:
    explicit inline vector(bsl::mempool* pool,
                           size_t capacity=32):
            _pool(pool) {
        _init(capacity);
    }
    inline vector(const vector<bool> & value):
            _pool(value._pool) {
        _assign(value);
    }
    inline vector<bool>& operator=(const vector<bool> &value) {
        _free();
        _assign(value);
        return *this;
    }
    ~vector() {
        _free();
    }
    //============================================================
    //accessor.
    inline bool get(size_t index) const {
        if(index>=_size){
            throw bsl::OutOfBoundException() << BSL_EARG;
        }
        return _storage[index];
    }
    inline bool operator [](size_t index) const {
        if(index>=_size){
            throw bsl::OutOfBoundException() << BSL_EARG;
        }
        return _storage[index];
    }
    inline bool* mutable_get(size_t index) {
        if(index>=_size){
            _resize(index+1);
        }
        return &(_storage[index]);
    }
    inline bool& operator[](size_t index) {
        if(index>=_size){
            _resize(index+1);
        }
        return _storage[index];
    }
    inline bool GetWithoutCheck(size_t index) const {
        return _storage[index];
    }
    inline bool* MutableGetWithoutCheck(size_t index) {
        return &(_storage[index]);
    }
    //============================================================
    //other.
    inline void resize(size_t index) {
        _resize(index);
    }
    inline size_t size() const {
        return _size;
    }
    inline void push_back(bool value) {
        _resize(_size+1);
        _storage[_size-1]=value;
    }
    inline void push_back(const bool* addr,size_t xsize) {
        _resize(_size+xsize);
        size_t offset=_size-xsize;
        for(size_t i=0;i<xsize;i++){
            _storage[offset+i]=addr[i];
        }
    }
    inline void pop_back() {
        _size-=1;
    }
    inline void clear() {
        _clear();
    }
    inline void detach() {
        //pass.
    }
    inline const bool* Address() const {
        return &_storage[0];
    }
    //============================================================
    //iterator.
    //boolODO(zhangyan04):
    template<typename Ref,
             typename Ptr>
    class normal_iterator {
        bool* _storage;
        size_t _iter;
      public:
        normal_iterator():
                _storage(NULL),
                _iter(0){
        }
        normal_iterator(bool* storage,
                        size_t iter):
                _storage(storage),
                _iter(iter) {
        }
        normal_iterator(const normal_iterator<Ref,Ptr>& value):
                _storage(value._storage),
                _iter(value._iter) {
        }
        Ref operator*() const {
            return _storage[_iter];
        }
        Ptr operator->() const {
            return &(_storage[_iter]);
        }
        //operator.
        normal_iterator<Ref,Ptr> & operator++() {
            _iter++;
            return *this;
        }
        normal_iterator<Ref,Ptr> operator++(int) {
            normal_iterator<Ref,Ptr> tmp(*this);
            _iter++;
            return tmp;
        }
        normal_iterator & operator=(const normal_iterator<Ref,Ptr>& value){
            _storage=value._storage;
            _iter=value._iter;
            return *this;
        }
        bool operator==(const normal_iterator<Ref,Ptr>& value) const {
            return _iter==value._iter;
        }
        bool operator!=(const normal_iterator<Ref,Ptr>& value) const {
            return _iter!=value._iter;
        }
    };
    typedef normal_iterator<bool&,bool*> iterator;
    typedef normal_iterator<const bool&,const bool*> const_iterator;
    iterator begin() {
        return iterator(_storage,0);
    }
    iterator end() {
        return iterator(_storage,_size);
    }
    const_iterator begin() const {
        return const_iterator(_storage,0);
    }
    const_iterator end() const {
        return const_iterator(_storage,_size);
    }
    //============================================================
  private:
    inline void _init(size_t capacity) {
        _capacity=capacity;
        _size=0;
        _storage=(bool*)(_pool->malloc(sizeof(bool)*_capacity));
        if(!_storage) {
            throw bsl::BadAllocException() << BSL_EARG;
        }
        //因为空洞vector依赖这个行为,WTF
        memset(_storage,0,sizeof(bool)*capacity);
    }
    inline void _clear() {
        _size=0;
    }
    inline void _free() {
        _pool->free(_storage,sizeof(bool)*_capacity);
        _capacity=0;
    }
    inline void _assign(const vector<bool> &value) {
        _capacity=value._capacity;
        _size=value._size;
        _storage=(bool*)(_pool->malloc(sizeof(bool)*_capacity));
        if(!_storage) {
            throw bsl::BadAllocException() << BSL_EARG;
        }
        bsl::xmemcpy(_storage,value._storage,sizeof(bool)*_size);
    }
    inline void _resize(size_t xsize) {
        if(xsize>_capacity){
            size_t capacity=_capacity;
            while(capacity<xsize) {
                capacity*=2;
            }
            bool* storage=(bool*)(_pool->malloc(sizeof(bool)*capacity));
            if(!storage) {
                throw bsl::BadAllocException() << BSL_EARG;
            }
            //需要清空，因为空洞vector依赖这个行为
            //WTF.:(.
            memset(storage,0,sizeof(bool)*capacity);
            bsl::xmemcpy(storage,_storage,sizeof(bool)*_size);
            _pool->free(_storage,sizeof(bool)*_capacity);
            _storage=storage;
            _capacity=capacity;
        }
        _size=xsize;
    }
    //============================================================
#ifndef IDL_NOT_USE_DEPRECATED_IF
  public:
    //deprecated.
    inline bool* m_get(size_t index) {
        if(index>=_size){
            _resize(index+1);
        }
        return &(_storage[index]);
    }
    inline bool unsafe_get(size_t index) const {
        return _storage[index];
    }
    inline bool& unsafe_get(size_t index) {
        return _storage[index];
    }
    inline const bool* array() const{
        return _storage;
    }
#endif   
};


#define GEN_CODE_VECTOR_SCALAR_TYPE(type) template<>                    \
    class vector<type>:public vector_scalar<type>{                      \
      public:                                                           \
      explicit inline vector(bsl::mempool *pool)                        \
              :vector_scalar<type>(pool){                               \
      }                                                                 \
      inline vector(const vector<type> &value)                          \
              :vector_scalar<type>(                                     \
                  static_cast< const vector_scalar<type>& >(value)){    \
      }                                                                 \
      inline vector<type>& operator=(const vector<type>& value){        \
          *(static_cast< vector_scalar<type>* >(this))=                 \
                  static_cast< const vector_scalar<type> & >(value);    \
          return *this;                                                 \
      }                                                                 \
    }
GEN_CODE_VECTOR_SCALAR_TYPE(int8_t);
GEN_CODE_VECTOR_SCALAR_TYPE(uint8_t);
GEN_CODE_VECTOR_SCALAR_TYPE(int16_t);
GEN_CODE_VECTOR_SCALAR_TYPE(uint16_t);
GEN_CODE_VECTOR_SCALAR_TYPE(int32_t);
GEN_CODE_VECTOR_SCALAR_TYPE(uint32_t);
GEN_CODE_VECTOR_SCALAR_TYPE(int64_t);
GEN_CODE_VECTOR_SCALAR_TYPE(uint64_t);
GEN_CODE_VECTOR_SCALAR_TYPE(float);
GEN_CODE_VECTOR_SCALAR_TYPE(double);
class IDLObject;
class McpackBean;
class CompackBean;
GEN_CODE_VECTOR_SCALAR_TYPE(IDLObject*);
GEN_CODE_VECTOR_SCALAR_TYPE(McpackBean*);
GEN_CODE_VECTOR_SCALAR_TYPE(CompackBean*);                                        
#undef GEN_CODE_VECTOR_SCALAR_TYPE
//============================================================


//==============================Map==============================
// TODO(zhangyan04):we need to give map much more funtionalities.
template<typename T>
class map {
    vector<string> _keys;
    vector<T> _values;
  public:
    explicit inline map(bsl::mempool* _pool):
            _keys(_pool),
            _values(_pool) {
    }
    //============================================================
    //accessor.
    inline const vector<string> & keys() const {
        return _keys;
    }
    inline const vector<T> & values() const {
        return _values;
    }
    inline vector<string> *mutable_keys() {
        return &_keys;
    }
    inline vector<T> *mutable_values() {
        return &_values;
    }
    //============================================================
    //other.
    inline size_t size() const {
        if(_keys.size()!=_values.size()) {
            throw bsl::OutOfBoundException() << BSL_EARG;
        }
        return _keys.size();
    }
    inline void resize(size_t xsize) {
        _keys.resize(xsize);
        _values.resize(xsize);
    }
    inline void clear() {
        _keys.clear();
        _values.clear();
    }
    inline void detach() {
        _keys.detach();
        _values.detach();
    }
    //============================================================
#ifndef IDL_NOT_USE_DEPRECATED_IF
    //deprecated.
    inline vector<string> & keys() {
        return _keys;
    }
    inline vector<T> & values() {
        return _values;
    }
#endif
};
//============================================================

//==============================Unknown==============================
//未知字段.
class Unknown {
    struct Item {
        void* data;
        size_t size;
    };
    typedef bsl::pool_allocator<Item> Allocator;
    typedef std::vector<Item,Allocator> storage_type;
    bsl::mempool* _pool;
    storage_type _storage;
    inline void _assign(const Unknown& value) {
        const storage_type& value_storage=value._storage;
        size_t xsize=value_storage.size();
        _storage.resize(xsize);
        for(size_t i=0; i<xsize; i++) {
            Item item=value_storage[i];
            void* data=(_pool->malloc(item.size));
            if(!data) {
                throw bsl::BadAllocException() << BSL_EARG;
            }
            memcpy(data,item.data,item.size);
            Item* place=&(_storage[i]);
            place->data=data;
            place->size=item.size;
        }
    }
  public:
    explicit inline Unknown(bsl::mempool* pool):
            _pool(pool),
            _storage(Allocator(pool)) {
    }
    inline Unknown(const Unknown& value):
            _pool(value._pool),
            _storage(Allocator(value._pool)) {
        _assign(value);
    }
    inline Unknown& operator=(const Unknown& value) {
        clear();
        _assign(value);
        return *this;
    }
    inline ~Unknown() {
        clear();
    }
    //============================================================
    //accessor.
    template<typename T>
    inline void push_back(const T& value) {
        T* buf=(T*)(_pool->malloc(sizeof(T)));
        *buf=value;
        Item item;
        item.data=buf;
        item.size=sizeof(T);
        _storage.push_back(item);
    }
    template<typename T>
    inline const T& get(size_t index) const {
        if(index>=_storage.size()) {
            throw bsl::OutOfBoundException() << BSL_EARG;
        }
        return *(T*)(_storage[index].data);
    }
    template<typename T>
    inline const T& GetWithoutCheck(size_t index) const {
        return *(T*)(_storage[index].data);
    }
    //============================================================
    //other.
    inline size_t size() const {
        return _storage.size();
    }
    inline void clear() {
        size_t xsize=_storage.size();
        for(size_t i=0; i<xsize; i++) {
            _pool->free(_storage[i].data,_storage[i].size);
        }
        _storage.clear();
    }
    inline void detach() {
        //pass.
    }
};
//============================================================


//==============================McpackBean==============================
class McpackBean {
  public:
    explicit inline McpackBean(bsl::mempool* pool):
            _ext(pool){
    }
    //Shallow Copy.
    inline McpackBean(const McpackBean& bean):
            _ext(bean._ext){
    }
    virtual ~McpackBean(){
    }
    //==============================
    //extension.
    vector<McpackBean*> _ext;
    inline void add_extension(McpackBean* object) {
        _ext.push_back(object);
    }
    inline void clear_extension() {
        _ext.clear();
    }
    //==============================
    //other.
    virtual void clear(){
        for(vector<McpackBean*>::iterator it=_ext.begin();
            it!=_ext.end();++it){
            McpackBean* bean=(*it);
            bean->clear();
        }
    }
    virtual void check() const{
        for(vector<McpackBean*>::const_iterator it=_ext.begin();
            it!=_ext.end();++it){
            const McpackBean* bean=(*it);
            bean->check();
        }
    }
    virtual void detach(){
        for(vector<McpackBean*>::iterator it=_ext.begin();
            it!=_ext.end();++it){
            McpackBean* bean=(*it);
            bean->detach();
        }
    }    
    //==============================
    //for mcpack1/mcpack2.
    virtual void load(const mc_pack_t*) {
        //TODO(zhangyan04):raise Exception.
    }
    virtual void LoadWithoutCheck(const mc_pack_t*) {
        //TODO(zhangyan04):raise Exception.
    }
    //for compatiblity.return 0 when it accepts item,else return -1
    virtual int load(const mc_pack_item_t&){
        //TODO(zhangyan04):raise Exception.
        return -1;
    }
    virtual size_t save(mc_pack_t*) const {
        //TODO(zhangyan04):raise Exception.
        return 0;
    }
};
typedef McpackBean Bean;

//==============================CompackBean==============================
class CompackBean {
  public:
    explicit inline CompackBean(bsl::mempool* pool):
            _ext(pool){
    }
    //Shallow Copy.
    inline CompackBean(const CompackBean& bean):
            _ext(bean._ext){
    }
    virtual ~CompackBean(){
    }
    //==============================
    //extension.
    vector<CompackBean*> _ext;
    inline void add_extension(CompackBean* object) {
        _ext.push_back(object);
    }
    inline void clear_extension() {
        _ext.clear();
    }
    //==============================
    //other.
    virtual void clear(){
        for(vector<CompackBean*>::iterator it=_ext.begin();
            it!=_ext.end();++it){
            CompackBean* bean=(*it);
            bean->clear();
        }
    }
    virtual void check() const{
        for(vector<CompackBean*>::const_iterator it=_ext.begin();
            it!=_ext.end();++it){
            const CompackBean* bean=(*it);
            bean->check();
        }
    }
    virtual void detach(){
        for(vector<CompackBean*>::iterator it=_ext.begin();
            it!=_ext.end();++it){
            CompackBean* bean=(*it);
            bean->detach();
        }
    }
    //==============================
    //for compack.
    virtual void load(const void*,size_t) {
        //TODO(zhangyan04):raise Exception.
    }
    virtual void LoadWithoutCheck(const void*,size_t) {
        //TODO(zhangyan04):raise Exception.
    }
    virtual size_t save(void*,size_t) const {
        //TODO(zhangyan04):raise Exception.
        return 0;
    }
#define GEN_CODE_COMPACK_BEAN_LOAD_INTERFACE(BUFFER)                    \
    virtual void load(const BUFFER &) {                                 \
    }                                                                   \
    virtual void load(::compack::Reader< BUFFER > &) {                  \
    }                                                                   \
    virtual void LoadWithoutCheck(const BUFFER &) {                     \
    }                                                                   \
    virtual void LoadWithoutCheck(::compack::Reader< BUFFER > &) {      \
    }
#define GEN_CODE_COMPACK_BEAN_SAVE_INTERFACE(BUFFER)                    \
    virtual size_t save(BUFFER *) const{                                \
        return 0;                                                       \
    }                                                                   \
    virtual void save(::compack::ObjectWriter< BUFFER > *) const{       \
    }
    GEN_CODE_COMPACK_BEAN_SAVE_INTERFACE(::compack::Buffer)
    GEN_CODE_COMPACK_BEAN_SAVE_INTERFACE(::compack::AutoBuffer)
    GEN_CODE_COMPACK_BEAN_SAVE_INTERFACE(::compack::WritevBuffer< ::compack::Buffer > )
    GEN_CODE_COMPACK_BEAN_SAVE_INTERFACE(::compack::WritevBuffer< ::compack::AutoBuffer > )
};

//==============================IVarBean==============================
class IVarBean{
  public:
    explicit inline IVarBean(bsl::mempool* /*pool*/){
    }
    virtual void clear(){
    }
    virtual void check() const{
    }
    virtual void detach(){
    }
    virtual ~IVarBean(){
    }
};

//==============================IDLObject==============================
class IDLObject{
};
//==============================Constraint Function==============================
#define GEN_CODE_RANGE_IMPLEMENT(type)                                  \
    inline void range(type a,type b,type c){                            \
        if(a<b || a>c){                                                 \
            throw BreakConstraintException() << BSL_EARG << "value " << a; \
        }                                                               \
    }
GEN_CODE_RANGE_IMPLEMENT(int32_t)
GEN_CODE_RANGE_IMPLEMENT(uint32_t)
GEN_CODE_RANGE_IMPLEMENT(int64_t)
GEN_CODE_RANGE_IMPLEMENT(uint64_t)
GEN_CODE_RANGE_IMPLEMENT(float)
GEN_CODE_RANGE_IMPLEMENT(double)
#undef GEN_CODE_RANGE_IMPLEMENT

} // namespace idl

#endif // COM_IDLCOMPILER_RUNTIME_H_
