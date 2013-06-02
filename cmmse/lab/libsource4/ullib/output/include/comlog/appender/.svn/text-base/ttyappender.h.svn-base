/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file appender/ttyappender.h
 * @brief 
 *  
 **/


#ifndef  __APPENDER_TTYAPPENDER_H_
#define  __APPENDER_TTYAPPENDER_H_

#include "comlog/appender/appender.h"

namespace comspace
{

class TtyAppender : public Appender
{
public:
	TtyAppender();
	int open(void *);
	int close(void *);
	int print(Event *evt);
	int binprint(void *, int);

public:
	static Appender *getAppender(com_device_t &dev);
	static Appender *tryAppender(com_device_t &dev);
protected:
	friend class Appender;
};

};













#endif  //__APPENDER/TTYAPPENDER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
