/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file bsl_xcompool.h
 * @brief 
 *  
 **/


#ifndef  __BSL_XCOMPOOL_H_
#define  __BSL_XCOMPOOL_H_


#include "bsl_xmempool.h"
#include <set>

namespace bsl
{
class xcompool : public mempool
{
	size_t _maxblksiz;
	void * _buffer;
	size_t _bufsiz;
	xmempool _pool;

	typedef std::set<void *> SET;
	SET _set;
public:
	xcompool() {
		_maxblksiz = 0;
		_buffer = NULL;
	}
	~xcompool() {
		destroy();
	}
	/**
	 * @brief 初始化内存池
	 *
	 * @param [in/out] maxblk   : size_t 每个slab管理的内存大小
	 * @param [in/out] bmin   : size_t	slab管理的最小内存单元
	 * @param [in/out] bmax   : size_t	slab管理的最大内存单元
	 * @param [in/out] rate   : float	slab的递增因子
	 * @return  int  成功返回0, 其他失败
	 * @retval   
	 * @see 
	 * @note 
	**/
	int create (size_t maxblk = 1<<20, 
			size_t bmin = sizeof(void *), size_t bmax = (1<<16), 
			float rate = 2.0f);

	/**
	 * @brief 销毁内存池
	 *
	 * @return  void 
	 * @retval   
	 * @see 
	 * @note 
	**/
	void destroy();

	void * malloc (size_t size);

	void free (void *ptr, size_t size);

	/**
	 * @brief 回收pool分配的内存
	 *
	 * @return  void 
	 * @retval   
	 * @see 
	 * @note 
	**/
	void clear ();

};
};



#endif  //__BSL_XCOMPOOL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
