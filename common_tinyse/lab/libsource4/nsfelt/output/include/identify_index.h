#ifndef _IDENTIFY_INDEX_H_
#define _IDENTIFY_INDEX_H_

#include "dfa_index.h"

typedef struct IDENTDICT
{
    unsigned short *m_statestab;
    unsigned char range;
    int m_statescnt;
    int m_recsize;
    rec_tab_t *m_rectab;
//    rep_dict_t *m_repdict;
}ident_dict_t;

ident_dict_t *
load_identdict(char *path, char *dictname);

void
free_identdict(ident_dict_t *identdict);

int
identify(char *query, ident_dict_t *identdict);

#endif //_IDENTIFY_INDEX_H_
