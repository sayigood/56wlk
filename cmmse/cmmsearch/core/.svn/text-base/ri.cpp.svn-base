/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: ri.cpp,v 1.7 2009/11/18 20:27:50 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file ri.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/04/07 23:33:53
 * @version $Revision: 1.7 $ 
 * @brief 实时索引读写
 *  
 **/
#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include <algorithm>
#include <vector>
#include "fileblock.h"
#include "mfiles.h"
#include "hashmap.h"
#include "mc_cache.h"
#include "tinyse.h"
#include "fblock_util.h"
//#include "cache_util.h"
#include "lock.h"
using namespace ansx;

using namespace std;


#define RI0_SUFFIX_FILE ".ri_l0"
#define RI1_SUFFIX_FILE ".ri_l1"
#define RI2_SUFFIX_FILE ".ri_l2"


inline int cmp_sign(sign64_t s1, sign64_t s2)
{
    if(s1.sign1 < s2.sign1)
    {
        return -1;
    } else if(s1.sign1 > s2.sign1)
    {
        return 1;
    } else
    {
        if(s1.sign2 < s2.sign2)
            return -1;
        else if(s1.sign2 > s2.sign2)
            return 1;
        else
            return 0;

    }

    return 0;
}

class sort_less_t
{
  public:
    bool operator() (const ts_index_t & a, const ts_index_t & b)const
    {
        return a.id < b.id;
    }
};
inline int merge_inv_list(ts_inv_list_t & in1, ts_inv_list_t & in2,
                          ts_inv_list_t & ou)
{

    uint32 num1 = 0;
    uint32 num2 = 0;

    ou.term = in1.term; //

    num1 =
        (in1.ind_list.size() >
         TS_INDEX_MAXNUM) ? TS_INDEX_MAXNUM : in1.ind_list.size();
    num2 =
        ((in2.ind_list.size() + num1)) >
        TS_INDEX_MAXNUM ? (TS_INDEX_MAXNUM - num1) : in2.ind_list.size();

    ou.ind_list.resize(num1 + num2);

    std::copy(in2.ind_list.end() - num2, in2.ind_list.end(), ou.ind_list.begin());  ///将旧的索引截断 
    std::copy(in1.ind_list.end() - num1, in1.ind_list.end(),
              ou.ind_list.begin() + num2);
    return 0;
}

int merge_ri(ts_ri_t * src1, ts_ri_t * src2, ts_ri_t * dest)
{
    int opret = 0;

    opret = dest->clear();
    if(opret < 0)
    {
        return -1;
    }
    ts_inv_list_t in1, in2, ou; //todo : 这里涉及内存分配，可能影响性能

    opret = src1->it_init();
    if(opret < 0)
    {
        return -2;
    }
    opret = src2->it_init();
    if(opret < 0)
    {
        return -3;
    }
    opret = dest->wt_init();
    if(opret < 0)
    {
        return -4;
    }


    bool cur1 = true, cur2 = true;
    bool end1 = false, end2 = false;
    while(!src1->it_is_end() || !src2->it_is_end())
    {

        if(cur1)
        {
            if(!src1->it_is_end())
                src1->it_get_and_next(in1);
            else
                end1 = true;
        }
        if(cur2)
        {
            if(!src2->it_is_end())
                src2->it_get_and_next(in2);
            else
                end2 = true;
        }


        if(end1)
        {
            dest->wt_write(in2);
            cur1 = false;
            cur2 = true;
        } else if(end2)
        {
            dest->wt_write(in1);
            cur1 = true;
            cur2 = false;
        } else
        {

            int ret = cmp_sign(in1.term, in2.term);
            if(ret < 0)
            {
                dest->wt_write(in1);
                cur1 = true;
                cur2 = false;
            } else if(ret > 0)
            {
                dest->wt_write(in2);
                cur1 = false;
                cur2 = true;

            } else if(0 == ret)
            {
                merge_inv_list(in1, in2, ou);
                dest->wt_write(ou);
                cur1 = true;
                cur2 = true;

            }
        }
    }
    src1->it_fini();
    src2->it_fini();
    dest->wt_fini();


    return 0;
}

