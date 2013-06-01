/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: bk.cpp,v 1.2 2009/01/22 07:32:28 linxb Exp $ 
 * 
 **************************************************************************/



/**
 * @file bk.cpp
 * @author yanghy(yanghuaiyuan@baidu.com)
 * @date 2008/06/25 17:05:14
 * @version $Revision: 1.2 $ 
 * @brief 
 *  
 **/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bcommand.h"
#include "lock.h"
#include "tinyse.h"
#define BK_SUFFIX_DIR ".bk"

int ts_bk_t::init(const char *path, int cmd_maxsize)
{
    bcmd = InitBCommand(path);
    if(NULL == bcmd)
    {
        return -1;
    }

    bcmd_bufsize = cmd_maxsize;
    bcmd_buf = new char[bcmd_bufsize];
    if(NULL == bcmd_buf)
    {
        return -2;
    }
    last_bpos = new _bc_pos_t[1];
    if(NULL == last_bpos)
    {
        return -3;
    }
    pthread_mutex_init(&mutex_lock, NULL);
    return GetLastBCommandPos((bcommand_t *) bcmd, (_bc_pos_t *) last_bpos);
}
int ts_bk_t::init(const char *dir, const char *file, int cmd_maxsize)
{
    char path[TS_FILEPATH_MAXSIZE];
    snprintf(path, sizeof(path), "%s/%s%s", dir, file, BK_SUFFIX_DIR);
    bcmd = InitBCommand(path);
    if(NULL == bcmd)
    {
        return -1;
    }

    bcmd_bufsize = cmd_maxsize;
    bcmd_buf = new char[bcmd_bufsize];
    if(NULL == bcmd_buf)
    {
        return -2;
    }
    last_bpos = new _bc_pos_t[1];
    if(NULL == last_bpos)
    {
        return -3;
    }
    pthread_mutex_init(&mutex_lock, NULL);
    return GetLastBCommandPos((bcommand_t *) bcmd, (_bc_pos_t *) last_bpos);
}

void ts_bk_t::push_cmd(uint32 command_type, const void *cmd, uint32 size)
{

    ans::lock _lock(&mutex_lock);
    PushBCommand((bcommand_t *) bcmd, command_type, cmd, (int)size,
                 bcmd_buf, bcmd_bufsize);
}

int ts_bk_t::_chk_and_open_file()
{

    bcommand_t *bc = (bcommand_t *) bcmd;
    char filename[TS_FILEPATH_MAXSIZE];
    for(; cur_file < bc->command_file_count; cur_file++)
    {
        snprintf(filename, sizeof(filename) - 1, "%s/%s%d",
                 bc->command_cache_path, COMMAND_DATA_FILE_NAME, cur_file);
        filename[sizeof(filename) - 1] = 0;
        _fp = fopen(filename, "r");
        if(NULL == _fp)
        {
            return -1;
        }


        if(cur_file == bc->cached_command_start_file)
        {
            fseek(_fp, bc->cached_command_start_off, SEEK_SET);
        }

        struct stat fs;
        if(0 == fstat(fileno(_fp), &fs))
            _curfile_size = fs.st_size;


        if(!feof(_fp) && ftell(_fp) != _curfile_size)
        {
            break;
        }

        fclose(_fp);
        _fp = NULL;
    }
    if(bc->command_file_count == cur_file || _fp == NULL || feof(_fp))
    {
        is_end = true;
    }
    return 0;
}

int ts_bk_t::get_cmd_init()
{
    if((uint32) bcmd_bufsize <= sizeof(int) * 2)
    {
        return -1;
    }
    bcommand_t *bc = (bcommand_t *) bcmd;
    cur_file = bc->cached_command_start_file;
    is_end = false;
    return _chk_and_open_file();
}


int ts_bk_t::get_cmd_and_next(char **cmd, uint32 *command_type, uint32 * size)
{
    if(is_end)
        return -1;

    char *command = bcmd_buf + sizeof(int) * 2;


    while(!feof(_fp))
    {

        if(1 != fread(bcmd_buf, sizeof(int), 1, _fp))
        {
            if(ferror(_fp))
            {
                fclose(_fp);
                _fp = NULL;
                return -1;
            }
            continue;
        }
        if(1 != fread(bcmd_buf + sizeof(int), sizeof(int), 1, _fp))
        {
            if(ferror(_fp))
            {
                fclose(_fp);
                _fp = NULL;
                return -1;
            }
            continue;
        }
        *command_type = *(int*)bcmd_buf;
        int clen = *(int *)(bcmd_buf + sizeof(int));
        int tlen = clen + sizeof(int) * 2;

        if(tlen > (int)bcmd_bufsize)
        {
            fseek(_fp, clen, SEEK_CUR);
            continue;   ///Ã¯π˝’‚Ãı√¸¡Ó
        }
        if((size_t) clen != fread(command, 1, clen, _fp))
        {
            if(ferror(_fp))
            {
                fclose(_fp);
                _fp = NULL;
                return -1;
            }
            continue;
        }
        *cmd = command;
        *size = clen;
        break;

    }

    if(ftell(_fp) == _curfile_size || feof(_fp))
    {
        fclose(_fp);
        _fp = NULL;

        cur_file++;
        return _chk_and_open_file();
    }


    return 0;
}


int ts_bk_t::get_cmd_fini()
{
    if(NULL != _fp)
        fclose(_fp);
    _fp = NULL;
    return 0;
}

bool ts_bk_t::get_cmd_is_end()
{
    return is_end;
}


int ts_bk_t::set_bkup_pos()
{
    ans::lock _lock(&mutex_lock);
    return SetCachedBCommandPos((bcommand_t *) bcmd, (_bc_pos_t *) last_bpos);
}

int ts_bk_t::get_bkup_pos()
{
    ans::lock _lock(&mutex_lock);
    return GetLastBCommandPos((bcommand_t *) bcmd, (_bc_pos_t *) last_bpos);
}









/* vim: set ts=4 sw=4 sts=4 tw=100 */
