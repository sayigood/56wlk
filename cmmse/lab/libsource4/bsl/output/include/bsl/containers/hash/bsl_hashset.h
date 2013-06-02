/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file hashset.h
 * @brief 
 *  
 **/


#ifndef  __BSL_HASHSET_H_
#define  __BSL_HASHSET_H_


#include <bsl/containers/hash/bsl_hashtable.h>

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
class hashset
{
	typedef _Key key_type;
	typedef _Key value_type;

	typedef hashset<_Key, _HashFun, _Equl, _InnerAlloc> _Self;
	typedef bsl_hashtable<_Key, _Key, _HashFun, _Equl,  param_select<_Key>, _InnerAlloc> hash_type;
	hash_type _ht;
public:
	typedef typename hash_type::iterator iterator;
	typedef typename hash_type::const_iterator const_iterator;

private:
	_Self & operator = (const _Self &) { return *this; }
public:
	/**
	 * @brief 返回迭代器的起始地址
	 *
	 * @return  iterator 
	 * @retval   
	 * @see 
	**/
	iterator begin() {
		return _ht.begin(); 
	}
	const_iterator begin() const {
		return _ht.begin();
	}

	/**
	 * @brief 返回迭代器的结束地址
	 *
	 * @return  iterator 
	 * @retval   
	 * @see 
	**/
	iterator end() { 
		return _ht.end(); 
	}
	const_iterator end() const {
		return _ht.end();
	}

	/**
	 * @brief 返回hash表目前有多少个元素
	 *
	 * @return  size_t 当前的元素个数
	 * @retval   
	 * @see 
	**/
	size_t size() const { 
		return _ht.size(); 
	}

	/**
	 * @brief 创建一张hashmap
	 *
	 * @param [in/out] bitems   : int	设置hash桶的大小
	 * @param [in/out] hf   : const _HashFun& 设置hash函数
	 * @param [in/out] eq   : const _Equl&	设置相等比较函数
	 * @return  int 返回	0		表示创建成功
	 * 				返回	其他	表示创建失败
	 * @retval   
	 * @see 
	**/
	int create(size_t bitems, const _HashFun &hf = _HashFun(), const _Equl & eq = _Equl()) {
		if (_ht.create(bitems, hf, eq) != 0) {
			return -1;
		}
		return 0;
	}

	/**
	 * @brief 销毁hash表
	 *
	 * @return  int 返回0表示删除成功，其他表示删除失败
	 * @retval   
	 * @see 
	**/
	int destroy() {
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
		//返回的是指针
		if (_ht.find(_ht._hashfun(k), k) == NULL) return HASH_NOEXIST;
		return HASH_EXIST;
	}
	int get(const key_type &k) {
		//返回的是指针
		if (_ht.find(_ht._hashfun(k), k) == NULL) return HASH_NOEXIST;
		return HASH_EXIST;
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
		return  _ht.set(_ht._hashfun(k), k, k);
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
		return _ht.erase(_ht._hashfun(k), k);
	}

	/**
	 * @brief 将其他容器对hash表进行附值
	 *
	 * @param [in/out] __begin   : _Iterator	迭代器的起始地址
	 * @param [in/out] __end   : _Iterator		迭代器的结束地址
	 * @return int 	0	表示附值成功，其他表示失败
	 * @retval   
	 * @see 
	**/
	template <class _Iterator>
	int assign(_Iterator __begin, _Iterator __end) {
		return _ht.assign(__begin, __end);
	}

	template <class _Archive>
	int serialization(_Archive &ar) {
		return bsl::serialization(ar, _ht);
	}

	template <class _Archive>
	int deserialization(_Archive &ar) {
		return bsl::deserialization(ar, _ht);
	}

	int clear() {
		return _ht.clear();
	}
};
}
#endif  //__HASHSET_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
