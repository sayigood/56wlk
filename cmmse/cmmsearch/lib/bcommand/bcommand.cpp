/*
 * $Id: bcommand.cpp,v 1.2 2009/01/22 07:32:29 linxb Exp $
 * Description: bcommand function, by liuhua.
 */

#include "bcommand.h"

#include <unistd.h>
#include <ul_log.h>

static FILE *fopen_nb(const char *filename, const char *mode)
{
    FILE *fp = fopen(filename, mode);
    if (NULL == fp) {
        ul_writelog(UL_LOG_WARNING, "fopen_nb : cannot open file [%s] (%m)", filename);
        return NULL;
    }

    setvbuf(fp, (char *)NULL, _IONBF, 0);

    return fp;
}

static bool read_head(FILE *fp, bcommand_t *bc)
{
    if (NULL == fp)
        return false;

    fseek(fp, 0L, SEEK_SET);
	char buf[512];
	const char* fmt= "%d\n%d\n%d\n%s\n%d\n%d\n";
	int ret = fread( buf, 1, sizeof(buf), fp );
	if( ret<=0 )
	{
		ul_writelog(UL_LOG_WARNING, "read head file fail. ret:%d", ret );
		return false;
	}
	if( 6 != sscanf( buf, fmt, &bc->command_count, &bc->cached_command_count,
			&bc->command_file_count, bc->command_cache_path, 
			&bc->cached_command_start_file, &bc->cached_command_start_off ) )
	{
		ul_writelog(UL_LOG_WARNING, "parse readed-headfile-data fail. ");
        bc->command_file_count = 1;
		return false;
	}
	if( bc->command_file_count<=0 )
	{
		ul_writelog(UL_LOG_WARNING, "command_file_count is zero" );
        bc->command_file_count = 1;
		return false;
	}
    return true;
}

static bool write_head(FILE *fp, const bcommand_t *bc)
{
    if (NULL == fp)
        return false;

    fseek(fp, 0L, SEEK_SET);
	char buf[512];
	memset( buf, 0, sizeof(buf) );
	const char* fmt= "%d\n%d\n%d\n%s\n%d\n%d\n";
	snprintf( buf, sizeof(buf), fmt, 
			bc->command_count, bc->cached_command_count,
			bc->command_file_count, bc->command_cache_path, 
			bc->cached_command_start_file, bc->cached_command_start_off );

	int ret = fwrite( buf, sizeof(buf), 1, fp );
	if( ret<=0 )
	{
		ul_writelog(UL_LOG_WARNING, "write head fail. ret:%d", ret );
		return false;
	}
    return true;
}

