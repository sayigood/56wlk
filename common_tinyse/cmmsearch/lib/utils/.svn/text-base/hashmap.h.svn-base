#ifndef _HASH_MAP_H
#define _HASH_MAP_H

/**
 *收集删除单元内存，在多读一写的情况下多线程安全
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ul_log.h>
#include <ul_func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <pthread.h>

#include "ans_log.h"
#include "myqueue.h"


namespace ansx{

enum{
	HASH_SUCCESS=0,
	HASH_VAL_EXIST,
	HASH_VAL_NO_EXIST,
	HASH_ACLLOMEM_ERROR,
};

/**
 * @class hash_map
 * @brief 一个模板类，有两个模板参数：key_t哈希表健值类型，value_t哈希表数值类型
 * */

#pragma pack (4) ///保持四字节对齐
template<class key_t>
int _hash_compare_fun( const key_t &k1, const key_t &k2 )
{
    return (k1==k2)?0:1;
}

template <class key_t, class value_t>
class hash_map
{
private:
	//哈希函数类型
	typedef unsigned int (*hash_fcn_t)(const key_t &);
	typedef int (*compare_t)(const key_t &, const key_t &);
	
	static const int BLOCK_SIZE = 256 * 1024;
	static const int BUFFER_SIZE = 4 * 1024;
	/**
	 * @struct node_t 
	 * @brief 哈希表结点类型，内部包含一个键值和一个数值以及一个指向下个结点的指针
	 * */
	typedef struct _node_t
	{
		key_t key;
		value_t value;
		struct _node_t * next;
	}node_t;
	
	/**
	 * @struct bucket_t
	 * @brief 哈希表桶结构，内部包含这个桶里第一个结点的指针
	 * */
	typedef struct _bucket_t
	{
		node_t * first;
	}bucket_t;
	
	/**
	 * @struct block_t
	 * @brief 结点块类型，内部包含了大块的内存，用于分配给哈希表使用，以及指向下一个块的指针
	 * */
	typedef struct _block_t
	{
		node_t node_block[BLOCK_SIZE];
		struct _block_t * next;
	}block_t;

	typedef struct _buffer_node
	{
		key_t key;
		value_t value;
	}buffer_node;

	struct delay_t {
		u_int reltime;
		node_t *addres;
	};

	xspace::MyQueue<delay_t> _delayqueue;
	u_int _delaytime;

public:
	hash_map()
	{
		setAllZero();
	}
	/**
	 * @brief 析构函数,即使释放内存
	 */
	~hash_map()
	{
		clear();
	}


	/**
	 * @brief 根据键值查找数据
	 *
	 * @param key_t 键值
	 * @param value_t 存储返回的数据
	 *
	 * @return 	成功返回 0
	 * 			失败返回 HASH_VAL_NO_EXIST
	 **/
	int get( const key_t &k, value_t *val=NULL )
	{
		assert( m_bucket&&m_first_block );

		int ret = HASH_VAL_NO_EXIST;

		unsigned int key = getHashKey( k );
		node_t * ptr = m_bucket[key].first;

		while( ptr ){
			if( /*ptr->key == k*/ m_equal( ptr->key, k )==0 ){
				if(val)
					*val = ptr->value;
				ret = 0;
				break;
			}else{
				ptr = ptr->next;
			}
		}

		return ret;
	}
	int get( const key_t &k, value_t **pval=NULL )
	{
		assert( m_bucket&&m_first_block );

		int ret = HASH_VAL_NO_EXIST;

		unsigned int key = getHashKey( k );
		node_t * ptr = m_bucket[key].first;

		while( ptr ){
			if( /*ptr->key == k*/ m_equal( ptr->key, k )==0 ){
				if(pval)
					*pval = &ptr->value;
				ret = 0;
				break;
			}else{
				ptr = ptr->next;
			}
		}

		return ret;
	}


