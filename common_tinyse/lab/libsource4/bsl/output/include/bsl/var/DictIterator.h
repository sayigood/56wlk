/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file DictIterator.h
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_DICT_ITERATOR_H__
#define  __BSL_VAR_DICT_ITERATOR_H__

#include "bsl/containers/string/bsl_string.h"
namespace bsl{
namespace var{
    //forward declaration
    class IVar;
    class IDictConstIteratorImpl;
    class IDictIteratorImpl;
    class DictConstIterator;
    class DictIterator;

    //interface definition
    class IDictIteratorImpl{
    public:
        typedef bsl::string          string_type;
        typedef IDictIteratorImpl        implement_type;
        typedef IDictConstIteratorImpl   const_implement_type;

        virtual ~IDictIteratorImpl() {}
        virtual const string_type&  key() const = 0;
        virtual IVar& value() const = 0;

        virtual void    assign( const implement_type& ) = 0;
        virtual bool    equal_to( const implement_type& ) const = 0;
        virtual bool    equal_to( const const_implement_type& ) const = 0;
        virtual void    iterate() = 0;
    };

    class IDictConstIteratorImpl{
    public:
        typedef bsl::string              string_type;
        typedef IDictIteratorImpl        implement_type;
        typedef IDictConstIteratorImpl   const_implement_type;

        virtual ~IDictConstIteratorImpl() {}
        virtual const string_type&  key() const = 0;
        virtual const IVar& value() const = 0;

        virtual void    assign( const implement_type& ) = 0;
        virtual void    assign( const const_implement_type& ) = 0;
        virtual bool    equal_to( const implement_type& ) const = 0;
        virtual bool    equal_to( const const_implement_type& ) const = 0;
        virtual void    iterate() = 0;
    };

    //iterator defintion
    class DictIterator{
    public:
        typedef DictIterator            iterator_type;
        typedef DictConstIterator       const_iterator_type;
        typedef IDictIteratorImpl        implement_type;
        typedef IDictConstIteratorImpl   const_implement_type;
        typedef implement_type *(* cloner_type )( const implement_type *, const void * ); 
        typedef void (* destroyer_type )( implement_type *, const void * ); 

        /**
         * @brief 构造函数
         *
         * @param [in] p_impl   : implement_type*   实现类型
         * @param [in] cloner   : cloner_type       复制函数指针
         * @param [in] destroyer   : destroyer_type 销毁函数指针
         * @param [in] p_alloc   : void*            内存分配器指针
         * @see 
        **/
        DictIterator( implement_type * p_impl, cloner_type cloner, destroyer_type destroyer, const void *p_alloc )
            :_p_impl(p_impl), _cloner(cloner), _destroyer(destroyer), _p_alloc(p_alloc) {}

        /**
         * @brief 复制构造函数
         *
         * @param [in] other   : const DictIterator&
         * @see 
        **/
        DictIterator( const DictIterator& other )
            :_p_impl(other._cloner(other._p_impl, other._p_alloc) ), _cloner(other._cloner), _destroyer(other._destroyer), _p_alloc(other._p_alloc) {}

        /**
         * @brief 析构函数
         *
         * @see 
        **/
        ~DictIterator(){
            _destroyer( _p_impl, _p_alloc );
        }

        /**
         * @brief 赋值运算符
         *
         * @param [in] other   : const DictIterator&
         * @return  DictIterator&  operator 
         * @retval   
         * @see 
        **/
        DictIterator& operator = ( const DictIterator& other ){
            _p_impl->assign( *other._p_impl );            
            return *this;
        }

        /**
         * @brief ->运算符重载
         *
         * @return  const implement_type*  
         * @retval   
         * @see 
        **/
        const implement_type* operator ->() const {
            return _p_impl;
        }

        /**
         * @brief *运算符重载
         *
         * @return  const implement_type*  
         * @retval   
         * @see 
        **/
        const implement_type& operator *() const {
            return *_p_impl;
        }

