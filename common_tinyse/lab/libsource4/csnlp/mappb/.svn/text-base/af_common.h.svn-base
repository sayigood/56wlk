#ifndef _AF_COMMON_H
#define _AF_COMMON_H

#include<vector>
#include <string>
#include "odict.h"
#include "ul_log.h"
#include "ul_conf.h"
#include "af_utils.h"

#include "af_sign.h"


using namespace std;

/**
  *
  *
  **/
#define FATAL_LOG_NLP(fmt, arg...) do { \
	ul_writelog(UL_LOG_FATAL, "[csnlp][%s() %s:%d] " fmt, __FUNCTION__, __FILE__, __LINE__, ## arg); \
} while (0)

/**
  *
  *
  **/
#define WARN_LOG_NLP(fmt, arg...) do { \
	ul_writelog(UL_LOG_WARNING, "[csnlp][%s() %s:%d] " fmt, __FUNCTION__, __FILE__, __LINE__, ## arg); \
} while (0)

/**
  *
  *
  **/
#define NOTICE_LOG_NLP(fmt, arg...) do { \
	ul_writelog(UL_LOG_NOTICE, fmt, ## arg); \
} while (0)

/**
  *
  *
  **/
#define DEBUG_LOG_NLP(fmt, arg...) do { \
    ul_writelog(UL_LOG_DEBUG, fmt, ## arg); \
}while (0)

#endif
