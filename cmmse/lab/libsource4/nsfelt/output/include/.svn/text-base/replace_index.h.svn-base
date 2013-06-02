#ifndef _REPLACE_INDEX_H_
#define _REPLACE_INDEX_H_

#include "dfa_index.h"

#define ERR_TRANS_DICT "bsdict/err_replace"
#define SYN_TRANS_DICT "bsdict/syn_replace"

typedef struct REPLACE_NODT_T
{
    int fromstr;
    int fromlen;
    int tostr;
    int tolen;
	int reqlen;
}rep_node_t;

typedef struct REPLACE_DICT_T
{
    int m_typenum;
    int m_bufsize;
    rep_node_t * m_nodes;
    char *m_buf;
}rep_dict_t;

typedef struct TRANSDICT
{
    unsigned short *m_statestab;
    unsigned char range;
    int m_statescnt;
    int m_recsize;
    rec_tab_t *m_rectab;
    rep_dict_t *m_repdict;
}trans_dict_t;

trans_dict_t *
load_transdict(char *path, char *dictname);

void
free_transdict(trans_dict_t *transdict);

int
replace(char *str, const char *substr, int len1,
		const char *newstr, int len2, int repind, int repend);

int
convert(char *query, trans_dict_t *transdict);

#endif //_REPLACE_INDEX_H_
