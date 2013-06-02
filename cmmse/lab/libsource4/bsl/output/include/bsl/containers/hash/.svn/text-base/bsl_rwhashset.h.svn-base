/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file bsl_rwhashset.h
 * @brief 
 *  
 **/


#ifndef  __BSL_RWHASHSET_H_
#define  __BSL_RWHASHSET_H_

#include <pthread.h>
#include <bsl/containers/hash/bsl_hashset.h>

namespace bsl
{

template <class _Key, /*作为hashkey的类型*/
		 /**
		  * hash 函数的仿函数，比如
		  * struct xhash {
		  * 	inline size_t operator () (const _Key &_1);
		  * };
		  **/
		 class _HashFun = xhash<_Key>,
		 /**
		  * 判断两个key相等的仿函数
		  * 比如 struct equal {
		  * 	inline bool operator () (const _Tp &_1, const _Tp &_2);
		  * };
		  */
		 class _Equl = std::equal_to<_Key>,	
		 /**
		  * 空间分配器，默认的空间分配器能够高效率的管理小内存，防止内存碎片
		  * 但是在容器生命骑内不会释放申请的内存
		  *
		  * bsl_alloc<_Key>做内存分配器，可以在容器生命期内释放内存，
		  * 但是不能有效防止内存碎片
		  */
		 class _InnerAlloc = bsl_sample_alloc<bsl_alloc<_Key>, 256>
		>
class bsl_rwhashset
{	
public:
	typedef _Key key_type;

	typedef bsl_rwhashset<_Key, _HashFun, _Equl, _InnerAlloc> _Self;
	typedef bsl_hashtable<_Key, _Key, _HashFun, _Equl, param_select<_Key>, _InnerAlloc> hash_type;
	typedef typename _InnerAlloc::pool_type::template rebind<BSL_RWLOCK_T>::other lockalloc_type;
	typedef typename hash_type::node_pointer node_pointer;

	BSL_RWLOCK_T *_locks;
	pthread_mutex_t _lock;
	size_t _lsize;
	lockalloc_type _alloc;
	hash_type _ht;

private:
	bsl_rwhashset (const _Self &) {}
	_Self & operator = (const _Self &) { return *this; }
public:
	bsl_rwhashset() {
		_locks = 0;
		_lsize = 0;
	}
	~bsl_rwhashset() {
		destroy();
	}

	BSL_RWLOCK_T & get_rwlock(size_t key) const {
		return _locks[key % _ht._bitems % _lsize];
	}
	/**
	 * @brief 创建一张线程安全的hash表
	 *
	 * @param [in/out] bitems   : size_t	hash桶个数
	 * @param [in/out] lsize   : size_t		读写锁个数
	 * @param [in/out] hf   : const _HashFun&	hash函数
	 * @param [in/out] eq   : const _Equl&		比较函数
	 * @return  int 返回 0表示创建成功，其他失败
	 * @retval  线程不安全
	 * @see 
	**/
	int create(size_t bitems, size_t lsize = 0, 
			const _HashFun &hf = _HashFun(), 
			const _Equl &eq = _Equl()) {
		if (bitems >= size_t(-1) / sizeof(void *)) {
			__BSL_ERROR("too large bitems, overflower");
			return -1;
		}
		destroy();

		if (lsize > bitems) {
			_lsize = bitems;
		} else {
			_lsize = lsize;
		}
		if (_lsize <= 0) {
			_lsize  = (1 + (size_t)((float)(0.05)*(float)bitems));
			if (_lsize > 100) {
				_lsize = 100;
			}
		}

		_alloc.create();

		_locks = _alloc.allocate(_lsize);
		if (_locks == 0) return -1;
		for(size_t i=0; i<_lsize; ++i) {
			BSL_RWLOCK_INIT(_locks+i);
		}

		pthread_mutex_init(&_lock, NULL);
		return _ht.create(bitems, hf, eq);
	}

	/**
	 * @brief 销毁hash表
	 *
	 * @return  int 返回0表示删除成功，其他表示删除失败
	 * @retval  线程不安全
	 * @see 
	**/
	int destroy() {
		if (_locks) {
			_alloc.deallocate(_locks, _lsize);
			pthread_mutex_destroy(&_lock);
		}
		_alloc.destroy();
		_locks = NULL;
		_lsize = 0;
		return _ht.destroy();
	}

