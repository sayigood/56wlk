/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: test_pack.cpp,v 1.1 2008/03/17 06:02:36 zhouzr Exp $
 *
 **************************************************************************/



/**
 * @file test_pack.c
 * @author lepton(wutao@baidu.com)
 * @date 2008/01/31 11:34:11
 * @version $Revision: 1.1 $
 * @brief
 *
 **/

#include <stdio.h>
#include "ul_log.h"
#include "ub.h"
#include "mc_pack.h"

void do_client(int fd)
{
	static unsigned int log_id=0;
	char buf[8192], tmp[256];
	int ret, i;
	char *err;
	nshead_t *req= (nshead_t *)buf;
	mc_pack_t *p, *p_a;
	req->id = 0;
	req->version = 1;
	req->log_id = log_id++;
	strcpy(req->provider, "test_client");
	req->magic_num = NSHEAD_MAGICNUM;
	p = mc_pack_create((char *) (req + 1), sizeof(buf)-sizeof(nshead_t));
	if (MC_PACK_PTR_ERR(p)){
		puts("create pack error");
		return;
	}
	ret=mc_pack_putf(p, "cmd:%s,msg:%s,uid:%u", "send", "hello!", 12345);
	if(ret!=3){
		puts("can't put pack item");
		return;
	}
	p_a=mc_pack_put_array(p, "msg_array");
	if(MC_PACK_PTR_ERR(p_a)){
		puts("create pack error");
		return;
	}
	for(i=0;i<10;i++){
		snprintf(tmp,sizeof(tmp), "hello%d", i);
		ret=mc_pack_put_str(p_a, NULL, tmp);
		if(ret<0){
			puts("pack array error!");
			return;
		}
	}
	mc_pack_close(p);
	req->body_len = mc_pack_get_size(p);
	ret=nshead_write(fd, req, 1000);
	if(ret<0)
		puts("send fail");
	else
		puts("send ok");

	ret=nshead_read(fd, req, sizeof(buf), 1000);
	if(ret<0)
		puts("can't read reply");
	else {
		if(!mc_pack_valid(p, req->body_len)){
			puts("return a failed packet");
			return;
		}
		ret=mc_pack_getf(p, "err:%p", &err);
		if(ret!=1){
			puts("parse reply error");
			return;
		}
		printf("return value:%s\n", err);
	}
	return;
}

void do_server(int fd)
{
	char buf[8192], tmp[8192];
	char *cmd, *msg;
	const char * msgi;
	unsigned int uid;
	int ret, c, i;
	nshead_t *res= (nshead_t *)buf;
	mc_pack_t *p = (mc_pack_t *)(res+1), *p_a;
	ret=nshead_read(fd, res, sizeof(buf), 1000);
	if(ret<0){
		puts("recv error!");
		return;
	}
	if(!mc_pack_valid(p, res->body_len)){
		puts("not a valid pack!");
		return;
	}
	ret=mc_pack_pack2str(p, tmp, sizeof(tmp));
	if(ret<0){
		puts("can't pack to str");
		return;
	}
	printf("%s\n", tmp);
	ret=mc_pack_getf(p, "cmd:%p,msg:%p,uid:%u", &cmd, &msg, &uid);
	if(ret!=3){
		puts("can't get all need item");
		return;
	}
	printf("cmd:%s msg: %s uid: %d\n",  cmd, msg, uid);
	p_a=mc_pack_get_array(p, "msg_array");
	if(MC_PACK_PTR_ERR(p_a)){
		puts("get arrary error");
		return;
	}
	c=mc_pack_get_item_count(p_a);
	for(i=0; i<c; i++){
		msgi=mc_pack_get_str_arr(p_a, i);
		if(MC_PACK_PTR_ERR(msgi)){
			printf("get arrary item %d error\n", i);
			return;
		}
		printf("msg %d:%s\n", i, msgi);
	}
	p = mc_pack_create((char *) (res + 1), sizeof(buf)-sizeof(nshead_t));
	if (MC_PACK_PTR_ERR(p)){
		puts("create pack error");
		return;
	}
	ret=mc_pack_putf(p, "err:%s", "ok");
	if(ret!=1){
		puts("can't put pack item");
		return;
	}
	mc_pack_close(p);
	res->body_len = mc_pack_get_size(p);
	ret=nshead_write(fd, res, 1000);
	if(ret<0)
		puts("send reply fail");
	else
		puts("send reply ok");
	return;
}

int main()
{
	int p[2];
	pid_t pid;

	if(socketpair(AF_UNIX, SOCK_STREAM, 0, p)<0){
		perror("can't pipe");
		return -1;
	}
	pid=fork();
	if(pid<0){
		perror("fork");
		return -1;
	}
	if(pid>0){
		close(p[1]);
		do_server(p[0]);
	} else {
		close(p[0]);
		do_client(p[1]);
	}
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
