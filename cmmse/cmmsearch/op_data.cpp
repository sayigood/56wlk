/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: op_data.cpp,v 1.7 2009/11/18 20:27:50 yanghy2 Exp $ 
 * 
 **************************************************************************/

/**
 * @file op_data.cpp
 *
 * @date 2008/04/11 19:14:21
 * @version $Revision: 1.7 $ 
 * @brief 全局初始化处理
 *  
 **/

#include "pub.h"



/**
 * @brief 读取配置
 *
 * @return  int 
 * @retval   =0 成功 ; !=0 失败
 * @see 
 * @author yanghy
 * @date 2008/03/10 11:37:16
**/
int cfg_init(int conf_build)
{
    ASSERT(conf_build == 0 || conf_build == 1);
    int opret = 0;

    ///打开配置文件
    ub_conf_data_t *pcfg =
        ub_conf_init(g_cfg.conf_dir, g_cfg.conf_name, conf_build);
    if(NULL == pcfg)
    {
        printf("ub_conf_init failed.[%s][%s]reason[%s]\n", g_cfg.conf_dir, g_cfg.conf_name, strerror(errno));  
		 ///这里没有线程安全问题
        return -1;
    }

printf("cfg_init 00001");

    UB_CONF_GETNSTR(pcfg, "log_dir", g_cfg.log_dir, sizeof(g_cfg.log_dir),
                    "日志路径");
    UB_CONF_GETNSTR(pcfg, "log_name", g_cfg.log_name, sizeof(g_cfg.log_name),
                    "日志名称");
    UB_CONF_GETINT(pcfg, "log_level", &g_cfg.log_level, "日志级别");

    ///打开日志需要在读取配置以后
    opret = ub_log_init(g_cfg.log_dir, g_cfg.log_name, 2000, g_cfg.log_level);
    if(0 != opret)
    {
        printf("ub_log_init failed.[%s]reason[%s]\n", g_cfg.log_name, strerror(errno)); ///这里没有线程安全问题
        return -1;
    }

UB_LOG_FATAL("cfg_init 00002");

    UB_CONF_GETNSTR(pcfg, "limits_path", g_cfg.limits_path,
                    sizeof(g_cfg.limits_path), "limits文件的路径");
UB_LOG_FATAL("cfg_init 00002aaaaaaa");
    UB_CONF_GETINT(pcfg, "ontime_sleep(s)", &g_cfg.ontime_sleep,
                   "ontime线程休眠时间");
    UB_CONF_GETNSTR(pcfg, "iplist_path", g_cfg.iplist_path,
                    sizeof(g_cfg.iplist_path), "ip白名单路径");
    UB_CONF_GETNSTR(pcfg, "data_path", g_cfg.data_path,
                    sizeof(g_cfg.data_path), "数据文件的路径");
	UB_CONF_GETINT(pcfg, "static_index", &g_cfg.static_index,
			"是否是静态索引:1,是;0,否");
    UB_CONF_GETSVR(pcfg, "cmmsearch", "update", &g_cfg.svr_update,
                   "更新线程配置");
    UB_CONF_GETSVR(pcfg, "cmmsearch", "query", &g_cfg.svr_query, "查询线程配置");
    UB_CONF_GETUINT(pcfg, "memory_maxsize(M)", &g_cfg.memory_maxsize,
                    "开辟的内存数量");
UB_LOG_FATAL("cfg_init 00002bbbbbbbbbbb");
	if (0 == g_cfg.static_index) {
		UB_CONF_GETINT(pcfg, "dump01_tmeval(s)", &g_cfg.dump01_tmeval,
				"0,1目录dump的时间间隔");
		UB_CONF_GETINT(pcfg, "dump2_tm", &g_cfg.dump2_tm, "dump到2目录的时间点");
		UB_CONF_GETINT(pcfg, "dump2_min_tmeval(s)", &g_cfg.dump2_min_tmeval,
				"2次dump到2目录的最短时间间隔");
		UB_CONF_GETNSTR(pcfg, "dump01_flag", g_cfg.dump01_flag,
				TS_FILEPATH_MAXSIZE, " 如果存在这个文件，就dump01");
	}
    UB_CONF_GETINT(pcfg, "query_readbuf_size(byte)", &g_cfg.query_readbuf_size,
                   "查询线程读buffer大小");
    UB_CONF_GETINT(pcfg, "query_writebuf_size(byte)", &g_cfg.query_writebuf_size,
                   "查询线程写buffer大小");
    UB_CONF_GETINT(pcfg, "update_readbuf_size(byte)", &g_cfg.update_readbuf_size,
                   "更新线程读buffer大小");
    UB_CONF_GETINT(pcfg, "update_writebuf_size(byte)", &g_cfg.update_writebuf_size,
                   "更新线程写buffer大小");
    UB_CONF_GETNSTR(pcfg, "data_type", g_cfg.data_type,
                    sizeof(g_cfg.data_type), "索引的数据类型");
    UB_CONF_GETNSTR(pcfg, "so_path", g_cfg.so_path, sizeof(g_cfg.so_path),
                    "so文件的路径");

    UB_CONF_GETUINT(pcfg, "data_maxnum", &g_cfg.data_maxnum,
                    "支持的数据个数");
    UB_CONF_GETUINT(pcfg, "brief_size(byte)", &g_cfg.brief_size,
                    "brief表大小");
    UB_CONF_GETUINT(pcfg, "fulltext_maxsize(byte)", &g_cfg.fulltext_maxsize,
                    "单个di数据最大长度");
    g_cfg.svr_update.thread_num = 1;
UB_LOG_FATAL("cfg_init 00002cccccccccccccc");

    ub_conf_close(pcfg);
    return 0;
}


