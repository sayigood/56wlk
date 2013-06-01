/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: test_pack2.c,v 1.1 2008/03/17 06:02:36 zhouzr Exp $
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
#include <stdlib.h>
#include <string.h>
#include "mc_pack.h"

int main()
{
	char buf[8192], tmp[8192];
	mc_pack_t *mc=(mc_pack_t *)malloc(8192);
	mc_pack_t *p;
	int ret;
	p=mc_pack_create(buf, sizeof(buf));
	if(MC_PACK_PTR_ERR(p)){
		puts("faint");
		return -1;
	}
	ret=mc_pack_putf(p, "cmd:%s", "SET");
	mc_pack_close(p);
	printf("%d\n",mc_pack_get_size(p));
	memcpy(mc, p, mc_pack_get_size(p));
	mc_pack_pack2str(mc, tmp, sizeof(tmp));
	printf("%s\n", tmp);
	return 0;
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
