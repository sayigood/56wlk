#include <stdio.h>
#include <stdlib.h>
#include "ul_log.h"
#include "ul_ccode.h"
//ccode sharemeory info
typedef struct _ccodeshminfo_t
{
	    int ccode_check_exit[MAX_CHECK_EXIT];
		int exit_urlnone, exit_short, exit_eng2gb, exit_gbbig52eng,
			exit_utf82none, exit_none2eng, exit_remain, exit_utf82eng;
}ccShmInfo_t;

/************ global function declare ****************/
int	ccode_init_shm ();
int ccode_log_addval (int type, int value);
