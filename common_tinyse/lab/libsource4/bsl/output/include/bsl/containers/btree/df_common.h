////====================================================================
//
// df_common.h - Pyramid / DFS / df-lib
//
//
//
// -------------------------------------------------------------------
//
// Description
//
//    collection of declaration in common lib
//
// -------------------------------------------------------------------
//
// Change Log
//
//
////====================================================================

#ifndef __DF_COMMON_H__
#define __DF_COMMON_H__

// Proclaim we are using SMP
#ifndef CONFIG_SMP
# define CONFIG_SMP
#endif

// Include header of df_def
#include "df_def.h"

// Include header of df_log
#include "df_log.h"

// Include header of atomic operator
#include "df_atomic.h"

// Include header of bit operator
#include "df_bitops.h"

#define DPF_TMP_DIR_OPTION "dpf_tmp_dir"
#define DPF_TMP_GRP_OPTION "dpf_tmp_grp"
#define DCS_LISTEN_PORT    "dcs_listenport"
#define DCS_CARRIER_PORT   "dcs_carrierport"

#define DCS_TABLE_NAME     "dcs_log"

#define DPF_AGENT_PORT      15801
#define DPF_CARRIER_PORT    15802

#define MAX_DFS_MASTER_PORT 15811
#define MIN_DFS_MASTER_PORT 15811

#define MAX_DCS_MASTER_PORT 15999
#define MIN_DCS_MASTER_PORT 15900
const int DCS_PORT_SCAN_INTERVAL = 3;

#define MAX_DTS_MASTER_PORT 15831
#define MIN_DTS_MASTER_PORT 15831

#define MAX_DLS_MASTER_PORT 15851
#define MIN_DLS_MASTER_PORT 15851

#define DPF_INVALID_PORT (-1)

// Definition of functions
extern void df_lib_printversion(void);

/* // 取得本地网卡信息 */
/* // @param(in/out) ip_buffer - 存放ip地址的缓冲区 */
/* // @param(in)   buffer_size - 缓冲区大小 */
/* // @param(out)  data_size   - 实际返回大小 */
/* // @return */
/* //        0 - success */
/* //        -1 - failure */
/* extern int get_local_netinfo(int* ip_buffer, int buffer_size, int& ret_size); */

/* // 根据ip值得到点分十进制的ip地址（线程安全） */
/* // @param(in) ip - 本地字节序的ip值 */
/* // @return */
/* //         非空   - 点分十进制的ip地址 */
/* //         NULL  - 错误 */
/* extern const char* get_addr_from_ip(int ip); */

/* // 从固定的端口范围内扫描得到可用的端口 */
/* // @param(in)  begin_port -- 默认初始扫描端口号 */
/* // @param(out) port -- 可用的端口号 */
/* // @return */
/* //          >=0  绑定到该端口的udp连接 */
/* //          -1  未找到可用的端口 */
/* extern int find_dcs_valid_port(int begin_port, int& port); */

/* #ifdef __x86_64__ */
/* // 取得本地processor个数 */
/* extern int get_processor_number(); */
/* #endif */

#endif // #ifndef __DF_COMMON_H__
