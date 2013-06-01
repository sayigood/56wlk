#ifndef _BSL_SLIST__
#define _BSL_SLIST__

#include <bsl/alloc/bsl_alloc.h>
#include <bsl/alloc/bsl_sample_alloc.h>
#include <bsl/archive/bsl_serialization.h> 
#if 0
template <typename _Tp>
class alloc
{
public:
	typedef _Tp* pointer;	//空间地址指针
	typedef _Tp* const const_pointer;	//常空间地址指针
	typedef _Tp& reference;	//常空间地址指针
	_Tp * allocate(int n)
	{
		return new _Tp[n];
	}
	void deallocate(_Tp * _x )
	{
		delete [] _x;
	}	

};
#endif
namespace bsl
{

template <typename _Tp, typename _Alloc>
struct slist_node_t
{
	typedef _Tp value_type;
	typedef slist_node_t<_Tp, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	node_pointer next;
	value_type val;
};

template <class _Tp, class _Alloc > class slist;

template<class _Tp, class _Ptr, class _Ref, class _Alloc>
class slist_const_iterator
{
private:
	typedef slist<_Tp, _Alloc> slist_t;
	//typedef std::forward_iterator_tag iterator_category;
public:
	typedef typename slist_t::value_type value_type;
	typedef typename slist_t::iterator iterator;
	typedef _Ptr pointer;
	typedef _Ref reference;
private:
	typedef slist_const_iterator<_Tp, _Ptr, _Ref, _Alloc> self;
	typedef slist_node_t<value_type, _Alloc> node_t;
	friend class slist<value_type, _Alloc>;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	slist_const_iterator(node_pointer __x)
	{
		_node = __x;
	}
public:
	node_pointer _node;
	slist_const_iterator()
	{
		_node = 0;
	}
	slist_const_iterator(const self& __x)
	{
		_node = __x._node;
	}
	slist_const_iterator(const iterator & __x)
	{
		_node = __x._node;
	}
	reference operator *() const
	{
		return _node->val;
	}
	bool operator == (const self & __iter) const
	{
		return (_node == __iter._node);
	}
	bool operator != (const self & __iter) const
	{
		return (_node != __iter._node);
	}
	pointer operator ->() const
	{
		return &_node->val;
	}
	self & operator ++ ()
	{
		_node = _node->next;
		return *this;
	}
	self operator ++ (int)
	{
		self iter = *this;
		++ *this;
		return iter;
	}
	
};

template<class _Tp, class _Ptr, class _Ref, class _Alloc>
class slist_iterator
{
private:
	typedef slist<_Tp, _Alloc> slist_t;
	//typedef std::forward_iterator_tag iterator_category;
public:
	typedef typename slist_t::value_type value_type;
	typedef typename slist_t::iterator iterator;
	typedef typename slist_t::const_iterator const_iterator;
	typedef _Ptr pointer;
	typedef _Ref reference;
private:
	typedef slist_iterator<_Tp, _Ptr, _Ref, _Alloc> self;
	typedef slist_node_t<value_type, _Alloc> node_t;
	friend class slist<value_type, _Alloc>;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	slist_iterator(node_pointer __x)
	{
		_node = __x;
	}
public:
	node_pointer _node;
	slist_iterator()
	{
		_node = 0;
	}
	slist_iterator(const self& __x)
	{
		_node = __x._node;
	}
	reference operator *() const
	{
		return _node->val;
	}
	bool operator == (const self & __iter) const
	{
		return (_node == __iter._node);
	}
	bool operator != (const self & __iter) const
	{
		return (_node != __iter._node);
	}
	pointer operator ->() const
	{
		return &_node->val;
	}
	self & operator ++ ()
	{
		_node = _node->next;
		return *this;
	}
	self operator ++ (int)
	{
		self iter = *this;
		++ *this;
		return iter;
	}
	
};
template <class _Tp, class _Alloc = bsl_sample_alloc<bsl_alloc<_Tp>, 256> >
class slist
{
private:
	typedef slist<_Tp,_Alloc> self;
	typedef slist_node_t<_Tp, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
public:
	typedef _Tp value_type;
	typedef _Tp *  pointer;
	typedef const _Tp * const_pointer;
	typedef _Tp & reference; 
	typedef const _Tp & const_reference; 
	typedef size_t size_type;
	typedef long difference_type;
	typedef slist_iterator<value_type, pointer, reference, _Alloc> iterator;
	typedef slist_const_iterator<value_type, const_pointer, const_reference, _Alloc> const_iterator;
private:
	typedef typename node_alloc_t::pointer node_pointer;	
	//typedef typename node_alloc_t::const_pointer node_const_pointer;	
	//typedef typename node_alloc_t::reference node_reference; 
	//typedef typename node_alloc_t::const_reference node_const_reference; 
	//	friend class slist_iterator<_Tp, _Alloc>;
	node_pointer _header;
	node_alloc_t _node_allocator;
	/**
	 * @brief 
	 * 私有拷贝构造函数，不应被使用
	 * @see 
	**/
	slist( const self&)
	{
		_header = 0;
	}
	/**
	 * @brief 
	 * 私有赋值函数，不应被使用
	 * @return  self operator 
	 * @retval   
	 * @see 
	**/
	self& operator = (const self&)
	{
		return *this; 
	}
public:
	/**
	 * @brief 
	 * 构造函数
	 * @see 
	**/
	slist()
	{
		_header = 0;
	}
	/**
	 * @brief 
	 * 为容器类分配内存
	 * @return  int 
	 * @retval  成功返回0，其他返回-1
	 * @see 
	**/
	int create()
	{
		return _node_allocator.create();

	}
	/**
	 * @brief 
	 * 释放容器类占用的内存
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int destroy()
	{
		this->clear();
		_node_allocator.destroy();
		return 0;

	}
	/**
	 * @brief 
	 * 析构函数
	 * @see 
	**/
	~slist()
	{
		destroy();
	}
	/**
	 * @brief 
	 * 从另一个容器复制副本
	 * @param [in/out] __x   : slist_t& 要拷贝的实例
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int assign (const self & __x)
	{
		if (&__x != this)
			return assign(__x.begin(), __x.end());
		else
		{
			return 0;
		}
	}

	/**
	 * @brief 
	 * 从迭代器复制副本
	 * @param [in/out] __begin   : __iterator
	 * @param [in/out] __end   : __iterator
	 * @return  template <typename __iterator>	 	int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	template <typename __iterator>	
	int assign(__iterator __begin, __iterator __end)
	{
		node_pointer tmp_header;
		node_pointer cur = reinterpret_cast<node_pointer>(&tmp_header);
		__iterator tp = __begin;
		while(tp != __end)
		{
			node_pointer tmp = _node_allocator.allocate(1);
			if (!tmp)
				goto err1;
			bsl::bsl_construct(&tmp->val, *tp);
			cur->next = tmp;
			cur = tmp;
			++tp;
		}
		cur->next = 0;
		clear();
		_header = tmp_header;
		return 0;
	err1:
		if(0 == tmp_header->next)
			return -1;
		else
			tmp_header = tmp_header->next;
		do
		{
			node_pointer tmp = tmp_header->next;
			bsl::bsl_destruct(&(tmp_header->val));
			_node_allocator.deallocate(tmp_header, 1);
			tmp_header = tmp;

		}
		while(cur != tmp_header);
		return -1;
	}

	/**
	 * @brief 
	 * 清除所有元素
 	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int clear()
	{
		while(_header)
		{
			pop_front();
		}
		return 0;
	}

	/**
	 * @brief 
	 * 检查容器是否为空
	 * @return  bool 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	inline bool empty() const
	{
		return _header == 0;
	}

	/**
	 * @brief 
	 * 将新元素加入到链表头
	 * @param [in/out] __x   : value_type&
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int push_front(const value_type &__x)
	{
		node_pointer tmp = _node_allocator.allocate(1);
		if (!tmp)
			goto err;
		bsl::bsl_construct(&tmp->val, __x);
		tmp->next = _header;
		_header = tmp;
		return 0;
	err:
		return -1;
	}

	/**
	 * @brief 
	 * 删除链表的头元素
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int pop_front()
	{
		if ( 0 == _header)
			return -1;
		node_pointer tmp = _header->next;
		bsl::bsl_destruct(&(_header->val));
		_node_allocator.deallocate(_header, 1);
		_header = tmp;
		return 0;
	}

	/**
	 * @brief 
	 * 将元素插入到__i指向的位置之后，如果迭代器指向空节点，则插入到头节点。
	 * @param [in/out] __i   : iterator
	 * @param [in/out] __x   : value_type&
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int insert_after( iterator __i,  const value_type &__x)
	{
		if( 0 == __i._node && _header != 0)
			return -1;
		node_pointer tmp = _node_allocator.allocate(1);
		if (!tmp)
			goto err;
		bsl::bsl_construct(&tmp->val, __x);
		if(__i._node)
		{
			tmp->next = __i._node->next;
			__i._node->next = tmp;
		}
		else
		{
			tmp->next = _header;
			_header = tmp;
		}
		return 0;
	err:
		return -1;
	}
	/**
	 * @brief 
	 * 删除__i指向的节点的下一个节点
	 * @param [in/out] __i   : iterator
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int erase_after ( iterator __i)
	{
		if( 0 == __i._node)
			return -1;
		node_pointer tmp = __i._node->next;
		__i._node->next = tmp->next;
		bsl::bsl_destruct(&(tmp->val));
		_node_allocator.deallocate(tmp, 1);
		return 0;
	}
	/**
	 * @brief 
	 * 删除__s和__e之间的节点，区间为(__s,__e)
	 * @param [in/out] __s   : iterator
	 * @param [in/out] __e   : iterator
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int erase_after ( iterator __s, iterator __e)
	{
		if(__s._node == 0)
			return -1;
		node_pointer s = __s._node->next;
		node_pointer e = __e._node;
		while(s != e)
		{
			node_pointer tmp = s;
			s = s->next;
			bsl::bsl_destruct(&(tmp->val));
			_node_allocator.deallocate(tmp, 1);
		}
		__s._node->next = __e._node;
		return 0;
	}
	/**
	 * @brief 
	 * 返回头元素的常引用
	 * @return  const_reference 
	 * @retval   
	 * @see 
	**/
	inline const_reference front() const
	{
		if( 0 == _header )
			abort();
		return _header->val;
	}
	
