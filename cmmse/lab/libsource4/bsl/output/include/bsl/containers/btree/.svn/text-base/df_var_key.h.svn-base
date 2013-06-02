////====================================================================
//
// df_str_key.h - Pyramid / DFS / df-lib
//
//
//
// -------------------------------------------------------------------
//
// Description
//
//    declaration and implementation of string-based (dfs_name_t and dts_string_t) key
//    class and template
//
// -------------------------------------------------------------------
//
// Change Log
//
//
////====================================================================

#ifndef __DF_STR_KEY_INCLUDE_H_
#define __DF_STR_KEY_INCLUDE_H_

//调试信息打印输出开关
//#ifndef DF_BT_PRINT_DEBUG_INFO
//#define DF_BT_PRINT_DEBUG_INFO
//#endif

# if defined __x86_64__
# else
#  error "MUST COMPILED ON x86_64"
# endif


//#include "ul_def.h"
#include "df_common.h"
#include "df_2d_ary.h"
#include "df_id_btree.h"

class dfs_key_id_index_t
{
public:
    uint64_t key_id;
    uint64_t key_index;
public:
    dfs_key_id_index_t() {
        init();
    };
    void init(void) {
        key_id = UNDEF_ID;
        key_index = UNDEF_INDEX;
    };
};



//key head in key buf
class dfs_key_head_t : public dfs_id_t
{
private:
    volatile uint16_t _mark;
    uint16_t _key_store_size;
    volatile uint32_t _ref_counter;
public:
    enum cconst_public {
        MAX_KEY_STOREE_SIZE = MAX_U_INT16-16,
    };
private:
    enum cconst_private {
        IN_USE_MARK  = 0x7f00,
        DELETED_MARK = 0x7e00,
    };
    const static uint32_t MAX_REF_COUNTER = MAX_U_INT32-3;
public:
    dfs_key_head_t() : dfs_id_t() {
        _mark = 0;
        _key_store_size = 0;
        _ref_counter = 0;
    };
    dfs_key_head_t(const dfs_key_head_t & src) :
        dfs_id_t((const dfs_id_t &)src),
        _mark(src._mark),
        _key_store_size(src._key_store_size),
        _ref_counter(src._ref_counter) {
        ;
    };
    ~dfs_key_head_t() {
    };
    const dfs_key_head_t & operator=(const dfs_key_head_t & src) {
        if (&src != this) {
            *((dfs_id_t *)this) = (const dfs_id_t &)src ;
            _mark               = src._mark             ;
            _key_store_size     = src._key_store_size   ;
            _ref_counter        = src._ref_counter      ;
        }
        return *this;
    };
    void init() { //初始化对象
        dfs_id_t::init();
        _mark = 0;
        _key_store_size = 0;
        _ref_counter = 0;
    };
public:
    int set_key_store_size(const uint32_t key_store_size) {
        int err = 0;
        if (key_store_size > MAX_KEY_STOREE_SIZE) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_SIZE);
        } else {
            _key_store_size = (uint16_t)key_store_size;
        }
        return err;
    };
    uint32_t get_key_store_size(void) const {
        return _key_store_size;
    };
    void set_in_use_mark(void) {
        atomic_exchange(&_mark, (uint16_t)IN_USE_MARK);
    };
    bool is_in_use_mark_set(void) const {
        return (((uint16_t)IN_USE_MARK) == _mark);
    };
    void set_del_mark(void) {
        atomic_exchange(&_mark, (uint16_t)DELETED_MARK);
    };
    bool is_del_mark_set(void) const {
        return (((uint16_t)DELETED_MARK) == _mark);
    };
    bool is_valid(void) const {
        return (is_in_use_mark_set() || is_del_mark_set());
    };
    //Return: 0 for success, other values for error
    int inc_ref_counter(uint64_t & ref_counter) {
        int err = 0;

        ref_counter = UNDEF_REF_COUNTER;
        if (_ref_counter < MAX_REF_COUNTER) {
            ref_counter = atomic_inc(&_ref_counter);
        } else {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_REF_COUNTER);
        }

        return err;
    };
    //Return: 0 for success, other values for error
    int dec_ref_counter(uint64_t & ref_counter) {
        int err = 0;

        ref_counter = UNDEF_REF_COUNTER;
        if (_ref_counter > 0) {
            ref_counter = atomic_dec(&_ref_counter);
        } else {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_REF_COUNTER);
        }

        return err;
    };
    uint64_t get_ref_counter(void) const {
        return _ref_counter;
    };
};


//
//二维缓冲区dfs_2d_key_buf_t<uint32_t KEY_BUF_SIZE>的单个缓冲区的管理单元
//特别注意：为了二维缓冲区能够扩展，该管理单元并不自动申请空间，析构时也不自动释放空间。
//
template<uint64_t KEY_BUF_SIZE = dfs_bt_const_t::DEF_KEY_BUF_SIZE>
class dfs_key_buf_t : public dfs_2d_base_t
{
private:
    char * _buf;
    volatile uint64_t _data_offset;  //读写同时访问
    volatile uint64_t _key_num;      //stored key number;读写同时访问
    volatile uint64_t _occupied_size; //stored total key size, including the head;读写同时访问
public:
    dfs_key_buf_t(void) {
        _buf = NULL;
        init();
    };
    virtual ~dfs_key_buf_t() {
    };
    const dfs_key_buf_t & operator=(const dfs_key_buf_t & src) {
        if (this != &src) {
            //*((dfs_2d_base_t *)this) = (const dfs_2d_base_t &)src;
            _buf            = src._buf              ;
            _key_num        = src._key_num          ;
            _data_offset    = src._data_offset      ;
            _occupied_size  = src._occupied_size    ;
        }
        return *this;
    };
    dfs_key_buf_t(const dfs_key_buf_t & src) :
        //dfs_2d_base_t((const dfs_2d_base_t &)src),
        _buf(src._buf),
        _data_offset(src._data_offset),
        _key_num(src._key_num),
        _occupied_size(src._occupied_size) {
        ;
    };
    void init(void) {
        //stored total data size, including the head
        atomic_exchange(&_occupied_size, 0);
        atomic_exchange(&_data_offset, 0);
        atomic_exchange(&_key_num, 0);      //stored key number;
    };
public:
    int alloc(void) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (NULL == _buf) {
            if ((_buf = new(std::nothrow) char[KEY_BUF_SIZE]) == NULL) {
                err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_NOMEM);
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, ERRINFO_BT_NOMEM, "_buf");
            }
        }
        init();

        return err;
    };
    void dealloc(void) {
        if (NULL != _buf) {
            delete [] _buf;
            _buf = NULL;
        }
        init();
    };
private:
    int _get_key_head(const uint64_t key_offset, dfs_key_head_t ** ppkey_head) const {
        dfs_key_head_t * pkey_head = NULL;
        uint64_t key_offset_in_buf = key_offset % KEY_BUF_SIZE;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (NULL != ppkey_head) {
            *ppkey_head = NULL;
        }

        if (NULL == _buf) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL _buf");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NULL_BUF_POINTER);
        } else if (key_offset_in_buf < sizeof(dfs_key_head_t) || key_offset_in_buf >= _data_offset) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "wrong key_offset_in_buf");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_POS);
        } else {
            pkey_head = (dfs_key_head_t *)(_buf+(key_offset_in_buf-sizeof(dfs_key_head_t)));

            if ((key_offset_in_buf+pkey_head->get_key_store_size()) > _data_offset) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "(key_offset_in_buf+pkey_head->get_key_store_size()) > _data_offset");
                DF_WRITE_LOG_US(log_level, "key_store_size=0x%x", pkey_head->get_key_store_size());
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_POS);
            } else if (NULL != ppkey_head) {
                *ppkey_head = pkey_head;
            }
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level, "key_offset=0x%lx", key_offset);
            DF_WRITE_LOG_US(log_level,
                            "key_offset_in_buf=0x%lx, sizeof(dfs_key_head_t)=0x%lx, _data_offset=0x%lx",
                            key_offset_in_buf, sizeof(dfs_key_head_t), _data_offset);
        }

        return err;
    };
public:
    bool is_enough_room_for_key(const uint64_t key_store_size) {
        return ((_data_offset+sizeof(dfs_key_head_t)+key_store_size) < KEY_BUF_SIZE);
    };
    //只有写线程调用，因此只有一个实例
    //输出：用来store key的缓冲区，至少key_store_size大小
    //return: 0 for success, other values for error
    int buf_add_key(
        const uint64_t key_store_size,
        char * & key_store_buf,
        uint64_t & key_offset_in_buf) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        key_store_buf = NULL;
        key_offset_in_buf = 0;
        if (NULL == _buf) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_buf is NULL");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NULL_BUF_POINTER);
        } else if ((_data_offset % sizeof(uint64_t)) != 0 || _data_offset > KEY_BUF_SIZE) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "wrong _data_offset");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_BUF_POS);
        } else if ((_data_offset+sizeof(dfs_key_head_t)+key_store_size) > KEY_BUF_SIZE) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "insufficient buffer size for key");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_SIZE);
        } else {
            dfs_key_head_t * pkey_head = (dfs_key_head_t *)(_buf+_data_offset);
            const uint64_t aligned_key_store_size = (key_store_size+sizeof(uint64_t)-1) & ~(sizeof(uint64_t)-1);
            const uint64_t new_data_offset = _data_offset + sizeof(dfs_key_head_t) + aligned_key_store_size;

            pkey_head->init();
            if ((err = pkey_head->set_key_store_size(key_store_size)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "pkey_head->set_key_store_size() returns 0x%x", err);
            } else {
                pkey_head->set_in_use_mark();

                key_store_buf = (char *)&(pkey_head[1]);
                key_offset_in_buf = key_store_buf-_buf;

                //stored total key size, including the head
                atomic_add(&_occupied_size, new_data_offset-_data_offset);
                atomic_exchange(&_data_offset, new_data_offset);
                atomic_inc(&_key_num);      //stored key number;
            }
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "_data_offset=%ld, key_store_size=%ld",
                            _data_offset, key_store_size);
            DF_WRITE_LOG_US(log_level,
                            "KEY_BUF_SIZE=%ld, sizeof(dfs_key_head_t)=%ld",
                            KEY_BUF_SIZE, sizeof(dfs_key_head_t));
        }

        return err;
    };
    //输出：用来store key的缓冲区和存储的key大小
    int buf_get_key_store_buf(
        const uint64_t key_offset,
        const char * & key_store_buf,
        uint64_t & key_store_size) const {
        dfs_key_head_t * pkey_head = NULL;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((err = _get_key_head(key_offset, &pkey_head)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_key_head() returns 0x%x", err);
        } else if (!pkey_head->is_valid()) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_head->is_valid() returns false");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_MARK);
        } else {
            key_store_buf = (const char *)&(pkey_head[1]);
            key_store_size = pkey_head->get_key_store_size();
        }

        return err;
    };
private:
    //读写线程都可能调用
    //return: 0 for success, other values for error
    int _del_key(const uint64_t key_offset) {
        dfs_key_head_t * pkey_head = NULL;
        uint64_t aligned_key_store_size = 0;
        uint64_t key_offset_in_buf = key_offset % KEY_BUF_SIZE;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((err = _get_key_head(key_offset, &pkey_head)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_key_head() returns 0x%x", err);
        } else if (!pkey_head->is_valid()) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_head->is_valid() returns false");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_MARK);
        } else if (pkey_head->get_ref_counter() != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level,
                            "pkey_head->get_ref_counter()=%ld",
                            pkey_head->get_ref_counter());
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_REF_COUNTER);
        } else {
            aligned_key_store_size =
                (pkey_head->get_key_store_size()+sizeof(uint64_t)-1) & ~(sizeof(uint64_t)-1);
            if ((key_offset_in_buf+aligned_key_store_size) > _data_offset
                || aligned_key_store_size > _occupied_size
                || _key_num <= 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "wrong key_offset_in_buf or key_store_size");
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_POS);
            } else {
                pkey_head->set_del_mark();

                //stored total key size, including the head
                atomic_add(&_occupied_size, -(sizeof(dfs_key_head_t)+aligned_key_store_size));
                atomic_dec(&_key_num);      //stored key number;
            }
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level, "key_offset=0x%lx", key_offset);
            DF_WRITE_LOG_US(log_level,
                            "key_offset_in_buf=0x%lx, aligned_key_store_size=%ld",
                            key_offset_in_buf, aligned_key_store_size);
            DF_WRITE_LOG_US(log_level,
                            "_key_num=%ld, _data_offset=0x%lx, _occupied_size=%ld",
                            _key_num, _data_offset, _occupied_size);
        }

        return err;
    };