STATIC int _cfg_check()
{
    return 0;
}

/**
 * @brief 检查配置的合法性
 *  这里不仅是ub_conf内部做得范围检查，还将检查一些配置的逻辑关系有没有问题
 * @return  int 
 * @retval   
 * @see 
 * @author yanghy
 * @date 2008/03/12 11:52:36
**/
int cfg_check()
{
    int ret = 0;

printf("cfg_check 00000");
    ret = cfg_init(0);
    if(0 != ret)
    {
        UB_LOG_FATAL("cfg_init failed.");
        return ret;
    }
printf("cfg_check 11111");
    return _cfg_check();

}


/**
 * @brief 获取默认接口
 *        如果用户没有实现句柄内的部分接口，系统将为它们置上默认的接口
 * @param [in/out] hd   : ts_handle_t*
 * @return  int 
 * @retval   
**/
STATIC int 
get_default_interface(ts_handle_t * hd)
{
UB_LOG_FATAL("cfg_init get_default_interface111");

    if(NULL == hd->global_init)
    {
UB_LOG_FATAL("cfg_init get_default_interface211");
        hd->global_init = DEFAULT_GLOBAL_INIT;
    }
UB_LOG_FATAL("cfg_init get_default_interface311");
    if(NULL == hd->query_thread_init)
    {
UB_LOG_FATAL("cfg_init get_default_interface411");
        hd->query_thread_init = DEFAULT_THREAD_INIT;
    }
UB_LOG_FATAL("cfg_init get_default_interface511");
    if(NULL == hd->update_thread_init)
    {
UB_LOG_FATAL("cfg_init get_default_interface611");
        hd->update_thread_init = DEFAULT_THREAD_INIT;
    }
UB_LOG_FATAL("cfg_init get_default_interface711");
    if(NULL == hd->ontime_session_proc)
    {
UB_LOG_FATAL("cfg_init get_default_interface811");
        hd->ontime_session_proc = DEFAULT_SESSION_INIT;
    }
UB_LOG_FATAL("cfg_init get_default_interface911");
    if(NULL == hd->query_handle)
    {
UB_LOG_FATAL("cfg_init get_default_interface011");
        hd->query_handle = new DEFAULT_QUERY_HANDLE();
    }
UB_LOG_FATAL("cfg_init get_default_interface111");
    if(NULL == hd->update_handle)
    {
UB_LOG_FATAL("cfg_init get_default_interface211");
        hd->update_handle = new DEFAULT_UPDATE_HANDLE();
    }

UB_LOG_FATAL("cfg_init get_default_interface311");

    return 0;
}