/* init function */
bcommand_t *InitBCommand(const char *command_path)
{
    char filename[256], dfilename[256];

    /* allocate memory */
    bcommand_t  *bcommand = (bcommand_t*)malloc(sizeof(bcommand_t));
    if (NULL == bcommand) {
        ul_writelog(UL_LOG_WARNING, "InitBCommand : cannot allocate memory for bcommand");
        return NULL;
    }
    bzero(bcommand, sizeof(bcommand_t));

    /* read index */
    snprintf(filename, sizeof(filename) - 1, "%s/%s", command_path, COMMAND_IDX_FILE_NAME);
    filename[sizeof(filename) - 1] = 0;
    snprintf(dfilename, sizeof(dfilename) - 1, "%s/%s0", command_path, COMMAND_DATA_FILE_NAME);
    dfilename[sizeof(dfilename) - 1] = 0;

    if ( 0 != access(filename, F_OK | R_OK | W_OK) ) 
	{ /* first */
        /* no need to read index */
        strncpy(bcommand->command_cache_path,command_path,sizeof(bcommand->command_cache_path)-1);
        bcommand->command_cache_path[sizeof(bcommand->command_cache_path) - 1] = 0;
        /* to create index file */
        bcommand->idx_command_handler = fopen_nb(filename, "w+");
        if (NULL == bcommand->idx_command_handler) {
            ul_writelog(UL_LOG_WARNING, "InitBCommand : cannot create file [%s] for bcommand", 
					filename);
            free(bcommand);
            return NULL;
        }
        /* to create first data file */
        bcommand->current_command_handler = fopen_nb(dfilename, "w+");
        if (NULL == bcommand->current_command_handler) {
            ul_writelog(UL_LOG_WARNING, "InitBCommand : cannot create file [%s] for bcommand", 
					dfilename);
            fclose(bcommand->idx_command_handler);
            free(bcommand);
            return NULL;
        }
        bcommand->command_file_count = 1;
        /* save head */
        write_head(bcommand->idx_command_handler, bcommand);
        /* to return */
        return bcommand;
    }

    /* read index actually */
    FILE *fp = fopen(filename, "r");
    if (NULL == fp) {
        ul_writelog(UL_LOG_WARNING,"InitBCommand:cannot open bcommand index file [%s]",filename);
        free(bcommand);
        return NULL;
    }
    if( false==read_head(fp, bcommand) )
	{
        ul_writelog(UL_LOG_WARNING,"InitBCommand:read_head" );
		fclose(fp);
        free(bcommand);
		return NULL;
	}

    strncpy(bcommand->command_cache_path, command_path, sizeof(bcommand->command_cache_path) - 1);
    bcommand->command_cache_path[sizeof(bcommand->command_cache_path) - 1] = 0;
    fclose(fp);

    /* to open index file */
	if( NULL==bcommand->idx_command_handler )
    	bcommand->idx_command_handler = fopen_nb(filename, "r+");
    if (NULL == bcommand->idx_command_handler) {
        ul_writelog(UL_LOG_WARNING, "InitBCommand : cannot open file [%s] for bcommand", filename);
        free(bcommand);
        return NULL;
    }

    /* open last command file */
    snprintf(dfilename, sizeof(dfilename) - 1, "%s/%s%d", command_path, COMMAND_DATA_FILE_NAME, 
        bcommand->command_file_count - 1);
    dfilename[sizeof(dfilename) - 1] = 0;
	if( NULL==bcommand->current_command_handler )
    	bcommand->current_command_handler = fopen_nb(dfilename,"r+");
    if (NULL == bcommand->current_command_handler) {
        ul_writelog(UL_LOG_WARNING,"InitBCommand:cannot open file[%s]", dfilename);
        fclose(bcommand->idx_command_handler);
        free(bcommand);
        return NULL;
    }

	fseek( bcommand->current_command_handler,0,SEEK_END );
    return bcommand;
}

/* push a command into the command restore */
/* 
 * clen is the buffer length of command
 *
 * command_type and command will be saved in command restore
 *
 * return value >=0  : succeed
 * return value ==-1 : fail
 */
int PushBCommand(bcommand_t *bc, unsigned int command_type,const void *command, int clen,char *buffer, unsigned int size)
{
    int tlen = clen + sizeof(int) * 2;

    if (tlen > (int)size) {
        ul_writelog(UL_LOG_WARNING, "PushBCommand : write buffer is not enough [%d > %ld]", 
				tlen, (unsigned long)size);
        return -1;
    }

    if (ftell(bc->current_command_handler) + tlen > MAX_COMMAND_FILE_SIZE) { 
        /* we need open a new file */
        fclose(bc->current_command_handler);
        bc->current_command_handler = NULL;
        /* create a new data file */
        char filename[256];
        snprintf(filename, sizeof(filename) - 1, "%s/%s%d", bc->command_cache_path, 
            COMMAND_DATA_FILE_NAME, bc->command_file_count++);
        filename[sizeof(filename) - 1] = 0;
        bc->current_command_handler = fopen_nb(filename, "w+");
        if (NULL == bc->current_command_handler) {
            ul_writelog(UL_LOG_WARNING, "PushBCommand : cannot create file [%s] for bcommand", 
					filename);
            return -1;
        }
        /* write head */
        write_head(bc->idx_command_handler, bc);
    }

    memcpy(buffer, &command_type, sizeof(int));
    memcpy(buffer + sizeof(int), &clen, sizeof(int));
    memcpy(buffer + sizeof(int) * 2, command, clen);

	long lastpos = ftell(bc->current_command_handler);
    if( fwrite(buffer, tlen, 1, bc->current_command_handler)<=0 )
	{
		ul_writelog( UL_LOG_WARNING, "PushBCommand: write failed. %m" );
		fseek( bc->current_command_handler, lastpos, SEEK_SET );
	}
    bc->command_count++;
    bc->cached_command_count++;

    if (0 == (bc->command_count % 1000 )) {
        write_head(bc->idx_command_handler, bc);
    }

    return 0;
}