	/**
	 * @brief 根据键值在hash中插入数据
	 * @param key_t  键值
	 * @param value_t 要插入hash的数据
	 * @param int 默认值等于0，表示如果值存在不覆盖，如果非零，覆盖原来的值。
	 *
	 * @return 插入或者替换成功成功返回: 0
	 * 			无法分配空间返回: HASH_ACLLOMEM_ERROR
	 * 			已经存在该值返回: HASH_VAL_EXIST
	 **/
	int set( const key_t &k, const value_t &val, int flag = 0 )
	{
		assert( m_bucket&&m_first_block );
		int ret = 0;

		unsigned int key = getHashKey( k );
		node_t *ptr = m_bucket[key].first;
		node_t *front=0;
		node_t *node=0;

		//是否已经存在
		while( ptr ){
			if( /*ptr->key == k*/ m_equal( ptr->key, k )==0 ){
				if( flag ){
					ptr->value = val;
					ret = 0;
					goto _SET_END;
				}
				ret = HASH_VAL_EXIST;
				goto _SET_END;
			}else{
				front = ptr;
				ptr = ptr->next;
			}
		}

		//分配空间
		node = getNodeSpace();
		if( node == 0 ){//分配空间失败
			_ANS_WARNING_LOG("[HASH] malloc memory error");
			ret = HASH_ACLLOMEM_ERROR;
			goto _SET_END;
		}

		node->key = k;
		node->value = val;
		node->next = 0;

		if( front ) front->next = node;
		else{
		    m_bucket[key].first = node;
		}

		m_node_num++;

_SET_END:
		return ret;
	}

	/**
	 * @brief 重HASH中删除数据
	 * @param key_t  键值
	 *
	 * @return 	删除成功返回: 0
	 * 			元素不存在返回: HASH_VAL_NO_EXIST
	 **/
	int erase(const key_t &k)
	{
		assert( m_bucket&&m_first_block );
		int ret= HASH_VAL_NO_EXIST;

		unsigned int key = getHashKey( k );
		node_t *ptr = m_bucket[key].first;
		node_t *front = 0;

		while( ptr ){
			if( /*ptr->key == k*/ m_equal(ptr->key,k)==0 ){
				if( front==0 ){
					m_bucket[key].first = ptr->next;
				}else{
					front->next = ptr->next;
				}
				_erase(ptr);

				
				m_node_num--;				
				ret = 0;
				goto _ERASE_END;
			}
			front = ptr;
			ptr = ptr->next;
		}

_ERASE_END:
		return ret;
	}

	/**
	 * @brief 获取HASH的数据量
	 *
	 * @return 返回hash中的数据数目
	 **/
	inline int size() const{
		return m_node_num;
	}
	
	int _erase(node_t *ptr)
	{

		delay_t node;
		u_int nowtime = (u_int)time(0);

		do {
			if (_delayqueue.pop_front(node) == 0) {
				if (node.reltime + _delaytime <= nowtime) {

					node.addres->next = m_free_node;
					m_free_node = node.addres;

				} else {
					_delayqueue.push_front(node);
					break;
				}
			} else {
				break;
			}
		} while (1);

		node.addres = ptr;
		node.reltime = nowtime;
		if (_delayqueue.push_back(node) != 0) {
			if (_delayqueue.pop_front(node) == 0) {
				
	//			if (node.reltime + _delaytime > nowtime) { ///强制sleep
	//				sleep(_delaytime - (nowtime - node.reltime));
	//			}
				UB_LOG_WARNING("delaytime is not enough:%d",_delaytime - (nowtime - node.reltime));
				node.addres->next = m_free_node;
				m_free_node = node.addres;
				node.addres = ptr;
				node.reltime = nowtime;
				_delayqueue.push_back(node);
			} else {
	//			sleep(_delaytime);///强制sleep
				ptr->next = m_free_node;
				m_free_node = ptr;

			}
		}
		return 0;
	};