int copy_ri(ts_ri_t * src, ts_ri_t * dest)
{
    int opret = 0;

    opret = dest->clear();
    if(opret < 0)
    {
        return -1;
    }
    ts_inv_list_t in;

    opret = src->it_init();
    if(opret < 0)
    {
        return -2;
    }

    opret = dest->wt_init();
    if(opret < 0)
    {
        return -3;
    }

    while(!src->it_is_end())
    {
        src->it_get_and_next(in);
        dest->wt_write(in);
    }


    src->it_fini();
    dest->wt_fini();


    return 0;
}



static const uint32 MEM_L1_BASE = 1024 * 1024 + 1;    /**<  每种数据固定分配1M的节点      */
inline uint32 _hash_fun(const sign64_t & key)
{
    return (key.sign1 + key.sign2) % MEM_L1_BASE;
}

inline int _hash_cmp_fun(const sign64_t & k1, const sign64_t & k2)
{
    return ((k1.sign1 == k2.sign1) && (k1.sign2 == k2.sign2)) ? 0 : 1;
}
int ts_mem_ri_t::init(const char *dir, const char *file)
{
    _size = 0;
	if (0 != pthread_rwlock_init(&rw_lock,NULL))
		return -1;

    hashmap = new hash_map < sign64_t, ts_inv_list_t * >();
	if (NULL == hashmap)
		return -2;

    return ((hash_map < sign64_t,
             ts_inv_list_t * >*)hashmap)->create(MEM_L1_BASE, _hash_fun,
                                                 _hash_cmp_fun);
}

int ts_mem_ri_t::read_inv_num(const sign64_t word, uint32 & num)
{
    ts_inv_list_t *plist = NULL;
    int opret =
        ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->get(word, &plist);
    if(NULL != plist && HASH_VAL_NO_EXIST != opret)
    {
        num = plist->ind_list.size();
    } else
    {
        num = 0;
        return 0;
    }

    return 0;
}
int ts_mem_ri_t::read(const sign64_t word, vector < ts_index_t > *out,
                      uint32 maxnum)
{
	pthread_rwlock_rdlock(&rw_lock);

    ts_inv_list_t *tmp = NULL;
    int opret =
        ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->get(word, &tmp);
    if(HASH_SUCCESS != opret )
    {
		pthread_rwlock_unlock(&rw_lock);
        return opret;
    }
    //out = *tmp;
    //out.term = tmp->term;
    uint32 copy_num =
        tmp->ind_list.size() > maxnum ? maxnum : tmp->ind_list.size();

    out->resize(copy_num);
 //   std::copy(tmp->ind_list.end() - copy_num, tmp->ind_list.end(), ///从拉链尾部开始读
 //           out->begin());
	std::copy(tmp->ind_list.begin() , tmp->ind_list.begin()+copy_num,
			out->begin());


	pthread_rwlock_unlock(&rw_lock);
    return 0;
}
int ts_mem_ri_t::read_at_pos(const sign64_t word,int pos, vector < ts_index_t > *out,
                      uint32 maxnum)
{
    pthread_rwlock_rdlock(&rw_lock);

    ts_inv_list_t *tmp = NULL;
    int opret =
        ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->get(word, &tmp);
    if(HASH_SUCCESS != opret )
    {    
        pthread_rwlock_unlock(&rw_lock);
        return opret;
    }    
 //   uint32 copy_num =
   //     tmp->ind_list.size() > maxnum ? maxnum : tmp->ind_list.size();
    uint32 left_len = tmp->ind_list.size() - pos ;
    if(left_len<=0)
    {    
        out->clear();
        pthread_rwlock_unlock(&rw_lock);
        return 0;
    }    
    uint32 copy_num = left_len >maxnum  ? maxnum  : left_len;



    out->resize(copy_num);
 //   std::copy(tmp->ind_list.end() - copy_num, tmp->ind_list.end(),
   //           out->begin());
   std::copy(tmp->ind_list.begin() + pos, tmp->ind_list.begin()+pos+copy_num,out->begin());


    pthread_rwlock_unlock(&rw_lock);
    return 0;
}