/* traverse all cached command */
/* 
 * traverse_handler is the handler for every command given by TraverseCachedBCommand
 *
 * return value >= 0  : succeed
 * return value == -1 : fail
 */
int TraverseCachedBCommand(bcommand_t *bc, 
    void (*traverse_handler)(int command_type, void *command, int clen, int *stop_now), 
    char *buffer, unsigned int size)
{
    char filename[256];
    char *command = buffer + sizeof(int) * 2;

    if (size <= sizeof(int) * 2) {
        ul_writelog(UL_LOG_WARNING, "Traverse: too small buffer [%ld]", (unsigned long)size);
        return -1;
    }

	int bcmd_count = 1;
    for (int i = bc->cached_command_start_file ; i < bc->command_file_count ; i++) {
        snprintf(filename, sizeof(filename) - 1, "%s/%s%d", bc->command_cache_path, 
            COMMAND_DATA_FILE_NAME, i);
        filename[sizeof(filename) - 1] = 0;
        FILE *fp = fopen(filename, "r");
        if (NULL == fp) {
            ul_writelog(UL_LOG_WARNING,"Traverse. openfile:%s failed.", filename);
            return -1;
        }
        if (i == bc->cached_command_start_file) fseek(fp, bc->cached_command_start_off, SEEK_SET);
        /* read command */
        while(!feof(fp)) {
            if (1 != fread(buffer, sizeof(int), 1, fp)) {
                if (ferror(fp)) {
                    ul_writelog(UL_LOG_WARNING, 
                        "Traverse:cannot read file [%s] for bcommand (1:%m)", filename);
                    fclose(fp);
                    return -1;
                }
                continue;
            }
            if (1 != fread(buffer + sizeof(int), sizeof(int), 1, fp)) {
                if (ferror(fp)) {
                    ul_writelog(UL_LOG_WARNING, 
                        "Traverse: cannot read file [%s] for bcommand (2:%m)", filename);
                    fclose(fp);
                    return -1;
                }
                continue;
            }
            int command_type = *(int*)buffer;
            int clen = *(int*)(buffer + sizeof(int));
            int tlen = clen + sizeof(int) * 2;
            int stop_now = 0;
            if (tlen > (int)size) {
                ul_writelog(UL_LOG_WARNING, 
                    "Traverse: not enough read buffer [%d > %ld], sikp command [%d:%d]", 
                    tlen, (unsigned long)size, command_type, clen);
                fseek(fp, clen, SEEK_CUR);
            }
            if ((size_t)clen != fread(command, 1, clen, fp)) {
                if (ferror(fp)) {
                    ul_writelog(UL_LOG_WARNING, 
                        "Traverse: cannot read file [%s] for bcommand (3:%m)", filename);
                    fclose(fp);
                    return -1;
                }
                continue;
            }
			ul_writelog( UL_LOG_TRACE, "[bcommand] Traverse count_id=%d cmd=%u len=%d", 
					bcmd_count++, command_type, clen );

            /* to handler */
            if (NULL != traverse_handler) {
                traverse_handler(command_type, command, clen, &stop_now);
            }
            if (stop_now) {
                fclose(fp);
                ul_writelog(UL_LOG_WARNING, "Traverse: stop by call hanlder");
                return 0;
            }
        }
        fclose(fp);
    }

    return 0;
}