public:
    //Return: 0 for success, other values for error
    int buf_inc_key_ref_counter(const uint64_t key_offset, uint64_t & ref_counter) {
        dfs_key_head_t * pkey_head = NULL;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        ref_counter = UNDEF_REF_COUNTER;
        if ((err = _get_key_head(key_offset, &pkey_head)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_key_head() returns 0x%x", err);
        } else if (!pkey_head->is_valid()) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_head->is_valid() returns false");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_MARK);
        } else if ((err = pkey_head->inc_ref_counter(ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_head->inc_ref_counter() returns 0x%x", err);
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level, "key_offset=0x%lx", key_offset);
        }

        return err;
    };
    //Return: 0 for success, other values for error
    int buf_dec_key_ref_counter(const uint64_t key_offset, uint64_t & ref_counter) {
        dfs_key_head_t * pkey_head = NULL;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        ref_counter = UNDEF_REF_COUNTER;
        if ((err = _get_key_head(key_offset, &pkey_head)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_key_head() returns 0x%x", err);
        } else if (!pkey_head->is_valid()) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_head->is_valid() returns false");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_MARK);
        } else if ((err = pkey_head->dec_ref_counter(ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_head->inc_ref_counter() returns 0x%x", err);
        } else if (0 == ref_counter && (err = _del_key(key_offset)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_del_key() returns 0x%x", err);
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level, "key_offset=0x%lx", key_offset);
        }

        return err;
    };
    int buf_get_key_ref_counter(const uint64_t key_offset, uint64_t & ref_counter) const {
        dfs_key_head_t * pkey_head = NULL;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        ref_counter = UNDEF_REF_COUNTER;
        if ((err = _get_key_head(key_offset, &pkey_head)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_key_head() returns 0x%x", err);
        } else if (!pkey_head->is_valid()) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_head->is_valid() returns false");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_MARK);
        } else if ((ref_counter = pkey_head->get_ref_counter()) == UNDEF_REF_COUNTER) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_head->get_ref_counter() = UNDEF_REF_COUNTER");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_REF_COUNTER);
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level, "key_offset=0x%lx", key_offset);
        }

        return err;
    };
public:
    uint64_t get_key_num(void) const {
        return _key_num;
    };
    uint64_t get_occupied_size(void) const {
        return _occupied_size;
    };
    uint64_t buf_get_buf_size(void) const {
        return (NULL == _buf) ? 0 : KEY_BUF_SIZE;
    };
    int buf_get_total_nums(uint64_t & total_ref_counter, uint64_t & del_key_num) const {
        dfs_key_head_t * pkey_head = NULL;
        uint64_t in_use_key_num = 0;
        uint64_t in_use_occupied_size = 0;
        uint64_t key_offset = 0;
        uint64_t key_offset_in_buf = 0;
        uint64_t aligned_key_store_size = 0;
        uint64_t ref_counter = 0;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        total_ref_counter  = 0;
        del_key_num = 0;

        if (NULL != _buf) {
            while (key_offset_in_buf < _data_offset) {
                key_offset_in_buf += sizeof(dfs_key_head_t);
                key_offset = key_offset_in_buf;

                if ((err = _get_key_head(key_offset, &pkey_head)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_get_key_head() returns 0x%x", err);
                    break;
                } else if (!pkey_head->is_valid()) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "pkey_head->is_valid() returns false");
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_MARK);
                    break;
                } else if ((ref_counter = pkey_head->get_ref_counter()) == UNDEF_REF_COUNTER) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "pkey_head->get_ref_counter() = UNDEF_REF_COUNTER");
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_REF_COUNTER);
                    break;
                }

                total_ref_counter += pkey_head->get_ref_counter();
                aligned_key_store_size =
                    (pkey_head->get_key_store_size()+sizeof(uint64_t)-1) & ~(sizeof(uint64_t)-1);

                if (pkey_head->is_del_mark_set()) {
                    ++del_key_num;
                } else if (pkey_head->is_in_use_mark_set()) {
                    ++in_use_key_num;
                    in_use_occupied_size += sizeof(*pkey_head) + aligned_key_store_size;
                } else {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "pkey_head is invalid (use_mark destroyed)");
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_MARK);
                    break;
                }

                key_offset_in_buf += aligned_key_store_size;
            }

            if (0 == err) {
                if (key_offset_in_buf != _data_offset) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "key_offset_in_buf != _data_offset");
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_POS);
                } else if (in_use_key_num != get_key_num()) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "in_use_key_num != get_key_num()");
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_POS);
                } else if (get_occupied_size() != in_use_occupied_size) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "in_use_occupied_size != get_key_num()");
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_POS);
                }
            }

            if (DF_UL_LOG_NONE != log_level) {
                DF_WRITE_LOG_US(log_level,
                                "key_offset_in_buf=0x%lx, _data_offset=0x%lx",
                                key_offset_in_buf, _data_offset);
                DF_WRITE_LOG_US(log_level,
                                "in_use_key_num=%ld, get_key_num()=0x%lx",
                                in_use_key_num, get_key_num());
                DF_WRITE_LOG_US(log_level,
                                "in_use_occupied_size=%ld, get_occupied_size()=0x%lx",
                                in_use_occupied_size, get_occupied_size());
            }
        }

        return err;
    };
};


//数组模板类，外部看是一维char数组，以uint64_t作为offset访问，数组元素类型是char。
//dfs_2d_key_buf_t<uint64_t KEY_BUF_SIZE>
//  模板参数：KEY_BUF_SIZE：常数，每个缓冲区的尺寸，建议为2的方幂
//特别说明：需要使用外部的锁机制来保证对数组的修改是单一的(即同一时间只能有一个线程修改数组)！
//规则：public接口仅提供“只读”操作，protected接口提供修改操作。
//
//内部是二维数组，每个缓冲区长度是KEY_BUF_SIZE。
//
//外部接口(public)：
//  规则：public接口仅提供“只读”操作，protected接口提供修改操作
//  构造函数：缺省构造函数;
//
template<uint64_t KEY_BUF_SIZE = dfs_bt_const_t::DEF_KEY_BUF_SIZE>
class dfs_2d_key_buf_t : virtual public dfs_bt_const_t
{
private:
    typedef dfs_key_buf_t<KEY_BUF_SIZE> KEY_BUF;
    typedef KEY_BUF * PKEY_BUF;
    enum cconst {
        MIN_BUF_ARY_SIZE = 256,
        MIN_FREE_QUEUE_SIZE = 512,
    };
protected:
    static const uint64_t UNDEF_OFFSET   = dfs_key_buf_t<KEY_BUF_SIZE>::UNDEF_OFFSET   ;
    static uint64_t extract_buf_index(const uint64_t offset) {
        return (offset/KEY_BUF_SIZE);
    };
    static uint64_t make_offset(const uint64_t buf_index, const uint64_t buf_offset) {
        return (buf_offset+buf_index*KEY_BUF_SIZE);
    };
private:
    dfs_init_t _init_state;
    volatile PKEY_BUF _buf_ary; //读写同时访问
    volatile uint64_t _buf_ary_size;  //读写同时访问
    volatile uint64_t _next_vend_buf_index;   //下一个即将分配的单元
    volatile uint64_t _cur_using_buf_index;   //当前正在使用的buf在_buf_ary中的位置；读写同时访问
    volatile uint64_t _free_counter; //wq队列中元素的个数
    dfs_2d_base_t _free_head;
    PKEY_BUF _realloc_buf_ary[MAX_2D_ARY_HOLD_REALLOC_NUM];
    volatile uint64_t     _realloc_counter;
    //statistic information purpose
    volatile uint64_t _key_del_counter;
    volatile uint64_t _key_add_counter;
    volatile uint64_t _key_ref_inc_counter;
    volatile uint64_t _key_ref_dec_counter;
public:
    dfs_2d_key_buf_t() {
        uint64_t j = 0;
        uint64_t n = 0;

        _free_counter = 0;
        _free_head.set_free();

        _next_vend_buf_index = 0;
        _cur_using_buf_index = UNDEF_INDEX;
        _buf_ary_size = MIN_BUF_ARY_SIZE;
        _buf_ary = NULL;

        n = len_of_ary(_realloc_buf_ary);
        for (j = 0; j < n; ++j) {
            _realloc_buf_ary[j] = NULL;
        }
        _realloc_counter = 0;

        _key_del_counter = 0;
        _key_add_counter = 0;
        _key_ref_inc_counter = 0;
        _key_ref_dec_counter = 0;

        init();
    };
    virtual ~ dfs_2d_key_buf_t() {
        uint64_t j = 0;
        uint64_t n = 0;

        if (NULL != _buf_ary) {
            n = _buf_ary_size;
            for (j = 0; j < n; ++j) {
                _buf_ary[j].dealloc();
            }
            delete[] _buf_ary;
            _buf_ary = NULL;
        }

        n = len_of_ary(_realloc_buf_ary);
        for (j = 0; j < n; ++j) {
            delete [] _realloc_buf_ary[j];
            _realloc_buf_ary[j] = NULL;
        }

        _init_state.set_destructed();
    };
    int init(void) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (_init_state.is_not_init_yet()) {
            //DFS_BT_DEBUG_TRAP;
            uint64_t first_buf_index = UNDEF_INDEX;

            if (NULL != _buf_ary) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "NULL != _buf_ary");
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NULL_BUF_POINTER);
            } else if ((_buf_ary = new(std::nothrow) KEY_BUF[_buf_ary_size]) == NULL) {
                err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_NOMEM);
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, ERRINFO_BT_NOMEM, "_buf_ary");
            } else if ((err = _acquire_buf(first_buf_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_acquire_buf() returns 0x%x", err);
            } else if ((_cur_using_buf_index = first_buf_index) == UNDEF_INDEX) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_cur_using_buf_index == UNDEF_INDEX");
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_ACQUIRE);
            }

            if (DF_UL_LOG_NONE != log_level) {
                DF_WRITE_LOG_US(log_level,
                                "_buf_ary=0x%p, _buf_ary_size=%ld",
                                _buf_ary, _buf_ary_size);
                DF_WRITE_LOG_US(log_level,
                                "first_buf_index=0x%lx, _cur_using_buf_index=0x%lx",
                                first_buf_index, _cur_using_buf_index);
            }

            _init_state.set_init_result(err);
        } else {
            err = _init_state.get_init_result();
        }

        return err;
    };
private:
    //statistic information purpose
    //called whenever an obj is deleted
    uint64_t _inc_key_del_counter(void) {
        return atomic_inc(&_key_del_counter);
    };
    //
    //called whenever a new obj is added
    uint64_t _inc_key_add_counter(void) {
        return atomic_inc(&_key_add_counter);
    };
    //
    //called whenever an key's ref_counter is incremented
    uint64_t _inc_key_ref_inc_counter(void) {
        return atomic_inc(&_key_ref_inc_counter);
    };
    //
    //called whenever an obj's ref_counter is decremented
    uint64_t _inc_key_ref_dec_counter(void) {
        return atomic_inc(&_key_ref_dec_counter);
    };
public:
    void clear_statistic_info(void) {
        atomic_exchange(&_key_del_counter, 0);
        atomic_exchange(&_key_add_counter, 0);
        atomic_exchange(&_key_ref_inc_counter, 0);
        atomic_exchange(&_key_ref_dec_counter, 0);
        return;
    };
    void log_statistic_info(
        const int log_level,
        const char * filename,
        const int lineno,
        const char * funcname,
        const char * btreename,
        const char * bt2dname) const {
        DF_WRITE_LOG_US(log_level, "%s,%d,%s", filename, lineno, funcname);
        DF_WRITE_LOG_US(log_level, "%s, %s", btreename, bt2dname);
        DF_WRITE_LOG_US(log_level, "_key_del_counter=%ld", _key_del_counter);
        DF_WRITE_LOG_US(log_level, "_key_add_counter=%ld", _key_add_counter);
        DF_WRITE_LOG_US(log_level, "_key_ref_inc_counter=%ld", _key_ref_inc_counter);
        DF_WRITE_LOG_US(log_level, "_key_ref_dec_counter=%ld", _key_ref_dec_counter);
        return;
    };
    void log_debug_info(
        const int log_level,
        const char * filename,
        const int lineno,
        const char * funcname,
        const char * btreename,
        const char * bt2dname) const {
        DF_WRITE_LOG_US(log_level, "%s,%d,%s", filename, lineno, funcname);
        DF_WRITE_LOG_US(log_level, "%s, %s", btreename, bt2dname);
        DF_WRITE_LOG_US(log_level, "_buf_ary_size=%ld", _buf_ary_size);
        DF_WRITE_LOG_US(log_level, "_next_vend_buf_index=%ld", _next_vend_buf_index);
        DF_WRITE_LOG_US(log_level, "_cur_using_buf_index=%ld", _cur_using_buf_index);
        DF_WRITE_LOG_US(log_level, "_free_head.get_next_index()=%ld", _free_head.get_next_index());
        DF_WRITE_LOG_US(log_level, "_free_counter=%ld", _free_counter);
    };
private:
    bool _is_valid_buf_index(const uint64_t buf_index) const {
        return (buf_index < _buf_ary_size); //the first one is used as _free_head
    };
    //读写线程都可能调用
    int _push_free(const uint64_t freed_index) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (!_is_valid_buf_index(freed_index)) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "Invalid buf index");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
        } else if (_buf_ary[freed_index].get_key_num() > 0
                   || _buf_ary[freed_index].get_occupied_size() > 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level,
                            "To be freed buf: get_key_num()=%ld, but get_occupied_size() is 0x%lx",
                            _buf_ary[freed_index].get_key_num(),
                            _buf_ary[freed_index].get_occupied_size());
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_REF_COUNTER);
        } else {
            _buf_ary[freed_index].dfs_2d_base_t::set_free();
            if ((err = _free_head.add_to_list(
                           freed_index,
                           (dfs_2d_base_t &)_buf_ary[freed_index])) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_free_head.add_to_list() returns 0x%x", err);
            } else {
                atomic_inc(&_free_counter);
            }
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "freed_index: 0x%lx, _buf_ary_size=%ld",
                            freed_index, _buf_ary_size);
        }

        return err;
    };
    //只有写线程调用，因此只有一个实例
    int _pop_free(uint64_t & ret_index) {
        uint64_t raw_data = 0;
        bool is_removed = false;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        ret_index = UNDEF_INDEX;

        raw_data = _free_head.get_raw_data();
        ret_index = dfs_2d_base_t::extract_next_index(raw_data);
        while (UNDEF_INDEX != ret_index) {
            is_removed = false;
            if (!_is_valid_buf_index(ret_index)) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level,
                                "Invalid buf index: 0x%lx, _buf_ary_size=%ld",
                                ret_index, _buf_ary_size);
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
                ret_index = UNDEF_INDEX;
            }
            //获得raw_data以及获得_buf_ary[ret_index].get_next_index()不是一个原子操作，
            //如果有多个_pop_free线程，则在获得raw_data后_buf_ary[ret_index].get_next_index()可能
            //发生变化(即使raw_data与ret_index没有变化)，因此该算法对多个_pop_free()线程并不安全。
            else if ((err = _free_head.remove_from_list(
                                raw_data,
                                _buf_ary[ret_index].dfs_2d_base_t::get_next_index(),
                                is_removed)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_free_head.remove_from_list() returns 0x%x", err);
                ret_index = UNDEF_INDEX;
                break;
            } else if (is_removed) {
                _buf_ary[ret_index].dfs_2d_base_t::set_in_use();
                atomic_dec(&_free_counter);
                break;
            }
            raw_data = _free_head.dfs_2d_base_t::get_raw_data();
            ret_index = dfs_2d_base_t::extract_next_index(raw_data);
        }

        return err;
    };
    int _acquire_buf(uint64_t & buf_index) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        buf_index = UNDEF_INDEX;
        if ((err = _pop_free(buf_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_pop_free() return 0x%x", err);
        } else if (UNDEF_INDEX == buf_index) {
            if (_next_vend_buf_index < _buf_ary_size) {
                buf_index = atomic_inc(&_next_vend_buf_index)-1;
            } else if ((err = _enlarge_buf_ary()) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_enlarge_buf_ary() return 0x%x", err);
            } else if (_next_vend_buf_index >= _buf_ary_size) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_next_vend_buf_index >= _buf_ary_size");
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_BUF_POS);
            } else {
                buf_index = atomic_inc(&_next_vend_buf_index)-1;
            }
        }
        if (UNDEF_INDEX != buf_index) {
            if ((err = _buf_ary[buf_index].alloc()) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_buf_ary[buf_index].alloc() return 0x%x", err);
            }
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "_next_vend_buf_index=0x%lx, _buf_ary_size=%ld, buf_index=0x%lx",
                            _next_vend_buf_index, _buf_ary_size, buf_index);
        }

        return err;
    };
    //return: 0 for success, otherwise error
    int _enlarge_buf_ary(void) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (_next_vend_buf_index >= _buf_ary_size) {
            //当前_buf_ary数组已满
            uint64_t j = 0;
            uint64_t new_buf_ary_size = _buf_ary_size * 2;
            uint64_t old_buf_ary_size = 0;
            PKEY_BUF new_buf_ary = NULL;
            PKEY_BUF old_buf_ary = NULL;

            if ((new_buf_ary = new(std::nothrow) KEY_BUF[new_buf_ary_size]) == NULL) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "alloc new_buf_ary fails");
                DF_WRITE_LOG_US(log_level,
                                "new_buf_ary_size=%ld, _buf_ary_size=%ld",
                                new_buf_ary_size, _buf_ary_size);
                err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_NOMEM);
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, ERRINFO_BT_NOMEM, "new_buf_ary");
            } else {
                j = 0;
                for (; j < _buf_ary_size; ++j) {
                    new_buf_ary[j] = _buf_ary[j];
                }
                old_buf_ary = (PKEY_BUF)atomic_exchange_pointer(
                                  (volatile pvoid *)&(_buf_ary),
                                  (pvoid)new_buf_ary);
                old_buf_ary_size = atomic_exchange(&_buf_ary_size, new_buf_ary_size);
                //把旧数组加入到缓冲区链，暂时不释放，因为其他读线程可能正在访问。
                delete [] _realloc_buf_ary[_realloc_counter];
                _realloc_buf_ary[_realloc_counter] = old_buf_ary;
                _realloc_counter = (_realloc_counter+1)%len_of_ary(_realloc_buf_ary);
            }
        }

        return err;
    };
