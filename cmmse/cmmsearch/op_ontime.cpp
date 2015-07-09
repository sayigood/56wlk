/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: op_ontime.cpp,v 1.5 2009/11/18 20:27:50 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file op_ontime.cpp
 *
 * @date 2008/04/07 22:47:37
 * @version $Revision: 1.5 $ 
 * @brief 定时监控线程
 *  
 **/
#include "pub.h"
static time_t _oldt = 0;
static int _day2_dumped = 0;
static int _dump2tm = 0;
/**
 * @brief 写limits文件
 *
 * @return  void 
 * @retval   
 * @see 
 * @author yanghy
 * @date 2008/03/17 16:02:47
**/
void write_limits()
{
    FILE *fm = fopen(g_cfg.limits_path, "w");

    if(NULL == fm)
    {
        UB_LOG_WARNING("can't open limits [file : %s]", g_cfg.limits_path);
    } else
    {

        fprintf(fm, "conf_%s_o_num : %u\n", g_cfg.data_type,
                g_cfg.data_maxnum);
        fprintf(fm, "conf_%s_mem_size : %u\n", g_cfg.data_type,
                g_cfg.memory_maxsize);
        fprintf(fm, "data_%s_o_num : %u\n", g_cfg.data_type,
                g_runtime.cm.o_size());
        fprintf(fm, "data_%s_mem_size : %u\n", g_cfg.data_type,
                get_memory_size());
        fprintf(fm, "data_%s_i_num : %u\n", g_cfg.data_type,
                g_runtime.cm.i_size());

        UB_FCLOSE(fm);
    }
}


/**
 * @brief 将当前目录号写入day_curdir文件
 *
 * @param curno   : int
 * @return  void 
 * @retval   
 * @author yanghy
 * @date Tue Oct 31 20:20:02 CST 2006
**/

STATIC int write_curdir(int cur_dir, bool is_day)
{
    ASSERT((cur_dir == 0 || cur_dir == 1));
    char path[TS_FILEPATH_MAXSIZE];
    if(is_day)
        snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path,
                 g_cfg.data_type, DAY_CURDIR);
    else
        snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path,
                 g_cfg.data_type, MON_CURDIR);

    FILE *fp = fopen(path, "w");
    if(NULL == fp)
    {
        UB_LOG_FATAL("fopen path[%s]failed.", path);
        return -1;
    }
    rewind(fp);
    fprintf(fp, "%d", cur_dir);
    fflush(fp);
    fclose(fp);
    fp = NULL;
    return 0;
}

/**
 * @brief 检查是否到了dump的条件
 *
 * @return  int 
 * @retval   
 * @see 
 * @author yanghy
 * @date 2008/03/17 16:10:54
**/

int chk_dump_condition()
{

    time_t nowt;
    time(&nowt);
    if(nowt - _oldt > g_cfg.dump01_tmeval || _is_fileexist(g_cfg.dump01_flag))
    {
        if(_is_fileexist(g_cfg.dump01_flag))
        {
            remove(g_cfg.dump01_flag);
        }
        _oldt = nowt;
        return (g_runtime.day_curdir == 0 ? 1 : 0);
    }



    struct tm nowtm;
    localtime_r(&nowt, &nowtm);

    uint32 tsize = get_memory_size();
	if (tsize >= g_cfg.memory_maxsize)
	{
		return 2;
	}
	if(nowtm.tm_hour == g_cfg.dump2_tm )
    {
        if(nowt - _dump2tm > g_cfg.dump2_min_tmeval &&
			0 == _day2_dumped)
        {
            _day2_dumped = 1;
            _dump2tm = nowt;
            return 2;
        }
    } else
    {
        _day2_dumped = 0;
    }

    if(g_runtime.need_merge)
    {
        return 3;
    }

    return -1;
}



int merge_mon()
{
    int opret = 0;
    char path[TS_FILEPATH_MAXSIZE];

    int mon_newdir = g_runtime.mon_curdir ? 0 : 1;
    opret =
        merge_ri(&g_runtime.day_ri[2],
                 &g_runtime.mon_ri[g_runtime.mon_curdir],
                 &g_runtime.mon_ri[mon_newdir]);
    if(opret < 0)
    {
        UB_LOG_FATAL("merge_ri failed.day[2]mon[%d]newmon[%d]ret[%d]",
                     g_runtime.mon_curdir, mon_newdir, opret);
        return opret;
    }
    g_runtime.mon_curdir = mon_newdir;

    opret = write_curdir(g_runtime.mon_curdir, false);
    if(opret < 0)
    {
        UB_LOG_FATAL("write_curdir failed.cur_dir[%d]path[%s]",
                     g_runtime.mon_curdir, g_cfg.data_path);
        safe_exit(1);
    }


    //rm need_merge
    snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path, g_cfg.data_type,
             NEED_MERGE);
    opret = remove(path);
    g_runtime.need_merge = 0;

    return 0;
}





