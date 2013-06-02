#ifndef _BSL_KV_BTREE_H_
#define _BSL_KV_BTREE_H_

#if defined __x86_64
#define BSL_BTREE_WORKS
//只有在64位下面才能够运行...
#include <functional>
#include "df_xz_btree.h"
namespace bsl
{
//BTREE错误返回值
enum {
    BTREE_UNKNOWN_ERROR=-1,
    BTREE_OK=0,
    BTREE_OVERWRITE=1,
    BTREE_INSERT_SUC=2,
    BTREE_EXIST=3,
    BTREE_NOEXIST=4,
};
//kv btree data unit.
template<typename K,
         typename V,
         typename LT=std::less<K>,
         typename GT=std::greater<K>,
         typename EQ=std::equal_to<K>,
         typename NEQ=std::not_equal_to<K> >
//这里很难使用Archive这个类
//因为BSL里面的Archive根本就没有序列化到内存的接口..
//typename Archive>
//kv btree内部应该包含的内容...
//应该包含V的所有接口..
struct _bsl_kv_btree_du {
    typedef _bsl_kv_btree_du<K,V,LT,GT,EQ,NEQ> Self;
    //这样就能够得到最终的数据内容了...
    typedef V value_type;
    //同时持有k,v.
    K _key;
    V _value;
    uint64_t _id;
    LT _lt;
    GT _gt;
    EQ _eq;
    NEQ _neq;
    //有默认的初始化构造函数.
    _bsl_kv_btree_du():_key(),_value(){        
        init();
    }
    _bsl_kv_btree_du(const K&k,const V&v):_key(k),_value(v) {
        init();
    }
    void init() {
        _id=UNDEF_ID;
    }
    //这个可以固定住..
    static uint64_t get_store_size() {
        return (uint64_t)sizeof(Self);
    }
    //必须使用archive实现...
    //但是archive没有实现copy到内存的接口...:(..
    int store(char *buf,const uint64_t buf_size,
              uint64_t &data_pos)const {
        int err=BTREE_OK;
        if(buf_size > data_pos &&
           (buf_size - data_pos)>= (uint64_t)sizeof(V)) {
            //V *tmp=(V*)(buf+data_pos);
            //*tmp=_value;
            //data_pos+=(uint64_t)sizeof(V);
            Self *tmp=(Self*)(buf+data_pos);
            *tmp=*this;
            data_pos+=(uint64_t)sizeof(Self);
        } else {
            err=BTREE_UNKNOWN_ERROR;
        }
        return err;
    }
    int load(char *buf,const uint64_t buf_size,
             uint64_t &data_pos) {
        int err=BTREE_OK;
        if(buf_size > data_pos &&
           (buf_size - data_pos)>= (uint64_t)sizeof(V)) {
            //V *tmp=(V*)(buf+data_pos);
            //_value=*tmp;
            //data_pos+=(uint64_t)sizeof(V);
            Self *tmp=(Self*)(buf+data_pos);
            *this=*tmp;
            data_pos+=(uint64_t)sizeof(Self);
        } else {
            err=BTREE_UNKNOWN_ERROR;
        }
        return err;
    }
    void set_id(uint64_t id) {
        _id=id;
    }
    uint64_t get_id()const {
        return _id;
    }
    //使用Compare对象实现比较接口..
    bool operator==(const K &k)const {
        return _eq(_key,k);
    }
    bool operator==(const Self &self)const {
        return _eq(_key,self._key);
    }

    bool operator!=(const K&k)const {
        return _neq(_key,k);
    }
    bool operator!=(const Self &self)const {
        return _neq(_key,self._key);
    }

    bool operator>(const K&k)const {
        return _gt(_key,k);
    }
    bool operator>(const Self &self)const {
        return _gt(_key,self._key);
    }

