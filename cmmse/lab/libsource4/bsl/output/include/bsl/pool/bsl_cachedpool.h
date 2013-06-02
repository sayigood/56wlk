/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @brief 
 *  
 **/


#ifndef  _BSL_CACHEDPOOL_H_
#define  _BSL_CACHEDPOOL_H_

#include "bsl_pool.h"

namespace bsl
{
class cached_mempool : public mempool
{
	static const int seg_size = 4096;
	size_t buf_size;
	char * free_space;
	void * memlist;
	public:
	inline cached_mempool()
	{
		buf_size = 0;
		free_space = 0;
		memlist = 0;
	}
	virtual void * malloc(size_t size);
	virtual	inline void free( void *, size_t)
	{
	}
	inline void clear()
	{
		if(0 == memlist)
			return;
		while(*(void **)memlist)
		{
			void * tmp = *(void **)memlist;
			::free(memlist);
			memlist = tmp;
		}
	}
	inline virtual ~cached_mempool()
	{
		while(memlist)
		{
			void * tmp = *(void **)memlist;
			::free(memlist);
			memlist = tmp;
		}
	}
};


}















#endif  //__/HOME/XIAOWEI/LIBSRC/BSL/POOL/BSL_CACHEDPOOL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
