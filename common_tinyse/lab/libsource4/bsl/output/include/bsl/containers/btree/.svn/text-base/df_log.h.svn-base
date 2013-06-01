////====================================================================
//
// df_log.h - Pyramid / DFS / df-lib
//
//
//
// -------------------------------------------------------------------
//
// Description
//
//    declaration of common logging lib
//
// -------------------------------------------------------------------
//
// Change Log
//
//
////====================================================================

#ifndef __DF_LOG_H__
#define __DF_LOG_H__

#ifdef WITH_UL_LOG
#include <ul_def.h>
#include <ul_log.h>
#include <comlog/comlog.h>
#include <time.h>

#define DF_LOG_NONE    "DF_NONE"
#define DF_LOG_FATAL   "DF_FATAL"
#define DF_LOG_WARNING "DF_WARNING"
#define DF_LOG_NOTICE  "DF_NOTICE"
#define DF_LOG_TRACE   "DF_TRACE"
#define DF_LOG_DEBUG   "DF_DEBUG"
#define DF_LOG_ALL     "DF_ALL"

#define DF_UL_LOG_NONE		0
#define DF_UL_LOG_FATAL	0x01    /**<   fatal errors */
#define DF_UL_LOG_WARNING	0x02    /**<   exceptional events */
#define DF_UL_LOG_NOTICE   0x04    /**<   informational notices */
#define DF_UL_LOG_TRACE	0x08    /**<   program tracing */
#define DF_UL_LOG_DEBUG	0x10    /**<   full debugging */
#define DF_UL_LOG_ALL		0xff    /**<   everything     */
#define DF_UL_LOG_LEVEL_COUNT 17

const char DF_LOG_LEVEL[DF_UL_LOG_LEVEL_COUNT][16] = {
    DF_LOG_NONE, DF_LOG_FATAL, DF_LOG_WARNING, DF_LOG_NONE, DF_LOG_NOTICE,
    DF_LOG_NONE, DF_LOG_NONE, DF_LOG_NONE, DF_LOG_TRACE, DF_LOG_NONE,
    DF_LOG_NONE, DF_LOG_NONE, DF_LOG_NONE, DF_LOG_NONE, DF_LOG_NONE,
    DF_LOG_NONE, DF_LOG_DEBUG
};

//当前日志级别，缺省为TRACE
//NOTE: 这是内部变量，请勿在程序代码中使用
class df_log_t
{
private:
    static int _df_log_level;
    static volatile time_t _df_last_fatal_tm;
    static ul_logstat_t _ul_stat;
    static char _ul_logname[256];

    // This class should never be instantiated
    // Keep its construction function as private
    df_log_t() {
    }
public:
    // Setter of _log_level
    inline static void set_log_level(int log_level) {
        _df_log_level = log_level;
    }

    // Getter of _log_level
    inline static int get_log_level(void) {
        return _df_log_level;
    }

    // Setter of _last_fatal_tm
    inline static void set_last_fatal_tm(void) {
        _df_last_fatal_tm = ::time(NULL);
    }

    // Getter of _last_fatal_tm
    inline static time_t get_last_fatal_tm(void) {
        return _df_last_fatal_tm;
    }

    /**
     * @brief
     *
     * @param [in/out] logpath   : const char*
     * @param [in/out] logname   : const char*
     * @param [in/out] log_level   : int
     * @param [in/out] log_size   : int
     * @return  int
     * @retval
     * @see
     * @note
    **/
    static int open_log(const char* logpath, const char* logname, int log_level, int log_size,
                        bool add_auxfile = true, bool write_all = false, bool async_write = false);

    /**
     * @brief
     *
     * @return  int
     * @retval
     * @see
     * @note
    **/
    inline static int close_log() {
        if (USING_COMLOG == com_logstatus()) {
            return com_closelog();
        } else {
            return ul_closelog(0);
        }
        return 0;
    }

    /**
     * @brief
     *
     * @return  int
     * @retval
     * @see
     * @note
    **/
    inline static int open_log_r() {
        if (USING_COMLOG == com_logstatus()) {
            return com_openlog_r();
        } else {
            return ul_openlog_r(_ul_logname, &_ul_stat);
        }
        return 0;
    }

    /**
     * @brief
     *
     * @return  int
     * @retval
     * @see
     * @note
    **/
    inline static int close_log_r() {
        if (USING_COMLOG == com_logstatus()) {
            return com_closelog_r();
        } else {
            return ul_closelog_r(0);
        }
        return 0;
    }
};

// DF_OPEN_LOG 用于打开日志，和ul_openlog一致，直接替换该函数调用即可
// NOTE: 请保证使用该宏打开日志
#define DF_OPEN_LOG(_logpath_, _logname_, _plogstat_, _log_size_) \
    (\
       df_log_t::open_log((_logpath_), (_logname_), ((_plogstat_)->events), (_log_size_), false, false, false))