int TraverseCachedBCommandEx(bcommand_t *bc, 
    void (*traverse_handler)(int command_type, void *command, int clen, int *stop_now, void *arg),
    void *ex_arg, char *buffer, unsigned int size)
{
    char filename[256];
    char *command = buffer + sizeof(int) * 2;

    if (size <= sizeof(int) * 2) {
        ul_writelog(UL_LOG_WARNING, "Traverse: too small buffer [%ld]", (unsigned long)size);
        return -1;
    }

    for (int i = bc->cached_command_start_file ; i < bc->command_file_count ; i++) {
        snprintf(filename, sizeof(filename) - 1, "%s/%s%d", bc->command_cache_path, 
            COMMAND_DATA_FILE_NAME, i);
        filename[sizeof(filename) - 1] = 0;
        FILE *fp = fopen(filename, "r");
        if (NULL == fp) {
            ul_writelog(UL_LOG_WARNING, "Traverse: cannot open file [%s] for bcommand",
                filename);
            return -1;
        }
        if (i == bc->cached_command_start_file) fseek(fp, bc->cached_command_start_off, SEEK_SET);
        /* read command */
        while(!feof(fp)) {
            if (1 != fread(buffer, sizeof(int), 1, fp)) {
                if (ferror(fp)) {
                    ul_writelog(UL_LOG_WARNING, 
                        "Traverse: cannot read file [%s] for bcommand (1:%m)", filename);
                    fclose(fp);
                    return -1;
                }
                continue;
            }
            if (1 != fread(buffer + sizeof(int), sizeof(int), 1, fp)) {
                if (ferror(fp)) {
                    ul_writelog(UL_LOG_WARNING, 
                        "Traverse: cannot read file [%s] for bcommand (2:%m)", filename);
                    fclose(fp);
                    return -1;
                }
                continue;
            }
            int command_type = *(int*)buffer;
            int clen = *(int*)(buffer + sizeof(int));
            int tlen = clen + sizeof(int) * 2;
            int stop_now = 0;
            if (tlen > (int)size) {
                ul_writelog(UL_LOG_WARNING, 
                    "Traverse: not enough read buffer [%d > %ld], sikp command [%d:%d]", 
                    tlen, (unsigned long)size, command_type, clen);
                fseek(fp, clen, SEEK_CUR);
            }
            if ((size_t)clen != fread(command, 1, clen, fp)) {
                if (ferror(fp)) {
                    ul_writelog(UL_LOG_WARNING, 
                        "Traverse: cannot read file [%s] for bcommand (3:%m)", filename);
                    fclose(fp);
                    return -1;
                }
                continue;
            }
            /* to handler */
            if (NULL != traverse_handler) {
                traverse_handler(command_type, command, clen, &stop_now, ex_arg);
            }
            if (stop_now) {
                fclose(fp);
                ul_writelog(UL_LOG_WARNING, "Traverse: stop by call hanlder");
                return 0;
            }
        }
        fclose(fp);
    }

    return 0;

}

/* get the last bcommand position */
/*
 * return value >= 0  : succeed
 * return value == -1 : fail
 */
int GetLastBCommandPos(bcommand_t *bc, bc_pos_t *pos)
{
    pos->file_no = bc->command_file_count - 1;
    pos->file_off = ftell(bc->current_command_handler);

    return 0;
}

/* modify the cached bcommand position */
/*
 * command_pos MUST be the return value of GetLastBCommandPos
 */
int SetCachedBCommandPos(bcommand_t *bc, const bc_pos_t *pos)
{
    bc->cached_command_start_file = pos->file_no;
    bc->cached_command_start_off = pos->file_off;

    write_head(bc->idx_command_handler, bc);

    return 0;
}
