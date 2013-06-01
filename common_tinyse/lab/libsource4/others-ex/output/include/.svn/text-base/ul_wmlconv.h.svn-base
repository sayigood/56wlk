/**
 * @file
 * @brief wml是web页面的一种格式，其格式与html相似，主要用于在wap手机上进行显示，本转换接口转换提供两部分的转换,\n
 * 一个是将html格式的url转换成wml格式的url，另一个是将纯文本（支持中文）转换成wml格式（unicode）内容。
 */




#ifndef __WML_CONV__
#define __WML_CONV__

#include "ul_fdict.h"


/**
 *  将html格式的url转换成wml格式的url
 *  
 *  @param[in]  url    url
 *  @param[in]  ulen   输入url的长度
 *  @param[in]  wapurl 输出的buf指针
 *  @param[in]  wlen   输出buf的长度
 *  @param[out] wapurl
 *  @return 输出url的长度
 */
int ul_convert_to_wml_url(char *url, int ulen, char *wapurl, int wlen);



/**
 *  将纯文本转换成wml格式的内容
 *  
 *  @param[in]  src             输入的html文本
 *  @param[in]  dest            输出buf的指针
 *  @param[in]  destlen         输出buf的长度
 *  @param[in]  pwmlKeepDict    需要保持的字符串的字典
 *  @param[in]  pwmlConvDict    需要转换的字符串的字典
 *  @param[in]  pwmlConvList    转换信息列表
 *  @param[out] dest            转换后的文本
 *  @return 输出url的长度
 */
int ul_convert_to_wml_content(char *src, char *dest, int destlen, fdict_t *pwmlKeepDict, fdict_t *pwmlConvDict, char *pwmlConvList);


/**
 *  初始化进行wml转换的环境
 *  
 *  @param[in]  无
 *  @param[out] 无
 *  @return 初始化是否成功
 * - 0   成功
 * - -1  失败
 *  @note 对全局变量进行初始化，无参数
 */
int ul_init_wml_conv_env(void);


/**
 *  加载wml转换的词典
 *  
 *  @param[in]  path            load字典的路径
 *  @param[in]  keep_name       保持字典的文件名
 *  @param[in]  conv_name       转换字典的文件名
 *  @param[in]  pwmlKeepDict    需要保持的字符串的字典的指针
 *  @param[in]  pwmlConvDict    需要转换的字符串的字典的指针
 *  @param[in]  pwmlConvList    转换信息列表的指针
 *  @param[out]  pwmlKeepDict   需要保持的字符串的字典
 *  @param[out]  pwmlConvDict   需要转换的字符串的字典
 *  @param[out]  pwmlConvList   转换信息列表
 *  @return 初始化是否成功
 * - 0   成功
 * - -1  失败
 */
int ul_load_wml_dict(char *path, char *keep_name, char *conv_name,
						fdict_t **pwmlKeepDict, fdict_t **pwmlConvDict, char **pwmlConvList);
						
						
						
/**
 *  释放wml转换词典

 *  
 *  @param[in]  pwmlKeepDict    需要保持的字符串的字典的指针
 *  @param[in]  pwmlConvDict    需要转换的字符串的字典的指针
 *  @param[in]  pwmlConvList    转换信息列表的指针
 *  @return 无
 */						
void ul_release_wml_dict(fdict_t *pwmlKeepDict, fdict_t *pwmlConvDict, char *pwmlConvList);

#endif
