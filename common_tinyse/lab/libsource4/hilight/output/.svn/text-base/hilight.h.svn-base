/**
 * @file	hilight.h
 * @brief	header file of hilight.cpp
 *
 */

#ifndef _HILIGHT_H
#define _HILIGHT_H

#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief	we call the segment result as word
 */
#define HILIGHT_WORD_SEP	'\1'
/**
 * @brief	we call the continuous characters separated by space as term
 */
#define HILIGHT_TERM_SEP	'\0'
/**
 * @brief	we will never hilight dot point
 */
#define HILIGHT_DOT		'.'
/**
 * @brief	suspension points
 */
#define HILIGHT_SUS		"..."

/**
 * @brief	max length of string holding html syntax
 */
#define HILIGHT_HTML_LEN	128
/**
 * @brief	prefix when print hilight result
 */
#define HILIGHT_HTML_PRE	"<font color=\"#c60a00\">"
/**
 * @brief	suffix when print hilight result
 */
#define HILIGHT_HTML_SUF	"</font>"

/**
 * @brief	max length of query after segment
 */
#define HILIGHT_QUERY_LEN	1024
/**
 * @brief	max number of terms of one query
 */
#define HILIGHT_TERM_NUM	128
/**
 * @brief	max number of snippets to be hilight of one text
 */
#define HILIGHT_SNIPPET_NUM	512

/**
 * @brief	error code of hilight lib
 */
typedef enum _hilight_err_t {
	hilight_err_ok	= 0,	/**< every thing is ok */
	hilight_err_arg	= -1,	/**< invalid argument */
} hilight_err_t;

/**
 * @brief	html syntax when printing hilight result
 */
typedef struct _hilight_html_t {
	char	prefix[HILIGHT_HTML_LEN];	/**< before hilight snippet */
	char	suffix[HILIGHT_HTML_LEN];	/**< after hilight snippet */
} hilight_html_t;

/**
 * @brief	attribute of hilight lib
 */
typedef struct _hilight_attr_t {
	bool		seg;	/**< text being segment or not */
	bool		trim_single;		  /**< trim single word or not       */
	hilight_html_t	html;	/**< prefix and suffix of hilight snippet */
} hilight_attr_t;

/**
 * @brief	one term of segment query
 */
typedef struct _hilight_term_t {
	bool		is_eng;	/**< is English term or not */
	const char	*ptr;	/**< start pointer */
	size_t		len;	/**< length */
} hilight_term_t;

/**
 * @brief	query being segment
 */
typedef struct _hilight_query_t {
	size_t		cur;				/**< current number of terms */
	size_t		cap;				/**< capability of terms */
	hilight_term_t	terms[HILIGHT_TERM_NUM];	/**< all the terms */
	char		cont[HILIGHT_QUERY_LEN];	/**< query string content */
} hilight_query_t;

/**
 * @brief	one snippet to be hilight
 */
typedef struct _hilight_snippet_t {
	const char	*term_ptr;	/**< identify the term this snippet belongs to  */
	const char	*word_begin;	/**< start pointer of word matched */
	const char	*word_end;	/**< end pointer of word matched */
	size_t		word_len;	/**< matched length at term */

	const char	*match_ptr;	/**< matched offset at text */
	size_t		match_len;	/**< matched length at text */

	bool		del;		/**< deleted or not */
} hilight_snippet_t;

/**
 * @brief	all the snippets to be hilight
 */
typedef struct _hilight_text_t {
	size_t			cur;				/**< current number of snippets */
	size_t			cap;				/**< capability of snippets */
	hilight_snippet_t	snippets[HILIGHT_SNIPPET_NUM];	/**< snippets to be hilight */
} hilight_text_t;

/**
 * @brief	main structure of hi lib
 */
typedef struct _hilight_t {
	hilight_query_t	query;	/**< query being segment */
	hilight_text_t	text;	/**< all the snippets to be hilight */
} hilight_t;

/**
 * @brief	get the error description according to error number
 *
 * @param[in]	errno	error number
 *
 * @return	error description
 */
extern const char *
hilight_strerr(int errno);

/**
 * @brief	init the hi lib (attribute)
 *
 * @param[in]	attr	attribute of hi lib, if NULL, using the default attribute
 *
 * @return	void
 */
extern void
hilight_init(hilight_attr_t *attr);

/**
 * @brief	fill the hilight_query_t struct of hilight_t using the data from hilight_word
 *		which is the word segment result of as
 *
 * @param[out]	hi	hilight_t structure
 * @param[in]	src	hilight word list, pls see the specification to
 *			get hilight word list's format
 *		        注意：飘红串必须遵循queryparser格式，小串用\1分隔，大串用\0分隔，
 *		        	末尾是以2个\0结尾(普通字符串是一个\0)
 *
 * @return	0 for success, < 0 for hi lib's errno
 */
extern int
hilight_reset(hilight_t *hi, const char *src);

/**
 * @brief	print the hilight result
 *
 * @param[out]	dst	target string buffer
 * @param[in]	len	max length of @dst
 * @param[in]	src	source text string
 * @param[in]	hi	containing the hilight word list
 * @param[in]	str_modifier const char * -  modifier的格式参考escape库的说明文档，
 * 					默认是":h"即html转义，常用的还有":j" - js转义
 *
 * @return	0 for success, < 0 for hi lib's errno
 */
extern int
hilight_print(char *dst, const size_t len, const char *src, hilight_t *hi, 
		const char *str_modifier = NULL);


/**
 * @brief	print the hilight result
 *
 * @param[out]	dst	target string buffer
 * @param[in]	len	max length of @dst
 * @param[in]	src	source text string
 * @param[in]	hi	containing the hilight word list
 * @param[in]	str_modifier const char * -  modifier的格式参考escape库的说明文档，
 * 					默认是":h"即html转义，常用的还有":j" - js转义
 * @param [in/out] attr   : hilight_attr_t* 外部可定制样式
 *
 * @return	0 for success, < 0 for hi lib's errno
 */

extern int
hilight_print_style(char *dst, const size_t len, const char *src, hilight_t *hi, 
		const char *str_modifier = NULL, hilight_attr_t *attr = NULL);

#endif	/* _HILIGHT_H */

/* vim: set ts=8 sw=4 tw=80 noet: */
