////===================================================================
//
// df_name.h Pyramid / DFS / df-lib
//
//
//
// -------------------------------------------------------------------
//
// Description
//
//    declaration and implementation of dfs_name_t,
//
// -------------------------------------------------------------------
//
// Change Log
//
//
////====================================================================


#ifndef __DFS_NAME_INCLUDE_H_
#define __DFS_NAME_INCLUDE_H_

//#include "ul_def.h"
#include "df_common.h"
#include "df_2d_ary.h"

//#ifndef DF_USE_ID_IN_STR_KEY
//#define DF_USE_ID_IN_STR_KEY
//#endif


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
class dfs_name_t : public dfs_bt_const_t
{
private:
    char * _buf;
    const char * _str;  //pointed to _buf if the latter is not null
    int64_t _len;         //should be the exact length of name (excluding the terminated null char).
    int64_t _buf_size;    //the size of name buf, 0 if name is just a pointer.
#ifdef  DF_USE_ID_IN_STR_KEY
    uint64_t _id;
#endif
public:
    dfs_name_t() :
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
    dfs_name_t(const char * str, const int64_t len) :
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
        ;
    };
    //取消此函数，以便避免调用者漏写的第二个参数
    //dfs_name_t(const char * str) :  //假定str以’\0’结尾
    //            _buf(NULL),
    //            _str(str),
    //            _len(strlen(str)),
    //            #ifdef DF_USE_ID_IN_STR
    //            _buf_size(0),
    //            _id(UNDEF_ID)
    //            #else
    //            _buf_size(0)
    //            #endif
    //{
    //    ;
    //};
    dfs_name_t(const dfs_name_t & src) :
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
        ;
    };
    const dfs_name_t & operator=(const dfs_name_t & src) {
        set_str(src.get_str(), src.get_len());
        return *this;
    };
    //取消此函数，以便避免调用者漏写的第二个参数
    //const dfs_name_t & operator=(const char * str)  //假定str以’\0’结尾
    //{
    //    set_str(str, strlen(str));
    //    return *this;
    //};
    void init(void) {
        _str = NULL;
        _len = 0;
#ifdef DF_USE_ID_IN_STR_KEY
        _id = UNDEF_ID;
#endif
    };
public:
    bool operator==(const dfs_name_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (0 == cmp);
    };
    bool operator!=(const dfs_name_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (0 != cmp);
    };
    bool operator>=(const dfs_name_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (cmp >= 0);
    };
    bool operator<=(const dfs_name_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (cmp <= 0);
    };
    bool operator>(const dfs_name_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (cmp > 0);
    };
    bool operator<(const dfs_name_t & src) const {
        int cmp = 1;
        this->compare(src, cmp);
        return (cmp < 0);
    };
public:
    //Begin: 原有接口
    //void set_name(const char * str, const uint32_t len)
    void set_name(const char * str, const int64_t len) {
        set_str(str, len);
    };
    //void set_name_len(const uint32_t len)
    void set_name_len(const int64_t len) {
        set_len(len);
    };
    const char * get_name(void) const {
        return get_str();
    };
    uint32_t get_name_len(void) const {
        return (uint32_t)get_len();
    };
    //void set_buf(char * buf, const uint32_t buf_size)
    //{
    //    set_buf(buf, (int64_t)buf_size);
    //};
#ifdef DF_USE_ID_IN_STR_KEY
    void set_name_id(const uint64_t id) {
        _id = id;
    };
    uint64_t get_name_id(void) const {
        return _id;
    };
#endif
    //End: 原有接口
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
            memcpy(_buf, str, copy_len);
            _buf[copy_len] = '\0';
            _str = _buf;       
        } else {
            log_level = DF_UL_LOG_NOTICE;
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_BUF_SIZE);
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_BUF_SIZE);
        }

        return err;
    };
    int copy(const dfs_name_t & src) {
        return copy(src.get_str(), src.get_len());
    };
    //End: btree required interface
