#ifndef WP_NOMEANS_H
#define WP_NOMEANS_H

struct packin_t
{
	char *inbuf;
	int slen;
};
int wp_init_nomeans_dict (const char *fname);
/*
bool wp_change_no_means_query (char *strquery, const wp_dict_pack_t* pDictPack,const search_seg_data_t* pSegData,int query_type);

bool wp_omit(char *strquery, const wp_dict_pack_t* pScwDict,const search_seg_data_t* pSegData,	int query_for_org_name);
*/
#endif
