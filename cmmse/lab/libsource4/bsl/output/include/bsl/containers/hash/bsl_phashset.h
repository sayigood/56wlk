/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file phashset.h
 * @brief 
 *  
 **/


#ifndef  __BSL_PHASHSET_H_
#define  __BSL_PHASHSET_H_

#include <bsl/containers/hash/bsl_phashtable.h>

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
class phashset : public bsl_phashtable<_Key, _Key, _HashFun, _Equl, 
	bsl::param_select<_Key>, _InnerAlloc>
{
public:
	typedef phashset<_Key, _HashFun, _Equl, _InnerAlloc> _Self;
	typedef bsl_phashtable<_Key, _Key, _HashFun, _Equl, bsl::param_select<_Key>, _InnerAlloc> _Base;
	typedef typename _Base::iterator iterator;
	typedef typename _Base::const_iterator const_iterator;
	typedef _Key value_type;
	typedef _Key key_type;
private:
	_Self & operator = (const _Self &) { return *this; }
public:
	/**
	 * create , set, get 方法都在基类里面
	 */
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
		return _Base::get(k);
	}
	int get(const key_type &k) {
		return _Base::get(k);
	}

	/**
	 * @brief 将key插入 hash表中
	 *
	 * @param [in/out] k   : const key_type&	key值	
	 * @return  int 	
	 *  			返回	-1	表示set调用出错, (无法分配新结点)
	 * 			其他均表示插入成功：插入成功分下面三个状态
	 * 				返回	HASH_INSERT_SEC	表示插入新结点成功
	 * 				返回	HASH_EXIST	表示hash表结点存在(在flag为0的时候返回)
	 * @retval   
	 * @see 
	**/
	int set(const key_type &k) {
		return _Base::set(k, k);
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
		return _Base::erase(k);
	}

};
	
};















#endif  //__PHASHSET_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