// 打开日志，将warning／fatal日志也输出到debug日志中，且使用异步方式写日志文件
#define DF_OPEN_LOG_EX(_logpath_, _logname_, _plogstat_, _log_size_) \
    (\
       df_log_t::open_log((_logpath_), (_logname_), ((_plogstat_)->events), (_log_size_), false, true, true))


#define DF_CLOSE_LOG() df_log_t::close_log()

// DF_WRITE_LOG 用于系统日志的记录
// NOTE：对于数据/展现日志请勿使用
#define DF_WRITE_LOG(_loglevel_, _fmt_, args...) \
    {\
        if ((_loglevel_) == DF_UL_LOG_FATAL)\
        {\
            df_log_t::set_last_fatal_tm();\
            if (USING_COMLOG != com_logstatus()) \
            { \
                ul_writelog(_loglevel_,\
                    "[%02X][%s][%d][%s] " _fmt_,\
                    (_loglevel_), __FILE__, __LINE__, __FUNCTION__, ##args);\
            } else {\
                com_writelog(DF_LOG_LEVEL[(_loglevel_)],\
                    "[%02X][%s][%d][%s] " _fmt_,\
                    (_loglevel_), __FILE__, __LINE__, __FUNCTION__, ##args);\
            }\
        }\
        else if ((_loglevel_) <= df_log_t::get_log_level())\
        {\
            if (USING_COMLOG != com_logstatus()) \
            {\
                ul_writelog((_loglevel_),\
                    "[%02X][%s][%d][%s] " _fmt_,\
                    (_loglevel_), __FILE__, __LINE__, __FUNCTION__, ##args);\
            } \
            else if ((_loglevel_) > DF_UL_LOG_DEBUG) \
            { \
                com_writelog(DF_LOG_ALL,\
                    "[%02X][%s][%d][%s] " _fmt_,\
                    (_loglevel_), __FILE__, __LINE__, __FUNCTION__, ##args);\
            } \
            else\
            {\
                com_writelog(DF_LOG_LEVEL[(_loglevel_)],\
                    "[%02X][%s][%d][%s] " _fmt_,\
                    (_loglevel_), __FILE__, __LINE__, __FUNCTION__, ##args);\
            }\
        }\
    }

// DF_WRITE_LOG_DEBUG用于系统调试日志的记录，正式发布会删除
// NOTE: 对于数据/展现日志请勿使用
#ifndef NDEBUG
#define DF_WRITE_LOG_DEBUG(_fmt_, args...) \
    if (USING_COMLOG != com_logstatus()) \
    {\
        ul_writelog(DF_UL_LOG_WARNING,\
            "[D][%s][%d][%s] " _fmt_,\
            __FILE__, __LINE__, __FUNCTION__, ##args);\
    } else { \
        com_writelog(DF_LOG_LEVEL[DF_UL_LOG_WARNING],\
            "[D][%s][%d][%s] " _fmt_,\
            __FILE__, __LINE__, __FUNCTION__, ##args);\
    }
#else
#define DF_WRITE_LOG_DEBUG(_fmt_, args...)
#endif

#define DF_WRITE_LOG_EX(_loglevel_, _fmt_, args...) \
    if ((_loglevel_) <= df_log_t::get_log_level())\
    {\
        if (USING_COMLOG != com_logstatus()) \
        {\
            ul_writelog((_loglevel_),\
                "[%s][%d][%s] " _fmt_,\
                __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
        else if ((_loglevel_) > DF_UL_LOG_DEBUG) \
        { \
            com_writelog(DF_LOG_ALL,\
                "[%s][%d][%s] " _fmt_,\
                __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
        else \
        { \
            com_writelog(DF_LOG_LEVEL[(_loglevel_)],\
                "[%s][%d][%s] " _fmt_,\
                __FILE__, __LINE__, __FUNCTION__, ##args); \
        }\
    }

// DF_WRITE_LOG_NOTICE_US 用于系统日志的记录
// 不打印文件名，函数，行号，增加微秒数
#define DF_WRITE_LOG_NOTICE_US(_loglevel_, _fmt_, args...) \
    {\
        if ((_loglevel_) == DF_UL_LOG_NOTICE)\
        {\
            struct timeval dfs_log_tm;\
            gettimeofday(&dfs_log_tm, NULL);\
            if (USING_COMLOG != com_logstatus()) \
            {\
                ul_writelog((_loglevel_),\
                    "[%lu][%lu] " _fmt_,dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,\
                    ##args);\
            } else {\
                com_writelog(DF_LOG_LEVEL[(_loglevel_)],\
                    "[%lu][%lu] " _fmt_,dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,\
                    ##args);\
            }\
        }\
    }

// DF_WRITE_LOG_US 用于系统日志的记录,带微秒数
// NOTE：对于数据/展现日志请勿使用
#define DF_WRITE_LOG_US(_loglevel_, _fmt_, args...) \
    {\
        if ((_loglevel_) <= df_log_t::get_log_level())\
        {\
            if (USING_COMLOG != com_logstatus()) \
            { \
                struct timeval dfs_log_tm;\
                gettimeofday(&dfs_log_tm, NULL);\
                ul_writelog((_loglevel_),\
                    "[%lu][%lu][%s][%d][%s] " _fmt_,\
                    dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,__FILE__, __LINE__, __FUNCTION__, ##args);\
            } else {\
                if ((_loglevel_) > DF_UL_LOG_DEBUG) \
                { \
                    struct timeval dfs_log_tm;\
                    gettimeofday(&dfs_log_tm, NULL);\
                    com_writelog(DF_LOG_ALL,\
                        "[%lu][%lu][%s][%d][%s] " _fmt_,\
                        dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,__FILE__, __LINE__, __FUNCTION__, ##args);\
                } \
                else \
                { \
                    struct timeval dfs_log_tm;\
                    gettimeofday(&dfs_log_tm, NULL);\
                    com_writelog(DF_LOG_LEVEL[(_loglevel_)],\
                        "[%lu][%lu][%s][%d][%s] " _fmt_,\
                        dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,__FILE__, __LINE__, __FUNCTION__, ##args);\
                    if ((_loglevel_) == DF_UL_LOG_FATAL || (_loglevel_) == DF_UL_LOG_WARNING)\
                    {\
                        if ((_loglevel_) == DF_UL_LOG_FATAL)\
                        {\
                            df_log_t::set_last_fatal_tm();\
                        }\
                        com_writelog(DF_LOG_LEVEL[(DF_UL_LOG_TRACE)],\
                            "[%lu][%lu][%s][%d][%s] " _fmt_,\
                            dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,__FILE__, __LINE__, __FUNCTION__, ##args);\
                    } \
                } \
            } \
        }\
    }

// AP_WRITE_LOG 用于系统日志的记录
// NOTE：对于数据/展现日志请勿使用
#define AP_WRITE_LOG(_loglevel_, _fmt_, args...) \
    {\
        if ((_loglevel_) == DF_UL_LOG_FATAL)\
        {\
            df_log_t::set_last_fatal_tm();\
            ul_writelog(DF_UL_LOG_FATAL,\
                "[%02X][%s][%d][%s] " _fmt_,\
                (_loglevel_), __FILE__, __LINE__, __FUNCTION__, ##args);\
        }\
        if ((_loglevel_) <= df_log_t::get_log_level())\
        {\
            ul_writelog(_loglevel_,\
                "[%02X][%s][%d][%s] " _fmt_,\
                (_loglevel_), __FILE__, __LINE__, __FUNCTION__, ##args);\
        }\
    }

// AP_WRITE_LOG_DEBUG用于系统调试日志的记录，正式发布会删除
// NOTE: 对于数据/展现日志请勿使用
#ifndef NDEBUG
#define AP_WRITE_LOG_DEBUG(_fmt_, args...) \
    ul_writelog(DF_UL_LOG_WARNING,\
        "[D][%s][%d][%s] " _fmt_,\
        __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define AP_WRITE_LOG_DEBUG(_fmt_, args...)
#endif

#define AP_WRITE_LOG_EX(_loglevel_, _fmt_, args...) \
    if ((_loglevel_) <= df_log_t::get_log_level())\
    { \
        ul_writelog(_loglevel_,\
            "[%s][%d][%s] " _fmt_,\
            __FILE__, __LINE__, __FUNCTION__, ##args); \
    }

// AP_WRITE_LOG_NOTICE_US 用于系统日志的记录
// 不打印文件名，函数，行号，增加微秒数
#define AP_WRITE_LOG_NOTICE_US(_loglevel_, _fmt_, args...) \
    {\
        if ((_loglevel_) == DF_UL_LOG_NOTICE)\
        {\
            struct timeval dfs_log_tm;\
            gettimeofday(&dfs_log_tm, NULL);\
            ul_writelog(_loglevel_,\
                "[%lu][%lu] " _fmt_,dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,\
                ##args);\
        }\
    }

// AP_WRITE_LOG_US 用于系统日志的记录,带微秒数
// NOTE：对于数据/展现日志请勿使用
#define AP_WRITE_LOG_US(_loglevel_, _fmt_, args...) \
    {\
        if ((_loglevel_) <= df_log_t::get_log_level())\
        {\
            struct timeval dfs_log_tm;\
            gettimeofday(&dfs_log_tm, NULL);\
            ul_writelog(_loglevel_,\
                "[%lu][%lu][%s][%d][%s] " _fmt_,\
                dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,__FILE__, __LINE__, __FUNCTION__, ##args);\
            if ((_loglevel_) == DF_UL_LOG_FATAL || (_loglevel_) == DF_UL_LOG_WARNING)\
            {\
                if ((_loglevel_) == DF_UL_LOG_FATAL)\
                {\
                    df_log_t::set_last_fatal_tm();\
                }\
                ul_writelog(DF_UL_LOG_TRACE,\
                    "[%lu][%lu][%s][%d][%s] " _fmt_,\
                    dfs_log_tm.tv_sec,dfs_log_tm.tv_usec,__FILE__, __LINE__, __FUNCTION__, ##args);\
            }\
        }\
    }

/////////////////////////////////////////////////////////////////////////
//下面这些是不使用ul_log的代码..不会打印任何日志...:).
#else
#include <time.h>

#define DF_LOG_NONE    "DF_NONE"
#define DF_LOG_FATAL   "DF_FATAL"
#define DF_LOG_WARNING "DF_WARNING"
#define DF_LOG_NOTICE  "DF_NOTICE"
#define DF_LOG_TRACE   "DF_TRACE"
#define DF_LOG_DEBUG   "DF_DEBUG"
#define DF_LOG_ALL     "DF_ALL"

#define DF_UL_LOG_NONE		0
#define DF_UL_LOG_FATAL	0x01    /**<   fatal errors */
#define DF_UL_LOG_WARNING	0x02    /**<   exceptional events */
#define DF_UL_LOG_NOTICE   0x04    /**<   informational notices */
#define DF_UL_LOG_TRACE	0x08    /**<   program tracing */
#define DF_UL_LOG_DEBUG	0x10    /**<   full debugging */
#define DF_UL_LOG_ALL		0xff    /**<   everything     */
#define DF_UL_LOG_LEVEL_COUNT 17

const char DF_LOG_LEVEL[DF_UL_LOG_LEVEL_COUNT][16] = {
    DF_LOG_NONE, DF_LOG_FATAL, DF_LOG_WARNING, DF_LOG_NONE, DF_LOG_NOTICE,
    DF_LOG_NONE, DF_LOG_NONE, DF_LOG_NONE, DF_LOG_TRACE, DF_LOG_NONE,
    DF_LOG_NONE, DF_LOG_NONE, DF_LOG_NONE, DF_LOG_NONE, DF_LOG_NONE,
    DF_LOG_NONE, DF_LOG_DEBUG
};

// DF_OPEN_LOG 用于打开日志，和ul_openlog一致，直接替换该函数调用即可
// NOTE: 请保证使用该宏打开日志
#define DF_OPEN_LOG(_logpath_, _logname_, _plogstat_, _log_size_)

// 打开日志，将warning／fatal日志也输出到debug日志中，且使用异步方式写日志文件
#define DF_OPEN_LOG_EX(_logpath_, _logname_, _plogstat_, _log_size_)

#define DF_CLOSE_LOG()

// DF_WRITE_LOG 用于系统日志的记录
// NOTE：对于数据/展现日志请勿使用
#define DF_WRITE_LOG(_loglevel_, _fmt_, args...)

#define DF_WRITE_LOG_DEBUG(_fmt_, args...)

#define DF_WRITE_LOG_EX(_loglevel_, _fmt_, args...)

// DF_WRITE_LOG_NOTICE_US 用于系统日志的记录
// 不打印文件名，函数，行号，增加微秒数
#define DF_WRITE_LOG_NOTICE_US(_loglevel_, _fmt_, args...)

// DF_WRITE_LOG_US 用于系统日志的记录,带微秒数
// NOTE：对于数据/展现日志请勿使用
#define DF_WRITE_LOG_US(_loglevel_, _fmt_, args...)

// AP_WRITE_LOG 用于系统日志的记录
// NOTE：对于数据/展现日志请勿使用
#define AP_WRITE_LOG(_loglevel_, _fmt_, args...)

// AP_WRITE_LOG_DEBUG用于系统调试日志的记录，正式发布会删除
// NOTE: 对于数据/展现日志请勿使用
#define AP_WRITE_LOG_DEBUG(_fmt_, args...)

#define AP_WRITE_LOG_EX(_loglevel_, _fmt_, args...)

// AP_WRITE_LOG_NOTICE_US 用于系统日志的记录
// 不打印文件名，函数，行号，增加微秒数
#define AP_WRITE_LOG_NOTICE_US(_loglevel_, _fmt_, args...)

// AP_WRITE_LOG_US 用于系统日志的记录,带微秒数
// NOTE：对于数据/展现日志请勿使用
#define AP_WRITE_LOG_US(_loglevel_, _fmt_, args...)

#endif

#endif // #ifndef __DF_LOG_H__