public:
    //  输出: offset, UNDEF_OFFSET for insufficient space
    //return: 0 for success, other values for error
    int buf_ary_add_key(
        const uint64_t key_store_size,
        char * & key_store_buf,
        uint64_t & key_offset) {
        uint64_t key_offset_in_buf = UNDEF_OFFSET;
        uint64_t new_buf_index = UNDEF_INDEX;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        _inc_key_add_counter();
        key_offset = UNDEF_OFFSET;
        if (key_store_size >= (KEY_BUF_SIZE-sizeof(dfs_key_head_t))) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "wrong key_store_size");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_SIZE);
        } else if (!_is_valid_buf_index(_cur_using_buf_index)) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "invalid _cur_using_buf_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
        } else {
            if (!_buf_ary[_cur_using_buf_index].is_enough_room_for_key(key_store_size)) {
                if ((err = _acquire_buf(new_buf_index)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_acquire_buf() returns 0x%x", err);
                } else if (UNDEF_INDEX == new_buf_index) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "invalid new_buf_index by _acquire_buf()");
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
                } else {
                    atomic_exchange(&_cur_using_buf_index, new_buf_index);
                }
            }
            if (0 == err) {
                if ((err = _buf_ary[_cur_using_buf_index].buf_add_key(
                               key_store_size,
                               key_store_buf,
                               key_offset_in_buf)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_acquire_buf() returns 0x%x", err);
                } else {
                    key_offset = make_offset(_cur_using_buf_index, key_offset_in_buf);
                }
            }
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "_next_vend_buf_index=0x%lx, _buf_ary_size=%ld, _cur_using_buf_index=0x%lx",
                            _next_vend_buf_index, _buf_ary_size, _cur_using_buf_index);
            DF_WRITE_LOG_US(log_level,
                            "key_store_size=%ld, KEY_BUF_SIZE=%ld, sizeof(dfs_key_head_t)=%ld",
                            key_store_size, KEY_BUF_SIZE, sizeof(dfs_key_head_t));
        }

        return err;
    };
    //Return: 0 for success, other values for error
    int buf_ary_inc_key_ref_counter(const uint64_t key_offset, uint64_t & ref_counter) {
        const uint64_t buf_index = extract_buf_index(key_offset);
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        _inc_key_ref_inc_counter();
        if (!_is_valid_buf_index(buf_index)) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "invalid buf_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
        } else if ((err = _buf_ary[buf_index].buf_inc_key_ref_counter(key_offset, ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_buf_ary[buf_index].buf_inc_key_ref_counter() returns 0x%x", err);
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "_next_vend_buf_index=0x%lx, _buf_ary_size=%ld, _cur_using_buf_index=0x%lx",
                            _next_vend_buf_index, _buf_ary_size, _cur_using_buf_index);
            DF_WRITE_LOG_US(log_level,
                            "key_offset=0x%lx, buf_index=0x%lx",
                            key_offset, buf_index);
        }

        return err;
    };
    //Return: 0 for success, other values for error
    int buf_ary_dec_key_ref_counter(const uint64_t key_offset, uint64_t & ref_counter) {
        const uint64_t buf_index = extract_buf_index(key_offset);
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        _inc_key_ref_dec_counter();
        if (!_is_valid_buf_index(buf_index)) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "Invalid buf_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
        } else if ((err = _buf_ary[buf_index].buf_dec_key_ref_counter(key_offset, ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "buf_dec_key_ref_counter() returns 0x%x", err);
        } else if (0 == ref_counter) {
            _inc_key_del_counter();
            if (0 == _buf_ary[buf_index].get_key_num()
                || 0 == _buf_ary[buf_index].get_occupied_size()) {
                if (0 != _buf_ary[buf_index].get_key_num()
                    || 0 != _buf_ary[buf_index].get_occupied_size()) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level,
                                    "get_key_num()=%ld, but get_occupied_size()=%ld",
                                    _buf_ary[buf_index].get_key_num(),
                                    _buf_ary[buf_index].get_occupied_size());
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NONE_EMPTY_BUF);
                }
                //当前正在使用的buf不能放到free_list
                else if (buf_index != _cur_using_buf_index) {
                    if ((err = _push_free(buf_index)) != 0) {
                        log_level = DF_UL_LOG_FATAL;
                        DF_WRITE_LOG_US(log_level, "_push_free() returns 0x%x", err);
                    } else {
                        //DF_WRITE_LOG_US(DF_UL_LOG_DEBUG, "_push_free(buf_index=0x%lx)", buf_index);
                    }
                }
            }
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level, "buf_index=0x%lx, key_offset=0x%lx,", buf_index, key_offset);
            DF_WRITE_LOG_US(log_level,
                            "_next_vend_buf_index=0x%lx, _buf_ary_size=%ld",
                            _next_vend_buf_index, _buf_ary_size);
            DF_WRITE_LOG_US(log_level,
                            "_cur_using_buf_index=0x%lx, _free_counter=%ld",
                            _cur_using_buf_index, _free_counter);
        }

        return err;
    };
    int buf_ary_get_key_ref_counter(const uint64_t key_offset, uint64_t & ref_counter) const {
        const uint64_t buf_index = extract_buf_index(key_offset);
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (!_is_valid_buf_index(buf_index)) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "invalid buf_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
        } else  if((err = _buf_ary[buf_index].buf_get_key_ref_counter(key_offset, ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_buf_ary[buf_index].buf_get_key_ref_counter() returns 0x%x", err);
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "_next_vend_buf_index=0x%lx, _buf_ary_size=%ld, _cur_using_buf_index=0x%lx",
                            _next_vend_buf_index, _buf_ary_size, _cur_using_buf_index);
            DF_WRITE_LOG_US(log_level,
                            "key_offset=0x%lx, buf_index=0x%lx",
                            key_offset, buf_index);
        }

        return err;
    };
    //  输出: 名字的指针、长度
    //return: 0 for success, other values for error
    int buf_ary_get_key_store_buf(
        const uint64_t key_offset,
        const char * & key_store_buf,
        uint64_t & key_store_size) const {
        const uint64_t buf_index = extract_buf_index(key_offset);
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (!_is_valid_buf_index(buf_index)) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "invalid buf_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
        } else if ((err = _buf_ary[buf_index].buf_get_key_store_buf(
                              key_offset,
                              key_store_buf,
                              key_store_size)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_buf_ary[buf_index].buf_get_key_store_buf() returns 0x%x", err);
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "_next_vend_buf_index=0x%lx, _buf_ary_size=%ld, _cur_using_buf_index=0x%lx",
                            _next_vend_buf_index, _buf_ary_size, _cur_using_buf_index);
            DF_WRITE_LOG_US(log_level,
                            "key_offset=0x%lx, buf_index=0x%lx",
                            key_offset, buf_index);
        }

        return err;
    };
public:
    uint64_t buf_ary_get_mem_size(void) const {
        uint64_t mem_size = _buf_ary_size*sizeof(_buf_ary[0]);
        uint64_t j = 0;

        while (j < _next_vend_buf_index) {
            mem_size += _buf_ary[j].buf_get_buf_size();
            ++j;
        }

        return mem_size;
    };
    int buf_ary_get_total_nums(uint64_t & total_ref_counter, uint64_t & total_key_num) const {
        uint64_t buf_ref_counter = 0;
        uint64_t buf_del_key_num = 0;
        uint64_t j = 0;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        total_ref_counter = 0;
        total_key_num = 0;
        while (j < _next_vend_buf_index) {
            if ((err = _buf_ary[j].buf_get_total_nums(buf_ref_counter, buf_del_key_num)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_buf_ary[j=0x%lx].buf_get_total_nums() returns 0x%x", j, err);
                break;
            } else {
                total_ref_counter += buf_ref_counter;
                total_key_num += _buf_ary[j].get_key_num();
            }
            ++j;
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "_next_vend_buf_index=0x%lx, _buf_ary_size=%ld",
                            _next_vend_buf_index, _buf_ary_size);
            DF_WRITE_LOG_US(log_level,
                            "_cur_using_buf_index=0x%lx, _free_counter=%ld",
                            _cur_using_buf_index, _free_counter);
        }

        return err;
    };
};


class dfs_vk_offset_t : public dfs_bt_const_t
{
private:
    uint64_t _offset;    //key offset in _key_buf
public:
    dfs_vk_offset_t() {
        _offset = UNDEF_OFFSET;
    };
    dfs_vk_offset_t(const uint64_t offset) {
        _offset = offset;
    };
    dfs_vk_offset_t(const dfs_vk_offset_t & src) {
        *this = src;
    };
    ~dfs_vk_offset_t() {
    };
    const dfs_vk_offset_t & operator=(const dfs_vk_offset_t & src) {
        if (&src != this) {
            _offset = src._offset;
        }
        return *this;
    };
    void init(const uint64_t offset = UNDEF_OFFSET) { //初始化对象
        _offset = offset;
    };
public:
    void set_offset(const uint64_t offset) {
        _offset = offset;
    };
    uint64_t get_offset(void) const {
        return _offset;
    };
public:
    //用户数据类型T需要支持以下接口：
    //  功能：获得该实例store的数据长度(字节)，该值不能超过MAX_T_STORE_SIZE
    static uint64_t get_store_size(void) {
        return 0;
    };
    //
    //  功能：存储该实例到buf
    //  输入：data_pos为存储数据的起始位置
    //  输出：data_pos为存储T后新的起始位置
    //  返回：for success, other values for error (例如缓冲区剩余空间不足)
    int store(char * buf, const uint64_t buf_size, uint64_t & data_pos) const {
        return 0;
    };
    //
    //  功能：从buf中装载到该实例
    //  输入：data_pos为装载T前缓冲区的数据位置
    //  输出：data_pos为装载T后缓冲区的数据位置
    //  返回：for success, other values for error(例如缓冲区内数据不足)
    int load(char * buf, const uint64_t data_len, uint64_t & data_pos) {
        return 0;
    };
};


//vk: Variable length Key
class dfs_vk_btree_fhead_ext_t
{
public:
    uint64_t total_leaf_node_num;
    uint64_t total_mid_node_num;
    uint64_t total_key_num;
    uint64_t reserve31[25];
};

class dfs_vk_btree_fhead_t : public dfs_btree_fhead_base_t, public dfs_vk_btree_fhead_ext_t
{
public:
    enum cconst_public {
        VK_BT_VER = 0x00000002, //Hihg-16-bit: main ver;
        VK_BT_TAG = (((int)'f')<<24) | (((int)'p')<<16) | (((int)'b')<<8) | (((int)'t')), //"fpbt"=0x66706274
        //出于兼容的原因，此处版本号和TAG使用了原FP_BTREE的值
    };
};


//_dfs_sf_btree_t : string and filepath (excluding id)
template<typename D, typename K, uint32_t ROW_SIZE>
class _dfs_sf_btree_t :
    virtual public dfs_bt_const_t,
