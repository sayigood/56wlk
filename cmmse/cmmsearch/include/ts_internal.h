/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: ts_internal.h,v 1.5 2009/11/18 20:27:50 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file tinyse_internal.h
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/06/18 17:34:32
 * @version $Revision: 1.5 $ 
 * @brief 内部接口
 *  
 **/


#ifndef  __TS_INTERNAL_H_
#define  __TS_INTERNAL_H_


#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
* @brief brief数据读写类
*/
class ts_mg_t
{

    uint32 _size;
    uint32 _num;
    char *_array;

  public:
    int init(uint32 item_size, uint32 item_num, const char *dir,
             const char *file);
    void write(uint32 id, const void *brief);
    void read(uint32 id, void *brief);
    int max_num();
};

/**
* @brief di数据读写类
*/
class ts_di_t
{
    bool _end;
    pointer_t fb_handle;
    pointer_t mf_handle;
  public:
    int init(uint32 data_maxsize, const char *dir, const char *file);
    int write(uint32 id, const void *data, uint32 size);
    int read(uint32 id, void *data, uint32 & size);
};


/**
* @brief 命令备份与回放
*/
class ts_bk_t
{
  private:
    pthread_mutex_t mutex_lock;
    pointer_t bcmd;
    pointer_t last_bpos;
    char *bcmd_buf;
    int bcmd_bufsize;

    int cur_file;
    FILE *_fp;
    bool is_end;

    long _curfile_size;

    int _chk_and_open_file();
  public:

    int init(const char *path, int cmd_maxsize);

    int init(const char *dir, const char *file, int cmd_maxsize);

    void push_cmd(uint32 cmd_no, const void *cmd, uint32 size);

    int get_cmd_init();
    bool get_cmd_is_end();
    int get_cmd_and_next(char **cmd, uint32 *command_type, uint32 * size);
    int get_cmd_fini();


    int get_bkup_pos();
    int set_bkup_pos();
};


/**
* @brief 内部id的分配与管理
*/
class ts_cm_t
{
  private:
    int i2o_fd;
    int o2i_fd;
    FILE *flag_fp;

    uint32 o_max;
    uint32 i_max;

    int _alloc(uint32 & outer_id, uint32 & id);
    bool is_new(uint32 & outer_id);
  public:
    int init(const char *dir, const char *file);
    int alloc(uint32 & outer_id, uint32 & id);
    int realloc(uint32 & outer_id, uint32 & id);
    uint32 get_inner_id(const uint32 outer_id);
    uint32 get_outer_id(const uint32 inner_id);

    uint32 o_size()
    {
        return o_max;
    };
    uint32 i_size()
    {
        return i_max;
    };
};



typedef struct ts_ri_item_t
{
    sign64_t term;
    uint32 id;
    uint32 weight;

} ts_ri_item_t;

/**
* @brief rindex倒排表信息
*/
typedef struct ts_inv_list_t
{
    sign64_t term;
	vector < ts_index_t > ind_list;

} ts_inv_list_t;



class ts_ri_t
{
  public:
    virtual int init(const char *dir, const char *file)
    {
        return 0;
    };
    virtual int read_inv_num(const sign64_t word, uint32 & num)
    {
        return 0;
    };
    virtual int read(const sign64_t word, vector < ts_index_t > *out,
                     uint32 maxnum)
    {
        return 0;
    };

    virtual int read_at_pos(const sign64_t word,int pos, vector < ts_index_t > *out,
                       uint32 maxnum)
    {   
        return 0;
    }   

    virtual int write(const ts_ri_item_t & item)
    {
        return 0;
    };



    virtual int it_init()
    {
        return 0;
    };
    virtual int it_get_and_next(ts_inv_list_t & out)
    {
        return 0;
    };
    virtual int it_is_end()
    {
        return 0;
    };
    virtual int it_fini()
    {
        return 0;
    };

    virtual int wt_init()
    {
        return 0;
    };
    virtual int wt_write(ts_inv_list_t & out)
    {
        return 0;
    };
    virtual int wt_fini()
    {
        return 0;
    };

    virtual int clear()
    {
        return 0;
    };
    virtual ~ ts_ri_t()
    {
    };
};



class ts_mem_ri_t:public ts_ri_t
{
  private:
    pointer_t hashmap;
    long _size;

    sign64_t *_it_membuf;
    uint32 _it_pos;
    uint32 _it_size;

	pthread_rwlock_t rw_lock;
  public:
      virtual int init(const char *dir, const char *file);
    virtual int read_inv_num(const sign64_t word, uint32 & num);
    //virtual int read(const sign64_t word,ts_inv_list_t& out,uint32 maxnum);
    virtual int read(const sign64_t word, vector < ts_index_t > *out,
                     uint32 maxnum);
    virtual int read_at_pos(const sign64_t word,int pos, vector < ts_index_t > *out,
                       uint32 maxnum);

    virtual int write(const ts_ri_item_t & item);


    virtual long size();

