/** $Id: main.cpp,v 1.5 2009/03/16 10:02:44 yanghy2 Exp $ */

#include <signal.h>
#include <sys/time.h>
#include "pub.h"
#include "opdata.h"
#include "nshead.h"
#include "talk.h"
#include "mc_pack.h"
#include "tinyse.h"

options_data_t opt_data;


static int load_options(int argc, char** argv );

typedef struct _req_t
{
	nshead_t nshead;
	ts_head_t head;
	char detail[1024*1024];

}req_t;

typedef struct res_t
{
	nshead_t nshead;
	ts_head_t head;
	char detail[1024*1024];



}res_t;


int main( int argc, char**argv )
{

	struct timeval s;
	gettimeofday(&s,NULL);

	signal(SIGPIPE, SIG_IGN);
	srand(s.tv_sec*1000000 + s.tv_usec);
	load_options( argc, argv );

	req_t req;
	res_t res;
	if (0 == strcmp(opt_data.cmd,"add"))
	{
		req.head.cmd_no = cmDataAdd;
	}
	else if (0 == strcmp(opt_data.cmd,"mod"))
	{
		req.head.cmd_no = cmDataMod;
	}
	else if (0 == strcmp(opt_data.cmd,"modbasic"))
	{
		req.head.cmd_no = cmDataModBasic;
	}
	else if (0 == strcmp(opt_data.cmd,"del"))
	{
		req.head.cmd_no = cmDataDel;
	}
	else if (0 == strcmp(opt_data.cmd,"undel"))
	{

		req.head.cmd_no = cmDataUndel;
	}
	else if (0 == strcmp(opt_data.cmd,"qcall"))
	{
		req.head.cmd_no = qcAll;
	}
	else if (0 == strcmp(opt_data.cmd,"qcdel"))
	{
		req.head.cmd_no = qcDel;
	}
	else if (0 == strcmp(opt_data.cmd,"qcundel"))
	{
		req.head.cmd_no = qcUndel;
	}
	
	uint32 body_len = 0;
	if (cmDataDel != req.head.cmd_no && cmDataUndel != req.head.cmd_no)
	{

		mc_pack_t *req_mc = mc_pack_create(req.detail,sizeof(req.detail));

		int opret = mc_pack_text2pack(opt_data.query,req_mc);
		if (opret<0)
		{
			fprintf(stderr,"Error mc_pack_putf: %d\n",opret);
		}
		body_len = mc_pack_get_size(req_mc);

	} else {
		const char *delimiters = ",: ";
		char *cp = strdupa(opt_data.query);
		char *token = strtok (cp, delimiters);  
		uint32 *ids = (uint32*)req.detail;
		uint32 id_num = 0;
		while (token)
		{
			ids[id_num++] = atoi(token);
			token = strtok(NULL,delimiters);	
		}
		body_len = id_num * sizeof(uint32);
	}


	req.head.page_no = opt_data.page_no;
	req.head.num_per_page = 10;

	strncpy(req.head.data_type,opt_data.data_type,sizeof(req.head.data_type)-1);
	req.nshead.log_id = rand();
	req.nshead.body_len = sizeof(ts_head_t) + body_len;


	int fd = -1;
	talkwith(fd,(nshead_t*)&req,(nshead_t*)&res,sizeof(res_t));


//	printf("req_num[%u]page_no[%u]\n",req.head.num_per_page,req.head.page_no);
//	printf("detail_len[%u]headlen[%lu]\n",res.nshead.body_len,sizeof(ts_head_t));
	printf("body_len[%u]ret_num[%u]disp_num[%u]\n",res.nshead.body_len,res.head.ret_num,res.head.disp_num);
	char tmp[1024*1024];
	memset(tmp,0,sizeof(tmp));
	mc_pack_t *res_mc = (mc_pack_t*)res.detail;
	//mc_pack_pack2str(res_mc, tmp, sizeof(tmp));
	mc_pack_pack2text(res_mc,tmp,sizeof(tmp),1);
	//printf("result : %s\n",tmp);
	printf("%s\n",tmp);
	
	return 0;
}

 

static int 
load_options(int argc, char** argv )
{
	myopt_def_t defs[] = 
	{
		/* name,comment,absname,type,imin,imax */
		{"cmd","add/mod/modbasic/del/undel 或者 qcall/qcdel/qcundel",'c',myoptString,0,0},
		{"datatype","数据类型",'d',myoptString,0,0},
		{"query","查询/提交数据,\n\t\tadd/mod/modbasic/qcall/qcdel/qcundel:请按照json格式填写 \"{key1:value1,key2:value2...}\"; \
					\n\t\tdel/undel:请直接填写整数列表，逗号分隔",'q',myoptString,0,0},
		{"ip","地址及端口(127.0.0.1:12345)",'i',myoptString,0,0},
		{"pageno","页号",'p',myoptInt,0,76},
		{ NULL,NULL,0,0,0,0}
	};

	MYOPT_START(mapopt,defs)

	myopt_item_t item;
	item  = myopt_get( mapopt, "datatype" );
	if (false == item.found_flag)
	{
		MYOPT_EXIT("datatype")
	}
	strncpy(opt_data.data_type ,item.vstr,sizeof(opt_data.data_type)-1);


	item  = myopt_get( mapopt, "query" );
	if (false == item.found_flag)
	{
		MYOPT_EXIT("query")
	}
	strncpy(opt_data.query ,item.vstr,sizeof(opt_data.query)-1);

	item  = myopt_get( mapopt, "cmd" );
	if (false == item.found_flag)
	{
		MYOPT_EXIT("cmd")
	}
	strncpy(opt_data.cmd ,item.vstr,sizeof(opt_data.cmd)-1);

	item  = myopt_get( mapopt, "ip" );
	if (false == item.found_flag)
	{
		MYOPT_EXIT("ip")
	}
	char *running = NULL;
	char *delimiters = ":";
	running = strdup(item.vstr);
	char *ip = strsep (&running, delimiters);
	if (NULL == ip)
	{
		MYOPT_EXIT("ip");
	}
	strncpy(opt_data.ip , ip,sizeof(opt_data.ip)-1); 
	char *port = strsep (&running, delimiters);
	if (NULL == port)
	{
		MYOPT_EXIT("port");
	}
	opt_data.port = atoi(port);


	item  = myopt_get( mapopt, "pageno" );
	if (false == item.found_flag)
	{
		MYOPT_EXIT("pageno")
	}
	opt_data.page_no = item.vint[0];

	if (running)	
		free(running);


	return 0;
}