public dfs_btree_t
{
protected:
    enum cconst_protected {
    };
private:
    //typedef T *PT;
    //typedef const T *PCT;
    typedef D DU;
    typedef DU *PDU;
    typedef const DU *PCDU;
    typedef K *PK;
    typedef const K *PCK;
protected:
    typedef dfs_bt2d_ary_t<DU, ROW_SIZE> t_ary_t;
private:
    dfs_init_t _init_state;
    dfs_2d_key_buf_t<dfs_bt_const_t::DEF_KEY_BUF_SIZE> _key_buf;
    t_ary_t & _t_2d_ary;
public:
    _dfs_sf_btree_t(t_ary_t & t_2d_ary) :
        dfs_btree_t(),
        _t_2d_ary(t_2d_ary) {
        //init();
    };
    virtual ~_dfs_sf_btree_t() {
        _init_state.set_destructed();
    };
    int init(const uint32_t bt_instance_pos, dfs_bt_root_t & new_wr_bt_root);
private:
    //Disable operator=() and copy constructor
    const _dfs_sf_btree_t & operator=(const _dfs_sf_btree_t & src);
    _dfs_sf_btree_t(const _dfs_sf_btree_t & src);
    //const _dfs_sf_btree_t & operator=(const _dfs_sf_btree_t & src)
    //{
    //    return *this;
    //};
    //_dfs_sf_btree_t(const _dfs_sf_btree_t & src) : dfs_btree_t()
    //{
    //};
protected:
    //int _dec_key_string_ref_counter(const uint64_t key_index, uint64_t & ref_counter);
    //int _inc_key_string_ref_counter(const uint64_t key_index, uint64_t & ref_counter);
    int _get_key_ref_counter(const uint64_t key_index, uint64_t & ref_counter) const;
private:
    int _sf_store_fhead(
        dfs_btree_store_info_t & store_info,
        int64_t & file_head_offset,
        dfs_vk_btree_fhead_t & vk_btree_fhead) const;
    int _sf_update_stored_fhead(
        dfs_btree_store_info_t & store_info,
        const int64_t file_head_offset,
        dfs_vk_btree_fhead_t & vk_btree_fhead) const;
    int _sf_load_fhead(
        dfs_btree_load_info_t & load_info,
        int64_t & file_head_offset,
        dfs_vk_btree_fhead_t & vk_btree_fhead);
protected:
    //  功能：把已经设置的checkpoint存储到文件中
    //  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
    //  返回：0 for success, other values for error
    int _sf_store_checkpointing(
        const dfs_bt_root_t & bt_root,
        dfs_btree_store_info_t & store_info) const;
    //
    //  功能：把先前存储在文件的checkpoint加载到内存中
    //  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
    //  返回：0 for success, other values for error
    int _sf_load_checkpointing(
        dfs_bt_root_t & bt_root,
        dfs_btree_load_info_t & load_info);
protected:
    //  return: 0 for success, other values for error
    inline int _sf_get_key_by_index(
        const uint64_t key_index,
        K & keyt,
        PCDU & pcdu) const;
    inline int _sf_get_key_by_obj(
        const DU & duobj,
        K & keyt) const;
protected:
    inline int _sf_compare_index_index(const uint64_t key1_index, const uint64_t key2_index, int & cmp) const;
    inline int _sf_compare_index_key(const uint64_t key1_index, const K & keyt2, int & cmp) const;
    inline int _sf_compare_index_obj(const uint64_t key1_index, const DU & duobj2, int & cmp) const;
    inline int _sf_compare_obj_obj(const DU & duobj1, const DU & duobj2, int & cmp) const;
    inline int _sf_compare_obj_key(const DU & duobj1, const K & keyt2, int & cmp) const;
protected:
    //return: 0 for success, other values for error
    //May return 0 even if obj_index is invalid and *ppdu is set to NULL.
    inline int _get_exist_obj_for_mutate(const uint64_t obj_index, PDU & pdu) const;
    //return: 0 for success, other values for error
    //Notice: the obj must exist.
    inline int _get_exist_obj(const uint64_t obj_index, PCDU & pcdu) const ;
    //return: 0 for success, other values for error
    //May return 0 even if obj_index is invalid and *ppcdu is set to NULL.
    inline int _get_may_not_exist_obj(const uint64_t obj_index, PCDU & pcdu) const;
//private:
protected:  //之所有要protected是因为_add_obj在_dfs_idsf_btree_t中的_idsf_insert/_idsf_insert_update()调用
    //return: 0 for success, other values for error
    //以obj为基础生成一个新的T对象
    //name string已经存在
    inline int _add_obj(const DU & duobj, uint64_t & obj_index);
    //name string不存在
    inline int _add_obj(const DU & duobj, const K & keyt, uint64_t & obj_index);
    //inline int _del_obj(const uint64_t obj_index);
protected:
    //  return: 0 for success, other values for error
    int _get_obj_ref_counter(const uint64_t obj_index, uint64_t & ref_counter) const;
private:
    int _inc_obj_ref_counter(const uint64_t obj_index, uint64_t & ref_counter);
    int _dec_obj_ref_counter(const uint64_t obj_index, uint64_t & ref_counter);
protected:
    ////  return: 0 for success, other values for error
    //virtual int vir_get_total_obj_ref_counter_in_ary(uint64_t & total_ref_counter) const;
    //  return: 0 for success, other values for error
    virtual int vir_get_total_in_use_obj_num_in_ary(uint64_t & total_in_use_obj_num) const;
    //  Return: 0 for success, other values for error
    virtual int vir_obj_store_size(
        const uint64_t obj_index,
        uint64_t & store_size,
        dfs_btree_store_info_t & store_info) const;
    //  return: 0 for success, other values for error
    virtual int vir_store_obj(
        const uint64_t obj_index,
        dfs_btree_store_info_t & store_info) const;
    //  return: 0 for success, other values for error
    virtual int vir_load_obj(
        uint64_t & obj_index,
        dfs_btree_load_info_t & load_info);
    ////  return: 0 for success, other values for error
    //virtual int vir_get_obj_ref_counter(const uint64_t obj_index, uint64_t & ref_counter) const;
    virtual int vir_inc_obj_ref_counter(const uint64_t obj_index);
    virtual int vir_dec_obj_ref_counter(const uint64_t obj_index);
    virtual int vir_inc_obj_ary_ref_counter(const uint64_t * obj_index_ary, const int32_t obj_num);
    virtual int vir_dec_obj_ary_ref_counter(const uint64_t * obj_index_ary, const int32_t obj_num);
protected:
    //
    //  return: 0 for success, other values for error
    //  cmp: -1, 0, 1 for less than, equal, great than
    //virtual int vir_compare_index_index(const uint64_t obj1_index, const uint64_t obj2_index, int & cmp) const;
    virtual int vir_compare_index_obj(
        const dfs_bt_root_t & bt_root,
        const uint64_t obj1_index,
        const void * pobj2,
        int & cmp) const;
    virtual int vir_compare_index_key(
        const dfs_bt_root_t & bt_root,
        const uint64_t obj1_index,
        const void * pkey2,
        int & cmp) const;

    virtual int vir_search_ary_by_index(
        const dfs_bt_root_t & bt_root,
        const uint64_t * obj_index_ary,
        const int32_t obj_num,
        const uint64_t obj2_index,
        int & ins_pos,
        uint64_t & obj_index) const;
    virtual int vir_search_ary_by_obj(
        const dfs_bt_root_t & bt_root,
        const uint64_t * obj_index_ary,
        const int32_t obj_num,
        const void * pobj2,
        int & ins_pos,
        uint64_t & obj_index) const;
    virtual int vir_search_ary_by_key(
        const dfs_bt_root_t & bt_root,
        const uint64_t * obj_index_ary,
        const int32_t obj_num,
        const void * pkey2,
        int & ins_pos,
        uint64_t & obj_index) const;
    virtual int vir_verify_index_ary(
        const dfs_bt_root_t & bt_root,
        const uint64_t * obj_index_ary,
        const int32_t obj_num,
        bool & is_inc_order) const;
protected:
    //  return: 0 for success, other values for error
    int sf_get_in_use_num_in_2d_ary(
        uint64_t & in_use_obj_num,
        uint64_t & in_use_leaf_node_num,
        uint64_t & in_use_mid_node_num) const;
    //  return: 0 for success, other values for error
    int _buf_ary_get_total_nums(
        uint64_t & buf_ary_total_ref_counter,
        uint64_t & buf_ary_total_key_num) const {
        return _key_buf.buf_ary_get_total_nums(
                   buf_ary_total_ref_counter,
                   buf_ary_total_key_num);
    }
protected:
    void _sf_clear_statistic_info(void) {
        _t_2d_ary.clear_statistic_info();
        this->_bt_clear_statistic_info();
    };
    void _sf_log_statistic_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname) const {
        _t_2d_ary.log_statistic_info(org_log_level, filename, lineno, funcname, "sf", "t_2d_ary");
        this->_bt_log_statistic_info(org_log_level, filename, lineno, funcname, "sf");
    };
    void _sf_log_debug_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname) const {
        _t_2d_ary.log_debug_info(org_log_level, filename, lineno, funcname, "sf", "t_2d_ary");
        this->_bt_log_debug_info(org_log_level, filename, lineno, funcname, "sf");
    };
    uint64_t _sf_get_mem_size(void) const {
        return this->_bt_get_mem_size()+_get_bt2d_ary_mem_size();
    };
    uint64_t _get_bt2d_ary_mem_size() const {
        return _t_2d_ary.get_mem_size();
    };
};


template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::init(
    const uint32_t bt_instance_pos,
    dfs_bt_root_t & new_wr_bt_root)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (_init_state.is_not_init_yet()) {
        if ((err = _key_buf.init()) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_key_buf.init() returns 0x%x", err);
        } else if ((err = _t_2d_ary.set_gc_info((void *)&_key_buf)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "t_ary_t::set_gc_info() returns 0x%x", err);
        } else if ((err = dfs_btree_t::init(bt_instance_pos, new_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "dfs_btree_t::init() returns 0x%x", err);
        }

        _init_state.set_init_result(err);
    } else {
        err = _init_state.get_init_result();
    }

    return err;
};


template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_get_key_ref_counter(
    const uint64_t key_index,
    uint64_t & ref_counter) const
{
    PCDU pcdu = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    ref_counter = UNDEF_REF_COUNTER;
    if ((err = _get_exist_obj(key_index, pcdu)) != 0 || NULL == pcdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_get_exist_obj(key_index...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = _key_buf.buf_ary_get_key_ref_counter(pcdu->get_offset(), ref_counter)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_key_buf.buf_ary_get_key_ref_counter() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_index=0x%lx, ref_counter=%lu",
                        key_index, ref_counter);
        if (NULL != pcdu) {
            DF_WRITE_LOG_US(log_level,
                            "offset=0x%lx, id=%ld",
                            pcdu->get_offset(), pcdu->get_id());
        }
    }

    return err;
};


template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_store_fhead(
    dfs_btree_store_info_t & store_info,
    int64_t & file_head_offset,
    dfs_vk_btree_fhead_t & vk_btree_fhead) const
{
    int err = 0;

    file_head_offset = store_info.get_cur_file_offset();

    memset(&vk_btree_fhead, 0, sizeof(vk_btree_fhead));
    vk_btree_fhead.size = sizeof(vk_btree_fhead);
    vk_btree_fhead.ver  = dfs_vk_btree_fhead_t::VK_BT_VER;
    vk_btree_fhead.major_tag  = store_info.get_major_tag();
    vk_btree_fhead.minor_tag = dfs_vk_btree_fhead_t::VK_BT_TAG;

    err = store_info.store_buf(&vk_btree_fhead, sizeof(vk_btree_fhead));

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_update_stored_fhead(
    dfs_btree_store_info_t & store_info,
    const int64_t file_head_offset,
    dfs_vk_btree_fhead_t & vk_btree_fhead) const
{
    int err = 0;

    vk_btree_fhead.total_data_len       = store_info.get_cur_file_offset() - file_head_offset;
    vk_btree_fhead.total_leaf_node_num  = store_info.get_total_leaf_node_num()    ;
    vk_btree_fhead.total_mid_node_num   = store_info.get_total_mid_node_num()     ;
    vk_btree_fhead.total_key_num        = store_info.get_total_key_num()          ;
    err = store_info.rewrite_data(&vk_btree_fhead, sizeof(vk_btree_fhead), file_head_offset);

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_load_fhead(
    dfs_btree_load_info_t & load_info,
    int64_t & file_head_offset,
    dfs_vk_btree_fhead_t & vk_btree_fhead)
{
    uint32_t unread_len = 0;
    const uint32_t unread_head = sizeof(dfs_vk_btree_fhead_t)-sizeof(dfs_btree_fhead_base_t);
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    file_head_offset = load_info.get_cur_file_offset();
    memset(&vk_btree_fhead, 0, sizeof(vk_btree_fhead));

    //此时最大长度不知道，暂时设置为基本头的长度
    load_info.set_max_file_offset(file_head_offset+sizeof(dfs_btree_fhead_base_t));
    //加载基本头信息
    if ((err = load_info.load_buf(
                   (void *)((dfs_btree_fhead_base_t *)&vk_btree_fhead),
                   sizeof(dfs_btree_fhead_base_t))) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "load_info.load_buf(dfs_btree_fhead_base_t) returns 0x%x", err);
    } else if (load_info.get_major_tag() != vk_btree_fhead.major_tag) {
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_MAJOR_TAG);
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_MAJOR_TAG);
    } else if (dfs_vk_btree_fhead_t::VK_BT_TAG != vk_btree_fhead.minor_tag) {
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_MINOR_TAG);
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_MINOR_TAG);
    } else if (dfs_vk_btree_fhead_t::VK_BT_VER < vk_btree_fhead.ver) {
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_HEAD_VER);
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_HEAD_VER);
    } else if (vk_btree_fhead.size < sizeof(dfs_btree_fhead_base_t)) {
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_HEAD_SIZE);
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_HEAD_SIZE);
    } else {
        unread_len = vk_btree_fhead.size-sizeof(dfs_btree_fhead_base_t);

        //设置准确的最大长度
        load_info.set_max_file_offset(file_head_offset+vk_btree_fhead.total_data_len);

        //加载扩展头信息：头信息尺寸可能不等于sizeof(vk_btree_fhead)
        if ((err = load_info.load_buf(
                       (void *)((dfs_vk_btree_fhead_ext_t *)&vk_btree_fhead),
                       MIN(unread_len, unread_head))) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(
                log_level,
                "load_info.load_buf(dfs_vk_btree_fhead_ext_t) returns 0x%x",
                err);
        } else if (unread_len > unread_head) {
            if ((err = load_info.skip_data_len(unread_len-unread_head)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(
                    log_level,
                    "load_info.load_buf(unread_len-unread_head) returns 0x%x",
                    err);
            }
        }
    }

    load_info.set_total_nums(
        vk_btree_fhead.total_leaf_node_num  ,
        vk_btree_fhead.total_mid_node_num   ,
        vk_btree_fhead.total_key_num);

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "vk_btree_fhead.total_data_len=%ld, file_head_offset=%ld",
                        vk_btree_fhead.total_data_len, file_head_offset);
        DF_WRITE_LOG_US(log_level,
                        "load_info.get_major_tag()=0x%x, vk_btree_fhead.major_tag=0x%x",
                        load_info.get_major_tag(), vk_btree_fhead.major_tag);
        DF_WRITE_LOG_US(log_level,
                        "dfs_vk_btree_fhead_t::VK_BT_TAG=0x%x, vk_btree_fhead.minor_tag=0x%x",
                        dfs_vk_btree_fhead_t::VK_BT_TAG, vk_btree_fhead.minor_tag);
        DF_WRITE_LOG_US(log_level,
                        "dfs_vk_btree_fhead_t::VK_BT_VER=0x%x, vk_btree_fhead.ver=0x%x",
                        dfs_vk_btree_fhead_t::VK_BT_VER, vk_btree_fhead.ver);
        DF_WRITE_LOG_US(log_level,
                        "unread_len=0x%x, unread_head=0x%x, vk_btree_fhead.size=0x%x",
                        unread_len, unread_head, vk_btree_fhead.size);
    }

    return err;
};