    bool operator<(const K&k)const {
        return _lt(_key,k);
    }
    bool operator<(const Self &self)const {
        return _lt(_key,self._key);
    }
};

//xz_btree_adapter..
//内部的操作都叫做unify_xxxx..
template<typename K,
         typename DU,
         unsigned int ROW_SIZE>
struct _bsl_kv_btree_xz_adapter {
    //这个真正持有一个对象...
    dfs_xz_btree_t<DU,K,ROW_SIZE>_btree;
    bool unify_get_smallest(DU &du,
                            const dfs_rc_indexer_t *prc_indexer=NULL)const {
        return _btree.get_smallest(du,prc_indexer);
    }
    bool unify_get_largest(DU &du,
                           const dfs_rc_indexer_t *prc_indexer=NULL)const {
        return _btree.get_largest(du,prc_indexer);
    }
    bool unify_get_smaller(const DU &p,DU &du,
                           const dfs_rc_indexer_t *prc_indexer=NULL)const {
        return _btree.get_smaller(p,du,NULL,prc_indexer);
    }
    bool unify_get_larger(const DU &p,DU &du,
                          const dfs_rc_indexer_t *prc_indexer=NULL)const {
        return _btree.get_larger(p,du,NULL,prc_indexer);
    }
    int unify_insert(const K &k,const DU &du) {
        return _btree.zy_insert(du);
    }
    int unify_insert_update(const K &k,const DU &du) {
        return _btree.zy_insert_update(du);
    }
    bool unify_search(const K &k,DU *du,
                      const dfs_rc_indexer_t *prc_indexer=NULL)const {
        return _btree.zy_search(k,*du,prc_indexer);
    }
    int unify_get_total_num(uint64_t &key,uint64_t &leaf,uint64_t &mid)const {
        return  _btree.get_total_num(key,leaf,mid);
    }
    bool unify_del(const K &k) {
        return _btree.del(k,NULL);
    }
    int unify_set_checkpoint_state(bool is_cancel) {
        return _btree.set_checkpoint_state(is_cancel,0);
    }
    int unify_snapshot(dfs_rc_indexer_t &rc_indexer) {
        return _btree.snapshot(rc_indexer);
    }
    int unify_store_checkpoint(const dfs_write_proc_t write_proc,
                               void * file_info,
                               const dfs_rc_indexer_t *prc_indexer=NULL) {
        return _btree.store_checkpointing(write_proc,file_info,NULL,prc_indexer);
    }
    int unify_load_checkpoint(const dfs_read_proc_t read_proc,
                              void * file_info) {
        return _btree.load_checkpointing(read_proc,file_info,NULL);
    }
    int unify_clear() {
        return _btree.clear();
    }
    int unify_set_batch_mutate_mode(bool batch_mode) {
        return _btree.set_batch_mutate_mode(batch_mode);
    }
    bool unify_get_batch_mutate_mode()const {
        return _btree.get_batch_mutate_mode();
    }
};

//这个是一个基类..
//提供遍历接口,这样iterator就只需要写一次了..:).
//纯粹就是一个接口...
template<typename K,
         typename DU>
struct _bsl_kv_btree_iterator_base {
    virtual bool get_smallest(DU &du)const=0;
    virtual bool get_largest(DU &du)const=0;
    virtual bool get_smaller(const DU &pdu,DU &du)const=0;
    virtual bool get_larger(const DU &pdu,DU &du)const=0;
    virtual ~_bsl_kv_btree_iterator_base() {
    }
};

//forward declaration.
template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_iterator;

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_const_iterator;

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_reverse_iterator;

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_const_reverse_iterator;

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_search;

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_build;

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_iterator {
    typedef _bsl_kv_btree_iterator_base<K,DU> kv_btree_type;
    typedef _bsl_kv_btree_iterator<K,DU,ROW_SIZE,BTreeAdapter> iterator;
    typedef _bsl_kv_btree_const_iterator<K,DU,ROW_SIZE,BTreeAdapter> const_iterator;
    //注意,这里得到的是DU的value_type..
    typedef typename DU::value_type * pointer;
    typedef typename DU::value_type & reference;
    kv_btree_type *_btree;
    //data.
    DU _du;
    //做一个_end的标记位...
    bool _end;
public:
    _bsl_kv_btree_iterator():_btree(NULL),_end(true) {
        //nothing.
    }
    _bsl_kv_btree_iterator(kv_btree_type *btree,
                           const DU &du,
                           bool end=false):_btree(btree),_du(du),_end(end) {
    }
    _bsl_kv_btree_iterator(const iterator & iter):
        _btree(iter._btree),_du(iter._du),_end(iter._end) {
    }
    _bsl_kv_btree_iterator(const const_iterator & iter):
        _btree(iter._btree),_du(iter._du),_end(iter._end) {
    }
    reference operator *() {
        return _du._value;
    }
    pointer operator->() {
        return &(_du._value);
    }
    bool operator == (const iterator & iter) const {
        //如果都true的话,那么==
        if(_end && iter._end) {
            return true;
        }
        return _du==(iter._du);
    }
    bool operator != (const iterator & iter)const {
        return  !(*this == iter);
    }
    iterator & operator++() {
        DU _tmp;
        bool next=_btree->get_larger(_du,_tmp);
        if(!next) {
            //设置true..
            //不修改任何内容...
            _end=true;
        } else {
            _du=_tmp;
        }
        return *this;
    }
    iterator operator++(int) {
        iterator iter=*this;
        ++*this;
        return iter;
    }
    iterator & operator--() {
        DU _tmp;
        bool prev=_btree->get_smaller(_du,_tmp);
        if(prev) {
            _du=_tmp;
        }
        return *this;
    }
    iterator operator--(int) {
        iterator iter=*this;
        --*this;
        return iter;
    }
};

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_reverse_iterator {
    typedef _bsl_kv_btree_iterator_base<K,DU> kv_btree_type;
    typedef _bsl_kv_btree_reverse_iterator<K,DU,ROW_SIZE,BTreeAdapter> reverse_iterator;
    typedef _bsl_kv_btree_const_reverse_iterator<K,DU,ROW_SIZE,BTreeAdapter> const_reverse_iterator;
    //注意,这里得到的是DU的value_type..
    typedef typename DU::value_type * pointer;
    typedef typename DU::value_type & reference;
    kv_btree_type *_btree;
    //data.
    DU _du;
    //做一个_end的标记位...
    bool _end;
public:
    _bsl_kv_btree_reverse_iterator():_btree(NULL),_end(true) {
        //nothing.
    }
    _bsl_kv_btree_reverse_iterator(kv_btree_type *btree,
                                   const DU &du,
                                   bool end=false):
        _btree(btree),_du(du),_end(end) {
    }
    _bsl_kv_btree_reverse_iterator(const reverse_iterator & iter):
        _btree(iter._btree),_du(iter._du),_end(iter._end) {
    }
    _bsl_kv_btree_reverse_iterator(const const_reverse_iterator & iter):
        _btree(iter._btree),_du(iter._du),_end(iter._end) {
    }
    reference operator *() {
        return _du._value;
    }
    pointer operator->() {
        return &(_du._value);
    }
    bool operator == (const reverse_iterator & iter) const {
        //如果都true的话,那么==
        if(_end && iter._end) {
            return true;
        }
        return _du==(iter._du);
    }
    bool operator != (const reverse_iterator & iter)const {
        return  !(*this == iter);
    }
    reverse_iterator & operator++() {
        DU _tmp;
        bool next=_btree->get_smaller(_du,_tmp);
        if(!next) {
            //设置true..
            //不修改任何内容...
            _end=true;
        } else {
            _du=_tmp;
        }
        return *this;
    }
    reverse_iterator operator++(int) {
        reverse_iterator iter=*this;
        ++*this;
        return iter;
    }
    reverse_iterator & operator--() {
        DU _tmp;
        bool prev=_btree->get_larger(_du,_tmp);
        if(prev) {
            _du=_tmp;
        }
        return *this;
    }
    reverse_iterator operator--(int) {
        reverse_iterator iter=*this;
        --*this;
        return iter;
    }
};

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_const_iterator {
    typedef _bsl_kv_btree_iterator_base<K,DU> kv_btree_type;
    typedef _bsl_kv_btree_iterator<K,DU,ROW_SIZE,BTreeAdapter> iterator;
    typedef _bsl_kv_btree_const_iterator<K,DU,ROW_SIZE,BTreeAdapter> const_iterator;
    typedef typename DU::value_type * pointer;
    typedef typename DU::value_type & reference;
    const kv_btree_type *_btree;
    //data.
    DU _du;
    bool _end;
public:
    _bsl_kv_btree_const_iterator():_btree(NULL),_end(true) {
        //nothing.
    }
    _bsl_kv_btree_const_iterator(const kv_btree_type *btree,
                                 const DU &du,
                                 bool end=false):
        _btree(btree),_du(du),_end(end) {
    }
    _bsl_kv_btree_const_iterator(const iterator & iter):
        _btree(iter._btree),_du(iter._du),_end(iter._end) {
    }
    _bsl_kv_btree_const_iterator(const const_iterator & iter):
        _btree(iter._btree),_du(iter._du),_end(iter._end) {
    }
    reference operator *() {
        return _du._value;
    }
    pointer operator->() {
        return &(_du._value);
    }
    bool operator == (const const_iterator & iter) const {
        if(_end && iter._end) {
            return true;
        }
        return _du==(iter._du);
    }
    bool operator != (const const_iterator & iter)const {
        return !(*this==iter);
    }
    const_iterator & operator++() {
        DU _tmp;
        bool next=_btree->get_larger(_du,_tmp);
        if(!next) {
            //设置true..
            _end=true;
        } else {
            _du=_tmp;
        }
        return *this;
    }
    const_iterator operator++(int) {
        const_iterator iter=*this;
        ++*this;
        return iter;
    }
    const_iterator & operator--() {
        DU _tmp;
        bool prev=_btree->get_smaller(_du,_tmp);
        if(prev) {
            _du=_tmp;
        }
        return *this;
    }
    const_iterator operator--(int) {
        const_iterator iter=*this;
        --*this;
        return iter;
    }
};

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_const_reverse_iterator {
    typedef _bsl_kv_btree_iterator_base<K,DU> kv_btree_type;
    typedef _bsl_kv_btree_reverse_iterator<K,DU,ROW_SIZE,BTreeAdapter> reverse_iterator;
    typedef _bsl_kv_btree_const_reverse_iterator<K,DU,ROW_SIZE,BTreeAdapter> const_reverse_iterator;
    typedef typename DU::value_type * pointer;
    typedef typename DU::value_type & reference;
    const kv_btree_type *_btree;
    //data.
    DU _du;
    bool _end;
public:
    _bsl_kv_btree_const_reverse_iterator():_btree(NULL),_end(true) {
        //nothing.
    }
    _bsl_kv_btree_const_reverse_iterator(const kv_btree_type *btree,
                                         const DU &du,
                                         bool end=false):
        _btree(btree),_du(du),_end(end) {
    }
    _bsl_kv_btree_const_reverse_iterator(const reverse_iterator & iter):
        _btree(iter._btree),_du(iter._du),_end(iter._end) {
    }
    _bsl_kv_btree_const_reverse_iterator(const const_reverse_iterator & iter):
        _btree(iter._btree),_du(iter._du),_end(iter._end) {
    }
    reference operator *() {
        return _du._value;
    }
    pointer operator->() {
        return &(_du._value);
    }
    bool operator == (const const_reverse_iterator & iter) const {
        if(_end && iter._end) {
            return true;
        }
        return _du==(iter._du);
    }
    bool operator != (const const_reverse_iterator & iter)const {
        return !(*this==iter);
    }
    const_reverse_iterator & operator++() {
        DU _tmp;
        bool next=_btree->get_smaller(_du,_tmp);
        if(!next) {
            //设置true..
            _end=true;
        } else {
            _du=_tmp;
        }
        return *this;
    }
    const_reverse_iterator operator++(int) {
        const_reverse_iterator iter=*this;
        ++*this;
        return iter;
    }
    const_reverse_iterator & operator--() {
        DU _tmp;
        bool prev=_btree->get_larger(_du,_tmp);
        if(prev) {
            _du=_tmp;
        }
        return *this;
    }
    const_reverse_iterator operator--(int) {
        const_reverse_iterator iter=*this;
        --*this;
        return iter;
    }
};

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_build:public _bsl_kv_btree_iterator_base<K,DU> {
    typedef _bsl_kv_btree_iterator<K,DU,ROW_SIZE,BTreeAdapter> iterator;
    typedef _bsl_kv_btree_const_iterator<K,DU,ROW_SIZE,BTreeAdapter> const_iterator;
    typedef _bsl_kv_btree_reverse_iterator<K,DU,ROW_SIZE,BTreeAdapter> reverse_iterator;
    typedef _bsl_kv_btree_const_reverse_iterator<K,DU,ROW_SIZE,BTreeAdapter> const_reverse_iterator;
    typedef BTreeAdapter btree_type;
    typedef K key_type;
    typedef typename DU::value_type value_type;
    typedef _bsl_kv_btree_search<K,DU,ROW_SIZE,BTreeAdapter> kv_btree_search_type;
    //持有的数据对象
    btree_type _btree;
    //end of iterator...
    iterator _end_iter;
    const_iterator _cst_end_iter;
    reverse_iterator _rend_iter;
    const_reverse_iterator _cst_rend_iter;
    _bsl_kv_btree_build() {
        _end_iter._end=true;
        _cst_end_iter._end=true;
        _rend_iter._end=true;
        _cst_rend_iter._end=true;
    }
    bool get_smallest(DU &du)const {
        return _btree.unify_get_smallest(du);
    }
    bool get_largest(DU &du)const {
        return _btree.unify_get_largest(du);
    }
    bool get_smaller(const DU &pdu,DU &du)const {
        return _btree.unify_get_smaller(pdu,du);
    }
    bool get_larger(const DU &pdu,DU &du)const {
        return _btree.unify_get_larger(pdu,du);
    }
    //insert (key,value) into btree
    //flag==0 如果值存在的话直接返回,否则就直接插入
    //flag!=0 如果值存在的话那么就会进行替换,否则就直接插入
    //返回值
    //-1表示内部错误....
    //BTREE_OVERWRITE 表示覆盖旧节点成功[flag!=0]
    //BTREE_INSERT_SUC 表示插入新节点成功
    //BTREE_EXIST 表示节点存在[flag=0]
    int set(const key_type &k,const value_type &v,int flag=0) {
        DU du(k,v);
        du._value=v;
        if(flag==0) {
            int ret=_btree.unify_insert(k,du);
            if(ret==0) {
                return BTREE_EXIST;
            } else if(ret==1) {
                return BTREE_INSERT_SUC;
            } else {
                return BTREE_UNKNOWN_ERROR;
            }
        } else {
            int ret=_btree.unify_insert_update(k,du);
            if(ret==0) {
                return BTREE_OVERWRITE;
            } else if(ret==1) {
                return BTREE_INSERT_SUC;
            } else {
                return BTREE_UNKNOWN_ERROR;
            }
        }
    }
    //BTREE_EXIST 项存在
    //BTREE_NOEXIST 项不存在..
    int get(const key_type &k,value_type *val=0) {
        if(_btree.unify_get_batch_mutate_mode()) {
            return BTREE_UNKNOWN_ERROR;
        }
        DU du;
        bool ok=_btree.unify_search(k,&du);
        if(ok) {
            if(val) {
                //做一个copy..
                *val=du._value;
            }
            return BTREE_EXIST;
        } else {
            return BTREE_NOEXIST;
        }
    }
    int get(const key_type &k,value_type *val=0)const {
        if(_btree.unify_get_batch_mutate_mode()) {
            return BTREE_UNKNOWN_ERROR;
        }
        DU du;
        bool ok=_btree.unify_search(k,&du);
        if(ok) {
            if(val) {
                //做一个copy...
                *val=du._value;
            }
            return BTREE_EXIST;
        } else {
            return BTREE_NOEXIST;
        }
    }
    iterator begin() {
        DU du;
        bool ok=get_smallest(du);
        return iterator(this,du,!ok);
    }
    const_iterator begin()const {
        DU du;
        bool ok=get_smallest(du);
        return const_iterator(this,du,!ok);
    }
    reverse_iterator rbegin() {
        DU du;
        bool ok=get_largest(du);
        return reverse_iterator(this,du,!ok);
    }
    const_reverse_iterator rbegin()const {
        DU du;
        bool ok=get_largest(du);
        return const_reverse_iterator(this,du,!ok);
    }
    iterator end() {
        return _end_iter;
    }
    const_iterator end()const {
        return _cst_end_iter;
    }
    reverse_iterator rend() {
        return _rend_iter;
    }
    const_reverse_iterator rend()const {
        return _cst_rend_iter;
    }
    //返回有多少个key.
    size_t size() const {
        uint64_t key=0,leaf=0,mid=0;
        _btree.unify_get_total_num(key,leaf,mid);
        return key;
    }
    //删除某个元素...
    //BTREE_EXIST 如果key存在
    //BTREE_NOEXIST 如果key不存在.
    int erase(const key_type &k) {
        bool ok=_btree.unify_del(k);
        if(ok) {
            return BTREE_EXIST;
        } else {
            return BTREE_NOEXIST;
        }
    }
    //清除,始终返回0.
    int clear() {
        int ret=_btree.unify_clear();
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
    //创建checkpoint
    //其实没有太多用途,因为你已经有了snapshot.
    //0成功,-1失败...
    int make_checkpoint() {
        int ret=_btree.unify_set_checkpoint_state(false);
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
    int end_checkpoint() {
        int ret=_btree.unify_set_checkpoint_state(true);
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
    int snapshot(kv_btree_search_type & search) {
        int ret=_btree.unify_snapshot(search._rc_indexer);
        search._btree=&_btree;
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
    int store(int fd,long long offset) {
        dfs_file_rw_info_t info;
        info.fd=fd;
        info.orig_offset=offset;
        info.reserved=0;//nothing.
        int ret=_btree.unify_store_checkpoint(dfs_write_proc,&info);
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
    int load(int fd,long long offset) {
        dfs_file_rw_info_t info;
        info.fd=fd;
        info.orig_offset=offset;
        int ret=_btree.unify_load_checkpoint(dfs_read_proc,&info);
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
    int make_batch_mutate() {
        int ret=_btree.unify_set_batch_mutate_mode(true);
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
    int end_batch_mutate() {
        int ret=_btree.unify_set_batch_mutate_mode(false);
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
};

template<typename K,
         typename DU,
         unsigned int ROW_SIZE,
         typename BTreeAdapter>
struct _bsl_kv_btree_search:public _bsl_kv_btree_iterator_base<K,DU> {
    typedef _bsl_kv_btree_iterator<K,DU,ROW_SIZE,BTreeAdapter> iterator;
    typedef _bsl_kv_btree_const_iterator<K,DU,ROW_SIZE,BTreeAdapter> const_iterator;
    typedef _bsl_kv_btree_reverse_iterator<K,DU,ROW_SIZE,BTreeAdapter> reverse_iterator;
    typedef _bsl_kv_btree_const_reverse_iterator<K,DU,ROW_SIZE,BTreeAdapter> const_reverse_iterator;
    typedef _bsl_kv_btree_build<K,DU,ROW_SIZE,BTreeAdapter> kv_btree_build_type;
    friend class _bsl_kv_btree_build<K,DU,ROW_SIZE,BTreeAdapter>;
    typedef BTreeAdapter btree_type;
    typedef K key_type;
    typedef typename DU::value_type value_type;
    //持有的数据对象...
    btree_type *_btree;
    dfs_rc_indexer_t _rc_indexer;
    iterator _end_iter;
    const_iterator _cst_end_iter;
    reverse_iterator _rend_iter;
    const_reverse_iterator _cst_rend_iter;
    _bsl_kv_btree_search() {
        _end_iter._end=true;
        _cst_end_iter._end=true;
        _rend_iter._end=true;
        _cst_rend_iter._end=true;
    }
    bool get_smallest(DU &du)const {
        return _btree->unify_get_smallest(du,&_rc_indexer);
    }
    bool get_largest(DU &du)const {
        return _btree->unify_get_largest(du,&_rc_indexer);
    }
    bool get_smaller(const DU &pdu,DU &du)const {
        return _btree->unify_get_smaller(pdu,du,&_rc_indexer);
    }
    bool get_larger(const DU &pdu,DU &du)const {
        return _btree->unify_get_larger(pdu,du,&_rc_indexer);
    }
    void let_it_go() {
        //释放回收节点..
        _rc_indexer.init();
    }
    ~_bsl_kv_btree_search() {
        let_it_go();
    }
    //BTREE_EXIST 项存在
    //BTREE_NOEXIST 项不存在..
    int get(const key_type &k,value_type *val=0) {
        DU du;
        bool ok=_btree->unify_search(k,&du,&_rc_indexer);
        if(ok) {
            if(val) {
                *val=du._value;
            }
            return BTREE_EXIST;
        } else {
            return BTREE_NOEXIST;
        }
    }
    int get(const key_type &k,value_type *val=0)const {
        DU du;
        bool ok=_btree->unify_search(k,&du,&_rc_indexer);
        if(ok) {
            if(val) {
                *val=du._value;
            }
            return BTREE_EXIST;
        } else {
            return BTREE_NOEXIST;
        }
    }
    iterator begin() {
        DU du;
        bool ok=get_smallest(du);
        return iterator(this,du,!ok);
    }
    const_iterator begin()const {
        DU du;
        bool ok=get_smallest(du);
        return const_iterator(this,du,!ok);
    }
    reverse_iterator rbegin() {
        DU du;
        bool ok=get_largest(du);
        return reverse_iterator(this,du,!ok);
    }
    const_reverse_iterator rbegin()const {
        DU du;
        bool ok=get_largest(du);
        return const_reverse_iterator(this,du,!ok);
    }
    iterator end() {
        return _end_iter;
    }
    const_iterator end()const {
        return _cst_end_iter;
    }
    reverse_iterator rend() {
        return _rend_iter;
    }
    const_reverse_iterator rend()const {
        return _cst_rend_iter;
    }
    int store(int fd,unsigned long long offset) {
        dfs_file_rw_info_t info;
        info.fd=fd;
        info.orig_offset=offset;
        int ret=_btree->unify_store_checkpoint(dfs_write_proc,&info,&_rc_indexer);
        if(ret==0) {
            return BTREE_OK;
        } else {
            return BTREE_UNKNOWN_ERROR;
        }
    }
};

template<typename K,typename V,
         unsigned int ROW_SIZE=dfs_bt_const_t::DEF_ROW_SIZE,
         typename LT=std::less<K>,
         typename GT=std::greater<K>,
         typename EQ=std::equal_to<K>,
         typename NEQ=std::not_equal_to<K> >
class bsl_kv_btree_search:
    public _bsl_kv_btree_search< K,
    //data unit..
    _bsl_kv_btree_du<K,V,LT,GT,EQ,NEQ>,
    ROW_SIZE,
    //adapter...
    _bsl_kv_btree_xz_adapter <K,_bsl_kv_btree_du<K,V,LT,GT,EQ,NEQ>,ROW_SIZE > >
{
};
template<typename K,typename V,
         unsigned int ROW_SIZE=dfs_bt_const_t::DEF_ROW_SIZE,
         typename LT=std::less<K>,
         typename GT=std::greater<K>,
         typename EQ=std::equal_to<K>,
         typename NEQ=std::not_equal_to<K> >
class bsl_kv_btree_build:
    public _bsl_kv_btree_build<  K,
    //data unit..
    _bsl_kv_btree_du<K,V,LT,GT,EQ,NEQ>,
    ROW_SIZE ,
    //adapter..
    _bsl_kv_btree_xz_adapter <K,_bsl_kv_btree_du<K,V,LT,GT,EQ,NEQ>,ROW_SIZE > >
{
};
}

#else
#undef BSL_BTREE_WORKS
#endif

#endif
