/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file phashmap.h
 * @brief 
 *  
 **/


#ifndef  __BSL_PHASHMAP_H_
#define  __BSL_PHASHMAP_H_

#include <bsl/containers/hash/bsl_phashtable.h>

namespace bsl
{

template <class _Key, /*作为hashkey的类型*/
		 class _Value,	/*作为hash value的类型*/
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
class phashmap : public bsl_phashtable<_Key, std::pair<_Key, _Value>, _HashFun, _Equl, 
	bsl::pair_first<std::pair<_Key, _Value> >, _InnerAlloc>
{
public:
	typedef std::pair<_Key, _Value> _Pair;
	typedef phashmap<_Key, _Value, _HashFun, _Equl, _InnerAlloc> _Self;
	typedef bsl_phashtable<_Key, _Pair, _HashFun, _Equl, bsl::pair_first<_Pair>, _InnerAlloc> _Base;
	typedef typename _Base::iterator iterator;
	typedef typename _Base::const_iterator const_iterator;

	typedef _Value value_type;
	typedef _Key key_type;

private:
	_Self & operator = (const _Self &) { return  *this; }
public:
	/**
	 * create方法参考phashtable的方法,直接继承
	 * destroy方法也直接继承
	 */

	/**
	 * @brief 根据指定key获取value值
	 *
	 * @param [in/out] k   : const key_type&	指定的查找key
	 * @param [in/out] val   : value_type*	获取的value值的存储地址
	 * 				如果value不为空，将*val附值查找的值
	 * @return  int 
	 *				返回 HASH_EXIST		表示hash值存在
	 *				返回 HASH_NOEXIST	表示hash值不存在
	 * @retval   
	 * @see 
	**/
	int get(const key_type &k, value_type *val = 0) const {
		return _Base::get_map(k, val);
	}
	int get(const key_type &k, value_type *val = 0) {
		return _Base::get_map(k, val);
	}

	/**
	 * @brief 将key 和 value 对 插入 hash表中
	 *
	 * @param [in/out] k   : const key_type&	key值	
	 * @param [in/out] val   : const value_type&	value值
	 * @param [in/out] flag   : int	
	 * 				flag ＝＝0，如果值存在直接返回，
	 * 				非0表示，如果值存在，替换旧值
	 * @return  int 	
	 *  			返回	-1	表示set调用出错, (无法分配新结点)
	 * 			其他均表示插入成功：插入成功分下面三个状态
	 * 				返回	HASH_OVERWRITE	表示覆盖旧结点成功(在flag非0的时候返回)
	 * 				返回	HASH_INSERT_SEC	表示插入新结点成功
	 * 				返回	HASH_EXIST	表示hash表结点存在(在flag为0的时候返回)
	 * @retval   
	 * @see 
	**/
	int set(const key_type &k, const value_type &val, int flag = 0) {
		return _Base::set(k, _Pair(k, val), flag);
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


#endif  //__PHASHMAP_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