//  功能：把已经设置的checkpoint存储到文件中
//  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
//  返回：0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_store_checkpointing(
    const dfs_bt_root_t & bt_root,
    dfs_btree_store_info_t & store_info) const
{
    dfs_vk_btree_fhead_t vk_btree_fhead;
    dfs_btree_fhead_t btree_fhead;
    int64_t file_head_offset = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    log_level = this->CKP_LOG_LEVEL;
    if ((err = _sf_store_fhead(store_info, file_head_offset, vk_btree_fhead)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_store_fhead() returns 0x%x", err);
    } else if ((err = this->_bt_store_tree(bt_root, store_info, btree_fhead)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_bt_store_tree() returns 0x%x", err);
    } else if ((err = _sf_update_stored_fhead(store_info, file_head_offset, vk_btree_fhead)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_update_stored_fhead() returns 0x%x", err);
    } else if ((err = store_info.flush_data()) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "(store_info.flush_data) returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "vk_btree_fhead.total_data_len=%ld, file_head_offset=%ld",
                        vk_btree_fhead.total_data_len, file_head_offset);
    }

    return err;
};


//  功能：把先前存储在文件的checkpoint加载到内存中
//  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
//  返回：0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_load_checkpointing(
    dfs_bt_root_t & new_bt_root,
    dfs_btree_load_info_t & load_info)
{
    dfs_vk_btree_fhead_t vk_btree_fhead;
    dfs_btree_fhead_t btree_fhead;
    int64_t file_head_offset = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    log_level = this->CKP_LOG_LEVEL;

    if ((err = _sf_load_fhead(load_info, file_head_offset, vk_btree_fhead)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_load_fhead() returns 0x%x", err);
    } else if ((err = this->_bt_load_tree(
                          new_bt_root,
                          load_info,
                          btree_fhead)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_bt_load_tree() returns 0x%x", err);
    }
    //move the file to the correct position
    else if ((err = load_info.file_seek(file_head_offset+vk_btree_fhead.total_data_len)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "load_info.file_seek() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "vk_btree_fhead.total_data_len=%ld, file_head_offset=%ld",
                        vk_btree_fhead.total_data_len, file_head_offset);
    }

    return err;
};

//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_get_key_by_index(
    const uint64_t key_index,
    K & keyt,
    PCDU & pcdu) const
{
    const char * key_store_buf = NULL;
    uint64_t key_store_size = 0;
    uint64_t key_store_pos = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    keyt.init();

    if ((err = _get_exist_obj(key_index, pcdu)) != 0 || NULL == pcdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_get_exist_obj(key_index...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = _key_buf.buf_ary_get_key_store_buf(
                          pcdu->get_offset(),
                          key_store_buf,
                          key_store_size)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_key_buf.buf_ary_get_key_store_buf() returns 0x%x", err);
    } else if ((err = keyt.load(key_store_buf, key_store_size, key_store_pos)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt.load() returns 0x%x", err);
    } else {
#ifdef DF_USE_ID_IN_STR_KEY
        //keyt.set_id(pcdu->get_id());
#endif
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_index=0x%lx",
                        key_index);
        if (NULL != pcdu) {
            DF_WRITE_LOG_US(log_level,
                            "offset=0x%lx, id=%ld",
                            pcdu->get_offset(), pcdu->get_id());
        }
    }

    return err;
};

//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_get_key_by_obj(
    const DU & duobj,
    K & keyt) const
{
    const char * key_store_buf = NULL;
    uint64_t key_store_size = 0;
    uint64_t key_store_pos = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    keyt.init();

    if ((err = _key_buf.buf_ary_get_key_store_buf(
                   duobj.get_offset(),
                   key_store_buf,
                   key_store_size)) != 0) {
        DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
                        "_key_buf.buf_ary_get_key_store_buf() returns 0x%x, offset=0x%lx, key_id=%lu",
                        err, duobj.get_offset(), duobj.get_id());
    } else if ((err = keyt.load(key_store_buf, key_store_size, key_store_pos)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt.load() returns 0x%x", err);
    } else {
#ifdef DF_USE_ID_IN_STR_KEY
        //keyt.set_id(duobj.get_id());
#endif
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_compare_index_index(
    const uint64_t key1_index,
    const uint64_t key2_index,
    int & cmp) const
{
    PCDU pcdu1 = NULL;
    PCDU pcdu2 = NULL;
    K keyt1;
    K keyt2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 1;
    if ((err = _sf_get_key_by_index(key1_index, keyt1, pcdu1)) != 0 || NULL == pcdu1) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_index() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = _sf_get_key_by_index(key2_index, keyt2, pcdu2)) != 0 || NULL == pcdu2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_index() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = keyt1.compare(keyt2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt1.compare(keyt2) returns 0x%x" , err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key1_index=0x%lx, key2_index=0x%lx",
                        key1_index, key2_index);
        DF_WRITE_LOG_US(log_level,
                        "key1_len=%ld, keyt2_len=%ld",
                        keyt1.get_len(), keyt2.get_len());
    }


    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_compare_index_key(
    const uint64_t key1_index,
    const K & keyt2,
    int & cmp) const
{
    PCDU pcdu1 = NULL;
    K keyt1;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 1;
    if ((err = _sf_get_key_by_index(key1_index, keyt1, pcdu1)) != 0 || NULL == pcdu1) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_index() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = keyt1.compare(keyt2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt1.compare(keyt2) returns 0x%x" , err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key1_index=0x%lx, keyt2_len=%ld",
                        key1_index, keyt2.get_len());
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_compare_index_obj(
    const uint64_t key1_index,
    const DU & duobj2,
    int & cmp) const
{
    PCDU pcdu1 = NULL;
    K keyt1;
    K keyt2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 2;
    if ((err = _sf_get_key_by_index(key1_index, keyt1, pcdu1)) != 0 || NULL == pcdu1) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_index() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = _sf_get_key_by_obj(duobj2, keyt2)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_obj() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = keyt1.compare(keyt2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt1.compare(keyt2) returns 0x%x", err);
        DF_WRITE_LOG_US(log_level, "keyt2_len=%ld", keyt2.get_len());
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key1_index=0x%lx, duobj2.get_offset()=0x%lx",
                        key1_index, duobj2.get_offset());
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_compare_obj_obj(
    const DU & duobj1,
    const DU & duobj2,
    int & cmp) const
{
    K keyt1;
    K keyt2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 2;
    if ((err = _sf_get_key_by_obj(duobj1, keyt1)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_obj(duobj1...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = _sf_get_key_by_obj(duobj2, keyt2)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_obj(duobj2...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = keyt1.compare(keyt2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt1.compare(keyt2) returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "duobj1.get_offset()=0x%lx, duobj2.get_offset()=0x%lx",
                        duobj1.get_offset(), duobj2.get_offset());
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_sf_compare_obj_key(
    const DU & duobj1,
    const K & keyt2,
    int & cmp) const
{
    K keyt1;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 2;
    if ((err = _sf_get_key_by_obj(duobj1, keyt1)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_obj(duobj1...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = keyt1.compare(keyt2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt1.compare(keyt2) returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "duobj1.get_offset()=0x%lx, keyt2.get_len()=%d",
                        duobj1.get_offset(), keyt2.get_len());
    }

    return err;
};

//return: 0 for success, other values for error
//Notice: the obj must exist.
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_get_exist_obj_for_mutate(
    const uint64_t obj_index,
    PDU & pdu) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = _t_2d_ary.get_exist_t_unit(obj_index, pdu)) != 0 || NULL == pdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_t_2d_ary.get_exist_t_unit() returns 0x%x", err);
        DF_WRITE_LOG_US(log_level, "obj_index=0x%lx, pdu=0x%p", obj_index, pdu);
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_INVALID_INDEX);
    }


    return err;
};
//return: 0 for success, other values for error
//Notice: the obj must exist.
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_get_exist_obj(
    const uint64_t obj_index, PCDU & pcdu) const
{
    PDU pdu = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    pcdu = NULL;
    if ((err = _t_2d_ary.get_exist_t_unit(obj_index, pdu)) != 0 || NULL == pdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_t_2d_ary.get_exist_t_unit() returns 0x%x", err);
        DF_WRITE_LOG_US(log_level, "obj_index=0x%lx, pdu=0x%p", obj_index, pdu);
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_INVALID_INDEX);
    } else {
        pcdu = pdu;
    }

    return err;
};
//return: 0 for success, other values for error
//May return 0 even if obj_index is invalid and *ppcdu is set to NULL.
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_get_may_not_exist_obj(
    const uint64_t obj_index,
    PCDU & pcdu) const
{
    PDU pdu = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    pcdu = NULL;
    if ((err = _t_2d_ary.get_may_not_exist_t_unit(obj_index, pdu)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_t_2d_ary.get_may_not_exist_t_unit() returns 0x%x", err);
        DF_WRITE_LOG_US(log_level, "obj_index=0x%lx, pdu=0x%p", obj_index, pdu);
    } else {
        pcdu = pdu;
    }

    return err;
};

//return: 0 for success, other values for error
//以obj为基础生成一个新的T对象
//name string已经存在
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_add_obj(
    const DU & duobj,
    uint64_t & obj_index)
{
    PDU pdu = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    obj_index = UNDEF_INDEX;

    if (duobj.get_offset() == UNDEF_OFFSET) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "duobj.get_offset() == UNDEF_OFFSET");
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_OFFSET);
    } else if ((err = _t_2d_ary.acquire_t(&duobj, obj_index, &pdu)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_t_2d_ary.acquire_t() returns 0x%x", err);
    } else if (UNDEF_INDEX == obj_index || NULL == pdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "UNDEF_INDEX == obj_index || NULL == pdu");
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_OBJ_ACQUIRE);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "duobj: key_id=%lu,obj_index=0x%lx",
                        duobj.get_id(), obj_index);
        DF_WRITE_LOG_US(log_level,
                        "obj_index=0x%lx",
                        obj_index);
    }

    return err;
};
//以obj为基础生成一个新的T对象
//name string已经存在
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_sf_btree_t<D, K, ROW_SIZE>::_add_obj(
    const DU & duobj,
    const K & keyt,
    uint64_t & obj_index)
{
    char * key_store_buf = NULL;
    const uint64_t key_store_size = keyt.get_store_size();
    uint64_t key_store_pos = 0;
    uint64_t key_offset = UNDEF_OFFSET;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    obj_index = UNDEF_INDEX;

    //add name
    if ((err = _key_buf.buf_ary_add_key(key_store_size, key_store_buf, key_offset)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_key_buf.buf_ary_add_key() returns 0x%x", err);
    } else if((err = keyt.store(key_store_buf, key_store_size, key_store_pos)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt.store() returns 0x%x", err);
    } else {
        DU duobj2(duobj);

        duobj2.set_offset(key_offset);
        if ((err = _add_obj(duobj2, obj_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_add_obj(duobj2, obj_index) returns 0x%x", err);
        }
    }

    return err;
};

//template<typename D, typename K, uint32_t ROW_SIZE>
//int _dfs_sf_btree_t<D, K, ROW_SIZE>::_del_obj(const uint64_t obj_index)
//{
//    PDU pdu = NULL;
//    int log_level = DF_UL_LOG_NONE;
//    int err = 0;
//
//    if ((err = this->_get_exist_obj_for_mutate(obj_index, pdu)) != 0 || NULL == pdu)
//    {
//        log_level = DF_UL_LOG_FATAL;
//        DF_WRITE_LOG_US(log_level, "_get_exist_obj_for_mutate() returns 0x%x", err);
//        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
//    }
//    else if ((err = pdu->gc_obj(this->bt_get_extra_info())) != 0)
//    {
//        log_level = DF_UL_LOG_FATAL;
//        DF_WRITE_LOG_US(log_level, "pcdu->gc_obj(this->bt_get_extra_info()) returns 0x%x", err);
//    }
//
//    return err;
//};


//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_get_obj_ref_counter(
    const uint64_t obj_index,
    uint64_t & ref_counter) const
{
    int err = 0;

    ref_counter = UNDEF_REF_COUNTER;
    if ((err = _t_2d_ary.get_t_ref_counter(obj_index, ref_counter)) != 0) {
        DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
                        "_t_2d_ary.get_t_ref_counter() returns 0x%x, obj_index=0x%lx, ref_counter=%ld",
                        err, obj_index, ref_counter);
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_inc_obj_ref_counter(
    const uint64_t obj_index,
    uint64_t & ref_counter)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //_bt_inc_key_ref_inc_counter();
    ref_counter = UNDEF_REF_COUNTER;
    if ((err = _t_2d_ary.inc_t_ref_counter(obj_index, ref_counter)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_t_2d_ary.inc_t_ref_counter() returns 0x%x", err);
        //err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
    } else if (0 == ref_counter || UNDEF_REF_COUNTER == ref_counter) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "ref_counter=%ld", ref_counter);
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj_index=0x%lx, ref_counter=%ld",
                        obj_index, ref_counter);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::_dec_obj_ref_counter(
    const uint64_t obj_index,
    uint64_t & ref_counter)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //DFS_BT_DEBUG_TRAP;
    //_bt_inc_key_ref_dec_counter();
    ref_counter = UNDEF_REF_COUNTER;
    if ((err = _t_2d_ary.dec_t_ref_counter(obj_index, ref_counter)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_t_2d_ary.dec_t_ref_counter() returns 0x%x", err);
        //err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
    } else if (UNDEF_REF_COUNTER == ref_counter) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "ref_counter=%ld", ref_counter);
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj_index=0x%lx, ref_counter=%ld",
                        obj_index, ref_counter);
    }

    return err;
};

////  return: 0 for success, other values for error
//virtual int vir_get_total_obj_ref_counter_in_ary(uint64_t & total_ref_counter) const
//{
//    int err = 0;

//    if ((err = _t_2d_ary.get_total_ref_counter(total_ref_counter)) != 0)
//    {
//        DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
//                "_t_2d_ary.get_total_ref_counter() returns 0x%x, total_ref_counter=%ld",
//                err, total_ref_counter);
//    }
//    else
//    {
//        --total_ref_counter;    //deduct the ref_counter of [0]
//    }

//    return err;
//};
//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_get_total_in_use_obj_num_in_ary(
    uint64_t & total_in_use_obj_num) const
{
    int err = 0;

    if ((err = _t_2d_ary.get_in_use_num(total_in_use_obj_num)) != 0) {
        DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
                        "_t_2d_ary.get_in_use_num() returns 0x%x, total_in_use_obj_num=%ld",
                        err, total_in_use_obj_num);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_obj_store_size(
    const uint64_t obj_index,
    uint64_t & store_size,
    dfs_btree_store_info_t & store_info) const
{
    PCDU pcdu = NULL;
    K keyt;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //DFS_BT_DEBUG_TRAP;
    store_size = 0;
    if(store_info.get_store_t_type() == dfs_btree_store_info_t::STORE_T_INDEX) {
        store_size = sizeof(uint64_t);
    } else if ((err = _sf_get_key_by_index(obj_index, keyt, pcdu)) != 0 || NULL == pcdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_index() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else {
        store_size = keyt.get_store_size()+pcdu->get_store_size();

    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "obj_index=0x%lx", obj_index);
    }

    return 0;
};

//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_store_obj(
    const uint64_t obj_index,
    dfs_btree_store_info_t & store_info) const
{
    PCDU pcdu = NULL;
    K keyt;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;


    //DFS_BT_DEBUG_TRAP;
    if(store_info.get_store_t_type() != dfs_btree_store_info_t::STORE_T_VALUE_KEEP_T_INDEX) {
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_TYPE);
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_TYPE);

    } else if ((err = this->_sf_get_key_by_index(obj_index, keyt, pcdu)) != 0 || NULL == pcdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_index() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    }
    //store key_len & key;
    else if ((err = keyt.store(
                        store_info.get_buf(),
                        store_info.get_buf_size(),
                        store_info.get_data_pos())) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt.store() returns 0x%x", err);
    }
    //store:
    else if ((err = pcdu->store(
                        store_info.get_buf(),
                        store_info.get_buf_size(),
                        store_info.get_data_pos())) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "pcdu->store() returns 0x%x", err);
    } else if ((err = store_info.keep_obj_index(obj_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "store_info.keep_obj_index() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj_index=0x%lx, store_info.buf_size/data_pos=%ld/%ld",
                        obj_index, store_info.get_buf_size(), store_info.get_data_pos());
        BT_DF_WRITE_LOG_BYTES(log_level, "keyt", keyt.get_str(), keyt.get_len());
    }

    return err;
};

