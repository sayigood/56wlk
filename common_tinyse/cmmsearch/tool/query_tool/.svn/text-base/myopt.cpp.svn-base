/** $Id: myopt.cpp,v 1.1 2008/08/22 04:03:35 yanghy2 Exp $ */

#include <getopt.h>
#include "myopt.h"
#if defined(__DATE__) && defined(__TIME__)
static const char BuildDate[] = __DATE__ " " __TIME__;
#else
static const char BuildDate[] = "unknown";
#endif 


static char g_myopt_name[MYOPT_MAXNUM][MYOPT_NAME_MAXSIZE];
static int g_myopt_num=0;
static char g_myopt_int_delim[32]=", \t-";

typedef std::map<char*, myopt_item_t>::iterator myopt_iterator_t;
void myopt_print_version(void)
{
	printf("Project    : %s\n", PROJNAME);
	printf("Version    : %s\n", VERSION);
	printf("Cvstag     : %s\n", CVSTAG);
	printf("BuildDate  : %s\n", BuildDate);
}
int myopt_print_usage( const char* module, const char* author, myopt_t &map_opt )
{
	char linebuf[1024];
	snprintf( linebuf, sizeof(linebuf), "Usage: %s [options] ...\n", module );
	printf( linebuf );
	myopt_iterator_t it=map_opt.begin();
	while( it!=map_opt.end() )
	{
		myopt_item_t &opt= it->second;
		char srange[64];
		srange[0]=0;
		if( myoptInt==opt.opt_type )
		{
			if( opt.vi_range[0]==opt.vi_range[1] &&
					-1==opt.vi_range[0] )
				srange[0]=0;
			else
				sprintf( srange, "range[%d,%d]",
						opt.vi_range[0], opt.vi_range[1] );
		}
		if( '\0'!=opt.ab )
		{
			snprintf( linebuf, sizeof(linebuf),	"  -%c --%s\t# %s %s\n", 
					opt.ab, opt.name, 
					opt.comment?opt.comment:"", srange);
		}
		else
		{
			snprintf( linebuf, sizeof(linebuf),	"     --%s\t# %s %s\n", 
					opt.name, 
					opt.comment?opt.comment:"", srange);
		}
		printf( linebuf );
		it++;
	}
	snprintf( linebuf, sizeof(linebuf),	"\n(%s, written by %s, %s.)\n", 
			module, author, __DATE__ " " __TIME__ );
	printf( linebuf );
	return 0;
}

myopt_item_t myopt_get( myopt_t &map_opt, char *name )
{
	char *ptr = NULL;
	for( int i=0; i<g_myopt_num; i++ )
		if( strcmp(name, g_myopt_name[i])==0 )
			ptr = g_myopt_name[i];
	assert( ptr );
	return map_opt[ptr];
}

int myopt_appendopt( myopt_t &map_opt, char *name, char *comment,
		char ab, int myopt_type, int imin, int imax )
{
	myopt_item_t item;
	memset( &item, 0, sizeof(item) );
	strcpy( g_myopt_name[g_myopt_num], name );
	item.name = g_myopt_name[g_myopt_num];
	item.comment = comment;
	item.ab = ab;
	item.opt_type = myopt_type;
	item.vi_range[0]=imin;
	item.vi_range[1]=imax;

	map_opt[item.name] = item;

	g_myopt_num ++;
	if( g_myopt_num>= MYOPT_MAXNUM )
	{
		printf("too many options. (maxnum:%d)\n", MYOPT_MAXNUM );
		exit(-1);
	}
	return 0;
}

int myopt_appendopt( myopt_t &map_opt, myopt_def_t *defines)
{
	int i=0;
	while (NULL != defines[i].name)
	{
		myopt_appendopt(map_opt,defines[i].name,defines[i].comment,
					defines[i].ab,defines[i].myopt_type,defines[i].imin,defines[i].imax);		
		i++;
	}
	myopt_appendopt(map_opt, "ver", "version info", 'v', myoptNone, 0,0 );
	return 0;
}

