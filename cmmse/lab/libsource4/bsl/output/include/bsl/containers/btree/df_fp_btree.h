////===================================================================
//
// df_fp_btree.h Pyramid / DFS / df-lib
//
//
//
// -------------------------------------------------------------------
//
// Description
//
//    declaration and implementation of dfs_fp_btree_t (btree) template
//
// -------------------------------------------------------------------
//
// Change Log
//
//
////====================================================================


#ifndef __DFS_FP_BTREE_INCLUDE_H_
#define __DFS_FP_BTREE_INCLUDE_H_

//调试信息打印输出开关
//#ifndef DF_BT_PRINT_DEBUG_INFO
//#define DF_BT_PRINT_DEBUG_INFO
//#endif

//#include "ul_def.h"
#include "df_common.h"
#include "df_2d_ary.h"
#include "df_btree.h"
#include "df_var_key.h"
#include "df_name.h"

//Begin: Obsolete interface:
#define dfs_fp_btree_fhead_t dfs_vk_btree_fhead_t
//End: Obsolete interface:

//Using for file path, e.g., /xyz/abc, /uvw/123/456, etc
//
//  每个filepath被分成prefix+base，对于文件而言，prefix是父目录，base是文件名，
//  对目录而言，prefix就是它自己，base是空。两个filepath比较大小时，先比较prefix部分，
//  如果不等，则结束比较，如果相等，则继续比较base部分。即：
//      fp1 = prefix1 + base1;
//      fp2 = prefix2 + base2;
//      if (prefix1 > prefix2) then fp1 > fp2;
//      if (prefix1 < prefix2) then fp1 < fp2;
//      if (prefix1 == prefix2) then fp1 >, ==, < fp2 if base1 >, ==, < base2
//  在进行prefix比较时，认为'/'小于其他任何非0字符，例如：
//      "/xyz/abc/" < /xyz/abcd/
//  在这种顺序关系下，同一目录下的文件总是“小于”同一目录下的子目录。
//


//fp => filepath
class dfs_fp_base_t : public dfs_vk_offset_t
{
private:
    uint64_t _prefix_id;
public:
    dfs_fp_base_t() : dfs_vk_offset_t(UNDEF_OFFSET), _prefix_id(UNDEF_ID) {
    };
    dfs_fp_base_t(const dfs_fp_base_t & src) :
        dfs_vk_offset_t(*((dfs_vk_offset_t *)this)),
        _prefix_id(src.get_prefix_id()) {
        //*this = src;
    };
    ~dfs_fp_base_t() {
    };
    const dfs_fp_base_t & operator=(const dfs_fp_base_t & src) {
        if (&src != this) {
            set_offset(src.get_offset());
            set_prefix_id(src.get_prefix_id());
        }
        return *this;
    };
    void init(const uint64_t offset = UNDEF_OFFSET) { //初始化对象
        dfs_vk_offset_t::set_offset(offset);
        _prefix_id = UNDEF_ID;
    };
public:
    void set_prefix_id(const uint64_t prefix_id) {
        _prefix_id = prefix_id;
    };
    uint64_t get_prefix_id(void) const {
        return _prefix_id;
    };
public:
    //用户数据类型T需要支持以下接口：
    //  功能：获得该实例store的数据长度(字节)，该值不能超过MAX_T_STORE_SIZE
    static uint64_t get_store_size(void) {
        return sizeof(uint64_t);
    };
    //
    //  功能：存储该实例到buf
    //  输入：data_pos为存储数据的起始位置
    //  输出：data_pos为存储T后新的起始位置
    //  返回：for success, other values for error (例如缓冲区剩余空间不足)
    int store(char * buf, const uint64_t buf_size, uint64_t & data_pos) const {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((buf_size-data_pos) < sizeof(uint64_t)) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_POS);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_POS);
        } else if (NULL == buf) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_BUF);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_BUF);
        } else {
            *((uint64_t *)(buf+data_pos)) = get_prefix_id();
            data_pos += sizeof(uint64_t);
        }

        return err;
    };
    //
    //  功能：从buf中装载到该实例
    //  输入：data_pos为装载T前缓冲区的数据位置
    //  输出：data_pos为装载T后缓冲区的数据位置
    //  返回：for success, other values for error(例如缓冲区内数据不足)
    int load(char * buf, const uint64_t data_len, uint64_t & data_pos) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((data_len-data_pos) < sizeof(uint64_t)) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_POS);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_POS);
        } else if (NULL == buf) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_BUF);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_BUF);
        } else {
            set_prefix_id(*((uint64_t *)(buf+data_pos)));
            data_pos += sizeof(uint64_t);
        }

        return err;
    };
};

//二维数组dfs_bt2d_ary_t的数据类型的基类
template<typename T>
class dfs_fp_du_t : public dfs_fp_base_t, public T
{
private:
    typedef T *PT;
    typedef const T *PCT;
public:
    dfs_fp_du_t() : dfs_fp_base_t(), T() {
    };
    const dfs_fp_du_t & operator=(const dfs_fp_du_t & src) {
        if (&src != this) {
            *((dfs_fp_base_t *)this) = (const dfs_fp_base_t &)src;
            *((T *)this) = (const T &)src;
        }
        return *this;
    };
    const dfs_fp_du_t & operator=(const T & tobj) {
        if (&tobj != ((T *)this)) {
            *((T *)this) = tobj;
        }
        return *this;
    };
    dfs_fp_du_t(const dfs_fp_du_t & src) : dfs_fp_base_t((const dfs_fp_base_t &)src), T((const T &)src) {
        //*this = src;
    };
    dfs_fp_du_t(const T & tobj) : dfs_fp_base_t(), T(tobj) {
    };
    ~dfs_fp_du_t() {
    };
    void init(void) { //初始化对象
        dfs_fp_base_t::init();
        T::init();
    };
public:
    void set_offset(const uint64_t offset) {
        dfs_fp_base_t::set_offset(offset);
    };
    uint64_t get_offset(void) const {
        return dfs_fp_base_t::get_offset();
    };
    void set_prefix_id(const uint64_t prefix_id) {
        return dfs_fp_base_t::set_prefix_id(prefix_id);
    };
    uint64_t get_prefix_id(void) const {
        return dfs_fp_base_t::get_prefix_id();
    };
public:
    //用户数据类型T需要支持以下接口：
    //  功能：获得该实例store的数据长度(字节)，该值不能超过MAX_T_STORE_SIZE
    uint64_t get_store_size(void) const {
        return (dfs_fp_base_t::get_store_size()+T::get_store_size());
    };
    //
    //  功能：存储该实例到buf
    //  输入：data_pos为存储数据的起始位置
    //  输出：data_pos为存储T后新的起始位置
    //  返回：for success, other values for error (例如缓冲区剩余空间不足)
    int store(char * buf, const uint64_t buf_size, uint64_t & data_pos) const {
        int err = 0;

        if ((err = dfs_fp_base_t::store(buf, buf_size, data_pos)) != 0) {
            DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
                            "dfs_fp_base_t::store() returns 0x%x, buf_size=%ld, data_pos=%ld",
                            err, buf_size, data_pos);
        } else if ((err = T::store(buf, buf_size, data_pos)) != 0) {
            DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
                            "T::store() returns 0x%x, buf_size=%ld, data_pos=%ld",
                            err, buf_size, data_pos);
        }

        return err;
    };
    //
    //  功能：从buf中装载到该实例
    //  输入：data_pos为装载T前缓冲区的数据位置
    //  输出：data_pos为装载T后缓冲区的数据位置
    //  返回：for success, other values for error(例如缓冲区内数据不足)
    int load(char * buf, const uint64_t data_len, uint64_t & data_pos) {
        int err = 0;

        if ((err = dfs_fp_base_t::load(buf, data_len, data_pos)) != 0) {
            DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
                            "dfs_fp_base_t::load() returns 0x%x, data_len=%ld, data_pos=%ld",
                            err, data_len, data_pos);
        } else if ((err = T::load(buf, data_len, data_pos)) != 0) {
            DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
                            "T::load() returns 0x%x, data_len=%ld, data_pos=%ld",
                            err, data_len, data_pos);
        }

        return err;
    };
public:
    //增加时所做的操作
    inline int action_while_added(void * pgc_info, const uint64_t t_index) {
        dfs_2d_key_buf_t<DEF_KEY_BUF_SIZE> * pkey_buf =
            (dfs_2d_key_buf_t<DEF_KEY_BUF_SIZE> *)pgc_info;
        uint64_t ref_counter = 0;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (NULL == pkey_buf) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pkey_buf");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_SET_GC_INFO);
        } else if ((err = pkey_buf->buf_ary_inc_key_ref_counter(get_offset(), ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_buf->buf_ary_inc_key_ref_counter() returns 0x%x", err);
        } else if (UNDEF_REF_COUNTER == ref_counter) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "ref_counter=%ld", ref_counter);
            err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
        }

        return err;
    };
    //删除前所做的操作
    inline int action_before_gc(void * pgc_info, const uint64_t t_index) {
        dfs_2d_key_buf_t<DEF_KEY_BUF_SIZE> * pkey_buf =
            (dfs_2d_key_buf_t<DEF_KEY_BUF_SIZE> *)pgc_info;
        uint64_t ref_counter = 0;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (NULL == pkey_buf) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pkey_buf");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_SET_GC_INFO);
        } else if ((err = pkey_buf->buf_ary_dec_key_ref_counter(get_offset(), ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pkey_buf->buf_ary_dec_key_ref_counter() returns 0x%x", err);
        } else if (UNDEF_REF_COUNTER == ref_counter) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "ref_counter=%ld", ref_counter);
            err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_REF_COUNTER);
        }

        return err;
    };
};



class dfs_fp_btree_const_t : virtual public dfs_bt_const_t
{
//public:
//    dfs_fp_btree_const_t()
//    {
//    };
//    ~dfs_fp_btree_const_t()
//    {
//    };
protected:
    enum cconst_protected {
        FP_SUFFIX_SF_BT_POS  = 0        ,   //fp_btree
        FP_SUFFIX_ID_BT_POS             ,   //fp_btree
        FP_PREFIX_SF_BT_POS             ,   //fp_btree
        FP_PREFIX_ID_BT_POS             ,   //fp_btree
    };
};



//the prefix portion
template<typename T, uint32_t ROW_SIZE>
class _dfs_pfx_btree_t :
    virtual public dfs_bt_const_t,
    virtual public dfs_fp_btree_const_t,
public _dfs_idsf_btree_t<dfs_fp_du_t<T>, dfs_name_t, ROW_SIZE>
{
protected:
    enum cconst_protected {

    };
private:
    typedef T *PT;
    typedef const T *PCT;
    typedef dfs_fp_du_t<T> DU;
    typedef DU *PDU;
    typedef const DU *PCDU;
    typedef dfs_name_t K;
    typedef K *PK;
    typedef const K *PCK;
protected:
    typedef _dfs_id_btree_t<DU, ROW_SIZE> id_btree_t;
    typedef _dfs_sf_btree_t<DU, dfs_name_t, ROW_SIZE> sf_btree_t;
    typedef _dfs_idsf_btree_t<DU, dfs_name_t, ROW_SIZE> idsf_btree_t;
private:
    dfs_init_t _init_state;
public:
    _dfs_pfx_btree_t() :
        _dfs_idsf_btree_t<DU, K, ROW_SIZE>() {
        //init();
    };
    virtual ~_dfs_pfx_btree_t() {
        _init_state.set_destructed();
    }
    int init(
        const uint32_t prefix_id_bt_instance_pos,
        const uint32_t prefix_sf_bt_instance_pos,
        dfs_bt_root_t & new_wr_bt_root);
private:
    //Disable operator=() and copy constructor
    const _dfs_pfx_btree_t & operator=(const _dfs_pfx_btree_t & src);
    _dfs_pfx_btree_t(const _dfs_pfx_btree_t & src);
    //const _dfs_pfx_btree_t & operator=(const _dfs_pfx_btree_t & src)
    //{
    //    return *this;
    //};
    //_dfs_pfx_btree_t(const _dfs_pfx_btree_t & src)
    //{
    //};
public:
    inline int pfx_inc_root_ref_counter(const dfs_bt_root_t & bt_root) const {
        return this->_idsf_inc_root_ref_counter(bt_root);
    };
    inline int pfx_dec_root_ref_counter(const dfs_bt_root_t & bt_root) const {
        return this->_idsf_dec_root_ref_counter(bt_root);
    };
public:
    //  获得一个对象的prefix_id和prefix_index并设置到namet
    //  return: 0 for success, other values for error
    int pfx_search(
        const dfs_bt_root_t & bt_root,
        const dfs_name_t & namet,
        dfs_key_id_index_t & prefix_id_index) const;
    //  功能：加入一个不存在对象(如果对象存在，则获得prefix_index和prefix_id)
    //  return: 0 for success, other values for error
    int pfx_insert(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const dfs_name_t & namet,
        const T & tobj,
        dfs_key_id_index_t & prefix_id_index);
    int pfx_verify_tree(const dfs_bt_root_t & bt_root) const {
        return this->_idsf_verify_tree(bt_root);
    };
    int pfx_empty_btree_check(
        const dfs_bt_root_t & bt_root,
        const bool is_should_empty) const {
        return this->_idsf_empty_btree_check(bt_root, is_should_empty);
    }
    int pfx_sanitary_check(const dfs_bt_root_t & bt_root) const {
        return this->_idsf_sanitary_check(bt_root);
    };
public:
    int pfx_compare_id_id(
        const dfs_bt_root_t & bt_root,
        const uint64_t prefix1_id,
        const uint64_t prefix2_id,
        int & cmp) const {
        return this->_idsf_compare_id_id(bt_root, prefix1_id, prefix2_id, cmp);
    };
    int pfx_compare_id_name(
        const dfs_bt_root_t & bt_root,
        const uint64_t prefix1_id,
        const dfs_name_t & namet2,
        int & cmp) const {
        return this->_idsf_compare_id_key(bt_root, prefix1_id, namet2, cmp);
    };
    //  return: 0 for success, other values for error
    //int pfx_inc_ref(const uint64_t prefix_id);
    //int pfx_dec_ref(const uint64_t prefix_id);
    //  功能：获得一个prefix_id的名字
    int pfx_get_name(
        const dfs_bt_root_t & bt_root,
        const uint64_t prefix_id,
        dfs_name_t & prefix) const;
//protected:
//    //增加/减少根节点引用计数
//    virtual int vir_inc_root_ref_counter(const dfs_bt_root_t & bt_root) const;
//    virtual int vir_dec_root_ref_counter(const dfs_bt_root_t & bt_root) const;
public:
    void pfx_set_cancel_checkpointing(bool is_cancel);
    void pfx_update_mutation_counter(const bool is_batch_mode);
    void pfx_update_max_cow_mutation_counter(void) const;
    int pfx_store_checkpointing(
        const dfs_bt_root_t & bt_root,
        dfs_btree_store_info_t & store_info) const;
    int pfx_load_checkpointing(
        dfs_bt_root_t & bt_root,
        dfs_btree_load_info_t & load_info);
public:
    //  return: 0 for success, other values for error
    int pfx_acquire_empty_root(
        dfs_bt_root_t & new_bt_root) {
        return this->_idsf_acquire_empty_root(new_bt_root);
    }
    void pfx_clear_statistic_info(void) {
        this->_idsf_clear_statistic_info();
    };
    void pfx_log_statistic_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname) const {
        this->_idsf_log_statistic_info(org_log_level, filename, lineno, funcname);
    };
    void pfx_log_debug_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname) const {
        this->_idsf_log_debug_info(org_log_level, filename, lineno, funcname);
    };
public:
    uint64_t pfx_get_mem_size(void) const {
        return this->_idsf_get_mem_size();
    };
};


template<typename T, uint32_t ROW_SIZE>
int _dfs_pfx_btree_t<T, ROW_SIZE>::init(
    const uint32_t prefix_id_bt_instance_pos,
    const uint32_t prefix_sf_bt_instance_pos,
    dfs_bt_root_t & new_wr_bt_root)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (_init_state.is_not_init_yet()) {
        if ((err = idsf_btree_t::init(
                       prefix_id_bt_instance_pos,
                       prefix_sf_bt_instance_pos,
                       new_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "idsf_btree_t::init() returns 0x%x", err);
        }
        _init_state.set_init_result(err);
    } else {
        err = _init_state.get_init_result();
    }

    return err;
};