//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_load_obj(
    uint64_t & obj_index,
    dfs_btree_load_info_t & load_info)
{
    DU duobj;
    K keyt;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //DFS_BT_DEBUG_TRAP;
    obj_index = UNDEF_INDEX;
    if(load_info.get_store_t_type() != dfs_btree_load_info_t::STORE_T_VALUE_KEEP_T_INDEX) {
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_TYPE);
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_TYPE);
    }
    //load key_len & key
    else if ((err = keyt.load(
                        load_info.get_buf(),
                        load_info.get_data_len(),
                        load_info.get_data_pos())) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "load_info.load_data_len() returns 0x%x", err);
    }
    //load:
    else if ((err = duobj.load(
                        load_info.get_buf(),
                        load_info.get_data_len(),
                        load_info.get_data_pos())) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "duobj.load() returns 0x%x", err);
    } else if (duobj.get_id() == UNDEF_ID) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "duobj.get_id() is UNDEF_ID after load");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_UNDEF_NAME_ID);
    } else {
        duobj.set_offset(UNDEF_OFFSET);
        if ((err = this->_add_obj(duobj, keyt, obj_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_add_obj() returns 0x%x", err);
        } else if (UNDEF_INDEX == obj_index) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "obj_index is UNDEF_INDEX after _add_obj()");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "load_info.data_len/data_pos=%ld/%ld",
                        load_info.get_data_len(), load_info.get_data_pos());
        BT_DF_WRITE_LOG_BYTES(log_level, "keyt", keyt.get_str(), keyt.get_len());
    }

    return err;
};


////  return: 0 for success, other values for error
//virtual int vir_get_obj_ref_counter(const uint64_t obj_index, uint64_t & ref_counter) const
//{
//    int log_level = DF_UL_LOG_NONE;
//    int err = 0;
//
//    if ((err = _get_obj_ref_counter(obj_index, ref_counter)) != 0)
//    {
//        log_level = DF_UL_LOG_FATAL;
//        DF_WRITE_LOG_US(log_level, "_get_obj_ref_counter() returns 0x%x", err);
//    }
//    else if (UNDEF_REF_COUNTER == ref_counter)
//    {
//        log_level = DF_UL_LOG_FATAL;
//        DF_WRITE_LOG_US(log_level, "ref_counter is UNDEF_REF_COUNTER");
//        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
//    }

//    if (DF_UL_LOG_NONE != log_level)
//    {
//        DF_WRITE_LOG_US(log_level, "obj_index=0x%lx, ref_counter=%ld", obj_index, ref_counter);
//    }

//    return err;
//};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_inc_obj_ref_counter(
    const uint64_t obj_index)
{
    uint64_t ref_counter = UNDEF_REF_COUNTER;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = _inc_obj_ref_counter(obj_index, ref_counter)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_inc_obj_ref_counter() returns 0x%x", err);
    } else if (0 == ref_counter || UNDEF_REF_COUNTER == ref_counter) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "ref_counter is UNDEF_REF_COUNTER");
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "obj_index=0x%lx, ref_counter=%ld", obj_index, ref_counter);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_dec_obj_ref_counter(
    const uint64_t obj_index)
{
    uint64_t ref_counter = UNDEF_REF_COUNTER;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //DFS_BT_DEBUG_TRAP;
    if ((err = _dec_obj_ref_counter(obj_index, ref_counter)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_dec_obj_ref_counter() returns 0x%x", err);
    } else if (UNDEF_REF_COUNTER == ref_counter) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "ref_counter is UNDEF_REF_COUNTER");
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "obj_index=0x%lx, ref_counter=%ld", obj_index, ref_counter);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_inc_obj_ary_ref_counter(
    const uint64_t * obj_index_ary,
    const int32_t obj_num)
{
    uint64_t ref_counter = UNDEF_REF_COUNTER;
    int32_t pos = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL == obj_index_ary) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "null obj_index_ary parameter");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else {
        for (pos = 0; pos < obj_num; ++pos) {
            ref_counter = UNDEF_REF_COUNTER;
            if ((err = _inc_obj_ref_counter(obj_index_ary[pos], ref_counter)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_inc_obj_ref_counter() returns 0x%x", err);
                DF_WRITE_LOG_US(log_level, "obj_index_ary[pos]=0x%lx", obj_index_ary[pos]);
                break;
            } else if (0 == ref_counter || UNDEF_REF_COUNTER == ref_counter) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "ref_counter=%ld", ref_counter);
                DF_WRITE_LOG_US(log_level, "obj_index_ary[pos]=0x%lx", obj_index_ary[pos]);
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
                break;
            }
        }
    }

    //不能在此处报告obj_index_ary[pos](因为obj_index_ary可能为NULL)
    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj_num=%d, pos=%d, ref_counter=%ld",
                        obj_num, pos, ref_counter);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_dec_obj_ary_ref_counter(
    const uint64_t * obj_index_ary,
    const int32_t obj_num)
{
    uint64_t ref_counter = UNDEF_REF_COUNTER;
    int32_t pos = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    DFS_BT_DEBUG_TRAP;
    if (NULL == obj_index_ary) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "null obj_index_ary parameter");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else {
        for (pos = 0; pos < obj_num; ++pos) {
            ref_counter = UNDEF_REF_COUNTER;
            if ((err = _dec_obj_ref_counter(obj_index_ary[pos], ref_counter)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_dec_obj_ref_counter() returns 0x%x", err);
                DF_WRITE_LOG_US(log_level, "obj_index_ary[pos]=0x%lx", obj_index_ary[pos]);
                break;
            } else if (UNDEF_REF_COUNTER == ref_counter) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "ref_counter=%ld", ref_counter);
                DF_WRITE_LOG_US(log_level, "obj_index_ary[pos]=0x%lx", obj_index_ary[pos]);
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
                break;
            }
        }
    }

    //不能在此处报告obj_index_ary[pos](因为obj_index_ary可能为NULL)
    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj_num=%d, pos=%d, ref_counter=%ld",
                        obj_num, pos, ref_counter);
    }

    return err;
};


//
//return: 0 for success, other values for error
//cmp: -1, 0, 1 for less than, equal, great than
//  return: 0 for success, other values for error
//  cmp: -1, 0, 1 for less than, equal, great than
//virtual int vir_compare_index_index(const uint64_t obj1_index, const uint64_t obj2_index, int & cmp) const
//{
//    int err = 0;

//    if ((err = _sf_compare_index_index(obj1_index, obj2_index, cmp)) != 0)
//    {
//        DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
//                "_sf_compare_index_index() returns 0x%x, obj1_index=0x%lx, obj2_index=0x%lx",
//                err, obj1_index, obj2_index);
//    }

//    return err;
//};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_compare_index_obj(
    const dfs_bt_root_t & bt_root,
    const uint64_t obj1_index,
    const void * pobj2,
    int & cmp) const
{
    PCDU pcdu2 = (PCDU)pobj2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 1;
    if (NULL == pcdu2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pcdu2");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = _sf_compare_index_obj(obj1_index, *pcdu2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_compare_index_offset() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj1_index=0x%lx, pobj2=0x%p, cmp=%d",
                        obj1_index, pobj2, cmp);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_compare_index_key(
    const dfs_bt_root_t & bt_root,
    const uint64_t obj1_index,
    const void * pkey2,
    int & cmp) const
{
    const K * pnamet2 = (const K *)pkey2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 1;
    if (NULL == pnamet2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pnamet2");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_NULL_POINTER);
    } else if ((err = _sf_compare_index_key(obj1_index, *pnamet2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_compare_index_key() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj1_index=0x%lx, pkey2=0x%p, cmp=%d",
                        obj1_index, pkey2, cmp);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_search_ary_by_index(
    const dfs_bt_root_t & bt_root,
    const uint64_t * obj_index_ary,
    const int32_t obj_num,
    const uint64_t obj2_index,
    int & ins_pos,
    uint64_t & obj_index) const
{
    //DFS_BT_DEBUG_TRAP;

    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    int cmp = 1;
    int32_t pos1 = 0;
    int32_t pos2 = 0;
    int32_t mid = 0;

    ins_pos = 0;
    obj_index = UNDEF_INDEX;
    if (UNDEF_INDEX == obj2_index) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "UNDEF_INDEX == obj2_index");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
    } else if (NULL == obj_index_ary) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == obj_index_ary");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else {
        pos1 = 0;
        pos2 = obj_num-1;
        while (pos1 <= pos2) {
            mid = (pos1+pos2)/2;
            if (UNDEF_INDEX == obj_index_ary[mid]) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "UNDEF_INDEX == obj_index_ary[mid]");
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_SUBKEY_INDEX);
                break;
            } else if ((err = _sf_compare_index_index(obj_index_ary[mid], obj2_index, cmp)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_sf_compare_index_index() returns 0x%x", err);
                break;
            } else if (0 == cmp) {
                //找到了
                pos1 = mid;
                break;
            } else if (cmp > 0) {
                pos2 = mid-1;
            } else { //if (cmp < 0)
                pos1 = mid+1;
            }
        }
        ins_pos = pos1;
        if (0 == err && 0 == cmp) {
            if (pos1 >= obj_num || pos1 < 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "cmp == 0 && (pos1 >= obj_num || pos1 < 0)");
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_SUBKEY_OVERFLOW);
            } else {
                obj_index = obj_index_ary[pos1];
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj2_index=0x%lx, ins_pos=%d, obj_index=0x%lx",
                        obj2_index, ins_pos, obj_index);
        DF_WRITE_LOG_US(log_level,
                        "pos1=%d, pos2=%d, mid=%d",
                        pos1, pos2, mid);
        if (NULL != obj_index_ary && obj_num > 0) {
            BT_DF_WRITE_LOG_INT64S(log_level, "obj_index_ary", obj_index_ary, obj_num);
        }
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_search_ary_by_obj(
    const dfs_bt_root_t & bt_root,
    const uint64_t * obj_index_ary,
    const int32_t obj_num,
    const void * pobj2,
    int & ins_pos,
    uint64_t & obj_index) const
{
    //DFS_BT_DEBUG_TRAP;

    const PCDU pcdu2 = (PCDU)pobj2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    int cmp = 1;
    int32_t pos1 = 0;
    int32_t pos2 = 0;
    int32_t mid = 0;

    ins_pos = 0;
    obj_index = UNDEF_INDEX;
    if (NULL == pcdu2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pnamet2");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_NULL_POINTER);
    } else if (NULL == obj_index_ary) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == obj_index_ary");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else {
        pos1 = 0;
        pos2 = obj_num-1;
        while (pos1 <= pos2) {
            mid = (pos1+pos2)/2;
            if (UNDEF_INDEX == obj_index_ary[mid]) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "UNDEF_INDEX == obj_index_ary[mid]");
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_SUBKEY_INDEX);
                break;
            } else if ((err = _sf_compare_index_obj(obj_index_ary[mid], *pcdu2, cmp)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level,
                                "_sf_compare_index_obj(obj_index_ary[mid]...) returns 0x%x", err);
                break;
            } else if (0 == cmp) {
                //找到了
                pos1 = mid;
                break;
            } else if (cmp > 0) {
                pos2 = mid-1;
            } else { //if (cmp < 0)
                pos1 = mid+1;
            }
        }
        ins_pos = pos1;
        if (0 == err && 0 == cmp) {
            if (pos1 >= obj_num || pos1 < 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "cmp == 0 && (pos1 >= obj_num || pos1 < 0)");
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_SUBKEY_OVERFLOW);
            } else {
                obj_index = obj_index_ary[pos1];
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "pobj2=0x%p, ins_pos=%d, obj_index=0x%lx",
                        pobj2, ins_pos, obj_index);
        DF_WRITE_LOG_US(log_level,
                        "pos1=%d, pos2=%d, mid=%d",
                        pos1, pos2, mid);
        if (NULL != obj_index_ary && obj_num > 0) {
            BT_DF_WRITE_LOG_INT64S(log_level, "obj_index_ary", obj_index_ary, obj_num);
        }
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_search_ary_by_key(
    const dfs_bt_root_t & bt_root,
    const uint64_t * obj_index_ary,
    const int32_t obj_num,
    const void * pkey2,
    int & ins_pos,
    uint64_t & obj_index) const
{
    //DFS_BT_DEBUG_TRAP;

    const K * pckey2 = (const K *)pkey2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    int cmp = 1;
    int32_t pos1 = 0;
    int32_t pos2 = 0;
    int32_t mid = 0;

    ins_pos = 0;
    obj_index = UNDEF_INDEX;
    if (NULL == pckey2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pnamet2");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_NULL_POINTER);
    } else if (NULL == obj_index_ary) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == obj_index_ary");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else {
        pos1 = 0;
        pos2 = obj_num-1;
        while (pos1 <= pos2) {
            mid = (pos1+pos2)/2;
            if (UNDEF_INDEX == obj_index_ary[mid]) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "UNDEF_INDEX == obj_index_ary[mid]");
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_SUBKEY_INDEX);
                break;
            } else if ((err = _sf_compare_index_key(obj_index_ary[mid], *pckey2, cmp)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level,
                                "_sf_compare_index_key(obj_index_ary[mid]...) returns 0x%x", err);
                break;
            } else if (0 == cmp) {
                //找到了
                pos1 = mid;
                break;
            } else if (cmp > 0) {
                pos2 = mid-1;
            } else { //if (cmp < 0)
                pos1 = mid+1;
            }
        }
        ins_pos = pos1;
        if (0 == err && 0 == cmp) {
            if (pos1 >= obj_num || pos1 < 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "cmp == 0 && (pos1 >= obj_num || pos1 < 0)");
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_SUBKEY_OVERFLOW);
            } else {
                obj_index = obj_index_ary[pos1];
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "pkey2=0x%p, ins_pos=%d, obj_index=0x%lx",
                        pkey2, ins_pos, obj_index);
        DF_WRITE_LOG_US(log_level,
                        "pos1=%d, pos2=%d, mid=%d",
                        pos1, pos2, mid);
        if (NULL != obj_index_ary && obj_num > 0) {
            BT_DF_WRITE_LOG_INT64S(log_level, "obj_index_ary", obj_index_ary, obj_num);
        }
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::vir_verify_index_ary(
    const dfs_bt_root_t & bt_root,
    const uint64_t * obj_index_ary,
    const int32_t obj_num,
    bool & is_inc_order) const
{
    PCDU pcdu1 = NULL;
    PCDU pcdu2 = NULL;
    int32_t j = 0;
    int32_t cmp = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    is_inc_order = false;

    if (NULL == obj_index_ary) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == obj_index_ary");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if (obj_num <= 1) {
        is_inc_order = true;
    } else {
        j = 0;
        if ((err = _get_exist_obj(obj_index_ary[j], pcdu2)) != 0 || NULL == pcdu2) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_exist_obj(obj_index_ary[j]...) returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            is_inc_order = true;
            ++j;
            while (j < obj_num) {
                pcdu1 = pcdu2;
                if ((err = _get_exist_obj(obj_index_ary[j], pcdu2)) != 0 || NULL == pcdu2) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_get_exist_obj(obj_index_ary[j]...) returns 0x%x", err);
                    err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
                    is_inc_order = false;
                    break;
                } else if ((err = _sf_compare_obj_obj(*pcdu1, *pcdu2, cmp)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_sf_compare_obj_obj() returns 0x%x", err);
                    is_inc_order = false;
                    break;
                } else if (cmp >= 0) {
                    is_inc_order = false;
                    break;
                }
                ++j;
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "j=%d, cmp=%d, is_inc_order=%d",
                        j, cmp, is_inc_order);
        if (NULL != obj_index_ary && obj_num > 0) {
            BT_DF_WRITE_LOG_INT64S(log_level, "obj_index_ary", obj_index_ary, obj_num);
        }
    }

    return err;
};