int myopt_parse( int argc, char **argv, myopt_t& map_opt )
{
	int  msize = sizeof(struct option)*(map_opt.size()+2);
	struct option *poption = (struct option*)malloc( msize );
	memset( poption, 0, msize );
	char optstring[2*MYOPT_MAXNUM+1]="h";

	struct{
		int ab;
		int i;
	}tmp_ab[MYOPT_MAXNUM];
	int tmp_abnum = 0;

	int i=0;
	myopt_iterator_t it=map_opt.begin();
	while( it!=map_opt.end() )
	{
		myopt_item_t &opt=it->second;

		memset( &poption[i], 0, sizeof(struct option) );
		poption[i].name = opt.name;
		poption[i].has_arg = (myoptNone!=opt.opt_type)?1:0;
		opt.found_flag = 0;
		opt.vdate[0]=opt.vdate[1]=0;
		poption[i].val = 0;
		if( 0!=opt.ab )
		{
			tmp_ab[tmp_abnum].ab=opt.ab;
			tmp_ab[tmp_abnum].i=i;
			tmp_abnum++;
			poption[i].val = opt.ab;
			if( poption[i].has_arg )
				sprintf( optstring+strlen(optstring), "%c:", opt.ab );
			else 
				sprintf( optstring+strlen(optstring), "%c", opt.ab );
		}

		i++;
		it++;
	}

	int option_index = 0;
	int c=0;

	int retval = 1;
	while( retval>0 && 
			-1!=(c=getopt_long( argc, argv, optstring, poption, &option_index )))
	{
		std::vector<int>  vct_int; 
		int optflag = 0;
		for( int _tmpi=0; _tmpi<tmp_abnum; _tmpi++ )
		{
			if( tmp_ab[_tmpi].ab == c )
			{
				optflag =1;
				option_index = tmp_ab[_tmpi].i;
				break;
			}
		}
		if( optflag || 0==c )
		{
			char *strname = (char*)(poption[option_index].name);
			myopt_item_t &opt =map_opt[ strname ];
			if( myoptNone != opt.opt_type )
			{
				opt.vstr = strdup(optarg);
			}

			if( myoptDate == opt.opt_type ){
				int yy1, mm1, dd1, yy2, mm2, dd2;
				struct tm tm1, tm2;
				memset( &tm1, 0, sizeof(tm1) );
				memset( &tm2, 0, sizeof(tm1) );
				int dateok = 0;
				if( 6==sscanf( optarg, 
							"%d-%d-%d:%d-%d-%d", 
							&yy1, &mm1, &dd1, &yy2, &mm2, &dd2 ) ){
					yy1%=2000; yy1+=2000-1900;
					mm1-=1;
					yy2%=2000; yy2+=2000-1900;
					mm2-=1;
					tm1.tm_year = yy1, tm1.tm_mon = mm1; 
					tm1.tm_mday = dd1, tm1.tm_hour=0;
					tm2.tm_year = yy2, tm2.tm_mon = mm2; 
					tm2.tm_mday = dd2, tm2.tm_hour=24;
					u_int time1 = (u_int)mktime( &tm1 );
					u_int time2 = (u_int)mktime( &tm2 );
					opt.vdate[0] = time1>time2?time2:time1;
					opt.vdate[1] = time1>time2?time1:time2;
					dateok = 1;
					if( time1<=0 || time2<=0 )
						retval=-1;
				}
				if( 0==dateok &&
						3==sscanf( optarg, "%d-%d-%d", &yy1, &mm1, &dd1))
				{
					yy1%=2000; yy1+=2000-1900;
					mm1-=1;
					tm1.tm_year = yy1, tm1.tm_mon = mm1;
					tm1.tm_mday = dd1, tm1.tm_hour=0;
					u_int time1 = (u_int)mktime( &tm1 );
					u_int time2 = time1 + 24*3600;
					opt.vdate[0] = time1;
					opt.vdate[1] = time2;
					dateok = 1;
					if( time1<=0 || time2<=0 )
						retval=-1;
				}
				if( 0==dateok )
					retval = -1;
			}
			if( myoptDay == opt.opt_type ){
				u_int val=0;
				if( sscanf( optarg, "%u", &val )==1 && val>0 )
				{
					time_t curtime = time(NULL);
					tm curtm = *localtime(&curtime);
					tm tm2=curtm;
					u_int time2 = (u_int)mktime( &tm2 );
					u_int time1 = time2 - val*24*3600;
					opt.vdate[0] = time1;
					opt.vdate[1] = time2;
				}
				else retval = -1;
			}

			if( myoptInt == opt.opt_type ){
				char *pstr = strdup( opt.vstr);
				char *ptok = strtok( pstr, g_myopt_int_delim );
				while( ptok && 1==retval )
				{
					int val=0;
					retval = -1;
					if( sscanf( ptok, "%d", &val )==1 )
						if( val>=opt.vi_range[0]&&val<=opt.vi_range[1] ){
							vct_int.push_back(val);
							retval = 1;
						}
					ptok = strtok( NULL, g_myopt_int_delim );
				}
				if( vct_int.size()==0 ){
					retval = -1;
				}
				else
				{
					if( vct_int.size()==1 )
					{
						opt.vint[0] = opt.vint[1] = *vct_int.begin();
					}
					opt.varr_num = (int)vct_int.size();
					opt.varr_int = (int*)malloc( sizeof(int)*(4+opt.varr_num) );
					memset( opt.varr_int, 0, sizeof(int)*(4+opt.varr_num) );
					std::vector<int>::iterator itint=vct_int.begin();
					int _tmp_i = 0;
					while( itint!=vct_int.end() )
					{
						if( _tmp_i<2 )
							opt.vint[_tmp_i] = *itint;
						opt.varr_int[_tmp_i] = *itint;
						itint++;
						_tmp_i ++;
					}
				}
			}
			if( -1==retval ){
				printf( "  parse option error. --%s %s\n",
						opt.name, opt.vstr );
				exit(0);
			}
			opt.found_flag = 1;
		}
		else
		{
			switch(c)
			{
				case 'h':
				case '?':
					retval = 0;
					break;
				default:
					retval = 0;
					break;
			}
		}
	}

	free( poption );
	return retval;
}



