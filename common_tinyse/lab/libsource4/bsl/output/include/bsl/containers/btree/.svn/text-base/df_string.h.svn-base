/**======================================================================
 *
 *  df_string.h - Pyramid / DTS /
 *
 *
 *
 *  -------------------------------------------------------------------
 *
 *  Description
 *
 *  -------------------------------------------------------------------
 *
 *  Change Log
======================================================================**/



#ifndef __DFS_STRING_INCLUDE_H_
#define __DFS_STRING_INCLUDE_H_

//#include "ul_def.h"
#include "df_common.h"
#include "df_2d_ary.h"

//#ifndef DF_USE_ID_IN_STR_KEY
//#define DF_USE_ID_IN_STR_KEY
//#endif


class df_string_t : public dfs_bt_const_t
{
private:
    static const uint64_t MAX_STR; //后面要用指针，其实例声明放到了df_btree.cpp中 //= MAX_U_INT64;
    static const int64_t MAX_STR_LEN = sizeof(MAX_STR);
    static const int64_t MIN_STR_LEN = 0;
    static const int64_t TABLE_ID_MAX_STR_LEN = MAX_STR_LEN + sizeof(uint64_t);
    static const int64_t TABLE_ID_MIN_STR_LEN = MIN_STR_LEN + sizeof(uint64_t);
    //enum cconst_private
    //{
    //    PURE_SMALLEST_STR_LEN       = 0,    //the length of the SMALLEST string
    //    PURE_LARGEST_STR_LEN        = sizeof(MAX_STR),   //the length of the LARGEST string
    //    TABLE_ID_SMALLEST_STR_LEN   = PURE_SMALLEST_STR_LEN + sizeof(uint64_t),
    //    TABLE_ID_LARGEST_STR_LEN    = PURE_LARGEST_STR_LEN + sizeof(uint64_t),
    //};
private:
    char * _buf;
    const char * _str;  //pointed to _buf if the latter is not null
    int64_t _len;         //should be the exact length of name (excluding the terminated null char).
    int64_t _buf_size;    //the size of name buf, 0 if name is just a pointer.
    //char  _def_buf[DEF_BUF_SIZE];
#ifdef  DF_USE_ID_IN_STR
    uint64_t _id;
#endif
public:
    df_string_t() :
        _buf(NULL),
        _str(NULL),
        _len(0),
#ifdef DF_USE_ID_IN_STR
        _buf_size(0),
        _id(UNDEF_ID)
#else
        _buf_size(0)
#endif
    {
    };
    df_string_t(const char * str, const int64_t len) :
        _buf(NULL),
        _str(str),
        _len((NULL == str || len <= 0) ? 0 : len),
#ifdef DF_USE_ID_IN_STR
        _buf_size(0),
        _id(UNDEF_ID)
#else
        _buf_size(0)
#endif
    {
    };
    //取消此函数，以便避免调用者漏写的第二个参数
    //str必须以'\0'结尾且'\0'不包含在df_string_t中
    //df_string_t(const char * str) :
    //            _buf(NULL),
    //            _str(str),
    //            _len((NULL == str) ? 0 : strlen(str)),
    //            #ifdef DF_USE_ID_IN_STR
    //            _buf_size(0),
    //            _id(UNDEF_ID)
    //            #else
    //            _buf_size(0)
    //            #endif
    //{
    //};
    df_string_t(const df_string_t & src) :
        _buf(NULL),
        _str(src.get_str()),
        _len(src.get_len()),
#ifdef DF_USE_ID_IN_STR
        _buf_size(0),
        _id(UNDEF_ID)
#else
        _buf_size(0)
#endif
    {
    };
    const df_string_t & operator=(const df_string_t & src) {
        set_str(src.get_str(), src.get_len());
        return *this;
    };
    //取消此函数，以便避免调用者漏写的第二个参数
    ////str必须以'\0'结尾且'\0'不包含在df_string_t中
    //const df_string_t & operator=(const char * str)
    //{
    //    set_str(str);
    //    return *this;
    //};
    void init(void) {
        _str = NULL;
        _len = 0;
#ifdef DF_USE_ID_IN_STR
        _id = UNDEF_ID;
#endif
    };
public:
    bool operator==(const df_string_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (0 == cmp);
    };
    bool operator!=(const df_string_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (0 != cmp);
    };
    bool operator>=(const df_string_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (cmp >= 0);
    };
    bool operator<=(const df_string_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (cmp <= 0);
    };
    bool operator>(const df_string_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (cmp > 0);
    };
    bool operator<(const df_string_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (cmp < 0);
    };
public:
    //Begin: btree required interface
    void set_str(const char * str, const int64_t len) {
        _str = str;
        _len = (NULL == str || len <= 0) ? 0 : len;
    };
    void set_len(const int64_t len) {
        _len = (len <= 0) ? 0 : len;
    };
    const char * get_str(void) const {
        return _str;
    };
    int64_t get_len(void) const {
        return _len;
    };
    //If  buf_size is large enough, then copy name and set str_len and return true;
    //otherwise, set _len to len and return ERRNO_BT_BUF_SIZE;
    int copy(const char * str, const int64_t len) {
        int64_t copy_len = (NULL == str || len <= 0) ? 0 : len;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        _len = copy_len;
        if (NULL != _buf && _buf_size > copy_len) {
            if (copy_len > 0) {
                memcpy(_buf, str, copy_len);
            }
            _buf[copy_len] = '\0';
            _str = _buf;       
        } else {
            log_level = DF_UL_LOG_NOTICE;
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_BUF_SIZE);
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_BUF_SIZE);
        }

        return err;
    };
    //取消此函数，以便避免调用者漏写的第二个参数
    ////str必须以'\0'结尾且'\0'不包含在df_string_t中
    //int copy(const char * str)
    //{
    //    return copy(str, (NULL == str) ? 0 : strlen(str));
    //};
    int copy(const df_string_t & src) {
        return copy(src.get_str(), src.get_len());
    };
    //End: btree required interface