        /**
         * @brief 前缀++运算符重载
         *
         * @return  DictIterator&  operator 
         * @retval   
         * @see 
        **/
        DictIterator& operator ++(){
            _p_impl->iterate();
            return *this;
        }

    private:
        implement_type  *_p_impl;
        cloner_type     _cloner;
        destroyer_type  _destroyer;
        const void      *_p_alloc;
    };

    class DictConstIterator{
    public:
        typedef DictIterator            iterator_type;
        typedef DictConstIterator       const_iterator_type;
        typedef IDictIteratorImpl        implement_type;
        typedef IDictConstIteratorImpl   const_implement_type;
        typedef const_implement_type *(* cloner_type )( const const_implement_type *, const void * ); 
        typedef void (* destroyer_type )( const_implement_type *, const void * ); 

        /**
         * @brief 构造函数
         *
         * @param [in] p_impl   : const_implement_type*
         * @param [in] cloner   : cloner_type
         * @param [in] destroyer   : destroyer_type
         * @param [in] p_alloc   : void*
         * @see 
        **/
        DictConstIterator( const_implement_type * p_impl, cloner_type cloner, destroyer_type destroyer, const void *p_alloc )
            :_p_impl(p_impl), _cloner(cloner), _destroyer(destroyer), _p_alloc(p_alloc) {}

        /**
         * @brief 复制构造函数
         *
         * @param [in] other   : const DictConstIterator&
         * @see 
        **/
        DictConstIterator( const DictConstIterator& other )
            :_p_impl(other._cloner(other._p_impl, other._p_alloc) ), _cloner(other._cloner), _destroyer(other._destroyer), _p_alloc(other._p_alloc) {}

        /**
         * @brief 析构函数
         *
         * @see 
        **/
        ~DictConstIterator(){
            _destroyer( _p_impl, _p_alloc );
        }

        /**
         * @brief 赋值运算符
         *
         * @param [in] other   : const DictConstIterator&
         * @return  DictConstIterator&  operator 
         * @retval   
         * @see 
        **/
        DictConstIterator& operator = ( const DictConstIterator& other ){
            _p_impl->assign( *other._p_impl );            
            return *this;
        }

        /**
         * @brief ->运算符重载
         *
         * @return  const const_implement_type* 
         * @retval   
         * @see 
        **/
        const const_implement_type* operator ->() const {
            return _p_impl;
        }

        /**
         * @brief *运算符重载
         *
         * @return  const const_implement_type& 
         * @retval   
         * @see 
        **/
        const const_implement_type& operator *() const {
            return *_p_impl;
        }

        /**
         * @brief 前缀++运算符重载
         *
         * @return  DictConstIterator&  operator 
         * @retval   
         * @see 
        **/
        DictConstIterator& operator ++(){
            _p_impl->iterate();
            return *this;
        }

    private:
        const_implement_type    *_p_impl;
        cloner_type             _cloner;
        destroyer_type          _destroyer;
        const void              *_p_alloc;
    };

    inline bool operator == ( const DictConstIterator& i, const DictConstIterator & j ) {
        return i->equal_to(*j);
    }

    inline bool operator == ( const DictConstIterator& i, const DictIterator & j ) {
        return i->equal_to(*j);
    }

    inline bool operator == ( const DictIterator& i, const DictConstIterator & j ) {
        return i->equal_to(*j);
    }

    inline bool operator == ( const DictIterator& i, const DictIterator & j ) {
        return i->equal_to(*j);
    }

    inline bool operator != ( const DictConstIterator& i, const DictConstIterator & j ) {
        return !i->equal_to(*j);
    }

    inline bool operator != ( const DictConstIterator& i, const DictIterator & j ) {
        return !i->equal_to(*j);
    }

    inline bool operator != ( const DictIterator& i, const DictConstIterator & j ) {
        return !i->equal_to(*j);
    }

    inline bool operator != ( const DictIterator& i, const DictIterator & j ) {
        return !i->equal_to(*j);
    }


}}   //namespace bsl::var

#endif  //__BSL_VAR_DICT_ITERATOR_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
