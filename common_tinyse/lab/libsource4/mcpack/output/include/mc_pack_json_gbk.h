/***************************************************************************
 *
 *
 **************************************************************************/



/**
 * @file mc_pack_text.h
 * @brief pack text description
 *
 **/


#ifndef  __MC_PACK_JSON_GBK_H_
#define  __MC_PACK_JSON_GBK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mc_pack_c.h"

    /**
     * @brief
     * 按照gbk格式处理
     * @param [in/out] txt   : const char*
     * @param [in/out] pack   : mc_pack_t*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_pack_json2pack_gbk(const char *txt, mc_pack_t *pack);

    /**
     * @brief
     * 按照gbk格式处理
     * @param [in/out] pack   : const mc_pack_t*
     * @param [in/out] txt_buf   : char*
     * @param [in/out] buf_size   : u_int
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_pack_pack2json_gbk(const mc_pack_t *pack, char *txt_buf, u_int buf_size);

#ifdef __cplusplus
}
#endif

#endif  //__MC_PACK_TEXT_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