	/**
	 * @brief 查询key_type是否存在
	 *
	 * @param [in/out] k   : const key_type&	指定的查找key
	 * @return  int 
	 *				返回 HASH_EXIST		表示hash值存在
	 *				返回 HASH_NOEXIST	表示hash值不存在
	 * @retval   
	 * @see 
	**/
	int get(const key_type &k) const {
		size_t key = _ht._hashfun(k);
		BSL_RWLOCK_T &__lock = get_rwlock(key);
		BSL_RWLOCK_RDLOCK(&__lock);
		int ret = HASH_EXIST;
		if (_ht.find(key, k) == NULL) {
			ret = HASH_NOEXIST;
		}
		BSL_RWLOCK_UNLOCK(&__lock);
		return ret;
	}
	int get(const key_type &k) {
		size_t key = _ht._hashfun(k);
		BSL_RWLOCK_T &__lock = get_rwlock(key);
		BSL_RWLOCK_RDLOCK(&__lock);
		int ret = HASH_EXIST;
		if (_ht.find(key, k) == NULL) {
			ret = HASH_NOEXIST;
		}
		BSL_RWLOCK_UNLOCK(&__lock);
		return ret;
	}

	/**
	 * @brief 将key插入hash表
	 *
	 * @param [in/out] k   : const key_type&
	 * @return  int 
	 * 			返回	-1表示set调用出错
	 * 			返回	HASH_EXIST	表示hash结点存在
	 * 			返回	HASH_INSERT_SEC	表示插入成功
	 * @retval   
	 * @see 
	**/
	int set(const key_type &k) {
		size_t key = _ht._hashfun(k);
		pthread_mutex_lock(&_lock);

		BSL_RWLOCK_T &__lock = get_rwlock(key);
		BSL_RWLOCK_WRLOCK(&__lock);
		int ret = _ht.set(key, k, k);
		BSL_RWLOCK_UNLOCK(&__lock);

		pthread_mutex_unlock(&_lock);
		return ret;
	}

	/**
	 * @brief 根据指定的key删除结点
	 *
	 * @param [in/out] k   : const key_type&
	 * @return  int 
	 * 		返回	HASH_EXIST表示结点存在并删除成功
	 * 		返回	HASH_NOEXIST表示结点不存在不用删除
	 * @retval   
	 * @see 
	**/
	int erase(const key_type &k) {
		size_t key = _ht._hashfun(k);
		pthread_mutex_lock(&_lock);

		BSL_RWLOCK_T &__lock = get_rwlock(key);
		BSL_RWLOCK_WRLOCK(&__lock);
		int ret = _ht.erase(key, k);
		BSL_RWLOCK_UNLOCK(&__lock);

		pthread_mutex_unlock(&_lock);
		return ret;
	}


	/**
	 * @brief 将数据清空，线程安全
	 *
	 * @return  int 
	 * @retval   
	 * @see 
	**/
	int clear() {
		//读写锁每被初始化,没有被create
		if (_locks == NULL) {
			return 0;
		}
		pthread_mutex_lock(&_lock);
		for (size_t i=0; i<_ht._bitems; ++i) {
			BSL_RWLOCK_T &__lock = get_rwlock(i);
			BSL_RWLOCK_WRLOCK(&__lock);
			//typename hash_type::node_t *nd = NULL;
			node_pointer nd = 0;
			while (_ht._bucket[i]) {
				nd = _ht._bucket[i];
				_ht._bucket[i] = _ht._sp_alloc.getp(_ht._bucket[i])->next;
				bsl::bsl_destruct(&_ht._sp_alloc.getp(nd)->val);
				_ht._sp_alloc.deallocate(nd, 1);
			}
			BSL_RWLOCK_UNLOCK(&__lock);
		}
		_ht.clear();//什么都发生，只清空一下标志位
		pthread_mutex_unlock(&_lock);
		return 0;
	}

	size_t size() const {
		return _ht.size();
	}

	template <class _Iterator>
	int assign(_Iterator __begin, _Iterator __end) {
		return _ht.assign(__begin, __end);
	}

};

};

#endif  //__BSL_RWHASHSET_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
