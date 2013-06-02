#ifndef __UB_PROCCMD_H__
#define __UB_PROCCMD_H__

#include "ul_def.h"
#include "nshead.h"
#include "ub_log.h"

///命令处理函数的类型
typedef int (*proc_cmd_callback_t) (int, nshead_t*, ub_buff_t*, nshead_t*, ub_buff_t*);


///命令和处理函数之间的对应关系表
typedef struct _ub_cmd_map_t {
	int                  cmd_no;            /**< 命令号                 */
	proc_cmd_callback_t  proc;              /**< 命令号对应的处理函数   */
} ub_cmd_map_t;


static inline int ub_process_cmdmap(const ub_cmd_map_t *cmd_map, int cmd_no, nshead_t *req_head, ub_buff_t *req_buf,
		nshead_t *res_head, ub_buff_t *res_buf)
{
	//proc_cmd_callback_t * callback = NULL;

	while (cmd_map->proc) {
		if (cmd_map->cmd_no==cmd_no) {
			return cmd_map->proc(cmd_no, req_head, req_buf, res_head, res_buf);
		}
		cmd_map++;
	}
	UB_LOG_WARNING("unsupport [cmd_no:%d] received!", cmd_no);

	return -1;
}

#endif

