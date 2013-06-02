/** $Id: myopt.h,v 1.1 2008/08/22 04:03:35 yanghy2 Exp $
 * get options, by zhaoxw, 17:30 2005-12-03
 **/
#ifndef __MY_OPTION_HEAD_FILE__
#define __MY_OPTION_HEAD_FILE__
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>

#define MYOPT_MAXNUM  256
#define MYOPT_NAME_MAXSIZE  80

#define	MYOPT_START(mapopt,defs) \
	myopt_t mapopt; { \
	myopt_appendopt(mapopt,defs); \
	int ret = myopt_parse( argc, argv, mapopt ); \
	if( ret<=0 ) \
	{ \
		myopt_print_usage( PROJNAME, AUTHOR, mapopt ); \
		exit(1); \
	} \
	if( myopt_get(mapopt,"ver").found_flag ) \
	{ \
		myopt_print_version(); \
		exit(1); \
	} }
#define MYOPT_EXIT(msg) \
	{ \
		myopt_print_usage( PROJNAME, AUTHOR, mapopt ); \
		printf("Error parse : "msg"\n");\
		exit(1); \
	} 
enum myopt_param_enum_t
{
	myoptNone = 0,  
	myoptString = 1,
	myoptInt = 2,  
	myoptDate = 3, 
	myoptDay = 4
};

typedef struct myopt_item_t
{
	char* name;
	char* comment;
	int   ab;    
	int   opt_type; 
	int   vi_range[2];//最小值, 最大值,

	int   vint[2];    //得到的两个整数. 如果时整数参数，而且参数个数少于2个,则这里就是其值
	int   found_flag; //1: found opt, otherwise, no such option.
	char  *vstr;    
	
	int   *varr_int;  //多个整数参数值的存放数组
	int   varr_num;   
	time_t vdate[2];   
}myopt_item_t;

typedef std::map<char*, myopt_item_t> myopt_t;

typedef struct myopt_def_t
{
	char *name;
	char *comment;
	char ab;
	int myopt_type;
	int imin;
	int imax;
} myopt_def_t;



myopt_item_t myopt_get( myopt_t &map_opt, char *name );
int myopt_appendopt( myopt_t &map_opt, myopt_def_t *defines);
int myopt_appendopt( myopt_t &map_opt, char *name, char *comment,
		char ab, int myopt_type, int imin, int imax );
int myopt_print_usage( const char* module, const char* author, myopt_t& map_opt );
void myopt_print_version(void);
int myopt_parse( int argc, char **argv, myopt_t& map_opt );

#endif
