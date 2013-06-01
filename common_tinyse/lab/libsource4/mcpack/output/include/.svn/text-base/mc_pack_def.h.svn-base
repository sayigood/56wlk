/***************************************************************************
 *
 *
 **************************************************************************/



/**
 * @file mc_pack_def.h
 *
 **/


#ifndef  __MC_PACK_DEF_H_
#define  __MC_PACK_DEF_H_

#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif
    typedef char mc_int8_t;
    typedef short mc_int16_t;
    typedef int mc_int32_t;
    typedef long long mc_int64_t;

    typedef unsigned char mc_uint8_t;
    typedef unsigned short mc_uint16_t;
    typedef unsigned int mc_uint32_t;

    typedef unsigned long long mc_uint64_t;

#ifndef __cplusplus
    typedef char mc_bool_t;
#define MC_TRUE 1
#define MC_FALSE 0
#else
    typedef bool mc_bool_t;
#define MC_TRUE true
#define MC_FALSE false
#endif

    enum mc_pack_version {
        VERSION_1 = 1,
        VERSION_2 = 2
    };

    enum mc_pack_error_t {
        MC_PE_SUCCESS = 0,			  /**< 成功       */
        MC_PE_NOT_FOUND = -1,		  /**< 未找到       */
        MC_PE_BAD_PARAM = -2,		  /**< 错误的参数，空指针等       */
        MC_PE_BAD_NAME = -3,		  /**< 名字不合法       */
        MC_PE_BAD_TYPE = -4,		  /**< 错误的类型       */
        MC_PE_NO_SPACE = -5,		  /**< 空间不足       */
        MC_PE_NAME_TOO_LONG = -6,	  /**< 名字太长       */
        MC_PE_ALREADY_EXIST = -7,	  /**< 已存在       */
        MC_PE_BAD_DATA = -8,		  /**< 数据错误       */
        MC_PE_NULL_POINTER = -9,	  /**< 空指针       */
        MC_PE_FOUND_BUT_TYPE = -10,	  /**< 空指针       */
        MC_PE_NO_TEMP_SPACE = -11,    /**< 临时缓冲区耗尽       */
        MC_PE_WRONG_OPEN_MODE = -12,   /**< 在读/写模式下，操作无效       */
        MC_PE_WRONG_PACK_TYPE = -13,  /**< 由于PACK的数组/对象类型，操作无效       */
        MC_PE_PROTOCOL_NOT_SUPPOT_OPERATION = -14,  /**< 底层协议不支持该操作       */
        MC_PE_SYSTEM_ERROR = -15,  /**< 底层协议不支持该操作       */
        MC_PE_NULL_VALUE = -16,    /**< 字段的值为null */
        MC_PE_UNKNOWN = -255		  /**< 未知错误       */
    };

    enum mc_v1pack_type_t {
        /* pack type       */
        MC_PT_BAD = 0x00,		  /**< 非法       */
        MC_PT_PCK = 0x01,		  /**< pack       */
        MC_PT_OBJ = 0x02,		  /**< object       */
        MC_PT_ARR = 0x04,		  /**< array       */

        /* item type       */
        MC_IT_UNKNOWN = 0x05,		  /**< 未知类型，可能使用了版本更高的协议 */
        MC_IT_BAD = 0x00,		  /**< 非法       */
        MC_IT_BIN = 0x10,		  /**< 二进制       */
        MC_IT_TXT = 0x20,		  /**< 文本       */

        MC_IT_SGN = 0x11,		  /**< 有符号       */
        MC_IT_UNS = 0x12,		  /**< 无符号       */
        MC_IT_32B = 0x14,		  /**< 32位       */
        MC_IT_64B = 0x18,		  /**< 64位       */

        MC_IT_BOOL = 0x30,       /**< BOOL类型   */
        MC_IT_NULL = 0x40,       /**< NULL值     */
        MC_IT_FLOAT = 0x50,      /**< 4字节浮点  */
        MC_IT_DOUBLE = 0x51,     /**< 8字节浮点　*/

        MC_IT_I32 = MC_IT_SGN | MC_IT_32B,		  /**< int32       */
        MC_IT_U32 = MC_IT_UNS | MC_IT_32B,		  /**< uint32       */
        MC_IT_I64 = MC_IT_SGN | MC_IT_64B,		  /**< int64       */
        MC_IT_U64 = MC_IT_UNS | MC_IT_64B		  /**< uint64       */
    };

    enum mc_pack_type_id {
        MCPACKV2_INVALID = 0x0,
        MCPACKV2_OBJECT = 0x10,
        MCPACKV2_ARRAY = 0x20,
        MCPACKV2_STRING = 0x50,
        MCPACKV2_RAW = 0x60,
        MCPACKV2_INT_8 = 0x11,
        MCPACKV2_INT_16 = 0x12,
        MCPACKV2_INT_32 = 0x14,
        MCPACKV2_INT_64 = 0x18,
        MCPACKV2_UINT_8 = 0x21,
        MCPACKV2_UINT_16 = 0x22,
        MCPACKV2_UINT_32 = 0x24,
        MCPACKV2_UINT_64 = 0x28,
        MCPACKV2_BOOL = 0x31,
        MCPACKV2_FLOAT = 0x44,
        MCPACKV2_DOUBLE = 0x48,
        MCPACKV2_DATE = 0x58,
        MCPACKV2_NULL = 0x61,
        MCPACKV2_SHORT_ITEM = 0x80,
        MCPACKV2_FIXED_ITEM = 0xf,
        MCPACKV2_DELETED_ITEM = 0x70
    };

    typedef struct {} mc_pack_t;
    typedef struct {
        const mc_pack_t * pack; /* 内部使用 */
        char * internal;      /* 内部使用 */
        int type;			  /**< see mc_pack_type_t       */
        size_t value_size;	  /**< value 数据的大小       */
        const char *key;			  /**< 指向字段名的指针       */
        const char *value;		  /**< 指向数据的指针       */
    } mc_pack_item_t;

    typedef struct {                /**< 全路径key */
        char *buf;                  /**< 字符串缓冲区 */
        int buflen;                 /**< 字符串长度 */
        int bufsize;                /**< 字符串缓冲区长度 */
    } full_key_t;


#define MC_PACK_OBJDELIM ','
#define MC_PACK_ARRDELIM ':'
#define MC_PACK_FIELD_NAME_LEN_MAX (1024)




#ifdef __cplusplus
}
#endif









#endif  //__MC_PACK_DEF_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