	/**
	 * @brief 将HASH里面的数据写入磁盘
	 * @note  本函数没有做大文件处理,在32位平台上,数据量不可大于2G
	 * @param char  文件名
	 *
	 *
	 * @return 成功返回0，失败-1
	 **/
	int savedata(const char *fn){
		assert( m_bucket&&m_first_block );

		//buffer_node *buffer = new buffer_node[BUFFER_SIZE];
		buffer_node *buffer = (buffer_node *) malloc( sizeof(buffer_node)*BUFFER_SIZE );
		if( buffer==0 ){
			_ANS_WARNING_LOG("can't malloc memory");
			return -1;
		}
		int bufcount=0;

		node_t *ptr= 0;
		int xcount = 0;
		int fd = open( fn, O_WRONLY|O_CREAT|O_TRUNC, 0666 );
		if( fd == -1 ){
			_ANS_WARNING_LOG("can't open [%s] file",fn);
			return -1;
		}


		unsigned int sizekey = sizeof( key_t );
		unsigned int sizeval = sizeof( value_t );

		if( write( fd, &sizekey, sizeof(unsigned int) ) != sizeof(unsigned int) ){
			goto _WRITE_ERROR;
		}
		if( write( fd, &sizeval, sizeof(unsigned int) ) != sizeof(unsigned int) ){
			goto _WRITE_ERROR;
		}

		for( int i=0; i<m_bucket_num; i++ ){
			ptr = m_bucket[i].first;
			while( ptr ){
				
				buffer[bufcount].key = ptr->key;
				buffer[bufcount].value = ptr->value;
				bufcount++;

				if( bufcount==BUFFER_SIZE ){
					if( write( fd, buffer, BUFFER_SIZE*sizeof(buffer_node) ) != (int)sizeof(buffer_node)*(int)BUFFER_SIZE )
						goto _WRITE_ERROR;
					xcount += BUFFER_SIZE;
					bufcount = 0;
				}
				
				ptr = ptr->next;
			}
		}

		if( bufcount>0 ){
			if( write( fd, buffer, bufcount*sizeof(buffer_node) ) != (int)(sizeof(buffer_node)*bufcount) )
				goto _WRITE_ERROR;
			xcount += bufcount;
		}
		_ANS_NOTICE_LOG("save [%d] data to [%s] file success",xcount,fn);
		close(fd);
		free(buffer);
		return 0;

_WRITE_ERROR:
		_ANS_WARNING_LOG("[HASH] Write File[%s] error",fn);
		close(fd);
		free(buffer);
		return -1;
	}


	/**
	 * @brief 从磁盘中读取信息追加到hash中
	 * @param char  文件名
	 *
	 * @return 成功返回0,失败-1.
	 **/
	int loaddata(const char *fn, int flag = 0 ){
		assert( m_bucket&&m_first_block );
		buffer_node *buffer = (buffer_node *)malloc(sizeof(buffer_node)*BUFFER_SIZE);
		if( buffer==0 ){
			_ANS_WARNING_LOG("can't malloc memory");
			return -1;
		}
		int bufcount;

		unsigned int sizekey,sizeval;
		int xcount = 0;

		int fd = open( fn, O_RDONLY );
		if( fd == -1 ){
			_ANS_WARNING_LOG("can't open [%s] file",fn);
			return -1;
		}
		if( read( fd, &sizekey, sizeof(unsigned int) ) != sizeof(unsigned int) )
			goto _READ_ERROR;
		if( sizekey != sizeof(key_t) )
			goto _READ_ERROR;
		if( read( fd, &sizeval, sizeof(unsigned int) ) != sizeof(unsigned int) )
			goto _READ_ERROR;
		if( sizeval != sizeof( value_t ) )
			goto _READ_ERROR;

		for(;;){
			bufcount = read( fd, buffer, sizeof(buffer_node)*BUFFER_SIZE );
			bufcount /= sizeof(buffer_node);
			if( bufcount<=0 )break;
			for( int i=0; i<bufcount; i++ ){
				if( set( buffer[i].key, buffer[i].value, flag ) == 0 )
					xcount++;
			}
			if( bufcount<BUFFER_SIZE ) break;
		}
		
		_ANS_NOTICE_LOG("load [%d] data from [%s] file success",xcount,fn);
		close(fd);
		free(buffer);
		return 0;

_READ_ERROR:
		_ANS_WARNING_LOG("read error [%s] file",fn);
		close(fd);
		free(buffer);
		return -1;
	}


	/**
	 * @brief 遍历hash表中的值
	 * @param fun  对hash表中每个值的处理函数
	 *
	 **/
	void list( void (*fun)(const key_t& k,const value_t &t, void *param), void *paramv ){
		assert( m_bucket&&m_first_block );
		assert(fun!=NULL);

		node_t *tmp = 0;
		for( int i=0; i<m_bucket_num; i++ ){
			tmp = m_bucket[i].first;
			while( tmp ){
				fun( tmp->key,tmp->value, paramv );
				tmp = tmp->next;
			}
		}
	}

	/**
	 * @brief 重置hash表，将hash表清空，但是分配的内存不回收
	 *
	 **/
	inline void reset()
	{
		assert( m_bucket&&m_first_block );
/*
		node_t *tmp = 0;
		for( int i=0; i<m_bucket_num; i++ ){
			tmp = m_bucket[i].first;
			while( tmp ){
			//	tmp->value.~value_t();
				tmp = tmp->next;
			}
		}
*/
		m_node_num = 0;
		m_free_num = BLOCK_SIZE;
		m_cur_block = m_first_block;
		m_free_node = 0;
		memset( m_bucket , 0, sizeof(bucket_t)*m_bucket_num );
	}

private:
	
