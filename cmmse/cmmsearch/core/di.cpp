/**
 * @file di.cpp
 * @author 
 * @date 2013.01.06 09:50:23
 * @version $Revision: 1.1 $ 
 * @brief di数据读写
 *  
 **/
#include <stdio.h>
#include <stdlib.h>
#include "fileblock.h"
#include "mfiles.h"
#include "tinyse.h"
#define DI1_SUFFIX_FILE ".di_idx"
#define DI2_SUFFIX_FILE ".di_data"
#pragma pack(4)
typedef struct _di_idx_t
{
    long long offset;
    uint32 size;
} di_idx_t;
#pragma pack()
int ts_di_t::init(uint32 data_maxsize, const char *dir, const char *file)
{
    int opret = 0;
    char path[TS_FILEPATH_MAXSIZE];
    snprintf(path, sizeof(path), "%s/%s%s", dir, file, DI1_SUFFIX_FILE);

    fb_handle = new fblock_t[1];
    if(NULL == fb_handle)
    {
        return -1;
    }
    opret =
        fblock_init((fblock_t *) fb_handle, file, path, sizeof(di_idx_t),
                    true, false);
    if(opret < 0)
    {
        return -2;
    }


    snprintf(path, sizeof(path), "%s/%s%s", dir, file, DI2_SUFFIX_FILE);
    mf_handle = new mfiles_t[1];
    if(NULL == mf_handle)
    {
        return -3;
    }

    opret =
        mfiles_init((mfiles_t *) mf_handle, file, path, MF_PER_FILE_MAX_LEN,
                    true, false);
    if(opret < 0)
    {
        return -4;
    }


    return 0;
}
int ts_di_t::write(uint32 id, const void *data, uint32 size)
{
    int opret = 0;

    di_idx_t idx = { ((mfiles_t *) mf_handle)->_cur_logiclen, size };

    opret = mfiles_append((mfiles_t *) mf_handle, data, size);
    if((uint32) opret != size)
    {
        return -1;
    }

    opret = fblock_write((fblock_t *) fb_handle, id, &idx, sizeof(idx));
    if(0 != opret)
    {
        return -2;
    }
    return 0;
}
int ts_di_t::read(uint32 id, void *data, uint32 & size)
{
    int opret = 0;
    di_idx_t idx;
    opret = fblock_read((fblock_t *) fb_handle, id, &idx, sizeof(idx));
    if(0 != opret)
    {
        return -1;
    }
    size = idx.size;
    opret = mfiles_pread((mfiles_t *) mf_handle, data, size, idx.offset);
    if((uint32) opret != size)
    {
        return -2;
    }

    return 0;
};


















/* vim: set ts=4 sw=4 sts=4 tw=100 */