/**
 * @brief 获取句柄
 *
 * @param [in] so_path   : const char*
 * @param [in] data_type   : const char*
 * @return  ts_handle_t* 
 * @retval   
**/
ts_handle_t *
get_handle(const char *so_path, const char *data_type)
{
    assert(so_path && data_type);

    int opret = 0;
    void *handle = NULL;

UB_LOG_FATAL("get_handle 0000001");
    handle = dlopen(so_path, RTLD_NOW); // 使用 RTLD_NOW, 把复杂性都放在初始化上
    if(!handle)
    {
        ul_writelog(UL_LOG_FATAL, "load mod[%s] failed[%s]", so_path,
                    dlerror());
UB_LOG_FATAL("get_handle 0000002--------NULL");
        return NULL;
    }
UB_LOG_FATAL("get_handle 0000002");



    char initializer_name[256];
    snprintf(initializer_name, sizeof(initializer_name), "ts_so_%s_handle",
             data_type);

    ts_handle_t *hd = NULL;

    hd = (ts_handle_t *) dlsym(handle, initializer_name);
UB_LOG_FATAL("get_handle 0000003");

    if(!hd)
    {
        ul_writelog(UL_LOG_FATAL,
                    "import_module: could not found extension init method[%s] error[%s]",
                    initializer_name, dlerror());
        return NULL;
    }
UB_LOG_FATAL("get_handle 0000004");
    /// 获取默认的接口
    opret = get_default_interface(hd);
    if(opret < 0)
    {
        UB_LOG_FATAL("get_default_interface failed.");
        return NULL;
    }


    return hd;
};