public:
    //If  buf_size is large enough, then copy name and set str_len and return true;
    //otherwise, set _len to the joined and return ERRNO_BT_BUF_SIZE;
    int join(const dfs_name_t & src1, const dfs_name_t & src2) {
        int64_t copy_len1 = (NULL == src1.get_str() || src1.get_len() <= 0) ? 0 : src1.get_len();
        int64_t copy_len2 = (NULL == src2.get_str() || src2.get_len() <= 0) ? 0 : src2.get_len();
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        _len = copy_len1+copy_len2;
        if (NULL != _buf && _buf_size > _len) {
            if (copy_len1 > 0) {
                memcpy(_buf, src1.get_str(), copy_len1);
            }
            if (copy_len2 > 0) {
                memcpy(_buf+copy_len1, src2.get_str(), copy_len2);
            }
            _buf[_len] = '\0';
        } else {
            log_level = DF_UL_LOG_NOTICE;
            err = DF_BT_SET_NORMAL_ERR(ERRNO_BT_BUF_SIZE);
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_BUF_SIZE);
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
    //prefix & suffix will pointer the address of this name
    bool split_filepath(dfs_name_t & prefix, dfs_name_t & suffix) const {
        const char * name = get_str();
        int64_t str_len = get_len();
        int64_t j = 0;
        bool is_valid = true;
        int64_t base_start_pos = 0;

        if (NULL == name) {
            is_valid = false;
        } else if (str_len > 0) {
            //if ('/' != (namet.get_str())[0])
            //{
            //    is_valid = false;
            //}
            for (j = str_len-1; j > 0; --j) {
                if ('\0' == name[j]) {
                    is_valid = false;
                    break;
                } else if ('/' == name[j]) {
                    if ('/' == name[j-1]) {
                        is_valid = false;
                        break;
                    } else {
                        break;
                    }
                }
            }
            base_start_pos = j+1;
        } else { //str_len == 0: null path
            base_start_pos = 0;
        }

        if (is_valid) {
            prefix.set_name(name, base_start_pos);
            suffix.set_name(name+base_start_pos, str_len-base_start_pos);
        }

        return is_valid;
    };

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
    //return: 0 for usccess, other values for error
    //  cmp: -1, 0, 1 for this less than, equal, great than src2
    int compare(const dfs_name_t & src2, int & cmp) const {
        const unsigned char * pstr1 = (const unsigned char * )get_str();
        const unsigned char * pstr2 = (const unsigned char * )src2.get_str();
        int64_t j = 0;
        int64_t len = 0;
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        cmp = -2;
        if (NULL == pstr1) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level,
                            "_compare() src1 is NULL, src2_len=%ld",
                            src2.get_len());
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_NULL_POINTER);
        } else if (NULL == pstr2) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level,
                            "_compare() src2 is NULL, this_len=%ld",
                            this->get_len());
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_KEY_NULL_POINTER);
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
                    //if (pstr1[j] != '/' && pstr2[j] != '/')
                    //{
                    cmp = ((unsigned char)pstr1[j] < (unsigned char)pstr2[j]) ? -1 : 1;
                    //}
                    //else
                    //{
                    //    cmp = (pstr1[j] == '/') ? -1 : 1;
                    //}
                    break;
                }
            }
        }

        return err;
    };
public:
#ifdef DF_USE_ID_IN_STR_KEY
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
        return (sizeof(uint32_t)+get_len());
    };
    //
    //  功能：存储该实例到buf
    //  输入：data_pos为存储数据的起始位置
    //  输出：data_pos为存储T后新的起始位置
    //  返回：for success, other values for error (例如缓冲区剩余空间不足)
    int store(char * buf, const uint64_t buf_size, uint64_t & data_pos) const {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((buf_size-data_pos) < sizeof(uint32_t)+get_len()) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_POS);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_POS);
        } else if (NULL == buf) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_STORE_BUF);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_STORE_BUF);
        } else {
            *((uint32_t *)(buf+data_pos)) = (uint32_t)get_len();
            data_pos += sizeof(uint32_t);
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

        if ((data_len-data_pos) < sizeof(uint32_t)) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_POS);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_POS);
        } else if (NULL == buf) {
            err = DF_BT_SET_INTERNAL_ERR(ERRNO_BT_LOAD_BUF);
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, ERRINFO_BT_LOAD_BUF);
        } else {
            str_len = *((uint32_t *)(buf+data_pos));
            data_pos += sizeof(uint32_t);
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
};


class dfs_path_t
{
public:
    dfs_name_t prefix;
    dfs_name_t suffix;
public:
    void init(void) {
        prefix.init();
        suffix.init();
    };
    //return: 0 for usccess, other values for error
    //  cmp: -1, 0, 1 for less than, equal, great than
    int compare(const dfs_path_t & patht2, int & cmp) {
        int log_level = DF_UL_LOG_NONE;
        int err = 0;

        if ((err = prefix.compare(patht2.prefix, cmp)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "prefix.compare() returns 0x%x", err);
        } else if (0 == cmp && (err = suffix.compare(patht2.suffix, cmp)) != 0) {
            log_level = DF_UL_LOG_FATAL;
            DF_WRITE_LOG_US(log_level, "suffix.compare() returns 0x%x", err);
        }

        if (DF_UL_LOG_NONE != log_level) {
            DF_WRITE_LOG_US(log_level,
                            "prefix1_len=%ld, prefix2_len=%ld",
                            prefix.get_len(), patht2.prefix.get_len());
            DF_WRITE_LOG_US(log_level,
                            "suffix1_len=%ld, prefix2_len=%ld",
                            suffix.get_len(), patht2.suffix.get_len());
        }


        return err;
    };
};

#endif //__DFS_NAME_INCLUDE_H_