template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_sf_btree_t<D, K, ROW_SIZE>::sf_get_in_use_num_in_2d_ary(
    uint64_t & in_use_obj_num,
    uint64_t & in_use_leaf_node_num,
    uint64_t & in_use_mid_node_num) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    in_use_obj_num = 0;
    in_use_leaf_node_num = 0;
    in_use_mid_node_num = 0;

    if ((err = _t_2d_ary.get_in_use_num(in_use_obj_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_t_2d_ary.get_in_use_num() returns 0x%x", err);
    } else if ((err = this->bt_get_in_use_node_num_in_2d_ary(
                          in_use_leaf_node_num,
                          in_use_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "bt_get_in_use_node_num_in_2d_ary() returns 0x%x", err);
    }

    return err;
};




//_dfs_idsf_btree_t : _id_btree + _sf_btree
template<typename D, typename K, uint32_t ROW_SIZE>
class _dfs_idsf_btree_t :
    virtual public dfs_bt_const_t,
private dfs_bt2d_ary_t<D, ROW_SIZE>,
public _dfs_sf_btree_t<D, K, ROW_SIZE>
{
protected:
    enum cconst_protected {

    };
private:
    //typedef T *PT;
    //typedef const T *PCT;
    //typedef T DU;
    typedef D DU;
    typedef DU *PDU;
    typedef const DU *PCDU;
    typedef K *PK;
    typedef const K *PCK;
protected:
    typedef dfs_bt2d_ary_t<DU, ROW_SIZE> t_ary_t;
    typedef void * pvoid;
    typedef _dfs_id_btree_t<DU, ROW_SIZE> id_btree_t;
    typedef _dfs_sf_btree_t<D, K, ROW_SIZE> sf_btree_t;
private:
    dfs_init_t _init_state;
    id_btree_t _id_btree;
public:
    _dfs_idsf_btree_t() :
        sf_btree_t(*((t_ary_t *)this)),
        _id_btree(*((t_ary_t *)this)) {
        //init();
    }
    virtual ~_dfs_idsf_btree_t() {
        _init_state.set_destructed();
    }
    int init(
        const uint32_t id_bt_instance_pos,
        const uint32_t sf_bt_instance_pos,
        dfs_bt_root_t & new_wr_bt_root);
private:
    //Disable operator=() and copy constructor
    const _dfs_idsf_btree_t & operator=(const _dfs_idsf_btree_t & src);
    _dfs_idsf_btree_t(const _dfs_idsf_btree_t & src);
    //const _dfs_idsf_btree_t & operator=(const _dfs_idsf_btree_t & src)
    //{
    //    return *this;
    //};
    //_dfs_idsf_btree_t(const _dfs_idsf_btree_t & src) :
    //            id_btree_t(*((t_ary_t *)this)),
    //            sf_btree_t(*((t_ary_t *)this))
    //{
    //};
protected:
    void _idsf_set_bt_instance_pos(const uint32_t id_bt_pos, const uint32_t sf_bt_pos);
public:
    //增加/减少根节点引用计数
    virtual int vir_inc_root_ref_counter(const dfs_bt_root_t & bt_root) const;
    virtual int vir_dec_root_ref_counter(const dfs_bt_root_t & bt_root) const;
protected:
    inline int _idsf_inc_root_ref_counter(const dfs_bt_root_t & bt_roo) const;
    inline int _idsf_dec_root_ref_counter(const dfs_bt_root_t & bt_roo) const;
    void _idsf_set_cancel_checkpointing(bool is_cancel);
    void _idsf_update_mutation_counter(const bool is_batch_mode);
    void _idsf_update_max_cow_mutation_counter(void) const;
    //
    int _idsf_store_checkpointing(
        const dfs_bt_root_t & bt_root,
        dfs_btree_store_info_t & store_info) const;
    int _idsf_load_checkpointing(
        dfs_bt_root_t & bt_root,
        dfs_btree_load_info_t & load_info);
protected:
    //  return: 0 for success, other values for error
    inline int _idsf_get_key_by_id(
        const dfs_bt_root_t & bt_root,
        const uint64_t key_id,
        K & keyt,
        PCDU & pcdu) const;
    int _idsf_search_id_by_index(
        const dfs_bt_root_t & bt_root,
        const uint64_t i_key_index,
        dfs_btree_drill_t & drill_info,
        uint64_t & o_key_index) const;
    int _idsf_search_id_by_id(
        const dfs_bt_root_t & bt_root,
        const uint64_t key_id,
        dfs_btree_drill_t & drill_info,
        uint64_t & key_index) const;
protected:
    int _idsf_compare_id_id(
        const dfs_bt_root_t & bt_root,
        const uint64_t name1_id,
        const uint64_t name2_id,
        int & cmp) const;
    int _idsf_compare_id_key(
        const dfs_bt_root_t & bt_root,
        const uint64_t name1_id,
        const K & keyt2,
        int & cmp) const;
protected:
    //  功能：加入B树(该项一定不存在)中并设置其T为tobj。
    //  输入：tobj
    //  输出：项的key_index(操作成功)或UNDEF_INDEX
    //  return: 0 for success, other values for error
    int _idsf_insert(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const dfs_btree_drill_t & sf_drill_info,
        const K & keyt,
        const DU & duobj,
        dfs_key_id_index_t & key_id_index);
    //  功能：更新B树(该项一定存在)中并设置其T为tobj。
    //  输入：tobj
    //  输出：项的新的key_index(操作成功)或UNDEF_INDEX
    //  return: 0 for success, other values for error
    int _idsf_update(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const dfs_btree_drill_t & sf_drill_info,
        const dfs_btree_drill_t & id_drill_info,
        const DU & duobj,
        dfs_key_id_index_t & key_id_index);
    //  return: 0 for success, other values for error
    int _idsf_del(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const dfs_btree_drill_t & sf_drill_info,
        const dfs_btree_drill_t & id_drill_info);
protected:
    //  功能：验证整个树的顺序和每个节点分支个数
    //  返回：返回0如果验证通过，返回非0(错误代码)如果验证失败
    int _idsf_verify_tree(const dfs_bt_root_t & bt_root) const;
    //  return: 0 for success, other values for error
    int _idsf_empty_btree_check(
        const dfs_bt_root_t & bt_root,
        const bool is_should_empty) const;
    //  return: 0 for success, other values for error
    int _idsf_sanitary_check(const dfs_bt_root_t & bt_root) const;
protected:
    //  return: 0 for success, other values for error
    //  Clear contents of the whole tree
    int _idsf_acquire_empty_root(
        dfs_bt_root_t & new_bt_root);
    void _idsf_clear_statistic_info(void);
    void _idsf_log_statistic_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname) const;
    void _idsf_log_debug_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname) const;
protected:
    uint64_t _idsf_get_mem_size(void) const;
};


