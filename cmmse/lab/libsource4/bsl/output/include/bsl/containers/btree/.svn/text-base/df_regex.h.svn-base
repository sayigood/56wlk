/**======================================================================
 *
 *  df_regex.h - Pyramid / df-lib
 *
 *
 *
 *  -------------------------------------------------------------------
 *
 *  Description
 *
 *     正则表达式匹配类
 *
 *  -------------------------------------------------------------------
 *
 *  Change Log
 *
 *
 =====================================================================**/
#ifndef _DF_REGEX_H_
#define _DF_REGEX_H_

#include <regex.h>

class df_regex_t
{
public:
    /**
     * @brief
     *
     * @see
     * @note
    **/
    df_regex_t();
    /**
     * @brief
     *
     * @see
     * @note
    **/
    ~df_regex_t();

public:
    // 模式初始化
    // @param(in): pattern - 模式
    // @param(in): flags - 匹配的option，同regcomp
    // @return: true - 成功
    //          false - 失败
    bool init(const char* pattern, int flags);

    // 检查字符串是否匹配
    // @param(in): text - 待匹配的字符串
    // @param(in): flags - 匹配的option，同regexec
    // @return: true - 匹配
    //          false - 不匹配
    bool match(const char* text, int flags);

    // 清空上一个模式
    void destroy(void);

private:
    bool _init;
    regmatch_t* _match;
    regex_t _reg;
    size_t _nmatch;
};

#endif