	/**
	 * @brief 
	 * 返回头元素的引用
	 * @return  reference 
	 * @retval
	 * @see 
	**/
	inline reference front()
	{
		if( 0 == _header )
			abort();
		return _header->val;
	}

	/**
	 * @brief 
	 * 返回指向头节点的常迭代器
	 * @return  const_iterator 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	inline const_iterator begin() const
	{
		return const_iterator(_header);
	}

	/**
	 * @brief 
	 * 返回指向头节点的迭代器
	 * @return  iterator 
	 * @retval   
	 * @see 
	**/
	inline iterator begin()
	{
		return iterator(_header);
	}

	/**
	 * @brief 
	 * 返回指向尾节点的常迭代器
	 * @return  const_iterator 
	 * @retval   
	 * @see 
	**/
	inline const_iterator end() const
	{
		return const_iterator(0);
	}

	/**
	 * @brief 
	 * 返回指向尾节点的迭代器
	 * @return  iterator 
	 * @retval   
	 * @see 
	**/
	inline iterator end()
	{
		return iterator(0);
	}
	/**
	 * @brief 
	 * 将链表反序排列
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int reverse()
	{
		node_pointer prev = 0;
		node_pointer cur = _header;
		while(cur != 0)
		{
			node_pointer tmp = cur->next;
			cur->next = prev;
			prev = cur;
			cur = tmp;
		}
		_header = prev;
		return 0;
	}
	
	/**
	 * @brief 
	 * 返回指向上一个节点的迭代器，复杂度O(n)
	 * @param [in/out] pos   : iterator
	 * @return  iterator 
	 * @retval   
	 * @see 
	**/
	iterator previous(iterator pos)
	{
		if(0 == pos._node)
			return iterator(0);
		node_pointer tp = _header;
		if(tp == pos._node)
			return iterator(0);
		while(tp != 0)
		{
			if(tp->next == pos._node)
				return iterator(tp);
			else
				tp = tp->next;
		}
		return iterator(0);
	}
	