/**
 * @brief 获取服务当前的状态
 *
 * @return  STATIC int 
 * @retval   
**/
STATIC int 
get_serv_status()
{
    int opret = 0;
    char path[TS_FILEPATH_MAXSIZE];
    struct stat buf;

    snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path, g_cfg.data_type,
             NEED_LOAD_DAY);
    if((stat(path, &buf) == 0 && buf.st_mode & S_IFREG))
    {
        g_runtime.need_load_day = 1;
    }

    snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path, g_cfg.data_type,
             NEED_MERGE);
    if((stat(path, &buf) == 0 && buf.st_mode & S_IFREG))
    {
        g_runtime.need_merge = 1;
    }

    FILE *fp = NULL;

    snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path, g_cfg.data_type,
             DAY_CURDIR);
    fp = fopen(path, "r");
    if(NULL == fp)
    {
        UB_LOG_FATAL("fopen path[%s]failed.", path);
        return -1;
    }
    opret = fscanf(fp, "%d", &g_runtime.day_curdir);
    if(1 != opret)
    {
        UB_LOG_FATAL("fscanf failed.[%s]", path);
        return -1;
    }
    fclose(fp);
    fp = NULL;

    snprintf(path, sizeof(path), "%s/%s.%s", g_cfg.data_path, g_cfg.data_type,
             MON_CURDIR);
    fp = fopen(path, "r");
    if(NULL == fp)
    {
        UB_LOG_FATAL("fopen path[%s]failed.", path);
        return -1;
    }
    opret = fscanf(fp, "%d", &g_runtime.mon_curdir);
    if(1 != opret)
    {
        UB_LOG_FATAL("fscanf failed.[%s]", path);
        return -1;
    }
    fclose(fp);
    fp = NULL;



    return 0;
}
/*
int resend_cmd()
{
	int opret = 0;
	
    /// 回放命令
    char *cmd = NULL;
    uint32 size = 0;
    opret = g_runtime.bk.get_cmd_init();
    if(opret < 0)
    {
        UB_LOG_FATAL("bk.get_cmd_init failed");
        return opret;
    }

    nshead_t tmp_head;
    nshead_t *req_head = NULL;
    nshead_t *res_head = &tmp_head;
    ub_buff_t req_buf, res_buf;
    int i = 0;
	uint32 inner_id = 0;
    while(!g_runtime.bk.get_cmd_is_end())
    {
        opret = g_runtime.bk.get_cmd_and_next(&cmd, &inner_id, &size);
        if(opret < 0)
        {
            break;
        }
        req_head = (nshead_t *) cmd;
        if(NULL == req_head || NULL == res_head)
        {
            UB_LOG_FATAL("get req_head[%ld] || res_head[%ld] failed.",
                         (long)req_head, (long)res_head);
            return -1;
        }

        req_buf.buf = (char *)(req_head + 1);
        req_buf.size = size - sizeof(nshead_t);
        res_buf.buf = (char *)(res_head + 1);
        res_buf.size = sizeof(nshead_t);
        int cmd_no =  ((ts_head_t*)req_buf.buf)->cmd_no;


        opret = ts_rebuild(cmd_no, inner_id, req_head,&req_buf, res_head, &res_buf);

        if(opret < 0)
        {
            UB_LOG_WARNING("ub_process_cmdmap failed.");
            continue;
        }
        UB_LOG_DEBUG("traverse cmd[%d]", i++);

    }

    opret = g_runtime.bk.get_cmd_fini();
    if(opret < 0)
    {
        UB_LOG_FATAL("bk.get_cmd_fini failed");
        return opret;
    }
	return 0;
}
*/
/**
 * @brief 初始化所有数据
 *   包括patch初始化，全局数据初始化等
 * @return  int 
 * @retval   
 * @see 
 * @author yanghy
 * @date 2008/03/10 11:38:45
**/
int data_init()
{
UB_LOG_FATAL("data_init 000000011111111");
    int opret = 0;
    ///获取扩展句柄
    g_runtime.handle = get_handle(g_cfg.so_path, g_cfg.data_type);
    if(NULL == g_runtime.handle)
    {
        UB_LOG_FATAL("get handle failed.so_path[%s]data_type[%s]",
                     g_cfg.so_path, g_cfg.data_type);
        return -1;
    }
UB_LOG_FATAL("data_init 000000022222222");
    ///句柄初始化，由用户自定义
    opret = g_runtime.handle->global_init();
    if(opret < 0)
    {
        UB_LOG_FATAL("g_runtime.handle->global_init failed.ret[%d]", opret);
        return opret;
    }

UB_LOG_FATAL("data_init 00000003333333333333");
    /** 初始化各种数据 **/

    ///加载模块状态
    opret = get_serv_status();
    if(opret < 0)
    {
        UB_LOG_FATAL("get_serv_stat failed.data_path[%s]data_type[%s]ret[%d]",
                     g_cfg.data_path, g_cfg.data_type, opret);
        return opret;
    }

    /// del_table init
	char path[TS_FILEPATH_MAXSIZE];
	snprintf(path,sizeof(path),"%s.del",g_cfg.data_type);
    opret =
        g_runtime.del_table.init(g_cfg.data_maxnum, g_cfg.data_path,path);
    if(opret < 0)
    {
        UB_LOG_FATAL
            ("del_table init failed.data_maxnum[%d]data_path[%s]data_type[%s]ret[%d]",
             g_cfg.data_maxnum, g_cfg.data_path, g_cfg.data_type, opret);

        return opret;
    }
	/// mod_table init
	snprintf(path,sizeof(path),"%s.mod",g_cfg.data_type);
    opret =
        g_runtime.mod_table.init(g_cfg.data_maxnum, g_cfg.data_path,path);
    if(opret < 0)
    {
        UB_LOG_FATAL
            ("mod_table init failed.data_maxnum[%d]data_path[%s]data_type[%s]ret[%d]",
             g_cfg.data_maxnum, g_cfg.data_path, g_cfg.data_type, opret);

        return opret;
    }
 

    /// cm init
    opret = g_runtime.cm.init(g_cfg.data_path, g_cfg.data_type);
    if(opret < 0)
    {
        UB_LOG_FATAL("cm init failed.data_path[%s]data_type[%s]ret[%d]",
                     g_cfg.data_path, g_cfg.data_type, opret);
        return opret;
    }
    /// mg init
    opret = g_runtime.mg.init(g_cfg.brief_size,
                              g_cfg.data_maxnum,
                              g_cfg.data_path, g_cfg.data_type);
    if(opret < 0)
    {
        UB_LOG_FATAL
            ("mg init failed.brief_size[%d]data_maxnum[%d]data_path[%s]data_type[%s]ret[%d]",
             g_cfg.brief_size, g_cfg.data_maxnum, g_cfg.data_path,
             g_cfg.data_type, opret);
        return opret;
    }
    /// di init
    opret =
        g_runtime.di.init(g_cfg.fulltext_maxsize, g_cfg.data_path,
                          g_cfg.data_type);
    if(opret < 0)
    {
        UB_LOG_FATAL
            ("di init failed.data_maxsize[%d]data_path[%s]data_type[%s]ret[%d]",
             g_cfg.fulltext_maxsize, g_cfg.data_path, g_cfg.data_type, opret);
        return opret;
    }
    /// ri init
    opret = g_runtime.mem_ri.init(g_cfg.data_path, g_cfg.data_type);
    if(opret < 0)
    {
        UB_LOG_FATAL("mem_ri init failed.data_path[%s]data_type[%s]ret[%d]",
                     g_cfg.data_path, g_cfg.data_type, opret);
        return opret;
    }
    char tmp_path[TS_FILEPATH_MAXSIZE];
    for(int i = 0; i < 3; i++)
    {
        snprintf(tmp_path, sizeof(tmp_path), "%s.day_%d", g_cfg.data_type, i);
        opret = g_runtime.day_ri[i].init(g_cfg.data_path, tmp_path);
        if(opret < 0)
        {
            UB_LOG_FATAL
                ("day_ri[%d] init failed.data_path[%s]data_type[%s]ret[%d]",
                 i, g_cfg.data_path, g_cfg.data_type, opret);
            return opret;
        }

    }
    for(int i = 0; i < 2; i++)
    {
        snprintf(tmp_path, sizeof(tmp_path), "%s.mon_%d", g_cfg.data_type, i);
        opret = g_runtime.mon_ri[i].init(g_cfg.data_path, tmp_path);
        if(opret < 0)
        {
            UB_LOG_FATAL
                ("mon_ri[%d] init failed.data_path[%s]data_type[%s]ret[%d]",
                 i, g_cfg.data_path, g_cfg.data_type, opret);
            return opret;
        }

    }

    ///加载数据
    if(g_runtime.need_load_day)
    {
        opret =
            copy_ri(&g_runtime.day_ri[g_runtime.day_curdir],
                    &g_runtime.mem_ri);
        if(opret < 0)
        {
            UB_LOG_FATAL("transfer_ri failed.day_curdir[%d]ret[%d]",
                         g_runtime.day_curdir, opret);
            return opret;
        }
    }
    ///数据回放
    opret =
        g_runtime.bk.init(g_cfg.data_path, g_cfg.data_type,
                          g_cfg.update_readbuf_size);
    if(opret < 0)
    {
        UB_LOG_FATAL("bk init failed.data_path[%s]data_type[%s]ret[%d]",
                     g_cfg.data_path, g_cfg.data_type, opret);
        return opret;
    }
UB_LOG_FATAL("data_init 0000000444444444444444");
    exit(0);
    /** 初始化各种数据 **/

    /// 回放命令
 //   return resend_cmd();

	return 0;
}



/**
 * @brief 获取内存的使用量
 *
 * @return  uint32 
 * @retval   
**/
uint32 get_memory_size()
{
    return g_runtime.mem_ri.size() / (1024 * 1024);
}



/* vim: set ts=4 sw=4 sts=4 tw=100 */
