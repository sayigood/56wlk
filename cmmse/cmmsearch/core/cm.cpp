/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: cm.cpp,v 1.3 2009/04/13 09:02:21 yanghy2 Exp $ 
 * 
 **************************************************************************/



/**
 * @file cm.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/04/11 15:14:44
 * @version $Revision: 1.3 $ 
 * @brief 
 *  
 **/
//#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "tinyse.h"
#define CM_SUFFIX_I2O ".cm_i2o"
#define CM_SUFFIX_O2I ".cm_o2i"
#define CM_SUFFIX_FLAG ".cm_flag"

/** 如果用写的方式打开文件，则会默认创建文件. */
static int _open_file(const char *filename, int flag)
{
    //readonly?O_RDONLY:(O_CREAT|O_WRONLY), 
    int fd = open(filename, flag,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    return fd;
}



int ts_cm_t::init(const char *path, const char *file)
{
    char namebuf[TS_FILEPATH_MAXSIZE];
    snprintf(namebuf, sizeof(namebuf), "%s/%s%s", path, file, CM_SUFFIX_I2O);
    int fd = _open_file(namebuf, O_CREAT | O_RDWR);
    if(fd <= 0)
    {
        return -1;
    }
    i2o_fd = fd;

    snprintf(namebuf, sizeof(namebuf), "%s/%s%s", path, file, CM_SUFFIX_O2I);
    fd = _open_file(namebuf, O_CREAT | O_RDWR);
    if(fd <= 0)
    {
        return -2;
    }
    o2i_fd = fd;

    snprintf(namebuf, sizeof(namebuf), "%s/%s%s", path, file, CM_SUFFIX_FLAG);
    flag_fp = fopen(namebuf, "r+");
    if(NULL == flag_fp)
    {
        return -3;
    }
    int ret = fscanf(flag_fp, "%u : %u\n", &i_max, &o_max);
    if(2 != ret)
    {
        return -4;
    }
    ///先填充id=0的位置
    i_max = (i_max == 0) ? 1 : i_max;
    o_max = (o_max == 0) ? 1 : o_max;

    return 0;
}

int ts_cm_t::_alloc(uint32 & outer_id, uint32 & id)
{

    id = i_max;
    i_max++;

    ssize_t ret =
        pwrite(i2o_fd, &outer_id, sizeof(uint32), sizeof(uint32) * id);
    if(ret != sizeof(uint32))
    {
        return -2;
    }
    ret = pwrite(o2i_fd, &id, sizeof(uint32), sizeof(uint32) * outer_id);
    if(ret != sizeof(uint32))
    {
        return -3;
    }

    if(o_max <= outer_id)
    {
        o_max = outer_id + 1;
    }

    fseek(flag_fp, 0, SEEK_SET);
    fprintf(flag_fp, "%d : %d\n", i_max, o_max);
    fflush(flag_fp);
    return 0;

}

int ts_cm_t::alloc(uint32 & outer_id, uint32 & id)
{

    if(!is_new(outer_id))
    {
        return -1;
    }

    return _alloc(outer_id, id);
}

int ts_cm_t::realloc(uint32 & outer_id, uint32 & id)
{
    if(is_new(outer_id))
    {
        return -1;
    }

    return _alloc(outer_id, id);
}

bool ts_cm_t::is_new(uint32 & outer_id)
{
   // if(outer_id >= o_max)
    if (0 == get_inner_id(outer_id))
    {
        return true;
    }
    return false;
}


uint32 ts_cm_t::get_inner_id(const uint32 outer_id)
{
    uint32 inner_id;
    ssize_t ret =
        pread(o2i_fd, &inner_id, sizeof(uint32), sizeof(uint32) * outer_id);
    if(ret != sizeof(uint32))
    {
        return 0;
    }
    return inner_id;
}

uint32 ts_cm_t::get_outer_id(const uint32 inner_id)
{
    uint32 outer_id;
    ssize_t ret =
        pread(o2i_fd, &outer_id, sizeof(uint32), sizeof(uint32) * inner_id);
    if(ret != sizeof(uint32))
    {
        return 0;
    }
    return outer_id;

}















/* vim: set ts=4 sw=4 sts=4 tw=100 */