    virtual int it_init();
    virtual int it_get_and_next(ts_inv_list_t & out);
    virtual int it_is_end();
    virtual int it_fini();

    virtual int wt_init();
    virtual int wt_write(ts_inv_list_t & out);
    virtual int wt_fini();

    virtual int clear();
};


class ts_disk_ri_t:public ts_ri_t
{
  private:
    pointer_t l0_handle;          /**< hashmap       */
	FILE* l0_fp;
    uint32 l1_num;
    uint32 l0_num;

    pthread_mutex_t mutex_lock;
    pointer_t l1_cache;
    pointer_t l1_handle;          /**< fileblock       */
    pointer_t l2_handle;          /**< mfiles       */
    int _read_l1(const sign64_t word, ts_disk_term_t & term);
    int _read_l2(long long offset, int dlen, vector < ts_index_t > *out);

    pointer_t _l1_reader;
    pointer_t _l1_writer;
    pointer_t _l2_reader;
    pointer_t _l2_writer;

    ts_index_t *_reader_buf;
    ts_index_t *_writer_buf;
    long long _writer_offset;
    //sign64_t _wt_last_term;

    char _dir[TS_FILEPATH_MAXSIZE];
    char _file[TS_FILEPATH_MAXSIZE];
  public:
    /**  不允许继承*/
      virtual int write(const ts_ri_item_t & item)
    {
        assert(0);
        return 0;
    };


    virtual int init(const char *dir, const char *file);
    virtual int read_inv_num(const sign64_t word, uint32 & num);
    virtual int read(const sign64_t word, vector < ts_index_t > *out,
                     uint32 maxnum);
    virtual int read_at_pos(const sign64_t word,int pos, vector < ts_index_t > *out,
                       uint32 maxnum);



    virtual int it_init();
    virtual int it_get_and_next(ts_inv_list_t & out);
    virtual int it_is_end();
    virtual int it_fini();

    virtual int wt_init();
    virtual int wt_write(ts_inv_list_t & out);
    virtual int wt_fini();

    virtual int clear();

};


int merge_ri(ts_ri_t * src1, ts_ri_t * src2, ts_ri_t * dest);

int copy_ri(ts_ri_t * src, ts_ri_t * dest);
////////////////////////////




class ts_ind_iterator_t
{
	vector<ts_index_t> buffer;

	ts_ri_t *ri;
	const sign64_t term;
	const int read_maxnum;
	bool is_end;
	int cur_block;
	ts_index_t *cur_index;
	ts_index_t *end_index;


	public:
	typedef ptrdiff_t difference_type;
	typedef input_iterator_tag iterator_category;
	typedef ts_index_t value_type;
	typedef ts_index_t self;
	typedef ts_index_t& reference;
	typedef ts_index_t* pointer;

	ts_ind_iterator_t() : ri(NULL),term(sign64_t()),read_maxnum(0){
		is_end = true;
	};

	ts_ind_iterator_t (sign64_t _term,int _size,ts_ri_t *_ri) : ri(_ri),term(_term),read_maxnum(_size)
	{
		is_end = false;
		cur_block = -1; 
		cur_index = NULL;
		end_index = NULL;
		buffer.clear();
		_next_block();

	};

	int size(){
		return read_maxnum;
	};

	reference operator*()
	{
		return *cur_index;

	};
	void increment()
	{
		if (cur_index != end_index) {

			cur_index++;

		} else {
			_next_block();

		}
	}

	reference operator ++()
	{
		increment();
		return (operator *());
	}
	self operator ++ (int)
	{
		reference tmp = operator *();

		increment();

		return tmp;

	};
	pointer operator ->()
	{
		return &(operator*());
	}
	/** 为了提高性能，简化比较 */
	bool operator  == (const ts_ind_iterator_t &other)
	{
		return is_end == other.is_end;
	}
	bool operator  != (const ts_ind_iterator_t &other)
	{
		return is_end != other.is_end;
	}
	private:
	void _next_block();


};
class ts_ind_reader_t
{
	public:
		typedef ts_ind_iterator_t  iterator;

	public:
		ts_ind_reader_t(ts_ri_t *_ri)
		{
			this->ri = _ri;
		}
		~ts_ind_reader_t() {};

		int read_ind_num(sign64_t &term);
		
		iterator& end()
		{
			return end_element;
		};
		iterator begin(sign64_t& _term,int _read_maxnum)
		{

			int ind_num = read_ind_num(_term);

			int num = _read_maxnum>ind_num?ind_num:_read_maxnum;
			return iterator(_term,num,ri);

		}


	private:
		ts_ind_reader_t();
		ts_ri_t *ri;
		iterator end_element;

};


////////////////////////////


#define BITSET_SUFFIX_FILE ".bitset"
class ts_bitset_t
{
	private:
		uint32 _num ;
		char *_array;
	public:
		int init(uint32 data_maxnum,const char* dir ,const char *file);

		void set(uint32 id,bool value);

		bool get(uint32 id);

		int reset();

};














#endif //__TS_INTERNAL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