/**
 * @brief 定时运行的线程
 *
 * @param [in] _usr_data   : char* 用户自定义数据
 * @return  int 
 * @retval   
 * @see 
 * @author yanghy
 * @date 2008/03/13 14:11:08
**/
int op_ontime()
{

    int opret = 0;
    int libno = -1;
    char path[TS_FILEPATH_MAXSIZE];
    opret = chk_dump_condition();
    switch (opret)
    {

    case -1:
        return 0;
    case 0:
    case 1:
    case 2:
        libno = opret;
        break;
    case 3:
        return merge_mon();
    default:
        UB_LOG_WARNING("chk dump condition err.ret[%d]", opret);
        safe_exit(1);
    }

    ///申请全局锁
    ans::lock _lock(&g_ts_lock);



    struct timeval s, e;
    gettimeofday(&s, NULL);



    ///写硬盘
    opret = copy_ri(&g_runtime.mem_ri, &g_runtime.day_ri[libno]);
    if(opret < 0)
    {
        UB_LOG_FATAL("dump_to_disk failed.libno[%d]ret[%d]", libno, opret);
        safe_exit(1);
    }
    /////重置命令回放点
    //opret = g_runtime.bk.set_bkup_pos();
    //if(opret < 0)
    //{
        //UB_LOG_FATAL("SetCachedBCommandPos failed.ret[%d]", opret);
        //safe_exit(1);
    //}
    ///写标记文件
    if(2 != libno)
    {
        g_runtime.day_curdir = libno;
        opret = write_curdir(g_runtime.day_curdir, true);
        if(opret < 0)
        {
            UB_LOG_FATAL("write_curdir failed.cur_dir[%d]path[%s]ret[%d]",
                         g_runtime.day_curdir, g_cfg.data_path, opret);
            safe_exit(1);
        }
        //creat need_load_day,表示如果重启indsrv，需要加载硬盘数据
        snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path,
                 g_cfg.data_type, NEED_LOAD_DAY);
        opret = creat(path, S_IRUSR | S_IRGRP | S_IROTH);
        if(opret > 0)
        {
            close(opret);
        }
        g_runtime.need_load_day = 1;
    } else  ///dump2标记文件
    {
        //touch need_merge
        snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path,
                 g_cfg.data_type, NEED_MERGE);
        opret = creat(path, S_IRUSR | S_IRGRP | S_IROTH);
        if(opret > 0)
        {
            close(opret);
        }
        g_runtime.need_merge = 1;
        //rm need_load_day
        snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path,
                 g_cfg.data_type, NEED_LOAD_DAY);
        opret = remove(path);
        g_runtime.need_load_day = 0;
        //reset memory
        g_runtime.mem_ri.clear();


    }



    gettimeofday(&e, NULL);
    UB_LOG_NOTICE("ontime_thread dtype[%s]libno[%d]mem[%u]M tm[%lu]us",
                  g_cfg.data_type, libno, get_memory_size(),
                  (e.tv_sec - s.tv_sec) * 1000000 + e.tv_usec - s.tv_usec);





    return 0;
}

void user_func()
{
    int opret = g_runtime.handle->ontime_session_proc();
    if(opret < 0)
    {
        safe_exit(1);
    }
}

/**
 * @brief 定时监控线程
 *
 * @return  void* 
 * @retval   
 * @see 
 * @author yanghy
 * @date 2008/03/23 01:56:19
**/
void *ontime_thread(void *)
{
    int ret = ub_log_initthread("ontime");
    if(0 != ret)
    {
        UB_LOG_FATAL("fail to open log for ontime!");
        return NULL;
    }

	sleep(10); /// 保证update_thread先运行
    time(&_oldt);
    _day2_dumped = 0;
    _dump2tm = 0;


    while(1)
    {
        write_limits();
        user_func();
		if (0 == g_cfg.static_index) {
        	op_ontime();
		}
        sleep((int)g_cfg.ontime_sleep);
    }
    return NULL;
}







/* vim: set ts=4 sw=4 sts=4 tw=100 */