int ts_mem_ri_t::write(const ts_ri_item_t & item)
{

	pthread_rwlock_wrlock(&rw_lock);
    ts_inv_list_t *plist = NULL;
    int opret =
        ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->get(item.term,
                                                                &plist);
    if(HASH_SUCCESS != opret  || NULL == plist)
    {
        opret =
            ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->set(item.term,
                                                                    new
                                                                    ts_inv_list_t
                                                                    ());
        if(HASH_SUCCESS != opret )
        {
			pthread_rwlock_unlock(&rw_lock);
			return -1;
        }
        opret =
            ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->get(item.term,
                                                                    &plist);
        if(HASH_SUCCESS != opret )
        {
			pthread_rwlock_unlock(&rw_lock);
            return -2;
        }
        plist->term = item.term;

    }
    ts_index_t index ;
	index.id =  item.id;
	index.sweight =  item.weight ;
    if(plist->ind_list.size() >= TS_INDEX_MAXNUM)
    {
		pthread_rwlock_unlock(&rw_lock);
		return 0;
    }
    plist->ind_list.push_back(index);
    _size += sizeof(ts_index_t);

	pthread_rwlock_unlock(&rw_lock);
	return 0;
}


long ts_mem_ri_t::size()
{
    return _size;
}

class sort_less1_t
{
  public:
    bool operator() (const sign64_t & a, const sign64_t & b)const
    {
        return (a.sign1 < b.sign1) ? true : (a.sign1 >
                                             b.sign1 ? false : (a.sign2 <
                                                                b.sign2));
    }
};


typedef struct
{
    sign64_t *mem_buf;
    int cur_pos;
} hashmap_array;

void list_hashmap(const sign64_t & key, ts_inv_list_t * const &item,
                  void *param)
{
    hashmap_array *parray = (hashmap_array *) param;
    parray->mem_buf[parray->cur_pos] = key;
    parray->cur_pos++;
}

int ts_mem_ri_t::it_init()
{
    _it_pos = 0;
    _it_size = ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->size();

    _it_membuf = new sign64_t[_it_size];
    if(NULL == _it_membuf)
    {
        return -1;
    }

    hashmap_array array;
    array.mem_buf = _it_membuf;
    array.cur_pos = 0;


    ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->list(list_hashmap,
                                                             &array);

    std::sort(_it_membuf, _it_membuf + _it_size, sort_less1_t());
    return 0;
}


int ts_mem_ri_t::it_get_and_next(ts_inv_list_t & out)
{

    int opret = 0;
    ts_inv_list_t *tmp = NULL;
    opret =
        ((hash_map < sign64_t,
          ts_inv_list_t * >*)hashmap)->get(_it_membuf[_it_pos], &tmp);
    if(HASH_VAL_NO_EXIST == opret)  ///没有这个term,创建
    {
        return -1;
    }
    out = *tmp;

    _it_pos++;
    return 0;
}


int ts_mem_ri_t::it_is_end()
{
    if(_it_pos >= _it_size)
        return 1;
    return 0;

}


int ts_mem_ri_t::it_fini()
{
    if(NULL != _it_membuf)
    {
        delete[]_it_membuf;
        _it_membuf = NULL;
    }
    return 0;
}



int ts_mem_ri_t::wt_init()
{
    return 0;
}


int ts_mem_ri_t::wt_write(ts_inv_list_t & out)
{
    ts_inv_list_t *plist = NULL;
    int opret =
        ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->get(out.term,
                                                                &plist);
    if(HASH_SUCCESS != opret  || NULL == plist)
    {
        opret =
            ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->set(out.term,
                                                                    new
                                                                    ts_inv_list_t
                                                                    ());
        if(HASH_SUCCESS != opret )
        {
            return -1;
        }
        opret =
            ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->get(out.term,
                                                                    &plist);
        if( HASH_SUCCESS != opret )
        {
            return -2;
        }

        plist->term = out.term;
    }

    plist->ind_list.resize(out.ind_list.size());
    std::copy(out.ind_list.begin(), out.ind_list.end(),
              plist->ind_list.begin());
    _size += out.ind_list.size() * sizeof(ts_index_t);

    return 0;
}


int ts_mem_ri_t::wt_fini()
{
    return 0;
}



void clear_hashmap(const sign64_t & key, ts_inv_list_t * const &item,
                   void *param)
{
    delete item;
}