//  获得一个对象的prefix_id和prefix_index并设置到namet
//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_search(
    const dfs_bt_root_t & bt_root,
    const dfs_name_t & namet,
    dfs_key_id_index_t & prefix_id_index) const
{
    uint64_t prefix_index = UNDEF_INDEX;
    dfs_btree_drill_t drill_info;
    PCDU pcdu = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    prefix_id_index.init();
    if ((err = sf_btree_t::_search_by_key(
                   bt_root,
                   (const void *)&namet,
                   drill_info,
                   prefix_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_search_by_key() returns 0x%x", err);
    }
    //exist
    else if (UNDEF_INDEX != prefix_index) {
        if ((err = _get_exist_obj(prefix_index, pcdu)) != 0 || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_exist_obj(prefix_index...) returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            prefix_id_index.key_id = pcdu->get_id();
            prefix_id_index.key_index = prefix_index;
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "prefix_index=0x%lx", prefix_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "namet", namet.get_str(), namet.get_len());
    }

    return err;
};


//  功能：加入一个不存在对象(如果对象存在，则获得prefix_index和prefix_id)
//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_insert(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const dfs_name_t & namet,
    const T & tobj,
    dfs_key_id_index_t & prefix_id_index)
{
    uint64_t ref_counter = UNDEF_REF_COUNTER;
    dfs_btree_drill_t sf_drill_info;
    DU duobj(tobj);
    uint64_t prefix_index = UNDEF_INDEX;
    PCDU pcdu = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    prefix_id_index.init();
    if ((err = sf_btree_t::_search_by_key(
                   old_bt_root,
                   (const void *)&namet,
                   sf_drill_info,
                   prefix_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_search_by_key() returns 0x%x", err);
    }
    //not existed, add it
    else if (UNDEF_INDEX == prefix_index) {
        duobj.set_prefix_id(NULL_ID);   //prefix本身没有prefix
        if ((err = this->_idsf_insert(
                       old_bt_root,
                       new_bt_root,
                       sf_drill_info,
                       namet,
                       duobj,
                       prefix_id_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pfx: _idsf_insert() returns 0x%x", err);
        } else if ((err = sf_btree_t::_get_obj_ref_counter(prefix_id_index.key_index, ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pfx: _idsf_get_obj_ref_counter() returns 0x%x", err);
        } else if (2 != ref_counter) { //a new added object must have a ref_counter of 2
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pfx: new added obj's ref_counter=%ld(should be 2)", ref_counter);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_REF_COUNTER);
        } else {
            prefix_index = prefix_id_index.key_index;
        }
    }
    //exist, then obtain id
    else {
        if ((err = sf_btree_t::_get_exist_obj(prefix_index, pcdu)) != 0 || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_exist_obj(prefix_index...) returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        }
        //verify the ref_counter
        else if ((err = sf_btree_t::_get_obj_ref_counter(prefix_index, ref_counter)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pfx: _idsf_get_obj_ref_counter() returns 0x%x", err);
        }
        //there are might be some un-referenced items (ref_counter is 2)
        else  if (UNDEF_REF_COUNTER == ref_counter || ref_counter < 2) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "pfx: existed obj has ref_counter=%ld(should >= 2)", ref_counter);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_REF_COUNTER);
        } else {
            prefix_id_index.key_id = pcdu->get_id();
            prefix_id_index.key_index = prefix_index;
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "str_len=%ld, prefix_id_index.key_id/index=%lu/0x%lx",
                        namet.get_len(), prefix_id_index.key_id, prefix_id_index.key_index);
    }

    return err;
};


//  功能：获得一个prefix_id的名字
//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_get_name(
    const dfs_bt_root_t & bt_root,
    const uint64_t prefix_id,
    dfs_name_t & prefix) const
{
    PCDU pcdu = NULL;
    int err = 0;

    if ((err = this->_idsf_get_key_by_id(bt_root, prefix_id, prefix, pcdu)) != 0 || NULL == pcdu) {
        DF_WRITE_LOG_US(DF_UL_LOG_FATAL, "_idsf_get_name() returns 0x%x, prefix_id=%lu", err, prefix_id);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_ID);
    }

    return err;
};


//    //  功能：引用计数+1
//    //  return: 0 for success, other values for error
//template<typename T, uint32_t ROW_SIZE>
//int _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_inc_ref(
//            const uint64_t prefix_id)
//{
//    uint64_t ref_counter = UNDEF_REF_COUNTER;
//    int err = 0;
//
//    if ((err = this->_idsf_inc_obj_ref_counter(prefix_id, ref_counter)) != 0)
//    {
//        DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
//                "_idsf_inc_obj_ref_counter() returns 0x%x, prefix_id=%lu",
//                err, prefix_id);
//    }
//
//    return err;
//};


//    //如果不存在，报错；如果存在，则引用计数-1
//template<typename T, uint32_t ROW_SIZE>
//int _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_dec_ref(
//            const uint64_t prefix_id)
//{
//    uint64_t ref_counter = UNDEF_REF_COUNTER;
//    int err = 0;
//
//    if ((err = this->_idsf_dec_obj_ref_counter(prefix_id, ref_counter)) != 0)
//    {
//        DF_WRITE_LOG_US(DF_UL_LOG_FATAL,
//                "_idsf_dec_obj_ref_counter() returns 0x%x, prefix_id=%lu",
//                err, prefix_id);
//    }
//
//    return err;
//};


////增加/减少根节点引用计数
//template<typename T, uint32_t ROW_SIZE>
//int _dfs_pfx_btree_t<T, ROW_SIZE>::vir_inc_root_ref_counter(
//        const dfs_bt_root_t & bt_root) const
//{
//    return this->_idsf_inc_root_ref_counter(bt_root);
//};
//template<typename T, uint32_t ROW_SIZE>
//int _dfs_pfx_btree_t<T, ROW_SIZE>::vir_dec_root_ref_counter(
//        const dfs_bt_root_t & bt_root) const
//{
//    return this->_idsf_dec_root_ref_counter(bt_root);
//};

template<typename T, uint32_t ROW_SIZE>
void _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_set_cancel_checkpointing(bool is_cancel)
{
    idsf_btree_t::_idsf_set_cancel_checkpointing(is_cancel);
    return;
};

template<typename T, uint32_t ROW_SIZE>
void _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_update_mutation_counter(const bool is_batch_mode)
{
    idsf_btree_t::_idsf_update_mutation_counter(is_batch_mode);
    return;
};

template<typename T, uint32_t ROW_SIZE>
void _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_update_max_cow_mutation_counter(void) const
{
    idsf_btree_t::_idsf_update_max_cow_mutation_counter();
    return;
};



template<typename T, uint32_t ROW_SIZE>
int _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_store_checkpointing(
    const dfs_bt_root_t & bt_root,
    dfs_btree_store_info_t & store_info) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    log_level = this->CKP_LOG_LEVEL;
    DF_WRITE_LOG_US(log_level, "pfx calling _idsf_store_checkpointing()...");
    if ((err = this->_idsf_store_checkpointing(bt_root, store_info)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_store_checkpointing() returns 0x%x", err);
    }

    return err;
};

template<typename T, uint32_t ROW_SIZE>
int _dfs_pfx_btree_t<T, ROW_SIZE>::pfx_load_checkpointing(
    dfs_bt_root_t & bt_root,
    dfs_btree_load_info_t & load_info)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    log_level = this->CKP_LOG_LEVEL;
    DF_WRITE_LOG_US(log_level, "pfx calling _idsf_load_checkpointing()...");
    if ((err = this->_idsf_load_checkpointing(bt_root, load_info)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_load_checkpointing() returns 0x%x", err);
    }

    return err;
};



template<typename T, uint32_t ROW_SIZE>
class _dfs_fp_btree_t :
    virtual public dfs_bt_const_t,
    virtual public dfs_fp_btree_const_t,
public _dfs_idsf_btree_t<dfs_fp_du_t<T>, dfs_name_t, ROW_SIZE>
{
protected:
    enum cconst_protected {
        //FP_BT_VER = 0x00000002, //Hihg-16-bit: main ver;
        FP_BT_TAG = (((int)'f')<<24) | (((int)'p')<<16) | (((int)'b')<<8) | (((int)'t')), //"fpbt"=0x66706274
    };
private:
    typedef T *PT;
    typedef const T *PCT;
    typedef dfs_fp_du_t<T> DU;
    typedef DU *PDU;
    typedef const DU *PCDU;
    typedef dfs_path_t K;
    typedef K *PK;
    typedef const K *PCK;
protected:
    typedef _dfs_idsf_btree_t<DU, dfs_name_t, ROW_SIZE> idsf_btree_t;
    typedef _dfs_id_btree_t<DU, ROW_SIZE> id_btree_t;
    typedef _dfs_sf_btree_t<DU, dfs_name_t, ROW_SIZE> sf_btree_t;
private:
    dfs_init_t _init_state;
    _dfs_pfx_btree_t<T, ROW_SIZE> _pfx_btree;
public:
    _dfs_fp_btree_t() : idsf_btree_t() {
        //init();
    }
    virtual ~_dfs_fp_btree_t() {
        _init_state.set_destructed();
    }
    int init(
        const uint32_t prefix_id_bt_instance_pos,
        const uint32_t prefix_sf_bt_instance_pos,
        const uint32_t suffix_id_bt_instance_pos,
        const uint32_t suffix_sf_bt_instance_pos,
        dfs_bt_root_t & new_wr_bt_root);
private:
    //Disable operator=() and copy constructor
    const _dfs_fp_btree_t & operator=(const _dfs_fp_btree_t & src);
    _dfs_fp_btree_t(const _dfs_fp_btree_t & src);
    //const _dfs_fp_btree_t & operator=(const _dfs_fp_btree_t & src)
    //{
    //    return *this;
    //};
    //_dfs_fp_btree_t(const _dfs_fp_btree_t & src) : idsf_btree_t()
    //{
    //};
protected:
    inline int _fp_inc_root_ref_counter(const dfs_bt_root_t & bt_root) const;
    inline int _fp_dec_root_ref_counter(const dfs_bt_root_t & bt_root) const;
    void _fp_set_cancel_checkpointing(bool is_cancel);
    void _fp_update_mutation_counter(const bool is_batch_mode);
    void _fp_update_max_cow_mutation_counter(void) const;
    //  功能：把已经设置的checkpoint存储到文件中(first prefix(fp,id), then suffix(fp,id))
    //  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
    //  返回：0 for success, other values for error
    int _fp_store_checkpointing(
        const dfs_bt_root_t & bt_root,
        dfs_btree_store_info_t & store_info) const;
    //
    //  功能：把先前存储在文件的checkpoint加载到内存中(first prefix(fp,id), then suffix(fp,id))
    //  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
    //  返回：0 for success, other values for error
    int _fp_load_checkpointing(
        dfs_bt_root_t & bt_root,
        dfs_btree_load_info_t & load_info);
protected:
    //
    //  功能：获得B树最小元素
    //  返回：0 for success, other values for error
    int _fp_get_smallest(
        const dfs_bt_root_t & bt_root,
        uint64_t & o_key_id,
        K & o_keyt,
        PCDU & pcdu) const;
    //  功能：获得B树最大元素
    //  返回：0 for success, other values for error
    int _fp_get_largest(
        const dfs_bt_root_t & bt_root,
        uint64_t & o_key_id,
        K & o_keyt,
        PCDU & pcdu) const;
    //
    //  功能：获得刚好比输入的名字小的名字的key_id
    //  输入：namet：不需要在B树中存在
    //  返回：0 for success, other values for error
    int _fp_get_smaller(
        const dfs_bt_root_t & bt_root,
        const K & i_keyt,
        dfs_btree_drill_t & drill_info,
        uint64_t & o_key_id,
        K & o_keyt,
        PCDU & pcdu) const;
    int _fp_get_smaller(
        const dfs_bt_root_t & bt_root,
        const uint64_t i_key_id,
        dfs_btree_drill_t & drill_info,
        uint64_t & o_key_id,
        K & o_keyt,
        PCDU & pcdu) const;
    //
    //  功能：获得刚好比输入的名字大的名字的key_id
    //  输入：namet：不需要在B树中存在
    //  输出：如果pkey_id/pnamet/pt不是NULL，则对应项的值被复制到*pkey_id/*pnamet/*pt
    //  返回：true如果有更大的元素，否则返回false
    int _fp_get_larger(
        const dfs_bt_root_t & bt_root,
        const K & i_keyt,
        dfs_btree_drill_t & drill_info,
        uint64_t & o_key_id,
        K & o_keyt,
        PCDU & pcdu) const;
    int _fp_get_larger(
        const dfs_bt_root_t & bt_root,
        const uint64_t i_key_id,
        dfs_btree_drill_t & drill_info,
        uint64_t & o_key_id,
        K & o_keyt,
        PCDU & pcdu) const;

protected:
    //通过获得patht.prefx的key_id以加快搜索
    int _fp_search_by_key(
        const dfs_bt_root_t & bt_root,
        const K & keyt,
        dfs_btree_drill_t & drill_info,
        uint64_t & key_index) const;
    //  功能：把项tobj插入或更新到B树中。
    //       insert：如果T的id为UNDEF_ID则为其申请一个新ID。如果T的id在B树中不存在，则加入B树，否则不做操作并返回UNDEF_ID。
    //       update：如果T的id在B树存在就更新其值为tobj，否则返回UNDEF_ID。
    //       insert_update：先执行insert，如果该项存在就再执行update。
    //  输入：patht：目录或文件名
    //        如果原来项存在且old_pt非空，则其值复制到*old_pt。
    //  return: 0 for success, other values for error
    int _fp_insert(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const K & keyt,
        const T & tobj,
        T * old_pt,
        bool & is_old_exist,
        dfs_key_id_index_t & key_id_index); //old or new
    int _fp_update(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const K & keyt,
        const T & tobj,
        T * old_pt,
        bool & is_old_exist,
        dfs_key_id_index_t & key_id_index); //old or new
    int _fp_update(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const uint64_t new_key_id,
        const T & tobj,
        T * old_pt,
        bool & is_old_exist,
        dfs_key_id_index_t & key_id_index); //old or new
    int _fp_insert_update(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const K & keyt,
        const T & tobj,
        T * old_pt,
        bool & is_old_exist,
        dfs_key_id_index_t & key_id_index); //old or new
    //
    //  功能：如果key存在则删除
    //  return: 0 for success, other values for error
    int _fp_del(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const K & keyt,
        T * old_pt,
        bool & is_old_exist,
        dfs_key_id_index_t & key_id_index); //old or new
    int _fp_del(
        const dfs_bt_root_t & old_bt_root,
        dfs_bt_root_t & new_bt_root,
        const uint64_t key_id,
        T * old_pt,
        bool & is_old_exist,
        dfs_key_id_index_t & key_id_index); //old or new

protected:
    //  return: 0 for success, other values for error
    int _fp_get_exist_key_by_index(
        const dfs_bt_root_t & bt_root,
        const uint64_t key_index,
        K & keyt,
        PCDU & pcdu) const;
private:
    //  return: 0 for success, other values for error
    //  cmp: -1, 0, 1 for less than, equal, great than
    inline int _fp_compare_index_index(
        const dfs_bt_root_t & bt_root,
        const uint64_t key1_index,
        const uint64_t key2_index,
        int & cmp) const;
    inline int _fp_compare_index_key(
        const dfs_bt_root_t & bt_root,
        const uint64_t key1_index,
        const K & keyt2,
        int & cmp) const;
    inline int _fp_compare_index_obj(
        const dfs_bt_root_t & bt_root,
        const uint64_t key1_index,
        const DU & duobj2,
        int & cmp) const;
    inline int _fp_compare_obj_obj(
        const dfs_bt_root_t & bt_root,
        const DU & duobj1,
        const DU & duobj2,
        int & cmp) const;
    inline int _fp_compare_obj_key(
        const dfs_bt_root_t & bt_root,
        const DU & duobj1,
        const K & keyt2,
        int & cmp) const;
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
    //  功能：验证整个树的顺序和每个节点分支个数
    //  返回：返回0如果验证通过，返回非0(错误代码)如果验证失败
    int _fp_verify_tree(const dfs_bt_root_t & bt_root) const;
protected:
    //  return: 0 for success, other values for error
    int _fp_empty_btree_check(
        const dfs_bt_root_t & bt_root,
        const bool is_should_empty) const;
    //  return: 0 for success, other values for error
    //  Should be called in locked state and without checkpointing or other COW(load or store)
    int _fp_sanitary_check(const dfs_bt_root_t & bt_root) const;
protected:
    //  return: 0 for success, other values for error
    //  Clear contents of the whole tree
    int _fp_acquire_empty_root(
        dfs_bt_root_t & new_bt_root);
    void _fp_clear_statistic_info(void);
    void _fp_log_statistic_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname) const;
    void _fp_log_debug_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname) const;
    uint64_t _fp_get_mem_size(void) const;
};


template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::init(
    const uint32_t prefix_id_bt_instance_pos,
    const uint32_t prefix_sf_bt_instance_pos,
    const uint32_t suffix_id_bt_instance_pos,
    const uint32_t suffix_sf_bt_instance_pos,
    dfs_bt_root_t & new_wr_bt_root)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (_init_state.is_not_init_yet()) {
        if ((err = idsf_btree_t::init(
                       suffix_id_bt_instance_pos,
                       suffix_sf_bt_instance_pos,
                       new_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "idsf_btree_t::init() returns 0x%x", err);
        } else if ((err = _pfx_btree.init(
                              prefix_id_bt_instance_pos,
                              prefix_sf_bt_instance_pos,
                              new_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_pfx_btree.init() returns 0x%x", err);
        }

        _init_state.set_init_result(err);
    } else {
        err = _init_state.get_init_result();
    }

    return err;
};

template<typename T, uint32_t ROW_SIZE>
inline int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_inc_root_ref_counter(
    const dfs_bt_root_t & bt_root) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err1 = this->_idsf_inc_root_ref_counter(bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_inc_root_ref_counter() returns 0x%x", err);
    }
    if ((err1 = _pfx_btree.pfx_inc_root_ref_counter(bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_inc_root_ref_counter() returns 0x%x", err);
    }

    return err;
};

template<typename T, uint32_t ROW_SIZE>
inline int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_dec_root_ref_counter(
    const dfs_bt_root_t & bt_root) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err1 = this->_idsf_dec_root_ref_counter(bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_dec_root_ref_counter() returns 0x%x", err);
    }
    if ((err1 = _pfx_btree.pfx_dec_root_ref_counter(bt_root)) != 0) {
        err = err1;
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_dec_root_ref_counter() returns 0x%x", err);
    }

    return err;
};

template<typename T, uint32_t ROW_SIZE>
void _dfs_fp_btree_t<T, ROW_SIZE>::_fp_set_cancel_checkpointing(bool is_cancel)
{
    _pfx_btree.pfx_set_cancel_checkpointing(is_cancel);
    idsf_btree_t::_idsf_set_cancel_checkpointing(is_cancel);
    return;
};

template<typename T, uint32_t ROW_SIZE>
void _dfs_fp_btree_t<T, ROW_SIZE>::_fp_update_mutation_counter(const bool is_batch_mode)
{
    _pfx_btree.pfx_update_mutation_counter(is_batch_mode);
    idsf_btree_t::_idsf_update_mutation_counter(is_batch_mode);
    return;
};

template<typename T, uint32_t ROW_SIZE>
void _dfs_fp_btree_t<T, ROW_SIZE>::_fp_update_max_cow_mutation_counter(void) const
{
    _pfx_btree.pfx_update_max_cow_mutation_counter();
    idsf_btree_t::_idsf_update_max_cow_mutation_counter();
    return;
};

//  功能：把已经设置的checkpoint存储到文件中(first prefix(fp,id), then suffix(fp,id))
//  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_store_checkpointing(
    const dfs_bt_root_t & bt_root,
    dfs_btree_store_info_t & store_info) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    store_info.set_major_tag(FP_BT_TAG);
    if ((err = _pfx_btree.pfx_store_checkpointing(bt_root, store_info)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_store_checkpointing() returns 0x%x", err);
    } else if ((err = this->_idsf_store_checkpointing(bt_root, store_info)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_store_checkpointing() returns 0x%x", err);
    }

    return err;
};

//
//  功能：把先前存储在文件的checkpoint加载到内存中(first prefix(fp,id), then suffix(fp,id))
//  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_load_checkpointing(
    dfs_bt_root_t & bt_root,
    dfs_btree_load_info_t & load_info)
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    load_info.set_major_tag(FP_BT_TAG);
    if ((err = _pfx_btree.pfx_load_checkpointing(bt_root, load_info)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_load_checkpointing() returns 0x%x", err);
    } else if ((err = this->_idsf_load_checkpointing(bt_root, load_info)) != 0) {
        log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_load_checkpointing() returns 0x%x", err);
    }

    return err;
};


//
//  功能：获得B树最小元素
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_get_smallest(
    const dfs_bt_root_t & bt_root,
    uint64_t & o_key_id,
    K & o_keyt,
    PCDU & pcdu) const
{
    dfs_btree_drill_t drill_info;
    uint64_t o_key_index = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    o_key_id = UNDEF_ID;
    o_keyt.init();
    pcdu = NULL;

    if ((err = sf_btree_t::_get_smallest(bt_root, drill_info, o_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_smallest() returns 0x%x", err);
    }
    //存在
    else if (UNDEF_INDEX != o_key_index) {
        if ((err = _fp_get_exist_key_by_index(bt_root, o_key_index, o_keyt, pcdu)) != 0
            || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            o_key_id = pcdu->get_id();
        }
    }

    return err;
};
//  功能：获得B树最大元素
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_get_largest(
    const dfs_bt_root_t & bt_root,
    uint64_t & o_key_id,
    K & o_keyt,
    PCDU & pcdu) const
{
    dfs_btree_drill_t drill_info;
    uint64_t o_key_index = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    o_key_id = UNDEF_ID;
    o_keyt.init();
    pcdu = NULL;

    if ((err = sf_btree_t::_get_largest(bt_root, drill_info, o_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_largest() returns 0x%x", err);
    }
    //存在
    else if (UNDEF_INDEX != o_key_index) {
        if ((err = _fp_get_exist_key_by_index(bt_root, o_key_index, o_keyt, pcdu)) != 0
            || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            o_key_id = pcdu->get_id();
        }
    }

    return err;
};
//
//  功能：获得刚好比输入的名字小的名字的key_id
//  输入：namet：不需要在B树中存在
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_get_smaller(
    const dfs_bt_root_t & bt_root,
    const K & i_keyt,
    dfs_btree_drill_t & drill_info,
    uint64_t & o_key_id,
    K & o_keyt,
    PCDU & pcdu) const
{
    uint64_t o_key_index = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    o_key_id = UNDEF_ID;
    o_keyt.init();
    pcdu = NULL;

    if ((err = sf_btree_t::_get_smaller_by_key(
                   bt_root,
                   (void *)&i_keyt,
                   drill_info,
                   o_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_smaller_by_key() returns 0x%x", err);
    }
    //存在
    else if (UNDEF_INDEX != o_key_index) {
        if ((err = _fp_get_exist_key_by_index(bt_root, o_key_index, o_keyt, pcdu)) != 0
            || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            o_key_id = pcdu->get_id();
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        BT_DF_WRITE_LOG_BYTES(log_level, "i_prefix",
                              i_keyt.prefix.get_str(), i_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "i_suffix",
                              i_keyt.suffix.get_str(),  i_keyt.suffix.get_len());

        DF_WRITE_LOG_US(log_level, "o_key_id/index=%lu/0x%lx", o_key_id, o_key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "o_prefix",
                              o_keyt.prefix.get_str(), o_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "o_suffix",
                              o_keyt.suffix.get_str(),  o_keyt.suffix.get_len());
    }

    return err;
};
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_get_smaller(
    const dfs_bt_root_t & bt_root,
    const uint64_t i_key_id,
    dfs_btree_drill_t & drill_info,
    uint64_t & o_key_id,
    K & o_keyt,
    PCDU & pcdu) const
{
    const dfs_sbt_root_t & sbt_root = idsf_btree_t::sf_btree_t::_bt_get_sbt_root(bt_root);
    K i_keyt;
    dfs_btree_drill_t id_drill_info;
    uint64_t i_key_index = UNDEF_INDEX;
    uint64_t o_key_index = UNDEF_INDEX;
    bool is_search = false;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    o_key_id = UNDEF_ID;
    o_keyt.init();
    pcdu = NULL;

    if (drill_info.not_match_tree(sbt_root)) {
        is_search = true;
    } else if ((err = sf_btree_t::_get_drill_last_obj(drill_info, i_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_drill_last_obj() returns 0x%x", err);
    } else if (UNDEF_INDEX == i_key_index) {
        is_search = true;
    } else if ((err = sf_btree_t::_get_exist_obj(i_key_index, pcdu)) != 0 || NULL == pcdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_get_exist_obj() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_INVALID_INDEX);
    } else if (i_key_id != pcdu->get_id()) {
        is_search = true;
    }

    if (is_search) {
        drill_info.init();
        id_drill_info.init();
        if ((err = this->_idsf_search_id_by_id(
                       bt_root,
                       i_key_id,
                       id_drill_info,
                       i_key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_idsf_search_id_by_id() returns 0x%x", err);
        }
        //不存在
        else if(UNDEF_INDEX == i_key_index) {
            log_level = DF_UL_LOG_NOTICE;
            DF_WRITE_LOG_US(log_level, "can't find key_id in btree");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_ID);
        }
    }

    if (0 == err) {
        //获得名字
        if ((err = _fp_get_exist_key_by_index(bt_root, i_key_index, i_keyt, pcdu)) != 0
            || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = sf_btree_t::_get_smaller_by_key(
                              bt_root,
                              (void *)&i_keyt,
                              drill_info,
                              o_key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_smaller_by_key() returns 0x%x", err);
        }
        //存在
        else if (UNDEF_INDEX != o_key_index) {
            if ((err = _fp_get_exist_key_by_index(bt_root, o_key_index, o_keyt, pcdu)) != 0
                || NULL == pcdu) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
            } else {
                o_key_id = pcdu->get_id();
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "i_key_id=%lu", i_key_id);
        BT_DF_WRITE_LOG_BYTES(log_level, "i_prefix",
                              i_keyt.prefix.get_str(), i_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "i_suffix",
                              i_keyt.suffix.get_str(),  i_keyt.suffix.get_len());

        DF_WRITE_LOG_US(log_level, "o_key_id/index=%lu/0x%lx", o_key_id, o_key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "o_prefix",
                              o_keyt.prefix.get_str(), o_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "o_suffix",
                              o_keyt.suffix.get_str(),  o_keyt.suffix.get_len());
    }

    return err;
};
//
//  功能：获得刚好比输入的名字大的名字的key_id
//  输入：namet：不需要在B树中存在
//  输出：如果pkey_id/pnamet/pt不是NULL，则对应项的值被复制到*pkey_id/*pnamet/*pt
//  返回：true如果有更大的元素，否则返回false
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_get_larger(
    const dfs_bt_root_t & bt_root,
    const K & i_keyt,
    dfs_btree_drill_t & drill_info,
    uint64_t & o_key_id,
    K & o_keyt,
    PCDU & pcdu) const
{
    uint64_t o_key_index = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    o_key_id = UNDEF_ID;
    o_keyt.init();
    pcdu = NULL;

    if ((err = sf_btree_t::_get_larger_by_key(
                   bt_root,
                   (void *)&i_keyt,
                   drill_info,
                   o_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_larger_by_key() returns 0x%x", err);
    }
    //存在
    else if (UNDEF_INDEX != o_key_index) {
        if ((err = _fp_get_exist_key_by_index(bt_root, o_key_index, o_keyt, pcdu)) != 0
            || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            o_key_id = pcdu->get_id();
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        BT_DF_WRITE_LOG_BYTES(log_level, "i_prefix",
                              i_keyt.prefix.get_str(), i_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "i_suffix",
                              i_keyt.suffix.get_str(),  i_keyt.suffix.get_len());

        DF_WRITE_LOG_US(log_level, "o_key_id/index=%lu/0x%lx", o_key_id, o_key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "o_prefix",
                              o_keyt.prefix.get_str(), o_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "o_suffix",
                              o_keyt.suffix.get_str(),  o_keyt.suffix.get_len());
    }

    return err;
};
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_get_larger(
    const dfs_bt_root_t & bt_root,
    const uint64_t i_key_id,
    dfs_btree_drill_t & drill_info,
    uint64_t & o_key_id,
    K & o_keyt,
    PCDU & pcdu) const
{
    const dfs_sbt_root_t & sbt_root = idsf_btree_t::sf_btree_t::_bt_get_sbt_root(bt_root);
    K i_keyt;
    dfs_btree_drill_t id_drill_info;
    uint64_t i_key_index = UNDEF_INDEX;
    uint64_t o_key_index = UNDEF_INDEX;
    bool is_search = false;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    o_key_id = UNDEF_ID;
    o_keyt.init();
    pcdu = NULL;

    if (drill_info.not_match_tree(sbt_root)) {
        is_search = true;
    } else if ((err = sf_btree_t::_get_drill_last_obj(drill_info, i_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_drill_last_obj() returns 0x%x", err);
    } else if (UNDEF_INDEX == i_key_index) {
        is_search = true;
    } else if ((err = sf_btree_t::_get_exist_obj(i_key_index, pcdu)) != 0 || NULL == pcdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_get_exist_obj() returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_INVALID_INDEX);
    } else if (i_key_id != pcdu->get_id()) {
        is_search = true;
    }

    if (is_search) {
        drill_info.init();
        id_drill_info.init();
        if ((err = this->_idsf_search_id_by_id(
                       bt_root,
                       i_key_id,
                       id_drill_info,
                       i_key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_idsf_search_id_by_id() returns 0x%x", err);
        }
        //不存在
        else if(UNDEF_INDEX == i_key_index) {
            log_level = DF_UL_LOG_NOTICE;
            DF_WRITE_LOG_US(log_level, "can't find key_id in btree");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_ID);
        }
    }

    if (0 == err) {
        //获得名字
        if ((err = _fp_get_exist_key_by_index(bt_root, i_key_index, i_keyt, pcdu)) != 0
            || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        }

        else if ((err = sf_btree_t::_get_larger_by_key(
                            bt_root,
                            (void *)&i_keyt,
                            drill_info,
                            o_key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_larger_by_key() returns 0x%x", err);
        }
        //存在
        else if (UNDEF_INDEX != o_key_index) {
            if ((err = _fp_get_exist_key_by_index(bt_root, o_key_index, o_keyt, pcdu)) != 0
                || NULL == pcdu) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
            } else {
                o_key_id = pcdu->get_id();
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "i_key_id=%lu", i_key_id);
        BT_DF_WRITE_LOG_BYTES(log_level, "i_prefix",
                              i_keyt.prefix.get_str(), i_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "i_suffix",
                              i_keyt.suffix.get_str(),  i_keyt.suffix.get_len());

        DF_WRITE_LOG_US(log_level, "o_key_id/index=%lu/0x%lx", o_key_id, o_key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "o_prefix",
                              o_keyt.prefix.get_str(), o_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "o_suffix",
                              o_keyt.suffix.get_str(),  o_keyt.suffix.get_len());
    }

    return err;
};


//通过获得patht.prefx的key_id以加快搜索
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_search_by_key(
    const dfs_bt_root_t & bt_root,
    const K & keyt,
    dfs_btree_drill_t & drill_info,
    uint64_t & key_index) const
{
    K keyt2(keyt);
    dfs_key_id_index_t prefix_id_index;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    key_index = UNDEF_INDEX;
    drill_info.init();

    if ((err = _pfx_btree.pfx_search(bt_root, keyt.prefix, prefix_id_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_insert() returns 0x%x", err);
    } else {
#ifdef DF_USE_ID_IN_STR_KEY
        //keyt.prefix.key_id might be UNDEF_ID;
        keyt2.prefix.set_name_id(prefix_id_index.key_id);
#endif
        if ((err = sf_btree_t::_search_by_key(
                       bt_root,
                       &keyt2,
                       drill_info,
                       key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::_search_by_key() returns 0x%x", err);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "prefix: id/index=%lu/0x%lx",
                        prefix_id_index.key_id,
                        prefix_id_index.key_index);
        DF_WRITE_LOG_US(log_level, "key_index=0x%lx", key_index);
        DF_WRITE_LOG_US(log_level,
                        "prefix/suffix_len=%ld/%ld",
                        keyt.prefix.get_len(), keyt.suffix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
    }

    return err;
};

//  功能：把项tobj插入或更新到B树中。
//       insert：如果T的id为UNDEF_ID则为其申请一个新ID。如果T的id在B树中不存在，则加入B树，否则不做操作并返回UNDEF_ID。
//       update：如果T的id在B树存在就更新其值为tobj，否则返回UNDEF_ID。
//       insert_update：先执行insert，如果该项存在就再执行update。
//  输入：patht：目录或文件名
//        如果原来项存在且old_pt非空，则其值复制到*old_pt。
//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_insert(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const K & keyt,
    const T & tobj,
    T * old_pt,
    bool & is_old_exist,
    dfs_key_id_index_t & key_id_index) //old or new
{
    dfs_btree_drill_t sf_drill_info;
    dfs_key_id_index_t prefix_id_index;
    K keyt2(keyt);
    DU new_duobj(tobj);
    PCDU pcdu = NULL;
    T tpfxobj;
    uint64_t old_key_index = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    new_bt_root = old_bt_root;  //复制后，如果前缀树没有更新，则其根节点不变
    key_id_index.init();   //在内部测试中，key_id输入值为UNDEF_ID或者期望的ID
    is_old_exist = false;

    //先加入前缀(如果已经存在则仅仅取回prefix_id和prefix_index于patht.prefix中)
    tpfxobj.init();   //前缀总是保存T初始值
    tpfxobj.set_id(UNDEF_ID);
    if ((err = _pfx_btree.pfx_insert(
                   old_bt_root,
                   new_bt_root,
                   keyt.prefix,
                   tpfxobj,
                   prefix_id_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_insert() returns 0x%x", err);
    } else if (UNDEF_ID == prefix_id_index.key_id) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "prefix_id is UNDEF_ID after _pfx_btree.pfx_insert()");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_UNDEF_PREFIX_ID);
    } else {
#ifdef DF_USE_ID_IN_STR_KEY
        keyt2.prefix.set_name_id(prefix_id_index.key_id);
#endif
        if ((err = sf_btree_t::_search_by_key(
                       old_bt_root,
                       &keyt2,
                       sf_drill_info,
                       old_key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::_search_by_key() returns 0x%x", err);
        }
        //项存在
        else if (UNDEF_INDEX != old_key_index) {
            if ((err = sf_btree_t::_get_exist_obj(old_key_index, pcdu)) != 0 || NULL == pcdu) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_get_exist_obj() returns 0x%x", err);
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_INVALID_INDEX);
            } else {
                key_id_index.key_id = pcdu->get_id();
                key_id_index.key_index = old_key_index;

                //复制旧对象：一定要在更新B树之前，因为之后就对象可能被释放并调用了init()
                if (NULL != old_pt) {
                    *old_pt = *((const T *)pcdu);
                }
                is_old_exist = true;
            }
        }
        //项不存在
        else {
            new_duobj.set_prefix_id(prefix_id_index.key_id);    //设置prefix_id
            //加入文件名本身
            if ((err = _idsf_insert(
                           old_bt_root,
                           new_bt_root,
                           sf_drill_info,
                           keyt.suffix,
                           new_duobj,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_idsf_insert() returns 0x%x", err);
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "prefix: id/index=%lu/0x%lx",
                        prefix_id_index.key_id,
                        prefix_id_index.key_index);
        DF_WRITE_LOG_US(log_level,
                        "str: id/index=%lu/0x%lx",
                        key_id_index.key_id,
                        key_id_index.key_index);
        DF_WRITE_LOG_US(log_level,
                        "prefix/suffix_len=%ld/%ld, pcdu=0x%p",
                        keyt.prefix.get_len(), keyt.suffix.get_len(),
                        pcdu);
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
    }

    return err;
};
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_update(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const K & keyt,
    const T & tobj,
    T * old_pt,
    bool & is_old_exist,
    dfs_key_id_index_t & key_id_index) //old or new
{
    dfs_btree_drill_t sf_drill_info;
    dfs_btree_drill_t id_drill_info;
    DU new_duobj;
    PCDU pcdu = NULL;
    uint64_t old_key_index = UNDEF_INDEX;   //update前的index
    uint64_t key_index2 = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    new_bt_root = old_bt_root;  //复制后，如果前缀树没有更新，则其根节点不变
    key_id_index.init();   //在内部测试中，key_id输入值为UNDEF_ID或者期望的ID
    is_old_exist = false;

    if ((err = _fp_search_by_key(
                   old_bt_root,
                   keyt,
                   sf_drill_info,
                   old_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_fp_search_by_key() returns 0x%x", err);
    }
    //项存在
    else if (UNDEF_INDEX != old_key_index) {
        if ((err = _get_exist_obj(old_key_index, pcdu)) != 0 || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_exist_obj(old_key_index...) returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_idsf_search_id_by_index(
                              old_bt_root,
                              old_key_index,
                              id_drill_info,
                              key_index2)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "this->_idsf_search_id_by_index() returns 0x%x", err);
        } else if (key_index2 != old_key_index) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "key_index2 != old_key_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        } else {
            //复制旧对象：一定要在更新B树之前，因为之后就对象可能被释放并调用了init()
            if (NULL != old_pt) {
                *old_pt = *((const T *)pcdu);
            }
            is_old_exist = true;

            new_duobj = *pcdu;  //获得offset, prefix_id...
            new_duobj = tobj;
            new_duobj.set_id(pcdu->get_id());
            if ((err = _idsf_update(
                           old_bt_root,
                           new_bt_root,
                           sf_drill_info,
                           id_drill_info,
                           new_duobj,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_idsf_update() returns 0x%x", err);
            }
        }
    }
    //项不存在
    else {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "to be updated path doesn't exist");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_NOT_EXIST);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "keyt: id/index=%lu/0x%lx",
                        key_id_index.key_id,
                        key_id_index.key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
    }

    return err;
};
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_update(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const uint64_t key_id,
    const T & tobj,
    T * old_pt,
    bool & is_old_exist,
    dfs_key_id_index_t & key_id_index) //old or new
{
    dfs_btree_drill_t sf_drill_info;
    dfs_btree_drill_t id_drill_info;
    DU new_duobj;
    PCDU pcdu = NULL;
    uint64_t old_key_index = UNDEF_INDEX;   //update前的index
    uint64_t key_index2 = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    new_bt_root = old_bt_root;  //复制后，如果前缀树没有更新，则其根节点不变
    key_id_index.init();   //在内部测试中，key_id输入值为UNDEF_ID或者期望的ID
    is_old_exist = false;

    if ((err = this->_idsf_search_id_by_id(
                   old_bt_root,
                   key_id,
                   id_drill_info,
                   old_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_search_id_by_id() returns 0x%x", err);
    }
    //项存在
    else if (UNDEF_INDEX != old_key_index) {
        if ((err = _get_exist_obj(old_key_index, pcdu)) != 0 || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_exist_obj(old_key_index...) returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = sf_btree_t::_search_by_index(
                              old_bt_root,
                              old_key_index,
                              sf_drill_info,
                              key_index2)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::_search_by_index() returns 0x%x", err);
        } else if (key_index2 != old_key_index) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "key_index2 != old_key_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        } else {
            //复制旧对象：一定要在更新B树之前，因为之后就对象可能被释放并调用了init()
            if (NULL != old_pt) {
                *old_pt = *((const T *)pcdu);
            }
            is_old_exist = true;

            new_duobj = *pcdu;  //获得offset, prefix_id...
            new_duobj = tobj;
            new_duobj.set_id(pcdu->get_id());
            if ((err = _idsf_update(
                           old_bt_root,
                           new_bt_root,
                           sf_drill_info,
                           id_drill_info,
                           new_duobj,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_idsf_update() returns 0x%x", err);
            }
        }
    }
    //项不存在
    else {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "to be updated key_id doesn't exist");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_ID);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_id=%lu",
                        key_id);
    }

    return err;
};
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_insert_update(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const K & keyt,
    const T & tobj,
    T * old_pt,
    bool & is_old_exist,
    dfs_key_id_index_t & key_id_index) //old or new
{
    dfs_btree_drill_t sf_drill_info;
    dfs_btree_drill_t id_drill_info;
    K keyt2(keyt);
    T tpfxobj;    //for prefix btree
    DU new_duobj(tobj);
    PCDU pcdu = NULL;
    uint64_t old_key_index = UNDEF_INDEX;   //update前的index
    uint64_t key_index2 = UNDEF_INDEX;
    dfs_key_id_index_t prefix_id_index;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    new_bt_root = old_bt_root;  //复制后，如果前缀树没有更新，则其根节点不变
    key_id_index.init();   //在内部测试中，key_id输入值为UNDEF_ID或者期望的ID
    is_old_exist = false;

    //先加入前缀(如果已经存在则仅仅取回prefix_id和prefix_index于patht.prefix中)
    tpfxobj.init();   //前缀总是保存T初始值
    tpfxobj.set_id(UNDEF_ID);
    if ((err = _pfx_btree.pfx_insert(
                   old_bt_root,
                   new_bt_root,
                   keyt.prefix,
                   tpfxobj,
                   prefix_id_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_insert() returns 0x%x", err);
    } else if (UNDEF_ID == prefix_id_index.key_id) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "prefix_id is UNDEF_ID after _pfx_btree.pfx_insert()");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_UNDEF_PREFIX_ID);
    } else {
#ifdef DF_USE_ID_IN_STR_KEY
        keyt2.prefix.set_name_id(prefix_id_index.key_id);
#endif
        if ((err = sf_btree_t::_search_by_key(
                       old_bt_root,
                       &keyt2,
                       sf_drill_info,
                       old_key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::_search_by_key() returns 0x%x", err);
        }
        //项存在
        else if (UNDEF_INDEX != old_key_index) {
            if ((err = _get_exist_obj(old_key_index, pcdu)) != 0 || NULL == pcdu) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_get_exist_obj(old_key_index...) returns 0x%x", err);
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
            } else if ((err = this->_idsf_search_id_by_index(
                                  old_bt_root,
                                  old_key_index,
                                  id_drill_info,
                                  key_index2)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "this->_idsf_search_id_by_index() returns 0x%x", err);
            } else if (key_index2 != old_key_index) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "key_index2 != old_key_index");
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
            } else {
                //复制旧对象：一定要在更新B树之前，因为之后就对象可能被释放并调用了init()
                if (NULL != old_pt) {
                    *old_pt = *((const T *)pcdu);
                }
                is_old_exist = true;

                new_duobj = *pcdu;  //获得offset, prefix_id...
                new_duobj = tobj;
                new_duobj.set_id(pcdu->get_id());
                if ((err = _idsf_update(
                               old_bt_root,
                               new_bt_root,
                               sf_drill_info,
                               id_drill_info,
                               new_duobj,
                               key_id_index)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_idsf_update() returns 0x%x", err);
                }
            }
        }
        //不存在
        else {
            new_duobj.set_prefix_id(prefix_id_index.key_id);    //设置prefix_id
            //加入文件名本身
            if ((err = _idsf_insert(
                           old_bt_root,
                           new_bt_root,
                           sf_drill_info,
                           keyt.suffix,
                           new_duobj,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_idsf_insert() returns 0x%x", err);
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "prefix: id/index=%lu/0x%lx",
                        prefix_id_index.key_id,
                        prefix_id_index.key_index);
        DF_WRITE_LOG_US(log_level,
                        "str: id/index=%lu/0x%lx",
                        key_id_index.key_id,
                        key_id_index.key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
    }

    return err;
};



//
//  功能：如果key存在则删除
//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_del(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const K & keyt,
    T * old_pt,
    bool & is_old_exist,
    dfs_key_id_index_t & key_id_index) //old or new
{
    dfs_btree_drill_t sf_drill_info;
    dfs_btree_drill_t id_drill_info;
    PCDU pcdu = NULL;
    uint64_t old_key_index = UNDEF_INDEX;
    uint64_t key_index2 = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    new_bt_root = old_bt_root;  //复制后，如果前缀树没有更新，则其根节点不变
    key_id_index.init();   //在内部测试中，key_id输入值为UNDEF_ID或者期望的ID
    is_old_exist = false;

    if ((err = _fp_search_by_key(
                   old_bt_root,
                   keyt,
                   sf_drill_info,
                   old_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_fp_search_by_key() returns 0x%x", err);
    }
    //不存在
    else if (UNDEF_INDEX == old_key_index) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "to be deleted path doesn't exist");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_NOT_EXIST);
    }
    //存在
    else {
        if ((err = this->_idsf_search_id_by_index(
                       old_bt_root,
                       old_key_index,
                       id_drill_info,
                       key_index2)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "this->_idsf_search_id_by_index() returns 0x%x", err);
        } else if (key_index2 != old_key_index) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "key_index2 != old_key_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        } else if ((err = _get_exist_obj(old_key_index, pcdu)) != 0 || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_exist_obj(old_key_index...) returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            //复制旧对象：一定要在更新B树之前，因为之后就对象可能被释放并调用了init()
            if (NULL != old_pt) {
                *old_pt = *((const T *)pcdu);
            }
            is_old_exist = true;

            if ((err = this->_idsf_del(
                           old_bt_root,
                           new_bt_root,
                           sf_drill_info,
                           id_drill_info)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_idsf_del() returns 0x%x", err);
            } else {
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "old_key_index=0x%lx, key_index2=0x%lx",
                        old_key_index, key_index2);
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
    }

    return err;
};

//  功能：如果key存在则删除
//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_del(
    const dfs_bt_root_t & old_bt_root,
    dfs_bt_root_t & new_bt_root,
    const uint64_t key_id,
    T * old_pt,
    bool & is_old_exist,
    dfs_key_id_index_t & key_id_index) //old or new
{
    dfs_btree_drill_t sf_drill_info;
    dfs_btree_drill_t id_drill_info;
    PCDU pcdu = NULL;
    uint64_t old_key_index = UNDEF_INDEX;
    uint64_t key_index2 = UNDEF_INDEX;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    new_bt_root = old_bt_root;  //复制后，如果前缀树没有更新，则其根节点不变
    key_id_index.init();   //在内部测试中，key_id输入值为UNDEF_ID或者期望的ID
    is_old_exist = false;

    if ((err = this->_idsf_search_id_by_id(
                   old_bt_root,
                   key_id,
                   id_drill_info,
                   old_key_index)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_search_id_by_id() returns 0x%x", err);
    }
    //不存在
    else if (UNDEF_INDEX == old_key_index) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "path id to be deleted doeesn't exist");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_ID);
    }
    //存在
    else {
        if ((err = sf_btree_t::_search_by_index(
                       old_bt_root,
                       old_key_index,
                       sf_drill_info,
                       key_index2)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::_search_by_index() returns 0x%x", err);
        } else if (key_index2 != old_key_index) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "key_index2 != old_key_index");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_DIFF_SF_ID_BTREE);
        } else if ((err = _get_exist_obj(old_key_index, pcdu)) != 0 || NULL == pcdu) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_exist_obj(old_key_index...) returns 0x%x", err);
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            //复制旧对象：一定要在更新B树之前，因为之后就对象可能被释放并调用了init()
            if (NULL != old_pt) {
                *old_pt = *((const T *)pcdu);
            }
            is_old_exist = true;

            if ((err = this->_idsf_del(
                           old_bt_root,
                           new_bt_root,
                           sf_drill_info,
                           id_drill_info)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_idsf_del() returns 0x%x", err);
            } else {
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "key_id=%lu", key_id);
        DF_WRITE_LOG_US(log_level,
                        "old_key_index=0x%lx, key_index2=0x%lx",
                        old_key_index, key_index2);
    }

    return err;
};

//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_get_exist_key_by_index(
    const dfs_bt_root_t & bt_root,
    const uint64_t key_index,
    K & keyt,
    PCDU & pcdu) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    keyt.init();

    if ((err = sf_btree_t::_sf_get_key_by_index(key_index, keyt.suffix, pcdu)) != 0 || NULL == pcdu) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_get_exist_obj(old_key_index...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else if ((err = _pfx_btree.pfx_get_name(bt_root, pcdu->get_prefix_id(), keyt.prefix)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_get_name() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "key_index=0x%lx", key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
    }

    return err;
};


//  return: 0 for success, other values for error
//  cmp: -1, 0, 1 for less than, equal, great than
template<typename T, uint32_t ROW_SIZE>
inline int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_compare_index_index(
    const dfs_bt_root_t & bt_root,
    const uint64_t key1_index,
    const uint64_t key2_index,
    int & cmp) const
{
    PCDU pcdu1 = NULL;
    PCDU pcdu2 = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 2;
    //Compare prefixes first
    if ((err = sf_btree_t::_get_exist_obj(key1_index, pcdu1)) != 0 || NULL == pcdu1) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_exist_obj(key1_index...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
    } else if ((err = sf_btree_t::_get_exist_obj(key2_index, pcdu2)) != 0 || NULL == pcdu2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_get_exist_obj(key2_index...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
    } else if ((err = _pfx_btree.pfx_compare_id_id(
                          bt_root,
                          pcdu1->get_prefix_id(),
                          pcdu2->get_prefix_id(),
                          cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.compare_id_id() returns 0x%x", err);
        DF_WRITE_LOG_US(log_level,
                        "prefixid1=%lu, prefix_id2=%lu",
                        pcdu1->get_prefix_id(), pcdu1->get_prefix_id());
    }
    //If the prefixes are equal, then compare the suffix
    else if (0 == cmp) {
        if ((err = sf_btree_t::_sf_compare_index_index(key1_index, key2_index, cmp)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_sf_compare_index_index() returns 0x%x", err);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key1_index=0x%lx, key2_index=0x%lx",
                        key1_index, key2_index);
    }

    return err;
};
//  return: 0 for success, other values for error
//  cmp: -1, 0, 1 for less than, equal, great than
template<typename T, uint32_t ROW_SIZE>
inline int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_compare_index_obj(
    const dfs_bt_root_t & bt_root,
    const uint64_t key1_index,
    const DU & duobj2,
    int & cmp) const
{
    PCDU pcdu1 = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 2;
    //Compare prefixes first
    if ((err = sf_btree_t::_get_exist_obj(key1_index, pcdu1)) != 0 || NULL == pcdu1) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_exist_obj(key1_index...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
    } else if ((err = _pfx_btree.pfx_compare_id_id(
                          bt_root,
                          pcdu1->get_prefix_id(),
                          duobj2.get_prefix_id(),
                          cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.compare_id_id() returns 0x%x", err);
        DF_WRITE_LOG_US(log_level,
                        "pcdu1->get_prefix_id()=%lu, duobj2.get_prefix_id()=%lu",
                        pcdu1->get_prefix_id(), duobj2.get_prefix_id());
    }
    //If the prefixes are equal, then compare the suffix
    else if (0 == cmp) {
        if ((err = sf_btree_t::_sf_compare_index_obj(key1_index, duobj2, cmp)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_sf_compare_index_obj() returns 0x%x", err);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key1_index=0x%lx, duobj2.get_prefix_id()=%lu",
                        key1_index, duobj2.get_prefix_id());
    }

    return err;
};
//return: 0 for success, other values for error
//cmp: -1, 0, 1 for less than, equal, great than
template<typename T, uint32_t ROW_SIZE>
inline int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_compare_index_key(
    const dfs_bt_root_t & bt_root,
    const uint64_t key1_index,
    const K & keyt2,
    int & cmp) const
{
    //DFS_BT_DEBUG_TRAP;

    PCDU pcdu1 = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 2;
    //Compare prefixes first
    if ((err = sf_btree_t::_get_exist_obj(key1_index, pcdu1)) != 0 || NULL == pcdu1) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::_get_exist_obj(key1_index...) returns 0x%x", err);
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_INVALID_INDEX);
    } else if((err = _pfx_btree.pfx_compare_id_name(
                         bt_root,
                         pcdu1->get_prefix_id(),
                         keyt2.prefix,
                         cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.compare_id_name() returns 0x%x", err);
        DF_WRITE_LOG_US(log_level, "pcdu1->get_prefix_id()=%lu", pcdu1->get_prefix_id());
    }
    //If the prefixes are equal, then compare the suffix
    else if (0 == cmp) {
        if ((err = sf_btree_t::_sf_compare_index_key(key1_index, keyt2.suffix, cmp)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_sf_compare_index_key() returns 0x%x", err);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "key1_index=0x%lx", key1_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "keyt2.prefix", keyt2.prefix.get_str(), keyt2.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "keyt2.suffix", keyt2.suffix.get_str(), keyt2.suffix.get_len());
    }

    return err;
};

//
//  return: 0 for success, other values for error
//  cmp: -1, 0, 1 for less than, equal, great than
//virtual int vir_compare_index_index(const uint64_t obj1_index, const uint64_t obj2_index, int & cmp) const
//{
//    return err;
//};
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::vir_compare_index_obj(
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
    } else if ((err = _fp_compare_index_obj(bt_root, obj1_index, *pcdu2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_fp_compare_index_obj() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj1_index=0x%lx, pobj2=0x%p, cmp=%d",
                        obj1_index, pobj2, cmp);
    }

    return err;
};
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::vir_compare_index_key(
    const dfs_bt_root_t & bt_root,
    const uint64_t obj1_index,
    const void * pkey2,
    int & cmp) const
{
    PCK pckey2 = (const K *)pkey2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    cmp = 1;
    if (NULL == pckey2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pckey2");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_NULL_POINTER);
    } else if ((err = _fp_compare_index_key(bt_root, obj1_index, *pckey2, cmp)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_fp_compare_index_key() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "obj1_index=0x%lx, pkey2=0x%p, cmp=%d",
                        obj1_index, pkey2, cmp);
    }

    return err;
};

template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::vir_search_ary_by_index(
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
    if (NULL == obj_index_ary) {
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
            } else if ((err = _fp_compare_index_index(
                                  bt_root,
                                  obj_index_ary[mid],
                                  obj2_index,
                                  cmp)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_compare_index_index() returns 0x%x", err);
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
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::vir_search_ary_by_obj(
    const dfs_bt_root_t & bt_root,
    const uint64_t * obj_index_ary,
    const int32_t obj_num,
    const void * pobj2,
    int & ins_pos,
    uint64_t & obj_index) const
{
    //DFS_BT_DEBUG_TRAP;

    PCDU pcdu2 = (PCDU)pobj2;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    int cmp = 1;
    int32_t pos1 = 0;
    int32_t pos2 = 0;
    int32_t mid = 0;

    ins_pos = 0;
    obj_index = UNDEF_INDEX;
    if (NULL == pobj2) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pobj2");
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
            } else if ((err = _fp_compare_index_obj(
                                  bt_root,
                                  obj_index_ary[mid],
                                  *pcdu2,
                                  cmp)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_compare_index_obj() returns 0x%x", err);
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
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::vir_search_ary_by_key(
    const dfs_bt_root_t & bt_root,
    const uint64_t * obj_index_ary,
    const int32_t obj_num,
    const void * pkey2,
    int & ins_pos,
    uint64_t & obj_index) const
{
    //DFS_BT_DEBUG_TRAP;

    PCK pckey2 = (const K *)pkey2;
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
        DF_WRITE_LOG_US(log_level, "NULL == pckey2");
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
            } else if ((err = _fp_compare_index_key(
                                  bt_root,
                                  obj_index_ary[mid],
                                  *pckey2,
                                  cmp)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_compare_index_key() returns 0x%x", err);
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

template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::vir_verify_index_ary(
    const dfs_bt_root_t & bt_root,
    const uint64_t * obj_index_ary,
    const int32_t obj_num,
    bool & is_inc_order) const
{
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
        is_inc_order = true;
        j = 0;
        ++j;
        while (j < obj_num) {
            if ((err = _fp_compare_index_index(
                           bt_root,
                           obj_index_ary[j-1],
                           obj_index_ary[j],
                           cmp)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_compare_index_index() returns 0x%x", err);
                is_inc_order = false;
                break;
            } else if (cmp >= 0) {
                is_inc_order = false;
                break;
            }
            ++j;
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


template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_verify_tree(
    const dfs_bt_root_t & bt_root) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = _pfx_btree.pfx_verify_tree(bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_verify_tree() returns 0x%x", err);
    } else if ((err = this->_idsf_verify_tree(bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_verify_tree() returns 0x%x", err);
    }

    return err;
};

//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_empty_btree_check(
    const dfs_bt_root_t & bt_root,
    const bool is_should_empty) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //在目前的实现下，_pfx_btree暂时不清除不用的对象，因此可能不空
    if ((err = _pfx_btree.pfx_empty_btree_check(bt_root, false)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_empty_btree_check() returns 0x%x", err);
    } else if ((err = this->_idsf_empty_btree_check(bt_root, is_should_empty)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_empty_btree_check() returns 0x%x", err);
    }

    return err;
};
//  return: 0 for success, other values for error
//  Should be called in locked state and without checkpointing or other COW(load or store)
template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_sanitary_check(
    const dfs_bt_root_t & bt_root) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = _pfx_btree.pfx_sanitary_check(bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_sanitary_check() returns: 0x%x", err);
    } else if ((err = this->_idsf_sanitary_check(bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_sanitary_check() returns: 0x%x", err);
    }

    return err;
};



template<typename T, uint32_t ROW_SIZE>
int _dfs_fp_btree_t<T, ROW_SIZE>::_fp_acquire_empty_root(
    dfs_bt_root_t & new_bt_root)
{
    int err = 0;
    int log_level = DF_UL_LOG_NONE;

    if ((err = this->_idsf_acquire_empty_root(new_bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_idsf_acquire_empty_root() returns 0x%x", err);
    } else if ((err = _pfx_btree.pfx_acquire_empty_root(new_bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_pfx_btree.pfx_acquire_empty_root() returns 0x%x", err);
    }

    return err;
};

template<typename T, uint32_t ROW_SIZE>
void _dfs_fp_btree_t<T, ROW_SIZE>::_fp_clear_statistic_info(void)
{
    _pfx_btree.pfx_clear_statistic_info();
    this->_idsf_clear_statistic_info();

    return;
};
template<typename T, uint32_t ROW_SIZE>
void _dfs_fp_btree_t<T, ROW_SIZE>::_fp_log_statistic_info(
    const int org_log_level,
    const char * filename,
    const int lineno,
    const char * funcname) const
{
    _pfx_btree.pfx_log_statistic_info(org_log_level, filename, lineno, funcname);
    this->_idsf_log_statistic_info(org_log_level, filename, lineno, funcname);
    return;
};
template<typename T, uint32_t ROW_SIZE>
void _dfs_fp_btree_t<T, ROW_SIZE>::_fp_log_debug_info(
    const int org_log_level,
    const char * filename,
    const int lineno,
    const char * funcname) const
{
    _pfx_btree.pfx_log_debug_info(org_log_level, filename, lineno, funcname);
    this->_idsf_log_debug_info(org_log_level, filename, lineno, funcname);
    return;
};

template<typename T, uint32_t ROW_SIZE>
uint64_t _dfs_fp_btree_t<T, ROW_SIZE>::_fp_get_mem_size(void) const
{
    return (this->_idsf_get_mem_size() + _pfx_btree.pfx_get_mem_size());
};



template<typename T = dfs_id_t, uint32_t ROW_SIZE = dfs_bt_const_t::DEF_ROW_SIZE>
class dfs_fp_btree_t :
    virtual public dfs_bt_const_t,
    virtual public dfs_fp_btree_const_t,
public dfs_btree_lock_t,
    //private dfs_rc_set_t<dfs_bt_root_t, dfs_bt_const_t::MAX_BT_RC_ROOT_NUM>,
private _dfs_fp_btree_t<T, ROW_SIZE>
{
protected:
    enum const_protected {
    };
private:
    typedef T *PT;
    typedef const T *PCT;
    typedef dfs_fp_du_t<T> DU;
    typedef DU *PDU;
    typedef const DU *PCDU;
    typedef dfs_path_t K;
    typedef _dfs_id_btree_t<DU, ROW_SIZE> id_btree_t;
    typedef _dfs_sf_btree_t<DU, dfs_name_t, ROW_SIZE> sf_btree_t;
    typedef _dfs_fp_btree_t<T, ROW_SIZE> fp_btree_t;
    typedef void * pvoid;
    typedef dfs_rc_set_t<dfs_bt_root_t, dfs_bt_const_t::MAX_BT_RC_ROOT_NUM>  rc_root_set_t;
private:
    dfs_init_t _init_state;
    rc_root_set_t _rc_root_set;
    dfs_rc_indexer_t _ckp_root_indexer;   //当前checkpoint根节点在rc_root_set_t中的位置
    volatile uint32_t _is_batch_mutate_mode;
    uint32_t _reserved;
public:
    dfs_fp_btree_t() {
        init();
    };
    virtual ~dfs_fp_btree_t() {
    };
    int init(void);
private:
    //Disable operator=() and copy constructor
    const dfs_fp_btree_t & operator=(const dfs_fp_btree_t & src);
    dfs_fp_btree_t(const dfs_fp_btree_t & src);
    //const dfs_fp_btree_t & operator=(const dfs_fp_btree_t & src)
    //{
    //    return *this;
    //};
    //dfs_fp_btree_t(const dfs_fp_btree_t & src)
    //{
    //};
private:
    //对当前的wr_root进行快照(需要更新_max_cow_mutation_counter)
    inline int _snapshot_wr_root_indexer(
        dfs_rc_indexer_t & rc_indexer) const;
    //获得当前wr_root，但不进行快照(也不更新_max_cow_mutation_counter)
    inline int _retrieve_wr_root_indexer(
        dfs_rc_indexer_t & rc_indexer,
        const dfs_bt_root_t *& pbt_root) const;
    //进行修改(insert/update...)后更新当前wr_root
    inline int _update_wr_root_indexer(const dfs_bt_root_t & bt_root);
    inline int _get_root(
        const dfs_rc_indexer_t & rc_indexer,
        const dfs_bt_root_t *& pbt_root) const;
protected:
    //增加/减少根节点引用计数
    virtual int vir_inc_root_ref_counter(const dfs_bt_root_t & bt_root) const;
    virtual int vir_dec_root_ref_counter(const dfs_bt_root_t & bt_root) const;
public:
    //功能：根据字符串搜索一个名字
    //输入：名字串(namet)/ID(key_id)
    //输出：如果搜索成功且pkey_id/pnamet/pt不是NULL，则对应项的值被复制到*pkey_id/*pnamet/*pt
    //	如果pnamet的缓冲区足够大，则名字串被复制且在尾部附加'\0'，否则只有str_len被设置。
    //返回：true如果对应的名字存在，false不存在。
    bool search(
        const dfs_name_t & namet,
        uint64_t * pkey_id,
        T * pt,
        const dfs_rc_indexer_t * prc_indexer = NULL) const ;
    bool search(
        const uint64_t key_id,
        dfs_name_t * pnamet,
        T * pt,
        const dfs_rc_indexer_t * prc_indexer = NULL) const;

    //  功能：is_batch_mutate_mode为true则允许batch mutation，seacrh等读操作时
    //        需要提供有效的dfs_rc_indexer_t；为false时禁止batch mutation，
    //        search等操作可以提供空的dfs_rc_indexer_t。
    //  说明：batch_mutate_mode为true时更新速度快，但search()等读操作的dfs_rc_indexer_t必须非空
    //  返回：0 for success, other values for error
    int set_batch_mutate_mode(
        const bool is_batch_mutate_mode,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
    bool get_batch_mutate_mode(void) const;
    //
    //  功能：把项tobj插入或更新到B树中。
    //        insert：如果T的id为UNDEF_ID则为其申请一个新ID。
    //                如果T的id在B树中不存在，则加入B树，否则不做操作并返回UNDEF_ID。
    //        update：如果T的id在B树存在就更新其值为tobj，否则返回UNDEF_ID。
    //        insert_update：先执行insert，如果该项存在就再执行update。
    //        如果项存在且pt非空，则其原值被复制到*pt中
    //  输入：项tobj
    //  返回：项的id(操作成功)或UNDEF_ID
    bool insert(
        const dfs_name_t & namet,
        uint64_t & key_id,
        const T * pct,
        T * old_pt = NULL,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
    bool update(
        const dfs_name_t & namet,
        const T * pct,
        T * old_pt = NULL,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
    bool update(
        const uint64_t key_id,
        const T * pct,
        T * old_pt = NULL,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
    bool insert_update(
        const dfs_name_t & namet,
        uint64_t & key_id,
        const T * pct,
        T * old_pt = NULL,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
    //
    //功能：如果一个项存在，则删除；否则，不做任何事情。
    //        如果原来项存在且old_pt非空，则其值复制到*old_pt。
    //返回：true如果项存在，false如果项不存在
    bool del(
        const dfs_name_t & namet,
        T * old_pt = NULL,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
    bool del(
        const uint64_t key_id,
        T * old_pt = NULL,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
public: //obsolete APIs
    int set_serial_read_write_state(
        const bool is_serial_read_write,
        bool & is_success,
        const dfs_btree_lock_hold_t * plock_hold = NULL) {
        return 0;
    };
    int get_serial_read_write_state(
        bool & is_serial_read_write,
        const dfs_btree_lock_hold_t * plock_hold = NULL) const {
        return 0;
    };
    //
public:
    //  return: 0 for success, other values for error
    int get_total_num(
        uint64_t & total_key_num,
        uint64_t & total_leaf_node_num,
        uint64_t & total_mid_node_num) const;
    //  return: 0 for success, other values for error
    int get_in_use_num_in_2d_ary(
        uint64_t & in_use_key_num,
        uint64_t & in_use_leaf_node_num,
        uint64_t & in_use_mid_node_num) const;
    //  return: 0 for success, other values for error
    int empty_btree_check(
        const bool is_should_empty,
        const dfs_btree_lock_hold_t * plock_hold = NULL) const;
    //  return: 0 for success, other values for error
    //  Should be called in locked state and without checkpointing or other COW(load or store)
    int sanitary_check(const dfs_btree_lock_hold_t * plock_hold = NULL);
    void clear_statistic_info(const dfs_btree_lock_hold_t * plock_hold = NULL);
    //should be called in locked state because of calling dfs_bt2d_ary_t::_free_list etc
    void log_statistic_info(
        const int log_level,
        const char * filename,
        const int lineno,
        const char * funcname,
        const dfs_btree_lock_hold_t * plock_hold = NULL) const;
    void log_debug_info(
        const int org_log_level,
        const char * filename,
        const int lineno,
        const char * funcname,
        const dfs_btree_lock_hold_t * plock_hold = NULL) const;
    //should be called in locked state because of calling dfs_bt2d_ary_t::_free_list etc
    uint64_t get_mem_size(const dfs_btree_lock_hold_t * plock_hold = NULL) const;
public:
    //
    //  功能：获得B树最小元素
    //  输出：如果pkey_id/pnamet/pt不是NULL且B树非空，则对应项的值被复制到*pkey_id/*pnamet/*pt
    //        如果pnamet的缓冲区足够大，则名字串被复制且在尾部附加'\0'，否则只有str_len被设置。
    //  返回：true如果最小的名字存在，false不存在(B树为空)
    bool get_smallest(
        uint64_t * pkey_id,
        dfs_name_t * pnamet,
        T * pt,
        const dfs_rc_indexer_t * prc_indexer = NULL) const;
    //  功能：获得B树最大元素
    //  输出：如果pkey_id/pnamet/pt不是NULL且B树非空，则对应项的值被复制到*pkey_id/*pnamet/*pt
    //        如果pnamet的缓冲区足够大，则名字串被复制且在尾部附加'\0'，否则只有str_len被设置。
    //  返回：true如果最大的名字存在，false不存在(B树为空)
    bool get_largest(
        uint64_t * pkey_id,
        dfs_name_t * pnamet,
        T * pt,
        const dfs_rc_indexer_t * prc_indexer = NULL) const;
    //
    //  功能：获得刚好比输入的名字小的名字的key_id
    //  输入：key_id：B树中存在的名字的key_id；
    //        namet：不需要在B树中存在
    //  输出：如果pkey_id/pnamet/pt，则对应项的值被复制到*pkey_id/*pnamet/*pt
    //        如果pnamet的缓冲区足够大，则名字串被复制且在尾部附加'\0'，否则只有str_len被设置。
    //  返回：true如果有更小的元素，否则返回false
    bool get_smaller(
        const dfs_name_t & namet,
        uint64_t * pkey_id,
        dfs_name_t * pnamet,
        T * pt,
        dfs_btree_drill_t * pdrill_info = NULL,
        const dfs_rc_indexer_t * prc_indexer = NULL) const;
    bool get_smaller(
        const uint64_t key_id,
        uint64_t * pkey_id,
        dfs_name_t * pnamet,
        T * pt,
        dfs_btree_drill_t * pdrill_info = NULL,
        const dfs_rc_indexer_t * prc_indexer = NULL) const;
    //
    //  功能：获得刚好比输入的名字大的名字的key_id
    //  输入：key_id：B树中存在的名字的key_id；
    //        namet：不需要在B树中存在
    //  输出：如果pkey_id/pnamet/pt不是NULL，则对应项的值被复制到*pkey_id/*pnamet/*pt
    //        如果pnamet的缓冲区足够大，则名字串被复制且在尾部附加'\0'，否则只有str_len被设置。
    //  返回：true如果有更大的元素，否则返回false
    bool get_larger(
        const dfs_name_t & namet,
        uint64_t * pkey_id,
        dfs_name_t * pnamet,
        T * pt,
        dfs_btree_drill_t * pdrill_info = NULL,
        const dfs_rc_indexer_t * prc_indexer = NULL) const;
    bool get_larger(
        const uint64_t key_id,
        uint64_t * pkey_id,
        dfs_name_t * pnamet,
        T * pt,
        dfs_btree_drill_t * pdrill_info = NULL,
        const dfs_rc_indexer_t * prc_indexer = NULL) const;
    //  功能：清空整个树。
    //  返回：0 for success, other values for error
    int clear(const dfs_btree_lock_hold_t * plock_hold = NULL);
    //
    //功能：验证整个树的顺序和每个节点分支个数
    //返回：返回0如果验证通过，返回非0(错误代码)如果验证失败
    int verify(const dfs_rc_indexer_t * prc_indexer = NULL) const;
public:
    //功能：设置或取消checkpoint状态
    //输入：is_cancel：true取消checkpoint点及进行中的checkpoing操作，false设置一个新的checkpoint点
    //      参数max_wait_microseconds不再使用
    //建议：仅用此函数取消进行中的checkpoint，通过snapshot()获得一个checkpoint
    //返回：0: 设置或取消成功，取消不存在的checkpoint状态也返回0
    int set_checkpoint_state(
        const bool is_cancel,
        const int64_t max_wait_microseconds,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
public: //obsolete APIs
    int get_checkpoint_state(void) const {
        return fp_btree_t::sf_btree_t::_bt_is_cancel_checkpointing();
    };
public:
    //获得当前B树的一个快照，不再使用后尽快调用rc_indexer.init()以释放快照占用的内存
    int snapshot(
        dfs_rc_indexer_t & rc_indexer,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
    //
    //  功能：把已经设置的checkpoint存储到文件中
    //  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
    //  数据： first prefix(first fp, then id), then suffix(first fp, then id))
    //  返回：0 for success, other values for error
    int store_checkpointing(
        const dfs_write_proc_t write_proc,
        void * file_info,
        const dfs_btree_lock_hold_t * plock_hold = NULL,
        const dfs_rc_indexer_t * prc_indexer = NULL);
    //
    //  功能：把先前存储在文件的checkpoint加载到内存中
    //  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
    //  数据： first prefix(first fp, then id), then suffix(first fp, then id))
    //  返回：0 for success, other values for error
    int load_checkpointing(
        const dfs_read_proc_t read_proc,
        void * file_info,
        const dfs_btree_lock_hold_t * plock_hold = NULL);
public:
    //Begin: Obsolete interface:
    uint64_t add(const char * name, const T * pct = NULL) {
        uint64_t ret_name_id = UNDEF_ID;
        dfs_name_t namet;

        namet.set_name(name,(uint32_t)strlen(name));
        bool is_success = insert_update(namet, ret_name_id, pct);
        return (is_success) ? ret_name_id : UNDEF_ID;
    };
    bool get_name(
        const uint64_t key_id,
        char * buf,
        const uint32_t buf_size,
        uint32_t & str_len) const {
        dfs_name_t namet;
        bool is_success = false;

        namet.set_buf(buf, buf_size);
        namet.set_name_len(0);
        is_success = search(key_id, &namet, NULL);
        str_len = namet.get_len();

        return is_success;
    };
    bool get_name_t_unit(
        const uint64_t key_id,
        T & tobj) const {
        return search(key_id, NULL, &tobj);
    };
    uint64_t search(const char * name, T * pt = NULL) {
        dfs_name_t namet;
        uint64_t ret_name_id = UNDEF_ID;
        bool is_success = false;

        namet.set_name(name,(uint32_t)strlen(name));
        is_success = search(namet, &ret_name_id, pt);

        return (is_success) ? ret_name_id : UNDEF_ID;
    };
    uint64_t get_smaller(const char * name, T * pt = NULL) const {
        uint64_t ret_name_id = UNDEF_ID;
        dfs_name_t namet;
        bool is_success = false;

        namet.set_name(name,(uint32_t)strlen(name));
        is_success = get_smaller(namet, &ret_name_id, NULL, pt);

        return (is_success) ? ret_name_id : UNDEF_ID;
    };
    uint64_t get_smaller(const uint64_t key_id, T * pt = NULL) const {
        uint64_t ret_name_id = UNDEF_ID;
        bool is_success = false;

        is_success = get_smaller(key_id, &ret_name_id, NULL, pt);

        return (is_success) ? ret_name_id : UNDEF_ID;
    };
    uint64_t get_larger(const char * name, T * pt = NULL) const {
        uint64_t ret_name_id = UNDEF_ID;
        dfs_name_t namet;
        bool is_success = false;

        namet.set_name(name,(uint32_t)strlen(name));
        is_success = get_larger(namet, &ret_name_id, NULL, pt);

        return (is_success) ? ret_name_id : UNDEF_ID;
    }
    uint64_t get_larger(const uint64_t key_id, T * pt = NULL) const {
        uint64_t ret_name_id = UNDEF_ID;
        bool is_success = false;

        is_success = get_larger(key_id, &ret_name_id, NULL, pt);

        return (is_success) ? ret_name_id : UNDEF_ID;
    };
    uint64_t get_smallest(T * pt = NULL) const {
        uint64_t ret_name_id = UNDEF_ID;
        bool is_exist = get_smallest(&ret_name_id, NULL, pt);
        return (is_exist) ? ret_name_id : UNDEF_ID;
    };
    uint64_t get_largest(T * pt = NULL) const {
        uint64_t ret_name_id = UNDEF_ID;
        bool is_exist = get_largest(&ret_name_id, NULL, pt);
        return (is_exist) ? ret_name_id : UNDEF_ID;
    };
    bool del(const char * name) {
        dfs_name_t namet;

        namet.set_name(name,(uint32_t)strlen(name));

        return del(namet);
    };
    //End: Obsolete interface:
};

template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::init(void)
{
    //const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    //dfs_rc_indexer_t old_rc_indexer;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (_init_state.is_not_init_yet()) {
        _is_batch_mutate_mode = 0;
        _reserved = 0;

        if ((err = dfs_btree_lock_t::init()) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "dfs_btree_lock_t::init() returns 0x%x", err);
        } else if ((err = _rc_root_set.init((dfs_btree_t *)this)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_rc_root_set::init() returns 0x%x", err);
        } else if ((err = _dfs_fp_btree_t<T, ROW_SIZE>::init(
                        FP_PREFIX_ID_BT_POS,
                        FP_PREFIX_SF_BT_POS,
                        FP_SUFFIX_ID_BT_POS,
                        FP_SUFFIX_SF_BT_POS,
                        new_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level,
                            "_dfs_fp_btree_t<T, ROW_SIZE>::init() returns 0x%x",
                            err);
        }
        //把rc_root_set_t的当前indexer设置为修改后的root
        else if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
        }

        _init_state.set_init_result(err);
    } else {
        err = _init_state.get_init_result();
    }

    return err;
};


//对当前的wr_root进行快照(需要更新_max_cow_mutation_counter)
template<typename T, uint32_t ROW_SIZE>
inline int dfs_fp_btree_t<T, ROW_SIZE>::_snapshot_wr_root_indexer(
    dfs_rc_indexer_t & rc_indexer) const
{
    const dfs_bt_root_t * pbt_root = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    //更新_max_cow_mutation_counter
    this->_fp_update_max_cow_mutation_counter();

    if ((err = _rc_root_set.retrieve_cur_index(rc_indexer)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_rc_root_set.retrieve_cur_index() returns 0x%x", err);
    } else if ((err = _get_root(rc_indexer, pbt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
    } else if (NULL == pbt_root) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else {
        ;
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "rc_indexer._index=%lu, pbt_root=0x%p",
                        ((dfs_rc_indexer_ext_t *)&rc_indexer)->get_index(), pbt_root);
    }

    return err;
};

//获得当前wr_root，但不进行快照(也不更新_max_cow_mutation_counter)
template<typename T, uint32_t ROW_SIZE>
inline int dfs_fp_btree_t<T, ROW_SIZE>::_retrieve_wr_root_indexer(
    dfs_rc_indexer_t & rc_indexer,
    const dfs_bt_root_t *& pbt_root) const

{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = _rc_root_set.retrieve_cur_index(rc_indexer)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_rc_root_set.retrieve_cur_index() returns 0x%x", err);
    } else if ((err = _get_root(rc_indexer, pbt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
    } else {
        ;
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "rc_indexer._index=%lu, pbt_root=0x%p",
                        ((dfs_rc_indexer_ext_t *)&rc_indexer)->get_index(), pbt_root);
    }

    return err;
};


//进行修改(insert/update...)后更新当前wr_root
template<typename T, uint32_t ROW_SIZE>
inline int dfs_fp_btree_t<T, ROW_SIZE>::_update_wr_root_indexer(const dfs_bt_root_t & bt_root)
{
    dfs_rc_indexer_t new_rc_indexer;
    dfs_rc_indexer_t old_rc_indexer;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = _rc_root_set.acquire_t(bt_root, new_rc_indexer)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_rc_root_set.update_cur_index() returns 0x%x", err);
    } else if ((err = _rc_root_set.update_cur_index(new_rc_indexer, old_rc_indexer)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_rc_root_set.update_cur_index() returns 0x%x", err);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "new_rc_indexer._index=%lu, old_rc_indexer._index=%lu",
                        ((const dfs_rc_indexer_ext_t *)&new_rc_indexer)->get_index(),
                        ((const dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
    }

    return err;
};

//根据indexer获得dfs_bt_root
template<typename T, uint32_t ROW_SIZE>
inline int dfs_fp_btree_t<T, ROW_SIZE>::_get_root(
    const dfs_rc_indexer_t & rc_indexer,
    const dfs_bt_root_t *& pbt_root) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (!rc_indexer.is_valid()) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "!rc_indexer.is_valid()");
        err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_INVALID_INDEX);
    } else if ((err = _rc_root_set.get_t_unit(rc_indexer, pbt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_rc_root_set.get_t_unit() returns 0x%x", err);
    } else if (NULL == pbt_root) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "rc_indexer._index=%lu, pbt_root=0x%p",
                        ((dfs_rc_indexer_ext_t *)&rc_indexer)->get_index(), pbt_root);
    }

    return err;
};

//增加/减少根节点引用计数
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::vir_inc_root_ref_counter(
    const dfs_bt_root_t & bt_root) const
{
    return this->_fp_inc_root_ref_counter(bt_root);
};
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::vir_dec_root_ref_counter(
    const dfs_bt_root_t & bt_root) const
{
    return this->_fp_dec_root_ref_counter(bt_root);
};

//
//  功能：搜索一个名字，如果找到且pkey_id/pt非空，则复制其key_id/T值到*pkey_id/*pt
//  输入：以’\0’结尾的名字或名字的ID
//  返回：true如果对应的名字存在，false不存在。
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::search(
    const dfs_name_t & namet,
    uint64_t * pkey_id,
    T * pt,
    const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    PCDU pcdu = NULL;
    uint64_t key_index = UNDEF_INDEX;
    dfs_btree_drill_t drill_info;
    K keyt;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL != pkey_id) {
        *pkey_id = UNDEF_ID;
    }
    if (NULL != pt) {
        pt->init();
    }

    if (namet.get_len() <= 0) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.get_len() <= 0");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if ('/' == (namet.get_str())[namet.get_len()-1]) { //不允许目录
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "input is directory");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (!namet.split_filepath(keyt.prefix, keyt.suffix)) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "input path can't be splitted");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_search_by_key(
                              *pbt_root,
                              keyt,
                              drill_info,
                              key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_search_by_key() returns 0x%x", err);
        }
        //存在
        else if (UNDEF_INDEX != key_index) {
            if ((err = fp_btree_t::sf_btree_t::_get_exist_obj(key_index, pcdu)) != 0
                || NULL == pcdu) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_get_exist_obj() returns 0x%x", err);
                err = DF_BT_SET_INTERNAL_ERR(this->ERRNO_BT_INVALID_INDEX);
            } else {
                if (NULL != pkey_id) {
                    *pkey_id = pcdu->get_id();
                }
                if (NULL != pt) {
                    *pt = *((const T *)pcdu);
                }
            }
        }
        //不存在
        else {
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "key_index=0x%lx", key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "namet", namet.get_str(), namet.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }

    return (0 == err && UNDEF_INDEX != key_index);
};


//
//  功能：获得一个名字的字符串，如果名字存在，若缓冲区非空且足够大，则复制其名字，否则只设置str_len。
//  输入：key_id：名字的key_id
//        buf：存放名字的缓冲区，可以为空
//        buf_size：缓冲区尺寸
//  输出：str_len获得的名字的长度(不包括结尾的’\0’)。如果str_len >= name_size，则说明名字缓冲区太小
//  返回：true如果对应的名字存在，false不存在。
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::search(
    const uint64_t key_id,
    dfs_name_t * pnamet,
    T * pt,
    const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    PCDU pcdu = NULL;
    uint64_t key_index = UNDEF_INDEX;
    dfs_btree_drill_t drill_info;
    K keyt;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL != pnamet) {
        pnamet->set_name_len(0);
    }
    if (NULL != pt) {
        pt->init();
    }

    if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = fp_btree_t::_idsf_search_id_by_id(
                              *pbt_root,
                              key_id,
                              drill_info,
                              key_index)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_idsf_search_id_by_id() returns 0x%x", err);
        }
        //存在
        else if (UNDEF_INDEX != key_index) {
            if ((err = this->_fp_get_exist_key_by_index(*pbt_root, key_index, keyt, pcdu)) != 0
                || NULL == pcdu) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_get_exist_key_by_index() returns 0x%x", err);
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
            } else {
                if (NULL != pnamet) {
                    pnamet->join(keyt.prefix, keyt.suffix);
                }

                if (NULL != pt) {
                    *pt = *((const T *)pcdu);
                }
            }
        }
        //不存在
        {
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_index=0x%lx, key_id=%lu",
                        key_index, key_id);
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }

    return (0 == err && UNDEF_INDEX != key_index);
};



//  功能：is_batch_mutate_mode为true则允许batch mutation，seacrh等读操作时
//        需要提供有效的dfs_rc_indexer_t；为false时禁止batch mutation，
//        search等操作可以提供空的dfs_rc_indexer_t。
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::set_batch_mutate_mode(
    const bool is_batch_mutate_mode,
    const dfs_btree_lock_hold_t * plock_hold)
{
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //如果是非batch mutate mode，则max_cow_mutation_counter需要更新到
        //当前mutation_counter以免读对象被原子操作修改。
        if (!is_batch_mutate_mode) {
            this->_fp_update_max_cow_mutation_counter();
        }

        atomic_exchange(&_is_batch_mutate_mode, (is_batch_mutate_mode) ? 1 : 0);

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    return err;
};

template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::get_batch_mutate_mode(void) const
{
    return (0 != _is_batch_mutate_mode);
};


//
//  功能：把项tobj插入或更新到B树中。
//        insert：如果T的id为UNDEF_ID则为其申请一个新ID。如果T的id在B树中不存在，则加入B树，否则不做操作。
//        update：如果T的id在B树存在就更新其值为tobj，否则返回UNDEF_ID。
//        insert_update：先执行insert，如果该项存在就再执行update。
//        如果原来项存在且old_pt非空，则其值复制到*old_pt。
//  输入：
//        如果str_len <= 0，则name以’\0’结尾的名字，否则str_len为其长度
//	输出：key_id：name的ID
//
//  返回：true for success, false for failure
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::insert(
    const dfs_name_t & namet,
    uint64_t & key_id,
    const T * pct,
    T * old_pt,
    const dfs_btree_lock_hold_t * plock_hold)
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    dfs_rc_indexer_t old_rc_indexer;
    dfs_key_id_index_t key_id_index;
    K keyt;
    DU new_duobj;
    DU old_duobj;
    bool is_old_exist = false;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if (NULL != pct) {
        new_duobj = *pct;
    } else {
        new_duobj.init();
    }
    old_duobj.init();
    //tobj.set_id(UNDEF_ID);    //

    //key_id = UNDEF_ID;   //在内部测试中，key_id输入值为UNDEF_ID或者期望的ID
    if (namet.get_len() <= 0) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.get_len() <= 0");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if ('/' == (namet.get_str())[namet.get_len()-1]) { //不允许目录
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "input is directory");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (!namet.split_filepath(keyt.prefix, keyt.suffix)) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "input path can't be splitted");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //如果不是batch mode，则同时更新_max_cow_mutation_counter从而禁止修改时的指针原子替换
        this->_fp_update_mutation_counter(get_batch_mutate_mode());

        if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
        } else if (NULL == pold_wr_bt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            new_wr_bt_root = *pold_wr_bt_root;
            if ((err = this->_fp_insert(
                           *pold_wr_bt_root,
                           new_wr_bt_root,
                           keyt,
                           new_duobj,
                           &old_duobj,
                           is_old_exist,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_insert() returns 0x%x", err);
            }
            //原来项存在
            else if (is_old_exist) {
            }
            //原来项不存在
            else { //if (NULL == pcdu)
                //把rc_root_set_t的当前indexer设置为修改后的root
                if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
                }
            }

            if (0 == err) {
                key_id = key_id_index.key_id;
            }
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    if (NULL != old_pt) {
        *old_pt = *((const T *)&old_duobj);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_id_index: key_id/index=%lu/0x%lx",
                        key_id_index.key_id,
                        key_id_index.key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "namet", namet.get_str(), namet.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "old_rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
        DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode()=%d", get_batch_mutate_mode());
    }

    return (0 == err && !is_old_exist);
};
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::update(
    const dfs_name_t & namet,
    const T * pct,
    T * old_pt,
    const dfs_btree_lock_hold_t * plock_hold)
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    dfs_rc_indexer_t old_rc_indexer;
    dfs_key_id_index_t key_id_index;
    DU new_duobj;
    DU old_duobj;
    bool is_old_exist = false;
    K keyt;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if (NULL != pct) {
        new_duobj = *pct;
    } else {
        new_duobj.init();
    }
    old_duobj.init();

    if (namet.get_len() <= 0) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.get_len() <= 0");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if('/' == (namet.get_str())[namet.get_len()-1]) { //不允许目录
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "Input is dir instead of file");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (!namet.split_filepath(keyt.prefix, keyt.suffix)) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.split_filepath() returns false");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //如果不是batch mode，则同时更新_max_cow_mutation_counter从而禁止修改时的指针原子替换
        this->_fp_update_mutation_counter(get_batch_mutate_mode());

        if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
        } else if (NULL == pold_wr_bt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            new_wr_bt_root = *pold_wr_bt_root;
            if ((err = this->_fp_update(
                           *pold_wr_bt_root,
                           new_wr_bt_root,
                           keyt,
                           new_duobj,
                           &old_duobj,
                           is_old_exist,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_update() returns 0x%x", err);
            }
            //原来项存在
            else if (is_old_exist) {
                //把rc_root_set_t的当前indexer设置为修改后的root
                if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
                }
            }
            //原来项不存在
            else {
                ;
            }
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    if (NULL != old_pt) {
        *old_pt = *((const T *)&old_duobj);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_id_index: key_id/index=%lu/0x%lx",
                        key_id_index.key_id,
                        key_id_index.key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "namet", namet.get_str(), namet.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "old_rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
        DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode()=%d", get_batch_mutate_mode());
    }

    return (0 == err && is_old_exist);
};
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::update(
    const uint64_t key_id,
    const T * pct,
    T * old_pt,
    const dfs_btree_lock_hold_t * plock_hold)
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    dfs_rc_indexer_t old_rc_indexer;
    dfs_key_id_index_t key_id_index;
    DU new_duobj;
    DU old_duobj;
    bool is_old_exist = false;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if (NULL != pct) {
        new_duobj = *pct;
    } else {
        new_duobj.init();
    }
    new_duobj.set_id(key_id);
    old_duobj.init();

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //如果不是batch mode，则同时更新_max_cow_mutation_counter从而禁止修改时的指针原子替换
        this->_fp_update_mutation_counter(get_batch_mutate_mode());

        if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
        } else if (NULL == pold_wr_bt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            new_wr_bt_root = *pold_wr_bt_root;
            if ((err = this->_fp_update(
                           *pold_wr_bt_root,
                           new_wr_bt_root,
                           key_id,
                           new_duobj,
                           &old_duobj,
                           is_old_exist,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_update() returns 0x%x", err);
            }
            //原来项存在
            else if (is_old_exist) {
                //把rc_root_set_t的当前indexer设置为修改后的root
                if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
                }
            }
            //原来项不存在
            else {
                ;
            }
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    if (NULL != old_pt) {
        *old_pt = *((const T *)&old_duobj);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "key_id=%lu", key_id);
        DF_WRITE_LOG_US(log_level,
                        "old_rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
        DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode()=%d", get_batch_mutate_mode());
    }

    return (0 == err && is_old_exist);
};
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::insert_update(
    const dfs_name_t & namet,
    uint64_t & key_id,
    const T * pct,
    T * old_pt,
    const dfs_btree_lock_hold_t * plock_hold)
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    dfs_rc_indexer_t old_rc_indexer;
    dfs_key_id_index_t key_id_index;
    DU new_duobj;
    DU old_duobj;
    bool is_old_exist = false;
    K keyt;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    key_id = UNDEF_ID;

    //if (NULL != old_pt)
    //{
    //    old_pt->init();
    //}
    if (NULL != pct) {
        new_duobj = *pct;
    } else {
        new_duobj.init();
    }
    old_duobj.init();

    if (namet.get_len() <= 0) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.get_len() <= 0");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if('/' == (namet.get_str())[namet.get_len()-1]) { //不允许目录
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "Input is dir instead of file");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (!namet.split_filepath(keyt.prefix, keyt.suffix)) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.split_filepath() returns false");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //如果不是batch mode，则同时更新_max_cow_mutation_counter从而禁止修改时的指针原子替换
        this->_fp_update_mutation_counter(get_batch_mutate_mode());

        if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
        } else if (NULL == pold_wr_bt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            new_wr_bt_root = *pold_wr_bt_root;
            if ((err = this->_fp_insert_update(
                           *pold_wr_bt_root,
                           new_wr_bt_root,
                           keyt,
                           new_duobj,
                           &old_duobj,
                           is_old_exist,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_insert_update() returns 0x%x", err);
            }
            //把rc_root_set_t的当前indexer设置为修改后的root
            else if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
            } else {
                key_id = key_id_index.key_id;
            }
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    if (NULL != old_pt) {
        *old_pt = *((const T *)&old_duobj);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "key_id_index: key_id/index=%lu/0x%lx",
                        key_id_index.key_id,
                        key_id_index.key_index);
        BT_DF_WRITE_LOG_BYTES(log_level, "namet", namet.get_str(), namet.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "old_rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
        DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode()=%d", get_batch_mutate_mode());
    }

    return (0 == err);
};


//功能：如果一个项存在，则删除；否则，不做任何事情。
//        如果项存在且pt非空，则其原值被复制到*pt中
//返回：true如果项存在，false如果项不存在
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::del(
    const dfs_name_t & namet,
    T * old_pt,
    const dfs_btree_lock_hold_t * plock_hold)
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    dfs_rc_indexer_t old_rc_indexer;
    dfs_key_id_index_t key_id_index;
    K keyt;
    //DU new_duobj;
    DU old_duobj;
    bool is_old_exist = false;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    //if (NULL != old_pt)
    //{
    //    old_pt->init();
    //}
    old_duobj.init();

    if (namet.get_len() <= 0) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.get_len() <= 0");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if ('/' == (namet.get_str())[namet.get_len()-1]) { //不允许目录
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "input is directory");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (!namet.split_filepath(keyt.prefix, keyt.suffix)) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "input path can't be splitted");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //如果不是batch mode，则同时更新_max_cow_mutation_counter从而禁止修改时的指针原子替换
        this->_fp_update_mutation_counter(get_batch_mutate_mode());

        if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
        } else if (NULL == pold_wr_bt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            new_wr_bt_root = *pold_wr_bt_root;
            if ((err = this->_fp_del(
                           *pold_wr_bt_root,
                           new_wr_bt_root,
                           keyt,
                           &old_duobj,
                           is_old_exist,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_del() returns 0x%x", err);
            }
            //原来项存在
            else if (is_old_exist) {
                //把rc_root_set_t的当前indexer设置为修改后的root
                if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
                }
            }
            //原来项不存在
            else {
            }
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    if (NULL != old_pt) {
        *old_pt = *((const T *)&old_duobj);
    }

    if (DF_UL_LOG_NONE != log_level) {
        BT_DF_WRITE_LOG_BYTES(log_level, "namet", namet.get_str(), namet.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "prefix", keyt.prefix.get_str(), keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "suffix", keyt.suffix.get_str(), keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "old_rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
        DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode()=%d", get_batch_mutate_mode());
    }

    return (0 == err && is_old_exist);
};
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::del(
    const uint64_t key_id,
    T * old_pt,
    const dfs_btree_lock_hold_t * plock_hold)
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    dfs_rc_indexer_t old_rc_indexer;
    dfs_key_id_index_t key_id_index;
    DU old_duobj;
    bool is_old_exist = false;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    //if (NULL != old_pt)
    //{
    //    old_pt->init();
    //}
    old_duobj.init();

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //如果不是batch mode，则同时更新_max_cow_mutation_counter从而禁止修改时的指针原子替换
        this->_fp_update_mutation_counter(get_batch_mutate_mode());

        if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
        } else if (NULL == pold_wr_bt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            new_wr_bt_root = *pold_wr_bt_root;
            if ((err = this->_fp_del(
                           *pold_wr_bt_root,
                           new_wr_bt_root,
                           key_id,
                           &old_duobj,
                           is_old_exist,
                           key_id_index)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_del() returns 0x%x", err);
            }
            //原来项存在
            else if (is_old_exist) {
                //把rc_root_set_t的当前indexer设置为修改后的root
                if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
                    log_level = DF_UL_LOG_FATAL;
                    DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
                }
            }
            //原来项不存在
            else {
            }
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    if (NULL != old_pt) {
        *old_pt = *((const T *)&old_duobj);
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "key_id=%lu", key_id);
        DF_WRITE_LOG_US(log_level,
                        "old_rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
        DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode()=%d", get_batch_mutate_mode());
    }

    return (0 == err && is_old_exist);
};

//  功能：清空整个树。
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::clear(
    const dfs_btree_lock_hold_t * plock_hold)
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    dfs_rc_indexer_t old_rc_indexer;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //如果不是batch mode，则同时更新_max_cow_mutation_counter从而禁止修改时的指针原子替换
        this->_fp_update_mutation_counter(false); //get_batch_mutate_mode();

        if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
        } else if (NULL == pold_wr_bt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else {
            //先复制根节点以便获得mutation counter, next_allocate_id等信息
            new_wr_bt_root = *pold_wr_bt_root;
            if ((err = this->_fp_acquire_empty_root(new_wr_bt_root)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_acquire_empty_root() returns 0x%x", err);
            }
            //把rc_root_set_t的当前indexer设置为修改后的root
            else if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
            }

            if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
                err = err1;
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "old_rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
    }


    return err;
};


//获得当前B树的一个快照
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::snapshot(
    dfs_rc_indexer_t & rc_indexer,
    const dfs_btree_lock_hold_t * plock_hold)
{
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //把max_cow_mutation_counter更新到当前mutation_counter以免对象被原子操作修改。
        this->_fp_update_max_cow_mutation_counter();

        if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&rc_indexer)->get_index());
    }


    return err;
};




//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::get_total_num(
    uint64_t & total_key_num,
    uint64_t & total_leaf_node_num,
    uint64_t & total_mid_node_num) const
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    //dfs_bt_root_t * pnew_wr_bt_root = NULL;
    dfs_rc_indexer_t old_rc_indexer;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
    } else if (NULL == pold_wr_bt_root) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
        err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
    } else {
        if ((err = sf_btree_t::bt_get_total_num(
                       *pold_wr_bt_root,
                       total_key_num,
                       total_leaf_node_num,
                       total_mid_node_num)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "sf_btree_t::bt_get_total_num() returns 0x%x", err);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "old_rc_indexer._index=%lu",
                        ((dfs_rc_indexer_ext_t *)&old_rc_indexer)->get_index());
    }

    return err;
};

//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::get_in_use_num_in_2d_ary(
    uint64_t & in_use_key_num,
    uint64_t & in_use_leaf_node_num,
    uint64_t & in_use_mid_node_num) const
{
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if ((err = sf_btree_t::sf_get_in_use_num_in_2d_ary(
                   in_use_key_num,
                   in_use_leaf_node_num,
                   in_use_mid_node_num)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, "sf_btree_t::sf_get_in_use_num_in_2d_ary() returns 0x%x", err);
    }

    return err;
};
//  return: 0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::empty_btree_check(
    const bool is_should_empty,
    const dfs_btree_lock_hold_t * plock_hold) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
    } else {
        if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else if ((err = _get_root(rc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if (is_should_empty && _rc_root_set.get_in_use_num() != 1) {
            DFS_BT_DEBUG_TRAP;

            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level,
                            "_rc_root_set.get_in_use_num() (=%ld) != 1",
                            _rc_root_set.get_in_use_num());
            DF_WRITE_LOG_US(log_level,
                            "_ckp_root_indexer=%ld",
                            ((dfs_rc_indexer_ext_t *)&_ckp_root_indexer)->get_index());
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_T_LEAK);
        }

        if (0 == err && (err = this->_fp_empty_btree_check(*pbt_root, is_should_empty)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_empty_btree_check() returns 0x%x", err);
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    return err;
};
//  return: 0 for success, other values for error
//  Should be called in locked state and without checkpointing or other COW(load or store)
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::sanitary_check(const dfs_btree_lock_hold_t * plock_hold)
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        if (_rc_root_set.get_in_use_num() != 1) {
            DFS_BT_DEBUG_TRAP;

            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level,
                            "_rc_root_set.get_in_use_num() (= %ld) != 1",
                            _rc_root_set.get_in_use_num());
            DF_WRITE_LOG_US(log_level,
                            "_ckp_root_indexer=%ld",
                            ((dfs_rc_indexer_ext_t *)&_ckp_root_indexer)->get_index());
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_T_LEAK);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else if ((err = _get_root(rc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_sanitary_check(*pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_sanitary_check() returns 0x%x", err);
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    return err;
};
template<typename T, uint32_t ROW_SIZE>
void dfs_fp_btree_t<T, ROW_SIZE>::clear_statistic_info(const dfs_btree_lock_hold_t * plock_hold)
{
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        this->_fp_clear_statistic_info();

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    return;
};
//should be called in locked state because of calling dfs_bt2d_ary_t::_free_list etc
template<typename T, uint32_t ROW_SIZE>
void dfs_fp_btree_t<T, ROW_SIZE>::log_statistic_info(
    const int org_log_level,
    const char * filename,
    const int lineno,
    const char * funcname,
    const dfs_btree_lock_hold_t * plock_hold) const
{
    uint64_t mem_size = 0;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        this->_fp_log_statistic_info(org_log_level, filename, lineno, funcname);
        mem_size = this->_fp_get_mem_size();
        DF_WRITE_LOG_US(log_level,"fp_btree in-use memory=%luMB", mem_size/1024/1024);

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    return;
};
template<typename T, uint32_t ROW_SIZE>
void dfs_fp_btree_t<T, ROW_SIZE>::log_debug_info(
    const int org_log_level,
    const char * filename,
    const int lineno,
    const char * funcname,
    const dfs_btree_lock_hold_t * plock_hold) const
{
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        this->_fp_log_debug_info(org_log_level, filename, lineno, funcname);

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    return;
};

//should be called in locked state because of calling dfs_bt2d_ary_t::_free_list etc
template<typename T, uint32_t ROW_SIZE>
uint64_t dfs_fp_btree_t<T, ROW_SIZE>::get_mem_size(const dfs_btree_lock_hold_t * plock_hold) const
{
    uint64_t mem_size = 0;
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        mem_size = this->_fp_get_mem_size();

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    return mem_size;
};

//
//  功能：获得B树最小元素
//  输出：如果pt不是NULL且B树非空，则最小项的值被复制到*pt
//  返回：true如果最小的名字存在，false不存在(B树为空)
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::get_smallest(
    uint64_t * pkey_id,
    dfs_name_t * pnamet,
    T * pt,
    const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    K o_keyt;
    PCDU pcdu = NULL;
    uint64_t o_key_id = UNDEF_ID;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL != pkey_id) {
        *pkey_id = UNDEF_ID;
    }
    if (NULL != pnamet) {
        pnamet->set_name_len(0);
    }
    if (NULL != pt) {
        pt->init();
    }

    if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_get_smallest(*pbt_root, o_key_id, o_keyt, pcdu)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_smallest() returns 0x%x", err);
        }
        //存在
        else if (NULL != pcdu) {
            if (NULL != pkey_id) {
                *pkey_id = o_key_id;
            }
            if (NULL != pnamet) {
                pnamet->join(o_keyt.prefix, o_keyt.suffix);
            }
            if (NULL != pt) {
                *pt = *((const T *)pcdu);
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "o_key_id=%lu", o_key_id);
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }

    return (0 == err && UNDEF_ID != o_key_id);
};

//  功能：获得B树最大元素
//  输出：如果pt不是NULL且B树非空，则最大项的值被复制到*pt
//  返回：true如果最大的名字存在，false不存在(B树为空)
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::get_largest(
    uint64_t * pkey_id,
    dfs_name_t * pnamet,
    T * pt,
    const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    K o_keyt;
    PCDU pcdu = NULL;
    uint64_t o_key_id = UNDEF_ID;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL != pkey_id) {
        *pkey_id = UNDEF_ID;
    }
    if (NULL != pnamet) {
        pnamet->set_name_len(0);
    }
    if (NULL != pt) {
        pt->init();
    }

    if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_get_largest(*pbt_root, o_key_id, o_keyt, pcdu)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_largest() returns 0x%x", err);
        }
        //存在
        else if (NULL != pcdu) {
            if (NULL != pkey_id) {
                *pkey_id = o_key_id;
            }
            if (NULL != pnamet) {
                pnamet->join(o_keyt.prefix, o_keyt.suffix);
            }
            if (NULL != pt) {
                *pt = *((const T *)pcdu);
            }
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "o_key_id=%lu", o_key_id);
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }

    return (0 == err && UNDEF_ID != o_key_id);
};



template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::get_smaller(
    const dfs_name_t & namet,
    uint64_t * pkey_id,
    dfs_name_t * pnamet,
    T * pt,
    dfs_btree_drill_t * pdrill_info,
    const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    dfs_btree_drill_t bck_drill_info;
    dfs_btree_drill_t & drill_info  = (NULL == pdrill_info) ? bck_drill_info : *pdrill_info;
    K i_keyt;
    K o_keyt;
    PCDU pcdu = NULL;
    uint64_t o_key_id = UNDEF_ID;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL != pkey_id) {
        *pkey_id = UNDEF_ID;
    }
    if (NULL != pnamet) {
        pnamet->set_name_len(0);
    }
    if (NULL != pt) {
        pt->init();
    }

    if (namet.get_len() <= 0) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.get_len() <= 0");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (!namet.split_filepath(i_keyt.prefix, i_keyt.suffix)) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.split_filepath() fails");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_get_smaller(
                              *pbt_root,
                              i_keyt,
                              drill_info,
                              o_key_id,
                              o_keyt,
                              pcdu)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_smaller() returns 0x%x", err);
        }
        //存在
        else if (NULL != pcdu) {
            if (NULL != pkey_id) {
                *pkey_id = o_key_id;
            }
            if (NULL != pnamet) {
                pnamet->join(o_keyt.prefix, o_keyt.suffix);
            }
            if (NULL != pt) {
                *pt = *((const T *)pcdu);
            }
        }
        //不存在
        else {
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        BT_DF_WRITE_LOG_BYTES(log_level, "namet", namet.get_str(), namet.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "i_prefix",
                              i_keyt.prefix.get_str(), i_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "i_suffix",
                              i_keyt.suffix.get_str(),  i_keyt.suffix.get_len());

        DF_WRITE_LOG_US(log_level, "o_key_id=%lu", o_key_id);
        BT_DF_WRITE_LOG_BYTES(log_level, "o_prefix",
                              o_keyt.prefix.get_str(), o_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "o_suffix",
                              o_keyt.suffix.get_str(),  o_keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }

    return (0 == err && UNDEF_ID != o_key_id);
};


//
//  功能：获得刚好比输入的名字小的名字的key_id
//  输入：key_id：B树中存在的名字的key_id；
//        namet：不需要在B树中存在
//  返回：true如果有更小的元素，否则返回false
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::get_smaller(
    const uint64_t key_id,
    uint64_t * pkey_id,
    dfs_name_t * pnamet,
    T * pt,
    dfs_btree_drill_t * pdrill_info,
    const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    dfs_btree_drill_t bck_drill_info;
    dfs_btree_drill_t & drill_info  = (NULL == pdrill_info) ? bck_drill_info : *pdrill_info;
    K o_keyt;
    PCDU pcdu = NULL;
    uint64_t o_key_id = UNDEF_ID;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL != pkey_id) {
        *pkey_id = UNDEF_ID;
    }
    if (NULL != pnamet) {
        pnamet->set_name_len(0);
    }
    if (NULL != pt) {
        pt->init();
    }

    if (UNDEF_ID == key_id) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "UNDEF_ID == key_id");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_ID);
    } else if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_get_smaller(
                              *pbt_root,
                              key_id,
                              drill_info,
                              o_key_id,
                              o_keyt,
                              pcdu)) != 0) {
            log_level = (ERRNO_BT_INVALID_ID == err) ?  DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_smaller() returns 0x%x", err);
        }
        //存在
        else if (NULL != pcdu) {
            if (NULL != pkey_id) {
                *pkey_id = o_key_id;
            }
            if (NULL != pnamet) {
                pnamet->join(o_keyt.prefix, o_keyt.suffix);
            }
            if (NULL != pt) {
                *pt = *((const T *)pcdu);
            }
        }
        //不存在
        else {
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "key_id=%lu", key_id);

        DF_WRITE_LOG_US(log_level, "o_key_id=%lu", o_key_id);
        BT_DF_WRITE_LOG_BYTES(log_level, "o_prefix",
                              o_keyt.prefix.get_str(), o_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "o_suffix",
                              o_keyt.suffix.get_str(),  o_keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }

    return (0 == err && UNDEF_ID != o_key_id);
};


//
//  功能：获得刚好比输入的名字大的名字的key_id
//  输入：key_id：B树中存在的名字的key_id；
//        namet：不需要在B树中存在
//  返回：true如果有更大的元素，否则返回false

template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::get_larger(
    const dfs_name_t & namet,
    uint64_t * pkey_id,
    dfs_name_t * pnamet,
    T * pt,
    dfs_btree_drill_t * pdrill_info,
    const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    dfs_btree_drill_t bck_drill_info;
    dfs_btree_drill_t & drill_info  = (NULL == pdrill_info) ? bck_drill_info : *pdrill_info;
    K i_keyt;
    K o_keyt;
    PCDU pcdu = NULL;
    uint64_t o_key_id = UNDEF_ID;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL != pkey_id) {
        *pkey_id = UNDEF_ID;
    }
    if (NULL != pnamet) {
        pnamet->set_name_len(0);
    }
    if (NULL != pt) {
        pt->init();
    }

    if (namet.get_len() <= 0) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.get_len() <= 0");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (!namet.split_filepath(i_keyt.prefix, i_keyt.suffix)) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "namet.split_filepath() fails");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
    } else if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_get_larger(
                              *pbt_root,
                              i_keyt,
                              drill_info,
                              o_key_id,
                              o_keyt,
                              pcdu)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_larger() returns 0x%x", err);
        }
        //存在
        else if (NULL != pcdu) {
            if (NULL != pkey_id) {
                *pkey_id = o_key_id;
            }
            if (NULL != pnamet) {
                pnamet->join(o_keyt.prefix, o_keyt.suffix);
            }
            if (NULL != pt) {
                *pt = *((const T *)pcdu);
            }
        }
        //不存在
        else {
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        BT_DF_WRITE_LOG_BYTES(log_level, "namet", namet.get_str(), namet.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "i_prefix",
                              i_keyt.prefix.get_str(), i_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "i_suffix",
                              i_keyt.suffix.get_str(),  i_keyt.suffix.get_len());

        DF_WRITE_LOG_US(log_level, "o_key_id=%lu", o_key_id);
        BT_DF_WRITE_LOG_BYTES(log_level, "o_prefix",
                              o_keyt.prefix.get_str(), o_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "o_suffix",
                              o_keyt.suffix.get_str(),  o_keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }

    return (0 == err && UNDEF_ID != o_key_id);
};


//
//  功能：获得刚好比输入的名字小的名字的key_id
//  输入：key_id：B树中存在的名字的key_id；
//        namet：不需要在B树中存在
//  返回：true如果有更小的元素，否则返回false
template<typename T, uint32_t ROW_SIZE>
bool dfs_fp_btree_t<T, ROW_SIZE>::get_larger(
    const uint64_t key_id,
    uint64_t * pkey_id,
    dfs_name_t * pnamet,
    T * pt,
    dfs_btree_drill_t * pdrill_info,
    const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    dfs_btree_drill_t bck_drill_info;
    dfs_btree_drill_t & drill_info  = (NULL == pdrill_info) ? bck_drill_info : *pdrill_info;
    K o_keyt;
    PCDU pcdu = NULL;
    uint64_t o_key_id = UNDEF_ID;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL != pkey_id) {
        *pkey_id = UNDEF_ID;
    }
    if (NULL != pnamet) {
        pnamet->set_name_len(0);
    }
    if (NULL != pt) {
        pt->init();
    }

    if (UNDEF_ID == key_id) {
        log_level = DF_UL_LOG_NOTICE;
        DF_WRITE_LOG_US(log_level, "UNDEF_ID == key_id");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_ID);
    } else if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_get_larger(
                              *pbt_root,
                              key_id,
                              drill_info,
                              o_key_id,
                              o_keyt,
                              pcdu)) != 0) {
            log_level = (ERRNO_BT_INVALID_ID == err) ?  DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_get_larger() returns 0x%x", err);
        }
        //存在
        else if (NULL != pcdu) {
            if (NULL != pkey_id) {
                *pkey_id = o_key_id;
            }
            if (NULL != pnamet) {
                pnamet->join(o_keyt.prefix, o_keyt.suffix);
            }
            if (NULL != pt) {
                *pt = *((const T *)pcdu);
            }
        }
        //不存在
        else {
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level, "key_id=%lu", key_id);

        DF_WRITE_LOG_US(log_level, "o_key_id=%lu", o_key_id);
        BT_DF_WRITE_LOG_BYTES(log_level, "o_prefix",
                              o_keyt.prefix.get_str(), o_keyt.prefix.get_len());
        BT_DF_WRITE_LOG_BYTES(log_level, "o_suffix",
                              o_keyt.suffix.get_str(),  o_keyt.suffix.get_len());
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }

    return (0 == err && UNDEF_ID != o_key_id);
};



//功能：验证整个树的顺序和每个节点分支个数
//返回：返回0如果验证通过，返回非0(错误代码)如果验证失败
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::verify(const dfs_rc_indexer_t * prc_indexer) const
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;

    if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
        if (get_batch_mutate_mode()) {
            log_level = DF_UL_LOG_WARNING;
            DF_WRITE_LOG_US(log_level, "get_batch_mutate_mode() is true but NULL == prc_indexer");
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = _snapshot_wr_root_indexer(rc_indexer)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
        } else {
            prc_indexer = &rc_indexer;
        }
    } else if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
        log_level = DF_UL_LOG_WARNING;
        DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
        err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
    }

    if (0 == err) {
        if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_get_root() returns 0x%x", err);
        } else if (NULL == pbt_root) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
        } else if ((err = this->_fp_verify_tree(*pbt_root)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_verify() returns 0x%x", err);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }


    return err;
};

//功能：设置或取消checkpoint状态
//输入：is_cancel：true取消checkpoint点及进行中的checkpoing操作，false设置一个新的checkpoint点
//      参数max_wait_microseconds不再使用
//建议：仅用此函数取消进行中的checkpoint，通过snapshot()获得一个checkpoint
//返回：0: 设置或取消成功，取消不存在的checkpoint状态也返回0
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::set_checkpoint_state(
    const bool is_cancel,
    const int64_t max_wait_microseconds,
    const dfs_btree_lock_hold_t * plock_hold)
{
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    //为了避免此时写线程修改btree
    //因此要锁住_mutate_locker()避免写线程工作。
    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        if (is_cancel) {
            _ckp_root_indexer.init();
        } else {
            if ((err = _snapshot_wr_root_indexer(_ckp_root_indexer)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_snapshot_wr_root_indexer() returns 0x%x", err);
            }
        }
        //取消或设置checkpoint
        this->_fp_set_cancel_checkpointing(is_cancel);

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    return err;
};


//  功能：把已经设置的checkpoint存储到文件中
//  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
//  数据： first prefix(first fp, then id), then suffix(first fp, then id))
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::store_checkpointing(
    const dfs_write_proc_t write_proc,
    void * file_info,
    const dfs_btree_lock_hold_t * plock_hold,
    const dfs_rc_indexer_t * prc_indexer)
{
    dfs_rc_indexer_t rc_indexer;
    const dfs_bt_root_t * pbt_root = NULL;
    dfs_btree_store_info_t store_info(write_proc, file_info, MAX_T_STORE_SIZE*4);
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;


    log_level = this->CKP_LOG_LEVEL;

    //Step 1. obtain store infomation (call _ckp_store_begin())
    //不论其他操作是否成功，_ckp_store_begin和_ckp_store_end都需要执行以清除ckp状态
    //Step 1. obtain store infomation
    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //清除“取消store/load状态”
        this->_fp_set_cancel_checkpointing(false); //is_cancel;

        if ((err = store_info.init()) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "store_info.init() returns 0x%x", err);
        } else {
            store_info.set_major_tag(this->FP_BT_TAG);
        }

        if (0 == err) {
            if (NULL == prc_indexer || !(prc_indexer->is_valid())) {
                rc_indexer = _ckp_root_indexer;
                prc_indexer = &rc_indexer;
            }

            if(!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)) {
                log_level = DF_UL_LOG_WARNING;
                DF_WRITE_LOG_US(log_level, "!_rc_root_set.is_valid_in_use_indexer(*prc_indexer)");
                err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_INVALID_INDEX);
            } else if ((err = _get_root(*prc_indexer, pbt_root)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "rc_root_set_t::_get_t_unit() returns 0x%x", err);
            } else if (NULL == pbt_root) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "NULL == pbt_root");
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
            }
        }

        //只要能够得到锁，就清空_ckp_root_indexer
        _ckp_root_indexer.init();

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    //Step 2. store the btree: first prefix(first fp, then id), then suffix(first fp, then id))
    if (0 == err) {
        if ((err = this->_fp_store_checkpointing(*pbt_root, store_info)) != 0) {
            log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_store_checkpointing() returns 0x%x", err);
        }
    }

    if (DF_UL_LOG_NONE != log_level) {
        DF_WRITE_LOG_US(log_level,
                        "prc_indexer=%p, prc_indexer->_index=%lu",
                        prc_indexer,
                        (NULL == prc_indexer) ?
                        UNDEF_INDEX : ((dfs_rc_indexer_ext_t *)prc_indexer)->get_index());
    }


    return err;
};


//
//  功能：把先前存储在文件的checkpoint加载到内存中
//  参数：file_info：文件的控制信息，例如文件句柄，调用store或load时文件数据的位置等。
//  数据： first prefix(first fp, then id), then suffix(first fp, then id))
//  返回：0 for success, other values for error
template<typename T, uint32_t ROW_SIZE>
int dfs_fp_btree_t<T, ROW_SIZE>::load_checkpointing(
    const dfs_read_proc_t read_proc,
    void * file_info,
    const dfs_btree_lock_hold_t * plock_hold)
{
    const dfs_bt_root_t * pold_wr_bt_root = NULL;
    dfs_bt_root_t new_wr_bt_root;
    dfs_rc_indexer_t old_rc_indexer;
    dfs_btree_load_info_t load_info(read_proc, file_info, MAX_T_STORE_SIZE*4);
    dfs_btree_lock_hold_t lock_hold;
    int log_level = DF_UL_LOG_NONE;
    int err = 0;
    int err1 = 0;

    log_level = this->CKP_LOG_LEVEL;

    //Step 1. preparation
    //Step 2. load the btree (prefix (first fp, then id), then suffix (first fp, then id))
    //Step 3. load the suffix btree
    //step 4. update root

    //Step 1. preparation
    if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
        log_level = DF_UL_LOG_FATAL;
        DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
    } else {
        //清除“取消store/load状态”
        this->_fp_set_cancel_checkpointing(false); //is_cancel;

        if ((err = load_info.init()) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "load_info.init() returns 0x%x", err);
        } else {
            load_info.set_major_tag(this->FP_BT_TAG);

            if ((err = _retrieve_wr_root_indexer(old_rc_indexer, pold_wr_bt_root)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_retrieve_wr_root_indexer() returns 0x%x", err);
            } else if (NULL == pold_wr_bt_root) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "NULL == pold_wr_bt_root");
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_OBJ_NULL_POINTER);
            } else {
                //先复制根节点以便获得mutation counter, next_allocate_id等信息
                new_wr_bt_root = *pold_wr_bt_root;
            }
        }

        if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
            err = err1;
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
        }
    }

    //Step 2. load the btree (prefix (first fp, then id), then suffix (first fp, then id))
    if (0 == err) {
        if ((err1 = this->_fp_load_checkpointing(new_wr_bt_root, load_info)) != 0) {
            err = err1;
            log_level = (ERRNO_BT_CKP_CANCELLED == err) ? DF_UL_LOG_NOTICE : DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "_fp_load_checkpointing() returns 0x%x", err);

            //发生错误后要删除已经生成的对象
            //先+1引用计数，再-1引用计数，这导致对象回收
            if ((err1 = this->_fp_inc_root_ref_counter(new_wr_bt_root)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_inc_root_ref_counter() returns 0x%x", err1);
            } else if ((err1 = this->_fp_dec_root_ref_counter(new_wr_bt_root)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_fp_dec_root_ref_counter() returns 0x%x", err1);
            }
        }
    }

    //step 3. update root
    if (0 == err) {
        if ((err = BT_MUTATE_LOCK_ACQUIRE(plock_hold, lock_hold, 0)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_ACQUIRE_ERR_INFO);
        } else {
            //如果不是batch mode，则同时更新_max_cow_mutation_counter从而禁止修改时的指针原子替换
            this->_fp_update_mutation_counter(false); //get_batch_mutate_mode()

            if((err = this->_update_wr_root_indexer(new_wr_bt_root)) != 0) {
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, "_update_wr_root_indexer() returns 0x%x", err);
            }

            //只要能够得到锁，就清空_ckp_root_indexer
            _ckp_root_indexer.init();

            if ((err1 = BT_MUTATE_LOCK_RELEASE(plock_hold, lock_hold)) != 0) {
                err = err1;
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, BT_MUTATE_LOCK_RELEASE_ERR_INFO);
            }
        }
    }

    return err;
};



#endif //__DFS_FP_BTREE_INCLUDE_H_

