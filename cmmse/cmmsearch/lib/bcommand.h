/*
 * $Id: bcommand.h,v 1.2 2009/01/22 07:32:29 linxb Exp $
 * Description: header file of bcommand,by liuhua.
 */

#ifndef __BCOMMAND_H__
#define __BCOMMAND_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_COMMAND_FILE_SIZE       (1500*1024*1024)         /* 1500M */
#define COMMAND_IDX_FILE_NAME       "bcommand.idx"
#define COMMAND_DATA_FILE_NAME      "bcommand."

/* bcommand structure */
typedef struct _bcommand_t {
    int   command_count;             /* command count */
    int   cached_command_count;      /* unsaved command count */
    int   command_file_count;        /* command file count */
    char  command_cache_path[256];   /* path of command file */
    int   cached_command_start_file; /* cached command start file no */
    int   cached_command_start_off;  /* offset of first cached command in start file */

// 下面这个变量用于修订在死机情况下的半条命令.
//	int   lastfile_save_pos;         /** 最后一个文件的最近开始修订半条命令的位置 */

    FILE  *idx_command_handler;      /* index file handler */
    FILE  *current_command_handler;  /* current command data file handler (the last file) */
}   bcommand_t;

typedef struct _bc_pos_t {
    int file_no;
    int file_off;
}   bc_pos_t;

/* init function */
bcommand_t *InitBCommand(const char *command_path);

/* push a command into the command restore */
/* 
 * clen is the buffer length of command
 *
 * command_type and command will be saved in command restore
 *
 * return value >=0  : succeed
 * return value ==-1 : fail
 */
int PushBCommand(bcommand_t *bc, unsigned int command_type, const void *command, int clen, char *buffer, unsigned int size);

/* traverse all cached command */
/* 
 * traverse_handler is the handler for every command given by TraverseCachedBCommand
 *
 * return value >= 0  : succeed
 * return value == -1 : fail
 */
int TraverseCachedBCommand(bcommand_t *bc,
    void (*traverse_handler)(int command_type, void *command, int clen, int *stop_now), 
    char *buffer, unsigned int size);
int TraverseCachedBCommandEx(bcommand_t *bc, 
    void (*traverse_handler)(int command_type, void *command, int clen, int *stop_now, void *arg),
    void *ex_arg, char *buffer, unsigned int size);

/* get the last bcommand position */
/*
 * return value >= 0  : succeed
 * return value == -1 : fail
 */
int GetLastBCommandPos(bcommand_t *bc, bc_pos_t *pos);

/* modify the cached bcommand position */
/*
 * command_pos MUST be the return value of GetLastBCommandPos
 */
int SetCachedBCommandPos(bcommand_t *bc, const bc_pos_t *pos);

#endif // __BCOMMAND_H__