int ts_mem_ri_t::clear()
{
	pthread_rwlock_wrlock(&rw_lock);

    ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->list(clear_hashmap,
                                                             NULL);
    ((hash_map < sign64_t, ts_inv_list_t * >*)hashmap)->reset();
    _size = 0;

	pthread_rwlock_unlock(&rw_lock);
	
	//pthread_rwlock_destroy(&rw_lock);//不能够destroy
    return 0;
};


static const uint32 MEM_L0_BASE = 1024 * 128 + 1;     /**<  每种数据固定分配1M的节点      */



int ts_disk_ri_t::init(const char *dir, const char *file)
{
    int opret = 0;
	// 内部赋值
    if(dir && dir != _dir)
    {
        strncpy(_dir, dir, sizeof(_dir) - 1);
        _dir[sizeof(_dir) - 1] = 0;
    }
    if(file && file != _file)
    {
        strncpy(_file, file, sizeof(_file) - 1);
        _file[sizeof(_file) - 1] = 0;
    }

	l0_fp = NULL;
	l0_handle = NULL;
	l1_handle = NULL;
	l1_cache = NULL;
	l2_handle = NULL;
	bool l0_exist = false;
    struct stat buf;

    char path[TS_FILEPATH_MAXSIZE];
    snprintf(path, sizeof(path), "%s/%s%s", _dir, _file, RI1_SUFFIX_FILE);
    // l1
    l1_handle =  malloc(sizeof( fblock_t));
    if(NULL == l1_handle)
    {
        opret = -1;
		goto fail;
    }
    opret =
        fblock_init((fblock_t *) l1_handle, _file, path,
                    sizeof(ts_disk_term_t), true, false);
    if(opret < 0)
    {
        opret = -2;
		goto fail;
    }
    l1_cache = (mc_cache *) mc_creat_cache(1024, sizeof(ts_disk_term_t));
	if (NULL == l1_cache)
	{
		opret = -3;
		goto fail;
	}

    // l2
    snprintf(path, sizeof(path), "%s/%s%s", _dir, _file, RI2_SUFFIX_FILE);
    l2_handle = malloc(sizeof(mfiles_t));
    if(NULL == l2_handle)
    {
        opret = -4;
		goto fail;
    }

    opret =
        mfiles_init((mfiles_t *) l2_handle, _file, path, MF_PER_FILE_MAX_LEN,
                    true, false);
    if(opret < 0)
    {
        opret = -5;
		goto fail;
    }


	//l0
    snprintf(path, sizeof(path), "%s/%s%s", dir, file, RI0_SUFFIX_FILE);
	if((stat(path, &buf) == 0 && buf.st_mode & S_IFREG))
	{
		l0_exist = true;
    }
	
    
	l0_handle = malloc( (size_t) sizeof(sign64_t) *  (1024 * 1024 * 1024 / TS_RI_BLOCK_NUM + 10));
	if(NULL == l0_handle)
    {
        opret = -6;
		goto fail;
    }

	if (l0_exist)
	{
		int max_curbid = ((fblock_t *) l1_handle)->max_curbid  ;
		if (max_curbid<0)
		{
			l0_num = 0;
		} else if (0 == max_curbid) 
		{
			l0_num = 1;
		} else if ( max_curbid>0 && 0 == (max_curbid % TS_RI_BLOCK_NUM)) 
		{
			l0_num =  max_curbid / TS_RI_BLOCK_NUM + 1;
		} else 
		{
			l0_num =  max_curbid / TS_RI_BLOCK_NUM + 2;
		}
		snprintf(path, sizeof(path), "%s/%s%s", dir, file, RI0_SUFFIX_FILE);
		l0_fp = fopen(path,"r+");
		if (NULL == l0_fp)
		{
			opret = -7;
			goto fail;
		}
		size_t num = fread(l0_handle, sizeof(sign64_t), l0_num, l0_fp);
		if (num != l0_num)
		{
			opret = -8;
			goto fail;
		}
		fclose(l0_fp);	
		l0_fp = NULL;

	} else 
	{///重建l0索引
		

		snprintf(path, sizeof(path), "%s/%s%s", dir, file, RI0_SUFFIX_FILE);
		l0_fp = fopen(path,"w+");
		if (NULL == l0_fp)
		{
			opret = -9;
			goto fail;
		}
		

		ts_disk_term_t tmp_term;
		int blockid = 0;
		int curbid = 0;
		while(blockid * (int)TS_RI_BLOCK_NUM <=
				((fblock_t *) l1_handle)->max_curbid)
		{
			curbid = blockid * TS_RI_BLOCK_NUM;
			opret =
				fblock_read(((fblock_t *) l1_handle), curbid,
						&tmp_term, sizeof(tmp_term));
			if(0 != opret)
			{
				opret = -10;	
				goto fail;
			}
			*(((sign64_t *) l0_handle) + blockid) = tmp_term.term;
			size_t num = fwrite(&tmp_term.term,sizeof(sign64_t),1,l0_fp);
			if (num != 1)
			{
				opret = -11;
				goto fail;
			}
			
			blockid++;
		}
		if(curbid != ((fblock_t *) l1_handle)->max_curbid
				&& ((fblock_t *) l1_handle)->max_curbid > 0)
		{
			opret =
				fblock_read((fblock_t *) l1_handle,
						((fblock_t *) l1_handle)->max_curbid, &tmp_term,
						sizeof(tmp_term));
			if(0 != opret)
			{
				opret = -12;
				goto fail;
			}
			*(((sign64_t *) l0_handle) + blockid) = tmp_term.term;
			size_t num = fwrite(&tmp_term.term,sizeof(sign64_t),1,l0_fp);
			if (num != 1)
			{
				opret = -13;
				goto fail;
			}
	
			blockid++;
		}
		l0_num = blockid;

		fclose(l0_fp);
		l0_fp = NULL;
	}

    opret = pthread_mutex_init(&mutex_lock, NULL);
	if (0 != opret)
	{	
		opret = -13;
		goto fail;
	}
	return 0;
fail:
	if (l0_fp)
	{
		fclose(l0_fp);	
		l0_fp = NULL;
	}
	if (l0_handle)
	{
		free(l0_handle);
		l0_handle = NULL;
	}
	if (l1_handle)
	{
		free( l1_handle);
		l1_handle = NULL;
	}
	if (l1_cache)
	{
		mc_destroy_cache((mc_cache*)l1_cache);
		l1_cache = NULL;
	}
	if (l2_handle)
	{
		free (l2_handle);
		l2_handle = NULL;
	}
	
	return opret ;
}

