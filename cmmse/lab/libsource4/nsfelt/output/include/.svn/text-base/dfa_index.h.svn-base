#ifndef DFA_INDEX_H
#define DFA_INDEX_H

#include <ul_log.h>
#include <ul_func.h>
#include <ul_file.h>

#ifndef MAX_QUERY_LEN
#define MAX_QUERY_LEN 80
#endif

typedef struct RECTAB
{
    unsigned short stat;
    unsigned short type;
}rec_tab_t;

int
bisearch(rec_tab_t *begin, rec_tab_t *end, unsigned short curstat);

inline unsigned short nextstat(unsigned char ch,
                        unsigned short curstat, int statescnt, unsigned short *statestab)
{
	return statestab[ch * statescnt + curstat];
}

inline int get_rectype(unsigned short curstat, int recsize, rec_tab_t *rectab)
{
	return bisearch(rectab, rectab + recsize, curstat);
}
/*
unsigned short
nextstat(unsigned char ch, unsigned char minch,
         unsigned short curstat, int statescnt, unsigned short *statestab);
int
get_rectype(unsigned short curstat, int recsize, rec_tab_t *rectab);
*/

inline int skiplen(unsigned char *p)
{
	if(0x81 <= p[0] && p[0] <= 0xfe)
	{
		if(0x40 <= p[1] && p[1] <= 0xfe)
			return 2;
		else if(0x30 <= p[1] && p[1] <= 0x39)
			return 4;
	}
	return 1;
}

#endif
