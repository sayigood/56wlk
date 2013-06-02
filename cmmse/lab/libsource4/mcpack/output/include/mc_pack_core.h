/***************************************************************************
 *
 *
 **************************************************************************/



/**
 * @file mc_v1pack_core.h
 * @brief mc_v1pack core api
 *
 **/
#ifndef __MC_PACK_CORE_H
#define __MC_PACK_CORE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <sys/types.h>
#include "mc_pack_def.h"
#define MC_PACK_VERSION (0x0000)




    /**
    * @brief mc_pack_item_t
    *   用于遍历的接口
    *   当 type 为 array/object 时，value_size 保存的是 item_count
    *   key 为名字，value 为指向 array/object 的指针
    *
    */

    typedef struct _mc_v1pack_t {
        char tag[4];
        u_int item_count;
        u_int bytes_left;
        u_int cur_pos;
        u_int internal_item_count;
        u_short version;
        u_short reserved;
        char data[0];
    } __attribute__((packed)) mc_v1pack_t;

    /**
     * @brief init a pack
     *		在指定的缓冲区上初始化一个 datapack
     *
     * @param [in] buf   : char*
     *		缓冲区
     * @param [in] buf_size   : u_int
     *		缓冲区大小，应至少比 sizeof(mc_v1pack_t) 大
     * @return  mc_v1pack_t*
     *		指向 pack 结构的指针
     * @retval MC_PACK_PTR_ERR(retval)
     * 		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    mc_v1pack_t *mc_v1pack_create(char *buf, u_int buf_size);

    /**
     * @brief get pack size
     *		取得 pack 的大小，只适用于 pack，不适用于 array/object
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 结构的指针
     * @return  int
     * @retval errno
     *		- >0 : pack size
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_size(const mc_v1pack_t * ppack);

    /**
     * @brief get pack type
     *		取得 pack 类型
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 结构的指针
     * @return  int
     * @retval
     *		- MC_PT_BAD
     *		- MC_PT_OBJ
     *		- MC_PT_ARR
     * @see
     * @note
    **/
    int mc_v1pack_get_type(const mc_v1pack_t * ppack);

    /**
     * @brief get item count of pack
     *		取得 pack 的 item count，可用于 array/object
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 结构的指针
     * @return  int
     * @retval errno
     *		- >=0 : item count
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_item_count(const mc_v1pack_t * ppack);

    /**
     * @brief init a array item in a pack
     *		在一个 pack 中创建一个数组型的 item
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		datapack 指针
     * @param [in] name   : const char*
     *		数组名，ppack 为数组时 name 必须为 NULL
     * @return  mc_v1pack_t*
     *		指向 array 的指针
     * @retval MC_PACK_PTR_ERR(retval)
     * 		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    mc_v1pack_t *mc_v1pack_put_array(mc_v1pack_t * ppack, const char *name);

    /**
     * @brief get array from pack
     *		从 pack 中取得 array 的指针
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		datapack 指针
     * @param [in] name   : const char*
     *		数组名
     * @return  mc_v1pack_t*
     *		指向 array 的指针
     * @retval MC_PACK_PTR_ERR(retval)
     * 		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    mc_v1pack_t *mc_v1pack_get_array(const mc_v1pack_t * ppack, const char *name);

    /**
     * @brief get array from array
     *		从 array 中取得子 array 的指针
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		array 指针
     * @param [in] arr_index   : u_int
     * @return  mc_v1pack_t*
     *		指向 array 的指针
     * @retval MC_PACK_PTR_ERR(retval)
     * 		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    mc_v1pack_t *mc_v1pack_get_array_arr(const mc_v1pack_t * ppack, u_int arr_index);

    /**
     * @brief init a object item in a pack
     *		在一个 pack 中创建一个结构类型的 item
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		datapack 指针
     * @param [in] name   : const char*
     *		结构名，ppack 为数组时 name 必须为 NULL
     * @return  mc_v1pack_t*
     *		指向 object 的指针
     * @retval MC_PACK_PTR_ERR(retval)
     * 		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    mc_v1pack_t *mc_v1pack_put_object(mc_v1pack_t * ppack, const char *name);

    /**
     * @brief get object from pack
     *		从 pack 中取得 object 的指针
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		datapack 指针
     * @param [in] name   : const char*
     *		结构名
     * @return  mc_v1pack_t*
     *		指向 object 的指针
     * @retval MC_PACK_PTR_ERR(retval)
     * 		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    mc_v1pack_t *mc_v1pack_get_object(const mc_v1pack_t * ppack, const char *name);

    /**
     * @brief get object from array
     *		从 array 中取得 object 的指针
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		array 指针
     * @param [in] arr_index   : u_int
     * @return  mc_v1pack_t*
     *		指向 object 的指针
     * @retval MC_PACK_PTR_ERR(retval)
     * 		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    mc_v1pack_t *mc_v1pack_get_object_arr(const mc_v1pack_t * ppack, u_int arr_index);

    /**
     * @brief check if a pointer is valid
     *		判断一个指针指向的是否为合法的 mc_v1pack_t 或 array/object mc_pack_item_t
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 或 array/object 的指针
     * @param [in] buf_size   : u_int
     * @return  int
     * @retval validation
     *		- 1 : valid
     *		- 0 : invalid
     * @see
     * @note
    **/
    int mc_v1pack_valid(const mc_v1pack_t * ppack, u_int buf_size);

    /**
     * @brief add int32 into pack
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		指向 pack 或 object/array 的指针
     * @param [in] name   : const char*
     *		字段名，ppack 为数组时 name 必须为 NULL
     * @param [in] value   : mc_int32_t
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_put_int32(mc_v1pack_t * ppack, const char *name, mc_int32_t value);

    /**
     * @brief
     * 放入一个32位浮点数
     * @param [in/out] ppack   : mc_v1pack_t*
     * @param [in/out] name   : const char*
     * @param [in/out] value   : float
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_put_float(mc_v1pack_t * ppack, const char *name, float value);

    /**
     * @brief
     * 从对象取出一个32位浮点数
     * @param [in/out] ppack   : const mc_v1pack_t*
     * @param [in/out] name   : const char*
     * @param [in/out] result   : float*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_get_float(const mc_v1pack_t * ppack, const char *name, float * result);

    /**
     * @brief
     * 从数组中取出浮点数
     * @param [in/out] ppack   : const mc_v1pack_t*
     * @param [in/out] arr_index   : u_int
     * @param [in/out] result   : float*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_get_float_arr(const mc_v1pack_t * ppack, u_int arr_index, float * result);

    /**
     * @brief
     * 放入一个64位浮点数
     * @param [in/out] ppack   : mc_v1pack_t*
     * @param [in/out] name   : const char*
     * @param [in/out] value   : double
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_put_double(mc_v1pack_t * ppack, const char *name, double value);

    /**
     * @brief
     * 从对象中取出64位浮点数
     * @param [in/out] ppack   : const mc_v1pack_t*
     * @param [in/out] name   : const char*
     * @param [in/out] result   : double*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_get_double(const mc_v1pack_t * ppack, const char *name, double * result);

    /**
     * @brief
     * 从数组中取出浮点数
     * @param [in/out] ppack   : const mc_v1pack_t*
     * @param [in/out] arr_index   : u_int
     * @param [in/out] result   : double*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_get_double_arr(const mc_v1pack_t * ppack, u_int arr_index, double * result);

    /**
     * @brief
     * 放入一个bool值，1字节长。注意读取时会忽略具体数值，仅保留bool值
     * @param [in/out] ppack   : mc_v1pack_t*
     * @param [in/out] name   : const char*
     * @param [in/out] value   : mc_bool_t
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_put_bool(mc_v1pack_t * ppack, const char *name, mc_bool_t value);

    /**
     * @brief
     * 从对象中读取bool值
     * @param [in/out] ppack   : const mc_v1pack_t*
     * @param [in/out] name   : const char*
     * @param [in/out] value   : mc_bool_t*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_get_bool(const mc_v1pack_t * ppack, const char *name, mc_bool_t * value);

    /**
     * @brief
     * 从数组中读取bool值
     * @param [in/out] ppack   : const mc_v1pack_t*
     * @param [in/out] arr_index   : u_int
     * @param [in/out] value   : mc_bool_t*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_get_bool_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_bool_t * value);

    /**
     * @brief
     * 放入一个空值
     * @param [in/out] ppack   : mc_v1pack_t*
     * @param [in/out] name   : const char*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_put_null(mc_v1pack_t * ppack, const char *name);

    /**
     * @brief
     * 判断一个对象的字段是否为空
     * @param [in/out] ppack   : const mc_v1pack_t*
     * @param [in/out] name   : const char*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_is_null(const mc_v1pack_t * ppack, const char *name);

    /**
     * @brief
     * 判断数组的一个成员是否为空
     * @param [in/out] ppack   : const mc_v1pack_t*
     * @param [in/out] arr_index   : u_int
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_is_null_arr(const mc_v1pack_t * ppack, u_int arr_index);

    /**
     * @brief
     * 从item中取出float值，会自动做类型转换，从double转为float时不判断范围
     * @param [in/out] pitem   : const mc_pack_item_t*
     * @param [in/out] res   : float*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_get_float_from_item(const mc_pack_item_t *pitem, float *res);

    /**
     * @brief
     * 从item中取出double值
     * @param [in/out] pitem   : const mc_pack_item_t*
     * @param [in/out] res   : double*
     * @return  int
     * @retval
     * @see
     * @note
    **/
    int mc_v1pack_get_double_from_item(const mc_pack_item_t *pitem, double *res);

    int mc_v1pack_get_bool_from_item(const mc_pack_item_t *pitem, mc_bool_t *res);
    /**
     * @brief add int32 into pack
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		指向 pack 或 object/array 的指针
     * @param [in] name   : const char*
     *		字段名，ppack 为数组时 name 必须为 NULL
     * @param [in] value   : mc_int32_t
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    /**
     * @brief get int32 from pack
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 或 object 的指针
     * @param [in] name   : const char*
     * @param [out] result   : mc_int32_t*
     *		结果
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_int32(const mc_v1pack_t * ppack, const char *name, mc_int32_t * result);

    /**
     * @brief add uint32 into pack
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		指向 pack 或 object 的指针
     * @param [in] name   : const char*
     * @param [in] value   : mc_uint32_t
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_put_uint32(mc_v1pack_t * ppack, const char *name, mc_uint32_t value);

    /**
     * @brief get uint32 from pack
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 或 object/array 的指针
     * @param [in] name   : const char*
     *		字段名，ppack 为数组时 name 必须为 NULL
     * @param [out] result   : mc_uint32_t*
     *		结果
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_uint32(const mc_v1pack_t * ppack, const char *name, mc_uint32_t * result);

    /**
     * @brief add uint64 into pack
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		指向 pack 或 object 的指针
     * @param [in] name   : const char*
     *		字段名，ppack 为数组时 name 必须为 NULL
     * @param [in] value   : mc_int64_t
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_put_int64(mc_v1pack_t * ppack, const char *name, mc_int64_t value);

    /**
     * @brief get int64 from pack
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 或 object 的指针
     * @param [in] name   : const char*
     * @param [out] result   : mc_int64_t*
     *		结果
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_int64(const mc_v1pack_t * ppack, const char *name, mc_int64_t * result);

    /**
     * @brief add uint64 into pack
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		指向 pack 或 object/array 的指针
     * @param [in] name   : const char*
     *		字段名，ppack 为数组时 name 必须为 NULL
     * @param [in] value   : mc_uint64_t
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_put_uint64(mc_v1pack_t * ppack, const char *name, mc_uint64_t value);

    /**
     * @brief get uint64 from pack
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 或 object 的指针
     * @param [in] name   : const char*
     * @param [out] result   : mc_uint64_t*
     *		结果
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_uint64(const mc_v1pack_t * ppack, const char *name, mc_uint64_t * result);

    /**
     * @brief add a string into pack
     *		向 pack 中加入字符串，会将结尾的 '\\0' 也加入
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		指向 pack 或 object/array 的指针
     * @param [in] name   : const char*
     *		字段名，ppack 为数组时 name 必须为 NULL
     * @param [in] value   : const char*
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_put_str(mc_v1pack_t * ppack, const char *name, const char *value);

    /**
     * @brief add a string into pack
     *		将 value 的前 n 个字节加入 pack 中，会自动补 '\\0'
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		指向 pack 或 object/array 的指针
     * @param [in] name   : const char*
     *		字段名，ppack 为数组时 name 必须为 NULL
     * @param [in] value   : const char*
     * @param [in] len   : u_int
     *		长度
     * @return  int
     * @retval   errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_put_nstr(mc_v1pack_t * ppack, const char *name, const char *value, u_int len);

    /**
     * @brief get a string from pack
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 或 object 的指针
     * @param [in] name   : const char*
     * @return  const char*
     *		指向 string 的指针
     * @retval  MC_PACK_PTR_ERR(retval)
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    const char *mc_v1pack_get_str(const mc_v1pack_t * ppack, const char *name);

    /**
     * @brief get a string from pack with default value
     *		取得字符串，如果未找到，返回默认值
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 或 object 的指针
     * @param [in] name   : const char*
     * @param [in] def_value   : const char*
     *		默认值，如果未找到，返回def_value
     * @return  const char*
     *		指向 string 的指针
     * @retval  MC_PACK_PTR_ERR(retval)
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    const char *mc_v1pack_get_str_def(const mc_v1pack_t * ppack, const char *name, const char *def_value);

    /**
     * @brief add raw data into pack
     *
     * @param [in] ppack   : mc_v1pack_t*
     *		指向 pack 或 object/array 的指针
     * @param [in] name   : const char*
     *		字段名，ppack 为数组时 name 必须为 NULL
     * @param [in] rawdata   : const void*
     * @param [in] size   : u_int
     *		raw data 大小
     * @return  int
     *		- 0 : succeed
     *		- <0 : failed
     * @retval errno
     * @see
     * @note
    **/
    int mc_v1pack_put_raw(mc_v1pack_t * ppack, const char *name, const void *rawdata, u_int size);

    /**
     * @brief get raw data from pack
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 pack 或 object 的指针
     * @param [in] name   : const char*
     * @param [out] size   : u_int*
     *		raw data 的大小
     * @return  const void*
     *		指向 raw data 的指针
     * @retval   MC_PACK_PTR_ERR(retval)
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    const void *mc_v1pack_get_raw(const mc_v1pack_t * ppack, const char *name, u_int *size);

    /**
     * @brief get int32 from array
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 array 的指针
     * @param [in] arr_index   : u_int
     * @param [out] result   : mc_int32_t*
     *		结果
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_int32_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_int32_t * result);

    /**
     * @brief get uint32 from array
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 array 的指针
     * @param [in] arr_index   : u_int
     * @param [out] result   : mc_uint32_t*
     *		结果
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_uint32_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_uint32_t * result);

    /**
     * @brief get int64 from array
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 array 的指针
     * @param [in] arr_index   : u_int
     * @param [out] result   : mc_int64_t*
     *		结果
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_int64_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_int64_t * result);

    /**
     * @brief get uint64 from array
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 array 的指针
     * @param [in] arr_index   : u_int
     * @param [out] result   : mc_uint64_t*
     *		结果
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_uint64_arr(const mc_v1pack_t * ppack, u_int arr_index, mc_uint64_t * result);

    /**
     * @brief get string from array
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 array 的指针
     * @param [in] arr_index   : u_int
     * @return  const char*
     *		指向 string 的指针
     * @retval   MC_PACK_PTR_ERR(retval)
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    const char *mc_v1pack_get_str_arr(const mc_v1pack_t * ppack, u_int arr_index);

    /**
     * @brief get string from array with default value
     *		用数组下标取得字符串，如果未找到，返回默认值
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 array 的指针
     * @param [in] arr_index   : u_int
     * @param [in] def_value   : const char*
     *		默认值，如果未找到，返回def_value
     * @return  const char*
     *		指向 string 的指针
     * @retval  MC_PACK_PTR_ERR(retval)
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    const char *mc_v1pack_get_str_arr_def(const mc_v1pack_t * ppack, u_int arr_index, const char *def_value);

    /**
     * @brief get raw data from array
     *
     * @param [in] ppack   : const mc_v1pack_t*
     *		指向 array 的指针
     * @param [in] arr_index   : u_int
     * @param [out] size   : u_int*
     *		raw data 的大小
     * @return  const void*
     *		指向 raw data 的指针
     * @retval  MC_PACK_PTR_ERR(retval)
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    const void *mc_v1pack_get_raw_arr(const mc_v1pack_t * ppack, u_int arr_index, u_int *size);

    /**
     * @brief get first item
     *
     * @param [in] ppack   : const mc_v1pack_t*
     * @param [out] pitem   : mc_pack_item_t*
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_first_item(const mc_v1pack_t * ppack, mc_pack_item_t * pitem);

    /**
     * @brief get next item
     *
     * @param [in] pitem_now   : const mc_pack_item_t*
     * @param [out] pitem_out   : mc_pack_item_t*
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_next_item(const mc_pack_item_t * pitem_now, mc_pack_item_t * pitem_out);

    /**
     * @brief get item by index
     *
     * @param [in] ppack   : const mc_v1pack_t*
     * @param [in] idx   : u_int
     * @param [out] pitem   : mc_pack_item_t*
     * @return  int
     * @retval errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_by_idx(const mc_v1pack_t * ppack, u_int idx, mc_pack_item_t * pitem);

    /**
     * @brief close a pack
     *		发送 pack 前必须调用此函数，将 pack 的剩余空间置为 0
     *
     * @param [in] ppack   : mc_v1pack_t*
     * @return  int
     * @retval  errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_close(mc_v1pack_t * ppack);

    /**
     * @brief reopen a pack
     *		重新打开一个 close 了的 pack
     *
     * @param [in] ppack   : mc_v1pack_t*
     * @param [in] buf_size   : u_int
     * @return  int
     * @retval  errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_reopen(mc_v1pack_t * ppack, u_int buf_size);

    /**
     * @brief copy item from one pack to another
     *		从一个 pack 中拷贝数据项到另一个 pack
     *
     * @param [in] src_pack   : const mc_v1pack_t*
     *		可以是 pack/array/object
     * @param [in] dst_pack   : mc_v1pack_t*
     *		可以是 pack/array/object，类型必须与 src_pack 一致
     * @param [in] name   : const char*
     *		如果为 NULL，则 copy 所有子元素
     * @return  int
     * @retval  errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_copy_item(const mc_v1pack_t * src_pack, mc_v1pack_t * dst_pack, const char *name);

    /**
     * @brief copy item from one array to another
     *
     * @param [in] src_arr   : const mc_v1pack_t*
     *		source array
     * @param [in] dst_arr   : mc_v1pack_t*
     *		destination array
     * @param [in] idx   : u_int
     *		array index
     * @return  int
     * @retval   errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_copy_item_arr(const mc_v1pack_t * src_arr, mc_v1pack_t * dst_arr, u_int idx);

    /**
     * @brief get item by name
     *
     * @param [in] ppack   : const mc_v1pack_t*
     * @param [in] name   : const char*
     * @param [out] pitem   : mc_pack_item_t*
     * @return  int
     * @retval   errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_item(const mc_v1pack_t * ppack, const char *name, mc_pack_item_t *pitem);

    /**
     * @brief get item by array index
     *
     * @param [in] ppack   : const mc_v1pack_t*
     * @param [in] idx   : u_int
     * @param [out] pitem   : mc_pack_item_t*
     * @return  int
     * @retval   errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_item_arr(const mc_v1pack_t * ppack, u_int idx, mc_pack_item_t *pitem);

    /**
     * @brief reset a pack
     *
     * @param [in] ppack   : mc_v1pack_t*
     * @return  int
     * @retval  errno
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_reset(mc_v1pack_t *ppack);

    /**
     * @brief callback function for mc_v1pack_foreach
     *
     * @param [in] pitem   : const mc_pack_item_t*
     *		one item in pack
     * @param [in] arg   : void*
     *		argument for callback
     * @return  int
     * @retval
     *		- <0 : stop and return to caller of foreach
     * @see
     * @note
    **/
    typedef int (*mc_v1pack_foreach_callback)(const mc_pack_item_t *pitem, void *arg);
    typedef int (*mc_v1pack_foreach_callback_ex)(const mc_pack_item_t *pitem, void *arg,full_key_t *key);

    /**
     * @brief for each item in pack do sth.
     *
     * @param [in] ppack   : const mc_v1pack_t*
     * @param [in] cb   : mc_v1pack_foreach_callback
     *		callback function
     * @param [in] arg   : void*
     *		argument for callback
     * @return  int
     * @retval
     *		- <0 : err returned by cb
     *		- >=0 : sum of retval of cb
     * @see
     * @note
    **/
    int mc_v1pack_foreach(const mc_v1pack_t *ppack, mc_v1pack_foreach_callback cb, void *arg);
    int mc_v1pack_foreach_ex(const mc_v1pack_t *ppack, mc_v1pack_foreach_callback_ex cb, void *arg,full_key_t *key);

    /**
     * @brief get int32 from item
     *
     * @param [in] pitem   : const mc_pack_item_t*
     * @param [out] res   : mc_int32_t*
     * @return  int
     * @retval
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_int32_from_item(const mc_pack_item_t *pitem, mc_int32_t *res);

    /**
     * @brief get uint32 from item
     *
     * @param [in] pitem   : const mc_pack_item_t*
     * @param [out] res   : mc_uint32_t*
     * @return  int
     * @retval
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_uint32_from_item(const mc_pack_item_t *pitem, mc_uint32_t *res);

    /**
     * @brief get int64 from item
     *
     * @param [in] pitem   : const mc_pack_item_t*
     * @param [out] res   : mc_int64_t*
     * @return  int
     * @retval
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_int64_from_item(const mc_pack_item_t *pitem, mc_int64_t *res);

    /**
     * @brief get uint64 from item
     *
     * @param [in] pitem   : const mc_pack_item_t*
     * @param [out] res   : mc_uint64_t*
     * @return  int
     * @retval
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
    **/
    int mc_v1pack_get_uint64_from_item(const mc_pack_item_t *pitem, mc_uint64_t *res);

    /**
     * @brief put item into pack
     *
     * @param [in] pack   : mc_v1pack_t*
     * @param [in] pitem   : const mc_pack_item_t*
     * @return  int
     * @retval
     *		- 0 : succeed
     *		- <0 : failed
     * @see
     * @note
     *	会取 item.key 的最末级名字，不自动处理子 pack 的情况
    **/
    int mc_v1pack_put_item(mc_v1pack_t *pack, const mc_pack_item_t *pitem);

#ifdef __cplusplus
}
#endif

#endif	//__MC_PACK_CORE_H

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