public:
    //取消此函数，以便避免调用者漏写的第二个参数
    ////str必须以'\0'结尾且'\0'不包含在df_string_t中
    //void set_str(const char * str)
    //{
    //    _str = str;
    //    _len = (NULL == str) ? 0 : strlen(str);
    //};
public:
    //设置为最小字符串
    int set_to_smallest_key(void) {
        set_str(NULL, MIN_STR_LEN);
        return 0;
    };
    bool is_smallest_key(void) const {
        return (get_len() == MIN_STR_LEN);
    };
    //设置为最大字符串
    int set_to_largest_key(void) {
        //memset(_def_buf, MAX_U_INT8, LARGEST_STR_LEN);
        set_str((const char *)&MAX_STR, (int64_t)MAX_STR_LEN);
        return 0;
    };
    bool is_largest_key(void) const {
        return (NULL != get_str()
                && get_len() >= MAX_STR_LEN
                && *((const uint64_t *)get_str()) == MAX_STR);
    };
    //>最小字符串 且 <最大字符串 则返回true，否则返回false
    bool is_normal_key(void) const {
        return (NULL != get_str()
                && get_len() > MIN_STR_LEN
                && (get_len() < MAX_STR_LEN
                    || *((const uint64_t *)get_str()) < MAX_STR));
    };
private:
    static void _hton(const uint64_t src_data, char * tag_buf) {
        uint64_t data = src_data;

        for (int64_t j = (int64_t)sizeof(data)-1; j >= 0; --j) {
            tag_buf[j] = (unsigned char)(data);
            data >>= 8;
        }

        return ;
    };
    static uint64_t _ntoh(const char * src_buf) {
        uint64_t data = 0;

        for (int64_t j = 0; j < (int64_t)sizeof(data); ++j) {
            data = (data<<8) + ((uint64_t)((unsigned char)src_buf[j]));
        }

        return data;
    };
