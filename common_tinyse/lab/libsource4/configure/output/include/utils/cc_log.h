/***************************************************************************
 * 
 * 
 **************************************************************************/



/**
 * @file cc_log.h
 * @brief 
 *  
 **/


#ifndef  __CC_LOG_H_
#define  __CC_LOG_H_

#include "ul_log.h"

namespace comcfg{
	class Log{
		static int _fatal,
			_warning,
			_notice,
			_trace,
			_debug;
		public:

		static void setFatal(int level){
			_fatal = level;
		}

		static void setWarning(int level){
			_warning = level;
		}

		static void setNotice(int level){
			_notice = level;
		}

		static void setTrace(int level){
			_trace = level;
		}
		
		static void setDebug(int level){
			_debug = level;
		}

		/**
		 * @brief 打开低等级的日志
		 *
		 * @return  void 
		 * @retval   
		 * @see 
		**/
		static void openLowLevel(){
			_debug = UL_LOG_DEBUG;
			_trace = UL_LOG_TRACE;
			_notice = UL_LOG_NOTICE;
		}

		static int fatal(){ return _fatal; }
		static int warning(){ return _warning; }
		static int notice(){ return _notice; }
		static int trace(){ return _trace; }
		static int debug(){ return _debug; }
	};
}
















#endif  //__CC_LOG_H_

		/* vim: set ts=4 sw=4 sts=4 tw=100 */
