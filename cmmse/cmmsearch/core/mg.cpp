/**
 * @file mg.cpp
 * @author
 * @date 2013.01.06 09:31:49
 * @version $Revision: 1.1 $ 
 * @brief brief数据读写
 *  
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tinyse.h"
#define MG_SUFFIX_FILE ".mg"
int ts_mg_t::init(uint32 item_size, uint32 item_num, const char *dir,
                  const char *file)
{

    _size = item_size;
    _num = item_num;

    char path[TS_FILEPATH_MAXSIZE];
    snprintf(path, sizeof(path), "%s/%s%s", dir, file, MG_SUFFIX_FILE);
    int fd = open(path, O_CREAT | O_RDWR, 0664);
    if(fd < 0)
    {
        return -1;
    }

    size_t dsize = (size_t) _size * _num;
    lseek(fd, dsize - 1, SEEK_SET);
    ::write(fd, "", 1);
    _array =
        (char *)mmap(NULL, dsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(_array == MAP_FAILED)
    {
        return -2;
    }



    return 0;
}
void ts_mg_t::write(uint32 id, const void *brief)
{
    if(id >= _num)
        return;
    memcpy(_array + id * _size, brief, _size);
}
void ts_mg_t::read(uint32 id, void *brief)
{
    if(id >= _num)
        return;
    memcpy(brief, _array + id * _size, _size);
}

int ts_mg_t::max_num()
{
    return _num;
};

















/* vim: set ts=4 sw=4 sts=4 tw=100 */