/*********************************
*   函数名：bin_search
*   进行查找mon索引的二分查找，适合查找block和最终的索引查?
*   输入：
*       post_term_t *index：待查区域。查block是0级索引，查索引就是1级索引
*       u_int sign1,sign2：待查签名
*       int index_num：待查区域的大小
*   输出：
*   int *offset：二分查找的结果。如果查找失败，则返回最后一次mid值
*   返回值：
*       int:=0查找失败
*       int:>0查找成功
**********************************/
template < class T >
    int bin_search(const T * pind, const int index_num, sign64_t s,
                   int *offset)
{
    int low, mid, high;

    if(pind == NULL || offset == NULL)
    {
        return -1;
    }

    bool gt_flag = false;   //是否是大于的情况
    high = index_num - 1;
    low = mid = 0;
    while(low <= high)
    {
        gt_flag = false;
        mid = (high + low) / 2;
        if(s.sign1 > pind[mid].sign1)
        {
            low = mid + 1;
            gt_flag = true;
        } else if(s.sign1 < pind[mid].sign1)
            high = mid - 1;
        else if(s.sign2 > pind[mid].sign2)
        {
            low = mid + 1;
            gt_flag = true;
        } else if(s.sign2 < pind[mid].sign2)
            high = mid - 1;
        else
        {
            *offset = mid;
            return 1;
        }
    }  //end of while
    if(gt_flag) //如果是大于的情况, 则表示大于mid的值
    {
        if(mid > index_num - 1) //大于最后一个，表示超出范围了.
            *offset = -1;
        else
            *offset = mid;
        return 0;
    } else
    {  //否则就是 小于的那个offset
        *offset = --mid;
        return 0;
    }
}





