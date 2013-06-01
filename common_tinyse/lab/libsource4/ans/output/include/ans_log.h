#ifndef _ANS_LOG_H
#define _ANS_LOG_H

#define _ANS_FATAL_LOG(fmt, arg...) do { \
	    ul_writelog(UL_LOG_FATAL, "[0][%s:%d]" fmt, __FILE__, __LINE__, ## arg); \
} while (0)

#define _ANS_WARNING_LOG(fmt, arg...) do { \
	    ul_writelog(UL_LOG_WARNING, "[0][%s:%d]" fmt, __FILE__, __LINE__, ## arg); \
} while (0)

#define _ANS_NOTICE_LOG(fmt, arg...) do { \
	    ul_writelog(UL_LOG_NOTICE, fmt, ## arg); \
} while (0)

#define _ANS_TRACE_LOG(fmt, arg...) do { \
	    ul_writelog(UL_LOG_TRACE, fmt, ## arg); \
} while (0)

#define _ANS_DEBUG_LOG(fmt, arg...) do { \
	    ul_writelog(UL_LOG_DEBUG, fmt, ## arg); \
} while (0)

#endif