	/**
	 * @brief 类成员变量清零
	 *
	 */
	inline void setAllZero()
	{
		m_bucket_num = m_node_num = m_free_num = 0;
		m_bucket = NULL;
		m_cur_block = NULL;
		m_free_node = NULL;
		m_first_block = NULL;
	}

	/**
	 * @brief 根据键值，重新得到hash值，主要防止键值超出bucket的最大范围
	 *
	 * @return hash key
	 **/
	inline unsigned int getHashKey( const key_t &val )
	{
		//防治错误的key产生
		return m_fcn(val)%(unsigned int)m_bucket_num;
	}
public:
	/**
	 * @brief 初始化HASH表的内存
	 *
	 * @param unsigned int  Hash表内bucket的大小
	 *
	 * @return 成功返回0，错误返回-1
	 **/
	int create(int hn, hash_fcn_t fcn, compare_t cft = _hash_compare_fun , 
				int qsize=1000000, int delaytime=10)
	{
		if (hn <= 0) {
			_ANS_WARNING_LOG("invalid input hn[%d] <=0", hn);
			return -1;
		}

		clear();//防止内存泄露

		if( hn ==0 || fcn == NULL || cft==NULL){
			_ANS_WARNING_LOG("unvalid hash_num[%d] or unvalid hash or compare function", hn);
			return -1;
		}
		m_fcn = fcn;//设置Hash函数
		m_equal = cft;
		m_bucket_num = hn;

		if( ( m_bucket = (bucket_t * )malloc( sizeof(bucket_t)*m_bucket_num ) ) == 0 ){
			_ANS_WARNING_LOG(" [HASH ERROR] Fail to new a hash bucket table [%zu].",
					m_bucket_num * sizeof( bucket_t )  );
			return -1;
		}
		memset( m_bucket, 0, sizeof( bucket_t ) * m_bucket_num );
		
		if( ( m_first_block = ( block_t * ) malloc( sizeof(block_t) ) ) == 0 ){
			_ANS_WARNING_LOG("[HASH ERROR] Fail to new a hash block [%zu]",
					sizeof( block_t ) );
			free( m_bucket );
			m_bucket = 0;
			return -1;
		}
		m_first_block->next = 0;//表示结尾
	
		m_cur_block = m_first_block;
		m_free_num = BLOCK_SIZE;
		
		if (qsize < 10000) {
			qsize = 10000;
		}
		if (delaytime < 0) {
			delaytime = 0;
		}
		_delaytime = delaytime;
		return _delayqueue.create(qsize);
	}

	/**
	 * @brief 删除hash表
	 *
	 **/
	void clear()
	{
		if( m_bucket ){
			free( m_bucket );
		}
		
		//free block
		block_t *tmp;
		while( m_first_block ){
			tmp = m_first_block;
			m_first_block = m_first_block->next;
			free(tmp);
		}

		setAllZero();

	}

private:
	/**
	 * @brief 为新加入的节点申请内存空间
	 *
	 * @return 返回空间地址，错误返回NULL
	 **/
	inline node_t * getNodeSpace()
	{
		if( m_free_node ){//free空间内有空间
			node_t *tmp = m_free_node;
			m_free_node = m_free_node->next;
			return tmp;
		}else{
			if( m_free_num==0 ){
				if( m_cur_block->next ){
					m_cur_block = m_cur_block->next;
				}else{
					block_t *btmp = (block_t *)malloc( sizeof(block_t) );
					if( btmp==0 ){
						_ANS_WARNING_LOG("[HASH ERROR] Fail to new a Hash Block[%zu]",sizeof(block_t));
						return NULL;
					}
					btmp->next = 0;
					m_cur_block->next = btmp;
					m_cur_block = btmp;
				}
				m_free_num = BLOCK_SIZE;
			}

			return &( m_cur_block->node_block[ --m_free_num ] );
		}

		return NULL;
	}

private:
	int m_bucket_num;             //number of buckets
	int m_node_num;               //number of node
	int m_free_num;          //number of free node

	bucket_t *m_bucket;       //point to the list of buckets
	block_t  *m_first_block;  //point to the first block
	block_t  *m_cur_block;    //point to the current block
	node_t   *m_free_node;    //the list of free node

	hash_fcn_t m_fcn;          //the hash function
	compare_t m_equal;

};

#pragma pack()
};
#endif