int ts_disk_ri_t::_read_l1(const sign64_t word, ts_disk_term_t & term)
{
    int opret = 0;

    uint32 sign[2] = { word.sign1, word.sign2 };
    {
        ans::lock _lock(&mutex_lock);
        opret = mc_seekitem((mc_cache *) l1_cache, sign, &term, sizeof(term));
        if(RT_NOTICE_PROCESSED == opret)
        {
            return 0;
        }
    }
    //l0
    int offset = -1;
    bin_search < sign64_t > ((sign64_t *) l0_handle, l0_num, word, &offset);
    if(-1 == offset)
    {
        memset(&term, 0, sizeof(term));
        return -1;
    }
    //l1
    ts_disk_term_t *tmp_array =
        (ts_disk_term_t *) malloc(sizeof(ts_disk_term_t) * TS_RI_BLOCK_NUM);
    if(NULL == tmp_array)
    {
        return -2;
    }
    int readnum =
        fblock_multiread((fblock_t *) l1_handle, offset * TS_RI_BLOCK_NUM,
                         TS_RI_BLOCK_NUM, tmp_array);
    if(readnum < 0)
    {
        free(tmp_array);
        return -3;
    }

    opret = bin_search < ts_disk_term_t > (tmp_array, readnum, word, &offset);
    if(1 != opret)
    {

        free(tmp_array);
        return -4;
    }
    term = tmp_array[offset];
    free(tmp_array);
    tmp_array = NULL;
    //
    {
        ans::lock _lock(&mutex_lock);
        opret = mc_additem((mc_cache *) l1_cache, sign, &term, sizeof(term));
        if(RT_NOTICE_PROCESSED != opret && RT_NOTICE_NONE_PROCESSED != opret)
        {
            return -5;
        }
    }
    return 0;
};

int ts_disk_ri_t::_read_l2(long long offset, int dlen,
                           vector < ts_index_t > *out)
{
    int opret = 0;
    ts_index_t *tmp_buf = (ts_index_t *) malloc(dlen + 1);
    if(NULL == tmp_buf)
    {
        return -1;
    }
    opret =
        mfiles_pread((mfiles_t *) l2_handle, (char *)tmp_buf, dlen, offset);
    if(dlen != opret)
    {
        free(tmp_buf);
        return -2;
    }
    int num = dlen / sizeof(ts_index_t);
    out->resize(num);
    std::copy(tmp_buf, tmp_buf + num, out->begin());    //
    free(tmp_buf);
    return 0;
}
int ts_disk_ri_t::read_inv_num(const sign64_t word, uint32 & num)
{
    int opret = 0;
    //仅读取l1索引
    ts_disk_term_t tmp_term;
    opret = _read_l1(word, tmp_term);
    if(opret < 0)
    {
        num = 0;
        return 0;
    }
    num = tmp_term.dlen / sizeof(ts_index_t);

    return 0;
}


int ts_disk_ri_t::read(const sign64_t word, vector < ts_index_t > *out,
                       uint32 maxnum)
{
    int opret = 0;
    ts_disk_term_t tmp_term;
    opret = _read_l1(word, tmp_term);
    if(opret < 0)
    {
        return -1;
    }
    //out.term = word;

    uint32 copy_len =
        ((uint32) tmp_term.dlen) >
        (maxnum * sizeof(ts_index_t)) ? (maxnum *
                                         sizeof(ts_index_t)) : (tmp_term.
                                                                dlen);

 //   long long offset = tmp_term.offset + (tmp_term.dlen - copy_len);///从拉链尾部开始读
	long long offset = tmp_term.offset ;

    return _read_l2(offset, copy_len, out);
}

int ts_disk_ri_t::read_at_pos(const sign64_t word,int pos, vector < ts_index_t > *out,
                       uint32 maxnum)
{       
    int opret = 0;
    ts_disk_term_t tmp_term;
    opret = _read_l1(word, tmp_term);
    if(opret < 0)
    {   
        return -1;
    }
    //out.term = word;
    
    int left_len =  tmp_term.dlen - pos * sizeof(ts_index_t) ;
    if(left_len<=0)
    {
        out->clear();
        return 0;
    }
    int copy_len = left_len >(int)(maxnum * sizeof(ts_index_t)) ?
                                (maxnum * sizeof(ts_index_t)) : left_len;

    //long long offset = tmp_term.offset + (tmp_term.dlen - copy_len); ///从拉链尾部开始读
    long long offset = tmp_term.offset + pos * sizeof(ts_index_t);

    return _read_l2(offset, copy_len, out);
}