template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_idsf_btree_t<D, K, ROW_SIZE>::init(
    const uint32_t id_bt_instance_pos,
    const uint32_t sf_bt_instance_pos,
    dfs_bt_root_t & new_wr_bt_root)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (_init_state.is_not_init_yet()) {
        if ((err = t_ary_t::init()) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "t_ary_t::init() returns 0x%x", err);
        } else if ((err = sf_btree_t::init(sf_bt_instance_pos, new_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::init() returns 0x%x", err);
        } else if ((err = _id_btree.init(id_bt_instance_pos, new_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_id_btree_t.init() returns 0x%x", err);
        }

        _init_state.set_init_result(err);
    } else {
        err = _init_state.get_init_result();
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
inline void _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_set_bt_instance_pos(
    const uint32_t id_bt_pos,
    const uint32_t sf_bt_pos)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err1 = _id_btree.id_set_bt_instance_pos(id_bt_pos)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_set_bt_instance_pos() returns 0x%x", err);
    }

    if ((err1 = sf_btree_t::_bt_set_bt_instance_pos(sf_bt_pos)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_bt_set_bt_instance_pos() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "id_bt_pos=%d, sf_bt_pos=%d",
                        id_bt_pos, sf_bt_pos);
    }

    return ;
};

//增加/减少根节点引用计数
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::vir_inc_root_ref_counter(
    const dfs_bt_root_t & bt_root) const
{
    return this->_idsf_inc_root_ref_counter(bt_root);
};
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::vir_dec_root_ref_counter(
    const dfs_bt_root_t & bt_root) const
{
    return this->_idsf_dec_root_ref_counter(bt_root);
};


//not necessary in locked state
//_root_node_index might be UNDEF_INDEX
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_inc_root_ref_counter(
    const dfs_bt_root_t & bt_root) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    //即使前一部分出错，后一部分也需要执行(例如清除ckp_state)
    if ((err1 = _id_btree.id_inc_root_ref_counter(bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_inc_root_ref_counter() returns 0x%x", err);
    }
    if ((err1 = sf_btree_t::_bt_inc_root_ref_counter(bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_bt_inc_root_ref_counter() returns 0x%x", err);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_dec_root_ref_counter(
    const dfs_bt_root_t & bt_root) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    //即使前一部分出错，后一部分也需要执行(例如清除ckp_state)
    if ((err1 = _id_btree.id_dec_root_ref_counter(bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_dec_root_ref_counter() returns 0x%x", err);
    }
    if ((err1 = sf_btree_t::_bt_dec_root_ref_counter(bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_bt_dec_root_ref_counter() returns 0x%x", err);
    }

    return err;
};


template<typename D, typename K, uint32_t ROW_SIZE>
void _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_set_cancel_checkpointing(bool is_cancel)
{
    sf_btree_t::_bt_set_cancel_checkpointing(is_cancel);
    _id_btree.id_set_cancel_checkpointing(is_cancel);
    return ;
};

template<typename D, typename K, uint32_t ROW_SIZE>
void _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_update_mutation_counter(const bool is_batch_mode)
{
    sf_btree_t::_bt_update_mutation_counter(is_batch_mode);
    _id_btree.id_update_mutation_counter(is_batch_mode);
    return;
};

template<typename D, typename K, uint32_t ROW_SIZE>
void _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_update_max_cow_mutation_counter(void) const
{
    sf_btree_t::_bt_update_max_cow_mutation_counter();
    _id_btree.id_update_max_cow_mutation_counter();
    return;
};



template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_store_checkpointing(
    const dfs_bt_root_t & bt_root,
    dfs_btree_store_info_t & store_info) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    log_level = this->CKP_LOG_LEVEL;

    store_info.clear_session_nums();

    //sf_btree part:
    store_info.set_store_t_value_keep_t_index(); //store T's value and keep indexes
    DF_WRITE_LOG_US(log_level, "begin _sf_store_checkpointing()...");
    if ((err = sf_btree_t::_sf_store_checkpointing(bt_root, store_info)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_sf_store_checkpointing() returns 0x%x", err);
    }
    //id_btree part:
    else {
        store_info.set_store_t_index(); //store T's indexes only
        DF_WRITE_LOG_US(log_level, "begin id (of sf) _id_store_checkpointing()...");
        if ((err = _id_btree.id_store_checkpointing(bt_root, store_info)) != 0) {
            log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_id_btree.id_store_checkpointing() returns 0x%x", err);
        }
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_load_checkpointing(
    dfs_bt_root_t & bt_root,
    dfs_btree_load_info_t & load_info)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    log_level = this->CKP_LOG_LEVEL;

    //load_info.set_major_tag(dfs_vk_btree_fhead_t::VK_BT_TAG);
    load_info.clear_session_nums();

    //sf_btree part:
    DF_WRITE_LOG_US(log_level, "begin _sf_load_checkpointing()...");
    if ((err = sf_btree_t::_sf_load_checkpointing(bt_root, load_info)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_sf_load_checkpointing() returns 0x%x", err);
    }
    //id_btree part:
    else {
        DF_WRITE_LOG_US(log_level, "begin id (of sf) id_load_checkpointing()...");
        if ((err = _id_btree.id_load_checkpointing(bt_root, load_info)) != 0) {
            log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_id_btree.id_load_checkpointing() returns 0x%x", err);
        }
    }

    return err;
};




//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
inline int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_get_key_by_id(
    const dfs_bt_root_t & bt_root,
    const uint64_t key_id,
    K & keyt,
    PCDU & pcdu) const
{
    dfs_btree_drill_t drill_info;
    uint64_t key_index = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    keyt.init();

    if ((err = _id_btree.id_search_by_key(bt_root, &key_id, drill_info, key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_search_by_key() returns 0x%x", err);
    } else if (UNDEF_INDEX == key_index) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level,
                        "_id_btree.id_search_by_key() can't find key_id=%lu",
                        key_id);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_ID);
    } else if ((err = sf_btree_t::_sf_get_key_by_index(key_index, keyt, pcdu)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_sf_get_key_by_index() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_id=%lu, key_index=0x%lx",
                        key_id, key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "keyt", keyt.get_str(), keyt.get_len());
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_search_id_by_index(
    const dfs_bt_root_t & bt_root,
    const uint64_t i_key_index,
    dfs_btree_drill_t & drill_info,
    uint64_t & o_key_index) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    drill_info.init();
    o_key_index = UNDEF_INDEX;
    if ((err = _id_btree.id_search_by_index(bt_root, i_key_index, drill_info, o_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_search_by_index() returns 0x%x", err);
    }

    return err;
};


template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_search_id_by_id(
    const dfs_bt_root_t & bt_root,
    const uint64_t key_id,
    dfs_btree_drill_t & drill_info,
    uint64_t & key_index) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    drill_info.init();
    key_index = UNDEF_INDEX;
    if ((err = _id_btree.id_search_by_key(bt_root, &key_id, drill_info, key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_search_by_key() returns 0x%x", err);
    }

    return err;
};


template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_compare_id_id(
    const dfs_bt_root_t & bt_root,
    const uint64_t name1_id,
    const uint64_t name2_id,
    int & cmp) const
{
    PCDU pcdu1 = NULL;
    PCDU pcdu2 = NULL;
    K keyt1;
    K keyt2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 1;
    if (name1_id == name2_id) {
        cmp = 0;
    } else if ((err = _idsf_get_key_by_id(bt_root, name1_id, keyt1, pcdu1)) != 0 || NULL == pcdu1) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_get_key_by_id() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_ID);
    } else if ((err = _idsf_get_key_by_id(bt_root, name2_id, keyt2, pcdu2)) != 0 || NULL == pcdu2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_get_key_by_id() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_ID);
    } else if ((err = keyt1.compare(keyt2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt1.compare(keyt2) returns 0x%x" , err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "name1_id=%lu, name2_id=%lu",
                        name1_id, name2_id);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_compare_id_key(
    const dfs_bt_root_t & bt_root,
    const uint64_t name1_id,
    const K & keyt2,
    int & cmp) const
{
    PCDU pcdu1 = NULL;
    K keyt1;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 1;
    if ((err = _idsf_get_key_by_id(bt_root, name1_id, keyt1, pcdu1)) != 0 || NULL == pcdu1) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_get_key_by_id() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_ID);
    } else if ((err = keyt1.compare(keyt2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "keyt1.compare(keyt2) returns 0x%x" , err);
    }


    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_id=%lu, namet2_len=%ld",
                        name1_id, keyt2.get_len());
    }

    return err;
};


//  功能：加入B树(该项一定不存在)中并设置其T为tobj。
//  输入：tobj
//  输出：项的key_index(操作成功)或UNDEF_INDEX
//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_insert(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const dfs_btree_drill_t & sf_drill_info,
    const K & keyt,
    const DU & duobj,
    dfs_key_id_index_t & key_id_index)
{
    //DU old_duobj;
    //DU new_duobj;
    uint64_t key_id = UNDEF_ID;
    uint64_t key_index = UNDEF_INDEX;
    bool is_old_exist = false;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //old_duobj.init();

    //以tobj为基础生成一个新的T对象
    if ((err = sf_btree_t::_add_obj(duobj, keyt, key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_add_obj() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_UNDEF_NAME_INDEX);
    } else if (UNDEF_INDEX == key_index) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "key_index is UNDEF_INDEX after _add_obj()");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
    }
    //加入id_btree and obtain key_id
    else if ((err = _id_btree.id_insert(
                        old_bt_root,
                        new_bt_root,
                        key_index,
                        NULL, //&old_duobj,
                        is_old_exist,
                        key_id)) != 0
             || UNDEF_ID == key_id) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_insert() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_ID);
    }
    //加入sf_btree
    else if ((err = sf_btree_t::_insert_to_btree(
                        old_bt_root,
                        new_bt_root,
                        key_index,
                        sf_drill_info)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_insert_to_btree() returns 0x%x", err);
    } else {
        key_id_index.key_id = key_id;
        key_id_index.key_index = key_index;
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_len=%ld",
                        keyt.get_len());
    }

    return err;
};

//  功能：更新B树(该项一定存在)中并设置其T为tobj。
//  输入：tobj
//  输出：项的key_index(操作成功)或UNDEF_INDEX
//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_update(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const dfs_btree_drill_t & sf_drill_info,
    const dfs_btree_drill_t & id_drill_info,
    const DU & duobj,
    dfs_key_id_index_t & key_id_index)
{
    uint64_t new_key_index = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    key_id_index.init();
    if ((key_id_index.key_id = duobj.get_id()) == UNDEF_ID) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "duobj.get_id() == UNDEF_ID");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_ID);
    } else if ((err = sf_btree_t::_add_obj(duobj, new_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_add_obj() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_UNDEF_NAME_INDEX);
    } else if (UNDEF_INDEX == new_key_index) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "new_key_index is UNDEF_INDEX after _add_obj()");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
    } else {
        //更新id_btree
        if ((err = _id_btree.id_update_of_btree(
                       old_bt_root,
                       new_bt_root,
                       new_key_index,
                       id_drill_info)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_id_btree.id_update_of_btree() returns 0x%x", err);
        }
        //更新sf_btree
        else if ((err = sf_btree_t::_update_of_btree(
                            old_bt_root,
                            new_bt_root,
                            new_key_index,
                            sf_drill_info)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_update_of_btree() returns 0x%x", err);
        } else {
            key_id_index.key_index = new_key_index;
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_id_index.key_id=%lu, key_id_index.key_index0x%lx",
                        key_id_index.key_id, key_id_index.key_index);
    }

    return err;
};


template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_del(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const dfs_btree_drill_t & sf_drill_info,
    const dfs_btree_drill_t & id_drill_info)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //copy the sf part
    //  return: 0 for success, other values for error
    if ((err = _id_btree.id_del_from_btree(old_bt_root, new_bt_root, id_drill_info)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level,
                        "_id_btree.id_del_from_btree() returns 0x%x, drill_ptr=%d",
                        err, id_drill_info.get_drill_ptr());
    } else if ((err = sf_btree_t::_del_from_btree(old_bt_root, new_bt_root, sf_drill_info)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level,
                        "sf_btree _del_from_btree() returns 0x%x, drill_ptr=%d",
                        err, sf_drill_info.get_drill_ptr());
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "sf_drill_info.drill_ptr=%d",
                        sf_drill_info.get_drill_ptr());
        DF_WRITE_LOG_US(log_level,
                        "err=0x%x, id_drill_info.drill_ptr=%d",
                        err, id_drill_info.get_drill_ptr());
    }

    return err;
};


//  功能：验证整个树的顺序和每个节点分支个数
//  返回：返回0如果验证通过，返回非0(错误代码)如果验证失败
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_verify_tree(
    const dfs_bt_root_t & bt_root) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = _id_btree.bt_verify_tree(bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.bt_verify_tree() returns 0x%x", err);
    } else if ((err = sf_btree_t::bt_verify_tree(bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::bt_verify_tree() returns 0x%x", err);
    }

    return err;
};

//
//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_empty_btree_check(
    const dfs_bt_root_t & bt_root,
    const bool is_should_empty) const
{
    uint64_t id_key_num = 0;
    uint64_t id_leaf_node_num = 0;
    uint64_t id_mid_node_num = 0;
    uint64_t sf_key_num = 0;
    uint64_t sf_leaf_node_num = 0;
    uint64_t sf_mid_node_num = 0;
    uint64_t id_in_use_key_num = 0;
    uint64_t id_in_use_leaf_node_num = 0;
    uint64_t id_in_use_mid_node_num = 0;
    uint64_t sf_in_use_key_num = 0;
    uint64_t sf_in_use_leaf_node_num = 0;
    uint64_t sf_in_use_mid_node_num = 0;
    uint64_t buf_ary_total_ref_counter = 0;
    uint64_t buf_ary_total_key_num = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;


    if ((err = sf_btree_t::bt_get_total_num(
                   bt_root,
                   sf_key_num,
                   sf_leaf_node_num,
                   sf_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::bt_get_total_num() returns: 0x%x", err);
    } else if ((err = _id_btree.bt_get_total_num(
                          bt_root,
                          id_key_num,
                          id_leaf_node_num,
                          id_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_get_total_num() returns: 0x%x", err);
    } else if ((err = sf_btree_t::sf_get_in_use_num_in_2d_ary(
                          sf_in_use_key_num,
                          sf_in_use_leaf_node_num,
                          sf_in_use_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_get_in_use_num_in_2d_ary() returns: 0x%x", err);
    } else if ((err = _id_btree.id_get_in_use_num_in_2d_ary(
                          id_in_use_key_num,
                          id_in_use_leaf_node_num,
                          id_in_use_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_get_in_use_num_in_2d_ary() returns: 0x%x", err);
    } else if ((err = sf_btree_t::_buf_ary_get_total_nums(
                          buf_ary_total_ref_counter,
                          buf_ary_total_key_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "buf_ary_get_total_nums() returns: 0x%x", err);
    } else if (sf_key_num != id_key_num) {
        DF_WRITE_LOG_US(log_level, "sf_key_num != id_key_num");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
    } else if (is_should_empty) {
        if (0 != sf_key_num || 1 != sf_leaf_node_num || 0 != sf_mid_node_num) {
            //empty btree must have a leaf root
            DF_WRITE_LOG_US(log_level, "none empty sf_btree");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NONE_EMPTY_BTREE);
        }
        if (0 != id_key_num || 1 != id_leaf_node_num || 0 != id_mid_node_num) {
            //empty btree must have a leaf root
            DF_WRITE_LOG_US(log_level, "none empty id_btree");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NONE_EMPTY_BTREE);
        }
        if (1 != sf_in_use_key_num || 2 != sf_in_use_leaf_node_num || 1 != sf_in_use_mid_node_num) {
            //[0] is reserved
            DF_WRITE_LOG_US(log_level, "none empty sf inuse 2d_ary");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NONE_EMPTY_BTREE);
        }
        if (1 != id_in_use_key_num || 2 != id_in_use_leaf_node_num || 1 != id_in_use_mid_node_num) {
            //[0] is reserved
            DF_WRITE_LOG_US(log_level, "none empty id in use 2d_ary");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NONE_EMPTY_BTREE);
        }
        if (0 != buf_ary_total_ref_counter || 0 != buf_ary_total_key_num) {
            //[0] is reserved
            DF_WRITE_LOG_US(log_level, "none empty buf_ary");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_NONE_EMPTY_BTREE);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        if (0 != sf_key_num || 1 != sf_leaf_node_num || 0 != sf_mid_node_num)
            DF_WRITE_LOG_US(log_level,
                            "sf_key_num=%ld, sf_leaf_node_num=%ld, sf_mid_node_num=%ld",
                            sf_key_num, sf_leaf_node_num, sf_mid_node_num);
        DF_WRITE_LOG_US(log_level,
                        "id_key_num=%ld, id_leaf_node_num=%ld, id_mid_node_num=%ld",
                        id_key_num, id_leaf_node_num, id_mid_node_num);

        DF_WRITE_LOG_US(log_level,
                        "sf_in_use_key_num=%ld, sf_in_use_leaf_node_num=%ld, sf_in_use_mid_node_num=%ld",
                        sf_in_use_key_num, sf_in_use_leaf_node_num, sf_in_use_mid_node_num);
        DF_WRITE_LOG_US(log_level,
                        "id_in_use_key_num=%ld, id_in_use_leaf_node_num=%ld, id_in_use_mid_node_num=%ld",
                        id_in_use_key_num, id_in_use_leaf_node_num, id_in_use_mid_node_num);

        DF_WRITE_LOG_US(log_level,
                        "buf_ary_total_ref_counter=%ld, buf_ary_total_key_num=%ld",
                        buf_ary_total_ref_counter, buf_ary_total_key_num);
    }

    return err;
};
//  return: 0 for success, other values for error
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_sanitary_check(
    const dfs_bt_root_t & bt_root) const
{
    uint64_t sf_key_num = 0;
    uint64_t id_key_num = 0;
    uint64_t sf_leaf_node_num = 0;
    uint64_t id_leaf_node_num = 0;
    uint64_t sf_mid_node_num = 0;
    uint64_t id_mid_node_num = 0;
    uint64_t in_use_sf_key_num = 0;
    uint64_t in_use_id_key_num = 0;
    uint64_t buf_ary_total_ref_counter = 0;
    uint64_t buf_ary_total_key_num = 0;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = sf_btree_t::_bt_sanitary_check(bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_bt_sanitary_check() returns: 0x%x", err);
    } else if ((err = _id_btree.id_sanitary_check(bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_sanitary_check() returns: 0x%x", err);
    } else if ((err = sf_btree_t::bt_get_total_num(
                          bt_root,
                          sf_key_num,
                          sf_leaf_node_num,
                          sf_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "bt_get_total_num() returns: 0x%x", err);
    } else if ((err = _id_btree.bt_get_total_num(
                          bt_root,
                          id_key_num,
                          id_leaf_node_num,
                          id_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_get_total_num() returns: 0x%x", err);
    } else if ((err = sf_btree_t::sf_get_in_use_num_in_2d_ary(
                          in_use_sf_key_num,
                          sf_leaf_node_num,
                          sf_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_get_in_use_num_in_2d_ary() returns: 0x%x", err);
    } else if ((err = _id_btree.id_get_in_use_num_in_2d_ary(
                          in_use_id_key_num,
                          id_leaf_node_num,
                          id_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_get_in_use_num_in_2d_ary() returns: 0x%x", err);
    } else if ((err = sf_btree_t::_buf_ary_get_total_nums(
                          buf_ary_total_ref_counter,
                          buf_ary_total_key_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "buf_ary_get_total_nums() returns: 0x%x", err);
    } else {
        if (sf_key_num != id_key_num) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_key_num != id_key_num");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        } else if (in_use_sf_key_num != in_use_id_key_num) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "in_use_sf_key_num != in_use_id_key_num");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        } else if (in_use_sf_key_num != (buf_ary_total_ref_counter+1)) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "in_use_sf_key_num != buf_ary_total_ref_counter");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        } else if ((sf_key_num+1) != in_use_sf_key_num) { //one more NULL_ID obj
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "(sf_key_num+1) != in_use_sf_key_num");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        } else if ((id_key_num+1) != in_use_id_key_num) { //one more NULL_ID obj
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "(id_key_num+1) != in_use_id_key_num");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "sf_key_num=%ld, id_key_num=%ld",
                        sf_key_num, id_key_num);
        DF_WRITE_LOG_US(log_level,
                        "in_use_sf_key_num=%ld, in_use_id_key_num=%ld",
                        in_use_sf_key_num, in_use_id_key_num);
        DF_WRITE_LOG_US(log_level,
                        "buf_ary_total_ref_counter=%ld, buf_ary_total_key_num=%ld",
                        buf_ary_total_ref_counter, buf_ary_total_key_num);
    }


    return err;
};

//  Clear contents of the whole tree
template<typename D, typename K, uint32_t ROW_SIZE>
int _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_acquire_empty_root(
    dfs_bt_root_t & new_bt_root)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err1 = _id_btree.id_acquire_empty_root(new_bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_id_btree.id_acquire_empty_root() returns: 0x%x", err);
    }

    if ((err1 = sf_btree_t::_acquire_empty_root(new_bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf: _acquire_empty_root() returns: 0x%x", err);
    }

    return err;
};

template<typename D, typename K, uint32_t ROW_SIZE>
void _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_clear_statistic_info(void)
{
    _id_btree.id_clear_statistic_info();
    sf_btree_t::_sf_clear_statistic_info();
};

template<typename D, typename K, uint32_t ROW_SIZE>
void _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_log_statistic_info(
    const int org_log_level,
    const char * filename,
    const int lineno,
    const char * funcname) const
{
    _id_btree.id_log_statistic_info(org_log_level, filename, lineno, funcname);
    sf_btree_t::_sf_log_statistic_info(org_log_level, filename, lineno, funcname);
};

template<typename D, typename K, uint32_t ROW_SIZE>
void _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_log_debug_info(
    const int org_log_level,
    const char * filename,
    const int lineno,
    const char * funcname) const
{
    _id_btree.id_log_debug_info(org_log_level, filename, lineno, funcname);
    sf_btree_t::_sf_log_debug_info(org_log_level, filename, lineno, funcname);
};


template<typename D, typename K, uint32_t ROW_SIZE>
uint64_t _dfs_idsf_btree_t<D, K, ROW_SIZE>::_idsf_get_mem_size(void) const
{
    return (sf_btree_t::_sf_get_mem_size() - sf_btree_t::_get_bt2d_ary_mem_size() +
            _id_btree.id_get_mem_size());
};




#endif //__DF_STR_KEY_INCLUDE_H_

