/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: debug.h,v 1.1 2008/08/22 04:02:42 yanghy2 Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file debug.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/04/26 14:21:39
 * @version $Revision: 1.1 $ 
 * @brief 
 *  
 **/


#ifndef  __DEBUG_H_
#define  __DEBUG_H_



#ifdef MY_DBUG
#define ASSERT(x)  do{assert(x);}while(0)
#define ASSERT_I(x,y) do {\
if ((x)!=(y)){\
	UB_LOG_WARNING("ASSERT_TEST: (%d) == (%d) FAILED",(x),(y));\
}else{\
	UB_LOG_NOTICE("ASSERT_TEST: (%d) == (%d) SUCCESS",(x),(y));\
}\
}while(0)

#define ASSERT_S(x,y) do {\
if (strcmp((x),(y))!=0){\
	UB_LOG_WARNING("ASSERT_TEST: (%s) == (%s) FAILED",(x),(y));\
}else{\
	UB_LOG_NOTICE("ASSERT_TEST: (%s) == (%s) SUCCESS",(x),(y));\
}\
}while(0)	
#else
#define	ASSERT(x) do{}while(0)
#define ASSERT_I(x,y) do{}while(0)
#define ASSERT_S(x,y) do{}while(0)
#endif

#define WARN_RETURN(fmt,arg...)  do {UB_LOG_WARNING(fmt,## arg); return -1;}while(0)
#define FATAL_RETURN(fmt,arg...)  do {UB_LOG_FATAL(fmt,## arg); return -1;}while(0)












#endif  //__DEBUG_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
