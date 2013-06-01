#include "fileblock.h"
#include "stdio.h"
#ifndef u_int
#define u_int unsigned int
#endif
typedef struct item_t{
	u_int a,b,c,d;
	char buf[254];
}item_t;

int do_one( u_int id, void* pitem,void* pex)
{
	item_t* p1 = (item_t*)pitem;
	if( p1->a!=id || p1->b!=id+1 || p1->c!=id+2 || p1->d!=id+3 ){
		printf("fail. id:%d a:%d b:%d c:%d d:%d\n", 
				id, p1->a, p1->b, p1->c, p1->d );
	}
	else
		printf("success. id:%d\n", id );
	return 0;
}
int main()
{
	fblock_t fb;
	int ret =0;
	ret = fblock_init( &fb, "test", "data/fb",sizeof(item_t), false, false );
	printf("init ret:%d\n", ret );

	int loopnum=10;
	item_t item;
	
	for( int i=0; i<loopnum; i++ )
	{
		int id = fb.max_curbid+1;
		item.a = id;
		item.b = id+1;
		item.c = id+2;
		item.d = id+3;
		ret = fblock_multiappend( &fb, 1, &item );
		printf("append max_curbid:%d ret:%d\n", fb.max_curbid, ret );
	}
	fblock_traverse_load( &fb, &item, sizeof(item), do_one, NULL );

	return 0;
}