	/**
	 * @brief 
	 * 返回指向上一个节点的迭代器，复杂度O(n)
	 * @param [in/out] pos   : const_iterator
	 * @return  const_iterator 
	 * @retval   
	 * @see 
	**/
	const_iterator previous(const_iterator pos) const
	{
		if(0 == pos._node)
			return const_iterator(0);
		node_pointer tp = _header;
		while(tp->next != pos._node)
		{
			tp = tp->next;
		}
		return const_iterator(tp);
	}
	/**
	 * @brief 
	 * 返回容器的节点数量
	 * @return  int 
	 * @retval   
	 * @see 
	**/
	size_type size() const
	{
		size_type l=0;
		node_pointer cur=_header;
		while(cur)
		{
			l++;
			cur=cur->next;
		}
		return l;
	}
	/**
	 * @brief 
	 * 返回容器所能容纳的最大节点数量
	 * @return  size_type 
	 * @retval   
	 * @see 
	**/
	inline size_type max_size() const
	{
		return (size_type)-1;
	}
	/**
	 * @brief 
	 * 和__x容器交换内容
	 * @param [in/out] __x   : self&
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	int swap( self & __x)
	{
		//if(__x
		_node_allocator.swap(__x._node_allocator);
		std::swap(_header, __x._header);
		//check();
		//__x.check();
		return 0;
	}
	/**
	 * @brief 
	 * 序列化
	 * @param [in/out] ar   : _Archive&
	 * @return  template <typename _Archive> 	int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	template <typename _Archive>
	int serialization(_Archive &ar)
	{
		size_type l = size();
		if(bsl::serialization(ar, l))
			goto err;
		for(iterator i=begin(); i!=end(); i++)
		{
			if(bsl::serialization(ar, *i))
				goto err;
		}
		return 0;
	err:
		return -1;
	}

	/**
	 * @brief 
	 * 反序列化
	 * @param [in/out] ar   : _Archive&
	 * @return  template <typename _Archive> 	int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	template <typename _Archive>
	int deserialization(_Archive &ar)
	{
		node_pointer cur;
		size_type l;
		clear();
		if(bsl::deserialization(ar, l))
			goto err;
		cur = reinterpret_cast<node_pointer>(&_header);
		for(size_type i=0; i<l; i++)
		{
			value_type tmp;
			if(bsl::deserialization(ar, tmp))
				goto err;
			if(insert_after(iterator(cur), tmp))
				goto err2;
			cur = cur->next;
		}
		return 0;
	err:
	err2:
		clear();
		return -1;

	}
#define __BSL_LIST_STACK_DEPTH 64
	/**
	 * @brief 
	 * 按递增顺序排序
	 * @return  int 
	 * @retval   成功返回0，其他返回-1
		tmp->val = __x;
	 * @see 
	**/
	inline int sort()
	{
		return sort(std::less<_Tp>());
	}
	/**
	 * @brief 
	 * 按Predicate指定的顺序排序
	 * @param [in/out] _p   : Predicate
	 * @return  template <typename Predicate>	 	int 
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	template <typename Predicate>	
	int sort(Predicate _p)
	{
		if( 0 == _header)
			return 0;
		node_pointer cur = _header;
		node_pointer counter[__BSL_LIST_STACK_DEPTH];
		for(int i=0; i<__BSL_LIST_STACK_DEPTH ; i++)
		{
			counter[i] = 0;
		}
	//	int fill=0;
		while( cur != 0)
		{
			node_pointer tmp = cur;
			cur = cur->next;
			tmp->next = 0;
			int i = 0;
			while(counter[i] != 0)
			{
				tmp = _merge(tmp,counter[i], _p);
				counter[i]=0;
				i++;
			}
			counter[i] = tmp;
	//		if(i > fill)
	//			fill = i;
		}
		for(int i = 1; i < __BSL_LIST_STACK_DEPTH ; i++)
		{
			counter[i] = _merge(counter[i-1], counter[i], _p);
		}
		_header = counter[__BSL_LIST_STACK_DEPTH - 1];
		return 0;
	}
	
	/**
	 * @brief 
	 * 将两个已排序的链表合并，合并后，__other将为空 
	 * @param [in/out] __other   : self&
	 * @return  int 
	 * @retval   
	 * @see 
	**/
	int merge(self & __other)
	{
		return merge(__other, std::less<_Tp>());
	}