public:
    //设置为最小字符串(table_id为串首，字节顺序为网络序)
    int set_to_smallest_key(const uint64_t table_id) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (NULL == _buf || _buf_size < TABLE_ID_MIN_STR_LEN) {
            log_level = DF_UL_LOG_NOTICE;
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_BUF_SIZE);
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_BUF_SIZE);
        } else {
            _hton(table_id, _buf);
            _str = _buf;
            _len = TABLE_ID_MIN_STR_LEN;
        }

        return err;
    };
    bool is_smallest_key(const uint64_t table_id) const {
        char buf[sizeof(uint64_t)];

        _hton(table_id, buf);

        return (get_str() != NULL
                && get_len() == TABLE_ID_MIN_STR_LEN
                && ((const uint64_t *)get_str())[0] == ((uint64_t *)buf)[0]);
    };
    //设置为最大字符串(table_id为串首，字节顺序为网络序)
    int set_to_largest_key(const uint64_t table_id) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (NULL == _buf || _buf_size < TABLE_ID_MAX_STR_LEN) {
            log_level = DF_UL_LOG_NOTICE;
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_BUF_SIZE);
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_BUF_SIZE);
        } else {
            _hton(table_id, _buf);
            *((uint64_t *)(_buf+sizeof(uint64_t))) = MAX_STR;
            _str = _buf;
            _len = TABLE_ID_MAX_STR_LEN;
        }

        return err;
    };
    bool is_largest_key(const uint64_t table_id) const {
        char buf[sizeof(uint64_t)];

        _hton(table_id, buf);

        return (NULL != get_str()
                && get_len() >= TABLE_ID_MAX_STR_LEN
                && ((const uint64_t *)get_str())[0] == ((uint64_t *)buf)[0]
                && ((const uint64_t *)get_str())[1] == MAX_STR);
    };
    //>最小字符串 且 <最大字符串 则返回true，否则返回false(table_id为串首，字节顺序为网络序)
    bool is_normal_key(const uint64_t table_id) const {
        char buf[sizeof(uint64_t)];

        _hton(table_id, buf);

        return (NULL != get_str()
                && get_len() > TABLE_ID_MIN_STR_LEN
                && ((const uint64_t *)get_str())[0] == ((uint64_t *)buf)[0]
                && (get_len() < TABLE_ID_MAX_STR_LEN
                    || ((const uint64_t *)get_str())[1] < MAX_STR));
    };
private:
    int _join(const char * str1, const int64_t len1, const char * str2, const int64_t len2) {
        const int64_t copy_len1 = (NULL == str1 || len1 <= 0) ? 0 : len1;
        const int64_t copy_len2 = (NULL == str2 || len2 <= 0) ? 0 : len2;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        _len = copy_len1+copy_len2;
        if (NULL != _buf && _buf_size > _len) {
            if (copy_len1 > 0) {
                memcpy(_buf, str1, copy_len1);
            }
            if (copy_len2 > 0) {
                memcpy(_buf+copy_len1, str2, copy_len2);
            }
            _buf[_len] = '\0';
            _str = _buf;
        } else {
            log_level = DF_UL_LOG_NOTICE;
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_BUF_SIZE);
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_BUF_SIZE);
        }

        return err;
    };
    int _join(const uint64_t table_id, const char * str2, const int64_t len2) {
        const int64_t copy_len1 = sizeof(uint64_t);
        const int64_t copy_len2 = (NULL == str2 || len2 <= 0) ? 0 : len2;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        _len = copy_len1+copy_len2;
        if (NULL != _buf && _buf_size > _len) {
            _hton(table_id, _buf);
            if (copy_len2 > 0) {
                memcpy(_buf+copy_len1, str2, copy_len2);
            }
            _buf[_len] = '\0';
            _str = _buf;
        } else {
            log_level = DF_UL_LOG_NOTICE;
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_BUF_SIZE);
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_BUF_SIZE);
        }

        return err;
    };
public:
    //If  buf_size is large enough, then copy name and set str_len and return true;
    //otherwise, set _len to the joined and return ERRNO_BT_BUF_SIZE;
    int join(const df_string_t & src1, const df_string_t & src2) {
        return _join(src1.get_str(), src1.get_len(), src2.get_str(), src2.get_len());
    };
    int join(const uint64_t table_id, const df_string_t & src2) {
        return _join(table_id, src2.get_str(), src2.get_len());
    };
    //pstrt will point to the original str after splitted
    int split(uint64_t * ptable_id, df_string_t * pstrt) const {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if (get_len() < (int64_t)sizeof(uint64_t) || get_str() == NULL) {
            log_level = DF_UL_LOG_NOTICE;
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_KEY_INVALID);
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_KEY_INVALID);
        } else {
            if (NULL != ptable_id) {
                *ptable_id = _ntoh(get_str());
            }
            if (NULL != pstrt) {
                pstrt->set_str(get_str()+sizeof(uint64_t), get_len()-(int64_t)sizeof(uint64_t));
            }
        }

        return err;
    };
public:
    void set_buf(char * buf, const int64_t buf_size) {
        _buf = buf;
        _str = _buf;
        _len = 0;
        _buf_size = buf_size;
    };
    char * get_buf(void) {
        return _buf;
    };
    int64_t get_buf_size(void) const {
        return _buf_size;
    };