int ts_disk_ri_t::it_init()
{
    int opret = 0;
    _l1_reader = malloc(sizeof(fblock_reader));
	if (NULL == _l1_reader)
	{
		return -1;
	}
    opret = ((fblock_reader *) _l1_reader)->init((fblock_t *) l1_handle);
    if(opret < 0)
    {
        return -2;
    }

	_l2_reader = malloc(sizeof(mfiles_reader));
	if (NULL == _l2_reader)
	{
		return -3;
	}
    opret = ((mfiles_reader *) _l2_reader)->init((mfiles_t *) l2_handle);
    if(opret < 0)
    {
        return -4;
    }

    _reader_buf =
        (ts_index_t *) malloc(sizeof(ts_index_t) * (TS_INDEX_MAXNUM + 1024));
    if(NULL == _reader_buf)
    {
        return -5;
    }

    return 0;
}


int ts_disk_ri_t::it_get_and_next(ts_inv_list_t & out)
{
    int opret = 0;
    ts_disk_term_t term;

    opret =
        ((fblock_reader *) _l1_reader)->get_item_and_next(&term,
                                                          sizeof(term));
    if(opret < 0)
    {
        return -1;
    }
    if(term.dlen > 0)
    {
        opret =
            ((mfiles_reader *) _l2_reader)->get_item_and_next(_reader_buf,
                                                              term.dlen);
        if(opret < 0)
        {
            return -2;
        }
    }
    out.term = term.term;
    int num = term.dlen / sizeof(ts_index_t);
    out.ind_list.resize(num);
    std::copy(_reader_buf, _reader_buf + num, out.ind_list.begin());
    return 0;
}


int ts_disk_ri_t::it_is_end()
{

    return ((fblock_reader *) _l1_reader)->is_done()
        || ((mfiles_reader *) _l2_reader)->is_done();
}


int ts_disk_ri_t::it_fini()
{
    if(NULL != _reader_buf)
    {
        free(_reader_buf);
        _reader_buf = NULL;
    }
    int opret = ((mfiles_reader *) _l2_reader)->fini();
    if(opret < 0)
    {
        return -1;
    }
	free(_l2_reader);
	_l2_reader = NULL;
    opret = ((fblock_reader *) _l1_reader)->fini();
    if(opret < 0)
    {
        return -2;
    }
	free(_l1_reader);
	_l1_reader = NULL;
    return 0;
}



int ts_disk_ri_t::wt_init()
{
    int opret = 0;
    //l0
	char path[TS_FILEPATH_MAXSIZE];
	if (NULL == l0_fp)
	{
		snprintf(path, sizeof(path), "%s/%s%s", _dir, _file, RI0_SUFFIX_FILE);
		l0_fp = fopen(path,"w+");
		if (NULL == l0_fp)
		{
			return -1;
		}
	}
    //l1
    _l1_writer = malloc(sizeof(fblock_writer));
	if (NULL == _l1_writer)
	{
		return -2;
	}
    opret = ((fblock_writer *) _l1_writer)->init((fblock_t *) l1_handle);
    if(opret < 0)
    {
        return -3;
    }
    //l2
    _l2_writer = malloc(sizeof(mfiles_writer));
	if (NULL == _l2_writer)
	{
		return -4;
	}
    opret = ((mfiles_writer *) _l2_writer)->init((mfiles_t *) l2_handle);
    if(opret < 0)
    {
        return -5;
    }

    _writer_buf =
        (ts_index_t *) malloc(sizeof(ts_index_t) * (TS_INDEX_MAXNUM + 1024));
    if(NULL == _writer_buf)
    {
        return -6;
    }

    _writer_offset = 0;
    l1_num = 0;
    return 0;
}