	/**
	 * @brief 
	 * 将两个已排序的链表合并，合并后，__other将为空
	 * @param [in/out] __other   : self&
	 * @param [in/out] __p   : _Predicator
	 * @return  template <typename _Predicator> 	int 
	 * @retval   
	 * @see 
	**/
	template <typename _Predicator>
	int merge(self & __other, _Predicator __p)
	{
		if(_header != 0 && __other._header != 0)
		{
			_node_allocator.merge(__other._node_allocator);
			_header = _merge(_header, __other._header, __p);
			__other._header = 0;
			return 0;
		}
		else if(__other._header != 0)
		{
			return swap(__other);
		}
		else
		{
			return 0;
		}
	}
	/**
	 * @brief 
	 * 从链表中删除与__x值相同的实例
	 * @param [in] __x   : const value_type&
	 * @return  int 
	 * @retval  成功返回0，其他返回-1 
	 * @see 
	**/
	int remove(const value_type & __x)
	{
		return remove_if(std::bind2nd(std::equal_to<_Tp>(), __x));
	}
	/**
	 * @brief 
	 * 根据条件函数删除元素
	 * @param [in] __p   : Predicate
	 * @return  template<class Predicate> 当Predicate返回true时，元素被移除
	 * @retval   成功返回0，其他返回-1
	 * @see 
	**/
	template<class Predicate> 
	int remove_if(Predicate __p) 
	{
		//为了避免对头节点特殊处理，这里强制将_header看作特殊的节点。
		node_pointer cur = reinterpret_cast<node_pointer>(&_header);
		while(cur->next != 0)
		{
			if(__p(cur->next->val))
			{
				node_pointer tmp = cur->next;
				cur->next = tmp->next;
				bsl::bsl_destruct(&(tmp->val));
				_node_allocator.deallocate(tmp, 1);
			}
			else
			{
				cur = cur->next;
			}
		}
		return 0;
	}
private:
	template <typename _Predicate>
	inline node_pointer _merge(node_pointer list1, node_pointer list2, const _Predicate &_p)
	{
		if(0 == list2)
			return list1;
		if(0 == list1)
			return list2;
		volatile node_pointer tmp_root;
		//为了避免对头节点特殊处理，这里强制将_header看作特殊的节点。
		node_pointer tp = (node_pointer)(&tmp_root);
		node_pointer a = list1;
		node_pointer b = list2;
		while(true)
		{
			if(_p(a->val,b->val))
			{
				tp->next = a;
				if(a->next != 0)
				{
					tp = a;
					a = a->next;
				}
				else
				{
					a->next = b;
					return tmp_root;
				}

			}
			else
			{
				tp->next = b;
				if(b->next != 0)
				{
					tp = b;
					b = b->next;
				}
				else
				{
					b->next = a;
					return tmp_root;
				}
			}
		}
	}
};

}

#endif