public:
    //
    //return: 0 for usccess, other values for error
    //  cmp: -1, 0, 1 for this less than, equal, great than src2
    int compare(const df_string_t & src2, int & cmp) const {
        const unsigned char * pstr1 = (const unsigned char * )get_str();
        const unsigned char * pstr2 = (const unsigned char * )src2.get_str();
        int64_t j = 0;
        int64_t len = 0;
        //int log_level = DF_UL_LOG_NONE;
        int err = 0;

        cmp = -2;
        if (NULL == pstr1 || get_len() <= 0) {
            cmp = (NULL == pstr2 || src2.get_len() <= 0) ? 0 : -1;
        } else if (NULL == pstr2 || src2.get_len() <= 0) {
            cmp = 1;
        }
#ifdef DF_USE_ID_IN_STR_KEY
        else if(get_id() == src2.get_id() && UNDEF_ID != get_id()) {
            cmp = 0;
        }
#endif
        else {
            if (get_len() == src2.get_len()) {
                len = get_len();
                if (pstr1 == pstr2) {
                    j = len;
                }
                cmp = 0;
            } else if (get_len() < src2.get_len()) {
                len = get_len();
                cmp = -1;
            } else {
                len = src2.get_len();
                cmp = 1;
            }

            while (j < len) {
                if (pstr1[j] == pstr2[j]) {
                    ++j;
                } else {
                    cmp = ((unsigned char)pstr1[j] < (unsigned char)pstr2[j]) ? -1 : 1;
                    break;
                }
            }
        }

        return err;
    };
public:
#ifdef DF_USE_ID_IN_STR
    void set_id(const uint64_t id) {
        _id = id;
    };
    uint64_t get_id(void) const {
        return _id;
    };
#endif
public:
    //用户数据类型T需要支持以下接口：
    //  功能：获得该实例store的数据长度(字节)，该值不能超过MAX_T_STORE_SIZE
    uint64_t get_store_size(void) const {
        return (sizeof(int64_t)+get_len());
    };
    //
    //  功能：存储该实例到buf
    //  输入：data_pos为存储数据的起始位置
    //  输出：data_pos为存储T后新的起始位置
    //  返回：for success, other values for error (例如缓冲区剩余空间不足)
    int store(char * buf, const uint64_t buf_size, uint64_t & data_pos) const {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((buf_size-data_pos) < (sizeof(int64_t)+get_len())) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_POS);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_POS);
        } else if (NULL == buf) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_BUF);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_BUF);
        } else {
            *((int64_t *)(buf+data_pos)) = get_len();
            data_pos += sizeof(int64_t);
            memcpy(buf+data_pos, get_str(), get_len());
            data_pos += get_len();
        }

        return err;
    };
    //
    //  功能：从buf中装载到该实例(不申请缓冲区)
    //  输入：data_pos为装载T前缓冲区的数据位置
    //  输出：data_pos为装载T后缓冲区的数据位置
    //  返回：for success, other values for error(例如缓冲区内数据不足)
    int load(const char * buf, const uint64_t data_len, uint64_t & data_pos) {
        int64_t str_len = 0;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((data_len-data_pos) < sizeof(int64_t)) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_POS);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_POS);
        } else if (NULL == buf) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_BUF);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_BUF);
        } else {
            str_len = *((int64_t *)(buf+data_pos));
            data_pos += sizeof(int64_t);
            if ((data_len-data_pos) < (uint64_t)str_len) {
                err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_POS);
                log_level = DF_UL_LOG_FATAL;
                DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_POS);
            } else {
                set_str((char *)(buf+data_pos), str_len);
                data_pos += str_len;
            }
        }

        return err;
    };
public:
    //
    //  功能：把table_id与当前实例join起来然后存储到buf
    //  输入：data_pos为存储数据的起始位置
    //  输出：data_pos为存储T后新的起始位置
    //  返回：for success, other values for error (例如缓冲区剩余空间不足)
    int join_store(const uint64_t table_id, char * buf, const uint64_t buf_size, uint64_t & data_pos) const {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((buf_size-data_pos) < (sizeof(int64_t)+sizeof(uint64_t)+get_len())) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_POS);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_POS);
        } else if (NULL == buf) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_BUF);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_BUF);
        } else {
            *((int64_t *)(buf+data_pos)) = sizeof(uint64_t)+get_len();
            data_pos += sizeof(int64_t);
            _hton(table_id, buf+data_pos);
            data_pos += sizeof(uint64_t);
            memcpy(buf+data_pos, get_str(), get_len());
            data_pos += get_len();
        }

        return err;
    };
};


#endif  //__DFS_STRING_INCLUDE_H_