int ts_disk_ri_t::wt_write(ts_inv_list_t & in)
{
    int opret = 0;
    int dlen = in.ind_list.size() * sizeof(ts_index_t);
    std::copy(in.ind_list.begin(), in.ind_list.end(), _writer_buf);
    opret = ((mfiles_writer *) _l2_writer)->write(_writer_buf, dlen);
    if(opret < 0)
    {
        return -1;
    }

    ts_disk_term_t term;
    term.term = in.term;
    term.offset = _writer_offset;
    term.dlen = dlen;
    opret = ((fblock_writer *) _l1_writer)->write(&term, sizeof(term));
    if(opret < 0)
    {
        return -2;
    }
    _writer_offset += dlen;

    if(0 == l1_num % TS_RI_BLOCK_NUM)
    {
        *((sign64_t *) l0_handle + l0_num) = in.term;
		fwrite(&in.term,sizeof(sign64_t),1,l0_fp);
        l0_num++;
    } else
    {
        *((sign64_t *) l0_handle + l0_num) = in.term;
    }
    l1_num++;
    //_wt_last_term = in.term;

    return 0;
}


int ts_disk_ri_t::wt_fini()
{
    if(_writer_buf)
    {
        free(_writer_buf);
        _writer_buf = NULL;
    }
    int opret = 0;
    opret = ((mfiles_writer *) _l2_writer)->fini();
    if(opret < 0)
    {
        return -1;
    }
	free(_l2_writer);
	_l2_writer = NULL;

    opret = ((fblock_writer *) _l1_writer)->fini();
    if(opret < 0)
    {
        return -2;
    }
	free(_l1_writer);
	_l1_writer = NULL;

    if(l1_num > 0 && 0 != (l1_num - 1) % TS_RI_BLOCK_NUM)
    {
        //  *((sign64_t*)l0_handle + l0_num) = _wt_last_term;
		fwrite((sign64_t*)l0_handle + l0_num,sizeof(sign64_t),1,l0_fp);
        l0_num++;
    }
	fclose(l0_fp);
	l0_fp = NULL;

    return 0;
}

int ts_disk_ri_t::clear()
{
    int opret = 0;
	//l0.disk
	char path[TS_FILEPATH_MAXSIZE];
    snprintf(path, sizeof(path), "%s/%s%s", _dir, _file, RI0_SUFFIX_FILE);
	remove(path);
	//l0.mem
	if (NULL != l0_handle)
	{
		free(l0_handle);
		l0_handle = NULL;
	}
    l0_num = 0;
 	//l1.cache
    mc_destroy_cache((mc_cache *) l1_cache);
	l1_cache = NULL;
    //l1
    opret = fblock_fini((fblock_t *) l1_handle, true);
    if(opret < 0)
    {
        return -2;
    }
	if (NULL != l1_handle)
	{
		free(l1_handle);
		l1_handle = NULL;
	}
	//l2
	opret = mfiles_fini((mfiles_t *) l2_handle, true);
	if(opret < 0)
	{
        return -3;
    }
	if (NULL != l2_handle)
	{
		free(l2_handle);
		l2_handle = NULL;
	}

	pthread_mutex_destroy(&mutex_lock);

    return init(_dir, _file);

};



//////////////////////////////////////////////////////////////////




void ts_ind_iterator_t::_next_block()
{
	if ((int)(buffer.size() + cur_block *TS_INDEX_IOBLOCK_NUM) >= read_maxnum)
	{   
		is_end = true;
		return ;
	}   

	cur_block ++; 
	uint32 read_num = TS_INDEX_IOBLOCK_NUM;
	if ((int)((cur_block+1) * TS_INDEX_IOBLOCK_NUM) >read_maxnum)
	{
		read_num = read_maxnum - (this->cur_block) * TS_INDEX_IOBLOCK_NUM;
	}
	ri->read_at_pos(term,cur_block*TS_INDEX_IOBLOCK_NUM,&buffer,read_num);

	cur_index = &(*buffer.begin());
	end_index = &(*(buffer.end()-1));

	if (0 == buffer.size())
	{
		is_end = true;
		return;
	}
};



int ts_ind_reader_t::read_ind_num(sign64_t &term)
{
	uint32 ind_num = 0;
	int opret = ri->read_inv_num(term, ind_num);
	if(opret < 0)
	{   
		return opret;
	}   

	return ind_num;
};









/* vim: set ts=4 sw=4 sts=4 tw=100 */
