/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file nshead.cpp
 * @brief 
 *  
 **/

#include "nshead.h"
#include <ul_net.h>
#include <ul_log.h>

static const char *nshead_errstr[] = {
	"NSHEAD_RET_SUCCESS",
	"NSHEAD_RET_EPARAM",
	"NSHEAD_RET_EBODYLEN",
	"NSHEAD_RET_WRITE",
	"NSHEAD_RET_READ",
	"NSHEAD_RET_READHEAD",
	"NSHEAD_RET_WRITEHEAD",
	"NSHEAD_RET_PEARCLOSE",
	"NSHEAD_RET_ETIMEDOUT",
	"NSHEAD_RET_EMAGICNUM",
	"NSHEAD_RET_UNKNOWN",
};

const char* nshead_error(int ret)
{
	if (ret > 0 || ret < 1 - (int)(sizeof(nshead_errstr) / sizeof(char*)))
		return nshead_errstr[-NSHEAD_RET_UNKNOWN];
	return nshead_errstr[-ret];
}

static int ns_read(int sock,void *buf,unsigned int len, int timeout)
{
	int rlen = ul_sreado_ms_ex(sock, buf, len, timeout);
	if (rlen == 0){
		return NSHEAD_RET_PEARCLOSE;
	}
	else if (rlen == -1 && errno == ETIMEDOUT){
		return NSHEAD_RET_ETIMEDOUT;
	}
	else if (rlen != (int)len){
		return NSHEAD_RET_READ;
	}
	return NSHEAD_RET_SUCCESS;
}

static int ns_write(int sock,void *buf,unsigned int len,int timeout)
{
	int wlen = ul_swriteo_ms_ex(sock, buf, len, timeout);
	if (wlen == 0){
		return NSHEAD_RET_PEARCLOSE;
	}
	else if (wlen == -1 && errno == ETIMEDOUT){
		return NSHEAD_RET_ETIMEDOUT;
	}
	else if (wlen != (int)len) {
		return NSHEAD_RET_WRITE;
	}
	return NSHEAD_RET_SUCCESS;
}

/* ========================== */
/* Read Part                  */
/* ========================== */

int
nshead_read(int sock, void *buf, size_t buf_size, int timeout, unsigned flags)
{
	if (buf_size < sizeof(nshead_t))
		return NSHEAD_RET_EPARAM;

	return nshead_read(sock, (nshead_t*)buf,
			(void*)((nshead_t*)buf + 1), 0,
			(void*)((nshead_t*)buf + 1), buf_size - sizeof(nshead_t),
			timeout, flags);
}

int
nshead_read(int sock, nshead_t *head, void *buf,
		size_t buf_size, int timeout,
		unsigned flags)
{
	return nshead_read(sock, head, buf, 0, buf, buf_size,
			timeout, flags);
}

int
nshead_read(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, size_t buf_size, int timeout,
		unsigned flags)
{
	int rlen;

	if (flags & NSHEAD_CHECK_PARAM) {
		/* 奇怪的检查们 */
		if (sock < 0 || !head)
			return NSHEAD_RET_EPARAM;
	}

	/* 读入 NSHEAD 头 */
	rlen = ul_sreado_ms_ex(sock, head, sizeof(nshead_t), timeout);
	if (rlen <= 0)
		goto nshead_read_fail;
	else if (rlen != sizeof(nshead_t)) {
		ul_writelog(UL_LOG_WARNING,
				"<%u> nshead_read head fail: ret %d want %u ERR[%m]",
				head->log_id, rlen, (unsigned int)req_size);
		return NSHEAD_RET_READHEAD;
	}

	/* 检查 Magic Number */
	if (flags & NSHEAD_CHECK_MAGICNUM) {
		if (head->magic_num != NSHEAD_MAGICNUM) {
			ul_writelog(UL_LOG_WARNING,
					"<%u> nshead_read magic num mismatch: ret %x want %x",
					head->log_id, head->magic_num, NSHEAD_MAGICNUM);
			return NSHEAD_RET_EMAGICNUM;
		}
	}

	/* 对 body 的长度进行检查, 过长和过短都不符合要求 */
	if (head->body_len < req_size || head->body_len - req_size > buf_size) {
		ul_writelog(UL_LOG_WARNING,
				"<%u> nshead_read body_len error: req_size=%u buf_size=%u body_len=%u",
				head->log_id, (unsigned int)req_size, (unsigned int)buf_size, head->body_len);
		return NSHEAD_RET_EBODYLEN;
	}

	/* Read Request */
	if (req_size > 0) {
		rlen = ul_sreado_ms_ex(sock, req, req_size, timeout);
		if (rlen <= 0)
			goto nshead_read_fail;
		else if (rlen != (int)req_size) {
			ul_writelog(UL_LOG_WARNING,
					"<%u> nshead_read fail: ret %d want %u ERR[%m]",
					head->log_id, rlen, (unsigned int)req_size);
			return NSHEAD_RET_READ;
		}
	}

	/* Read Detail */
	if (head->body_len - req_size > 0) {
		rlen = ul_sreado_ms_ex(sock, buf, head->body_len - req_size, timeout);
		if (rlen <= 0)
			goto nshead_read_fail;
		else if (rlen != (int)(head->body_len - req_size)) {
			ul_writelog(UL_LOG_WARNING,
					"<%u> nshead_read fail: ret %d want %d ERR[%m]",
					head->log_id, rlen, int(head->body_len - req_size));
			return NSHEAD_RET_READ;
		}
	}

	return NSHEAD_RET_SUCCESS;

nshead_read_fail:
	if (rlen == 0)
		return NSHEAD_RET_PEARCLOSE;

	ul_writelog(UL_LOG_WARNING,"<%u> nshead_read fail: ret=%d ERR[%m]",head->log_id, rlen);
	if (rlen == -1 && errno == ETIMEDOUT)
		return NSHEAD_RET_ETIMEDOUT;
	else
		return NSHEAD_RET_READ;
}

int
nshead_read_ex(int sock, void *buf, size_t buf_size, int timeout, unsigned flags)
{
	if (buf_size < sizeof(nshead_t))
		return NSHEAD_RET_EPARAM;

	return nshead_read_ex(sock, (nshead_t*)buf,
			(void*)((nshead_t*)buf + 1), 0,
			(void*)((nshead_t*)buf + 1), buf_size - sizeof(nshead_t),
			timeout, flags);
}

int
nshead_read_ex(int sock, nshead_t *head, void *buf,
		size_t buf_size, int timeout,
		unsigned flags)
{
	return nshead_read_ex(sock, head, buf, 0, buf, buf_size,
			timeout, flags);
}

int
nshead_read_ex(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, size_t buf_size, int timeout,
		unsigned flags)
{
	int rlen;

	if (flags & NSHEAD_CHECK_PARAM) {
		/* 奇怪的检查们 */
		if (sock < 0 || !head)
			return NSHEAD_RET_EPARAM;
	}

	/* 读入 NSHEAD 头 */
	rlen = ul_sreado_ms_ex2(sock, head, sizeof(nshead_t), timeout);
	if (rlen <= 0)
		goto nshead_read_fail;
	else if (rlen != sizeof(nshead_t)) {
		ul_writelog(UL_LOG_WARNING,
				"<%u> nshead_read_ex head fail: ret %d want %u ERR[%m]",
				head->log_id, rlen, (unsigned int)req_size);
		return NSHEAD_RET_READHEAD;
	}

	/* 检查 Magic Number */
	if (flags & NSHEAD_CHECK_MAGICNUM) {
		if (head->magic_num != NSHEAD_MAGICNUM) {
			ul_writelog(UL_LOG_WARNING,
					"<%u> nshead_read_ex magic num mismatch: ret %x want %x",
					head->log_id, head->magic_num, NSHEAD_MAGICNUM);
			return NSHEAD_RET_EMAGICNUM;
		}
	}

	/* 对 body 的长度进行检查, 过长和过短都不符合要求 */
	if (head->body_len < req_size || head->body_len - req_size > buf_size) {
		ul_writelog(UL_LOG_WARNING,
				"<%u> nshead_read_ex body_len error: req_size=%u buf_size=%u body_len=%u",
				head->log_id, (unsigned int)req_size, (unsigned int)buf_size, head->body_len);
		return NSHEAD_RET_EBODYLEN;
	}

	/* Read Request */
	if (req_size > 0) {
		rlen = ul_sreado_ms_ex2(sock, req, req_size, timeout);
		if (rlen <= 0)
			goto nshead_read_fail;
		else if (rlen != (int)req_size) {
			ul_writelog(UL_LOG_WARNING,
					"<%u> nshead_read_ex fail: ret %d want %u ERR[%m]",
					head->log_id, rlen, (unsigned int)req_size);
			return NSHEAD_RET_READ;
		}
	}

	/* Read Detail */
	if (head->body_len - req_size > 0) {
		rlen = ul_sreado_ms_ex2(sock, buf, head->body_len - req_size, timeout);
		if (rlen <= 0)
			goto nshead_read_fail;
		else if (rlen != (int)(head->body_len - req_size)) {
			ul_writelog(UL_LOG_WARNING,
					"<%u> nshead_read_ex fail: ret %d want %d ERR[%m]",
					head->log_id, rlen, int(head->body_len - req_size));
			return NSHEAD_RET_READ;
		}
	}

	return NSHEAD_RET_SUCCESS;

nshead_read_fail:
	if (rlen == 0)
		return NSHEAD_RET_PEARCLOSE;

	ul_writelog(UL_LOG_WARNING,"<%u> nshead_read_ex fail: ret=%d ERR[%m]",head->log_id, rlen);
	if (rlen == -1 && errno == ETIMEDOUT)
		return NSHEAD_RET_ETIMEDOUT;
	else
		return NSHEAD_RET_READ;
}


int nshead_readv_ex(int sock,nshead_t *head,iovec_t *iovs,int count,int timeout,unsigned flags)
{
	if (sock < 0 || head == NULL) {
		return NSHEAD_RET_READ;
	}
	int rlen = ul_sreado_ms_ex2(sock, head, sizeof(nshead_t), timeout);
	int res = NSHEAD_RET_SUCCESS;
	if (rlen == 0){
		res = NSHEAD_RET_PEARCLOSE;
	} else if (rlen == -1 && errno == ETIMEDOUT){
		res = NSHEAD_RET_ETIMEDOUT;
	} if (rlen != (int)sizeof(nshead_t)) {
		res = NSHEAD_RET_READHEAD;
	}
	if (res != NSHEAD_RET_SUCCESS) {
		ul_writelog(UL_LOG_WARNING,"nshead_readv_ex head error,%s,%m",nshead_error(res));	
		return res;
	}
	if (flags & NSHEAD_CHECK_MAGICNUM) {
		if (head->magic_num != NSHEAD_MAGICNUM) {
			ul_writelog(UL_LOG_WARNING,"<%u> nshead_readv_ex magic num mismatch: ret %x want %x",
					head->log_id, head->magic_num, NSHEAD_MAGICNUM);
			return NSHEAD_RET_EMAGICNUM;
		}
	}

	if (head->body_len == 0){
		return NSHEAD_RET_SUCCESS;
	}

	//head->body_len > 0 时，后续参数要安全
	if(iovs == NULL || count <= 0){
		ul_writelog(UL_LOG_WARNING,"<%u> nshead_readv_ex param error,iovs is null or count <= 0,body_len %u",
				head->log_id,head->body_len);
		return NSHEAD_RET_EPARAM;
	}

	unsigned int len = 0;
	for (int i = 0; i < count; i++){
		if (iovs[i].iov_base){
			len += iovs[i].iov_len;
		}
	}
	//保证空间总长度>head->body_len
	if (len < head->body_len){
		ul_writelog(UL_LOG_WARNING,"<%u> nshead_readv_ex param error,[body_len:%u > iovslen:%u]",
				head->log_id,head->body_len,len);
		return NSHEAD_RET_EBODYLEN;
	}
	res = ul_sreadv_ms(sock, iovs, count, timeout);
	if (res == 0){
		return NSHEAD_RET_PEARCLOSE;
	}
	else if (res == -1 && errno == ETIMEDOUT){
		return NSHEAD_RET_ETIMEDOUT;
	}
	return NSHEAD_RET_SUCCESS;
}

int nshead_readv(int sock,nshead_t *head,iovec_t *iovs,int count,int timeout,unsigned flags)
{
	if (sock < 0 || head == NULL){
		return NSHEAD_RET_EPARAM;
	}
	int res = ns_read(sock, head, sizeof(nshead_t), timeout);
	if (res != NSHEAD_RET_SUCCESS){
		ul_writelog(UL_LOG_WARNING,"nshead_readv_ex head error,%s,%m",nshead_error(res));
		return res;
	}
	if (flags & NSHEAD_CHECK_MAGICNUM) {
		if (head->magic_num != NSHEAD_MAGICNUM) {
			ul_writelog(UL_LOG_WARNING,"<%u> nshead_readv_ex magic num mismatch: ret %x want %x",
					head->log_id, head->magic_num, NSHEAD_MAGICNUM);
			return NSHEAD_RET_EMAGICNUM;
		}
	}

	if (head->body_len == 0){
		return NSHEAD_RET_SUCCESS;
	}

	//head->body_len > 0 时，后续参数要安全
	if(iovs == NULL || count <= 0){
		ul_writelog(UL_LOG_WARNING,"<%u> nshead_readv_ex param error,iovs is null or count <= 0,body_len %u",
				head->log_id,head->body_len);
		return NSHEAD_RET_EPARAM;
	}

	unsigned int len = 0;
	for (int i = 0; i < count; i++){
		if (iovs[i].iov_base){
			len += iovs[i].iov_len;
		}
	}
	//保证空间总长度>head->body_len
	if (len < head->body_len){
		ul_writelog(UL_LOG_WARNING,"<%u> nshead_readv_ex param error,[body_len:%u > iovslen:%u]",
				head->log_id,head->body_len,len);
		return NSHEAD_RET_EBODYLEN;
	}

	unsigned int leave_len = head->body_len;
	for (int i = 0; i < count; i++){
		if (iovs[i].iov_base == NULL || iovs[i].iov_len == 0){
			continue;
		}
		len = (leave_len > iovs[i].iov_len)?iovs[i].iov_len:leave_len;	//取小的
		res = ns_read(sock,iovs[i].iov_base,len,timeout);
		if (res != NSHEAD_RET_SUCCESS){
			ul_writelog(UL_LOG_WARNING,"<%u> nshead_readv_ex error,[i:%d,len:%u],%s,%m",head->log_id,i,len,nshead_error(res));
			return res;
		}
		leave_len -= len;
		if (leave_len <= 0){
			break;
		}
	}
	return NSHEAD_RET_SUCCESS;
}

/* ======================== */
/* Write Part               */
/* ======================== */
int
nshead_write_ex(int sock, nshead_t *head, int timeout,
		unsigned flags)
{
	if (flags & NSHEAD_CHECK_PARAM) {
		/* 奇怪的检查们 */
		if (sock < 0 || !head)
			return NSHEAD_RET_EPARAM;
	}
	if (flags & NSHEAD_CHECK_MAGICNUM) {
		head->magic_num = NSHEAD_MAGICNUM;
	}
	int res = ul_swriteo_ms_ex2(sock, head, sizeof(nshead_t) + head->body_len, timeout);

	if (res > 0) {
		return NSHEAD_RET_SUCCESS;
	}
	ul_writelog(UL_LOG_WARNING,
			"<%u> nshead_write_ex fail: ret=%d ERR[%m]", head->log_id, res);
	if (res == 0)
		return NSHEAD_RET_PEARCLOSE;
	else if (res == -1 && errno == ETIMEDOUT)
		return NSHEAD_RET_ETIMEDOUT;
	
	return NSHEAD_RET_WRITE;
}

int
nshead_write_ex(int sock, nshead_t *head, void *buf, int timeout,
		unsigned flags)
{
	if (flags & NSHEAD_CHECK_PARAM) {
		/* 奇怪的检查们 */
		if (sock < 0 || !head)
			return NSHEAD_RET_EPARAM;
	}
	if (flags & NSHEAD_CHECK_MAGICNUM) {
		head->magic_num = NSHEAD_MAGICNUM;
	}
	
	struct iovec ve[2];
	ve[0].iov_base = head;
	ve[0].iov_len = sizeof(nshead_t);
	ve[1].iov_base = buf;
	ve[1].iov_len = head->body_len;
		
	int res = ul_swritev_ms(sock, ve, 2, timeout);
	if (res > 0) {
		return NSHEAD_RET_SUCCESS;
	}
	ul_writelog(UL_LOG_WARNING,
			"<%u> nshead_write_ex fail: ret=%d ERR[%m]", head->log_id, res);
	if (res == 0)
		return NSHEAD_RET_PEARCLOSE;
	else if (res == -1 && errno == ETIMEDOUT)
		return NSHEAD_RET_ETIMEDOUT;
	
	return NSHEAD_RET_WRITE;
}

int
nshead_write_ex(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, int timeout, unsigned flags)
{
	if (flags & NSHEAD_CHECK_PARAM) {
		/* 奇怪的检查们 */
		if (sock < 0 || !head)
			return NSHEAD_RET_EPARAM;
	}
	/* 校验 size */
	if (head->body_len < req_size) {
		ul_writelog(UL_LOG_WARNING,
				"<%u> nshead_write_ex body_len error: req_size=%u body_len=%u",
				head->log_id, (unsigned int)req_size, head->body_len);
		return NSHEAD_RET_EBODYLEN;
	}

	if (flags & NSHEAD_CHECK_MAGICNUM)
		head->magic_num = NSHEAD_MAGICNUM;

	struct iovec ve[3];
	ve[0].iov_base = head;
	ve[0].iov_len = sizeof(nshead_t);
	ve[1].iov_base = req;
	ve[1].iov_len = req_size;
	ve[2].iov_base = buf;
	ve[2].iov_len = head->body_len - req_size;
	int res = ul_swritev_ms(sock, ve, 3, timeout);
	if (res > 0) {
		return NSHEAD_RET_SUCCESS;
	}
	ul_writelog(UL_LOG_WARNING,
			"<%u> nshead_write_ex fail: ret=%d ERR[%m]", head->log_id, res);
	if (res == 0)
		return NSHEAD_RET_PEARCLOSE;
	else if (res == -1 && errno == ETIMEDOUT)
		return NSHEAD_RET_ETIMEDOUT;
	
	return NSHEAD_RET_WRITE;
}


int
nshead_write(int sock, nshead_t *head, int timeout,
		unsigned flags)
{
	return nshead_write(sock, head,
			(void*)(head + 1), 0,
			(void*)(head + 1), timeout, flags);
}

int
nshead_write(int sock, nshead_t *head, void *buf, int timeout,
		unsigned flags)
{
	return nshead_write(sock, head, buf, 0, buf, timeout, flags);
}

int
nshead_write(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, int timeout, unsigned flags)
{
	int wlen;

	if (flags & NSHEAD_CHECK_PARAM) {
		/* 奇怪的检查们 */
		if (sock < 0 || !head)
			return NSHEAD_RET_EPARAM;
	}

	/* 校验 size */
	if (head->body_len < req_size) {
		ul_writelog(UL_LOG_WARNING,
				"<%u> nshead_write body_len error: req_size=%u body_len=%u",
				head->log_id, (unsigned int)req_size, head->body_len);
		return NSHEAD_RET_EBODYLEN;
	}

	if (flags & NSHEAD_CHECK_MAGICNUM)
		head->magic_num = NSHEAD_MAGICNUM;

	wlen = ul_swriteo_ms_ex(sock, head, sizeof(nshead_t), timeout);
	if (wlen <= 0)
		goto nshead_write_fail;
	else if (wlen != sizeof(nshead_t)) {
		ul_writelog(UL_LOG_WARNING,
				"<%u> nshead_write head fail: ret %d want %u ERR[%m]",
				head->log_id, wlen, (unsigned int)sizeof(nshead_t));
		return NSHEAD_RET_WRITEHEAD;
	}

	if (req_size > 0) {
		wlen = ul_swriteo_ms_ex(sock, req, req_size, timeout);
		if (wlen <= 0)
			goto nshead_write_fail;
		else if (wlen != (int)req_size) {
			ul_writelog(UL_LOG_WARNING,
					"<%u> nshead_write fail: ret %d want %u ERR[%m]",
					head->log_id, wlen, (unsigned int)req_size);
			return NSHEAD_RET_WRITE;
		}
	}

	if (head->body_len - req_size > 0) {
		wlen = ul_swriteo_ms_ex(sock, buf, head->body_len - req_size, timeout);
		if (wlen <= 0)
			goto nshead_write_fail;
		else if (wlen != (int)(head->body_len - req_size)) {
			ul_writelog(UL_LOG_WARNING,
					"<%u> nshead_write fail: ret %d want %u ERR[%m]",
					head->log_id, wlen, (unsigned int)req_size);
			return NSHEAD_RET_WRITE;
		}
	}

	return NSHEAD_RET_SUCCESS;

nshead_write_fail:
	ul_writelog(UL_LOG_WARNING,
			"<%u> nshead_write fail: ret=%d ERR[%m]",
			head->log_id, wlen);
	if (wlen == 0)
		return NSHEAD_RET_PEARCLOSE;
	else if (wlen == -1 && errno == ETIMEDOUT)
		return NSHEAD_RET_ETIMEDOUT;
	else
		return NSHEAD_RET_WRITE;
}

int nshead_writev_ex(int sock,nshead_t *head,iovec_t *iovs,int count,int timeout,unsigned flags)
{
	if (sock < 0 || head == NULL){
		return NSHEAD_RET_EPARAM;
	}
	unsigned int len = 0;
	if (head->body_len > 0){
		if(iovs == NULL || count <= 0){
			ul_writelog(UL_LOG_WARNING,"<%u> nshead_writev_ex param error,iovs null or count <= 0,body_len %u",
					head->log_id,head->body_len);
			return NSHEAD_RET_EPARAM;
		}

		for (int i = 0; i < count; i++){
			if (iovs[i].iov_base){
				len += iovs[i].iov_len;
			}
		}
		if (len != head->body_len){
			ul_writelog(UL_LOG_WARNING,"<%u> nshead_writev_ex param error,[body_len:%u,iovslen:%u]",
					head->log_id,head->body_len,len);
			return NSHEAD_RET_EBODYLEN;
		}
	}
	if (flags & NSHEAD_CHECK_MAGICNUM){
		head->magic_num = NSHEAD_MAGICNUM;
	}
	int wlen = ul_swriteo_ms_ex2(sock, head, sizeof(nshead_t), timeout);
	int res = NSHEAD_RET_SUCCESS;
	if (wlen == 0){ 
		res = NSHEAD_RET_PEARCLOSE;
	}   
	else if (wlen == -1 && errno == ETIMEDOUT){
		res = NSHEAD_RET_ETIMEDOUT;
	}   
	else if (wlen != (int)sizeof(nshead_t)) {
		res = NSHEAD_RET_WRITE;
	}   
	if (res != NSHEAD_RET_SUCCESS){
		ul_writelog(UL_LOG_WARNING,"<%u> nshead_writev_ex head error,%s,%m",head->log_id,nshead_error(res));
		return res;
	}
	if (head->body_len == 0){
		return NSHEAD_RET_SUCCESS;
	}
	res = ul_swritev_ms(sock, iovs, count, timeout);
	if (res == 0){
		return NSHEAD_RET_PEARCLOSE;
	}
	else if (res == -1 && errno == ETIMEDOUT){
		return NSHEAD_RET_ETIMEDOUT;
	}
	else if (res < 0) {
		return NSHEAD_RET_WRITE;
	}
	return NSHEAD_RET_SUCCESS;
}


int nshead_writev(int sock,nshead_t *head,iovec_t *iovs,int count,int timeout,unsigned flags)
{
	if (sock < 0 || head == NULL){
		return NSHEAD_RET_EPARAM;
	}
	unsigned int len = 0;
	if (head->body_len > 0){
		if(iovs == NULL || count <= 0){
			ul_writelog(UL_LOG_WARNING,"<%u> nshead_writev param error,iovs null or count <= 0,body_len %u",
					head->log_id,head->body_len);
			return NSHEAD_RET_EPARAM;
		}

		for (int i = 0; i < count; i++){
			if (iovs[i].iov_base){
				len += iovs[i].iov_len;
			}
		}
		if (len != head->body_len){
			ul_writelog(UL_LOG_WARNING,"<%u> nshead_writev param error,[body_len:%u,iovslen:%u]",
					head->log_id,head->body_len,len);
			return NSHEAD_RET_EBODYLEN;
		}
	}
	if (flags & NSHEAD_CHECK_MAGICNUM){
		head->magic_num = NSHEAD_MAGICNUM;
	}
	int res = ns_write (sock,head,sizeof(nshead_t),timeout);
	if (res != NSHEAD_RET_SUCCESS){
		ul_writelog(UL_LOG_WARNING,"<%u> nshead_writev head error,%s,%m",head->log_id,nshead_error(res));
		return res;
	}
	if (head->body_len == 0){
		return NSHEAD_RET_SUCCESS;
	}
	for (int i = 0; i < count; i++){
		if (iovs[i].iov_base == NULL || iovs[i].iov_len == 0){
			continue;
		}
		res = ns_write (sock,iovs[i].iov_base,iovs[i].iov_len,timeout);
		if (res != NSHEAD_RET_SUCCESS){
			ul_writelog(UL_LOG_WARNING,"<%u> nshead_writev error,[i:%d,len:%u],%s,%m",
					head->log_id,i,(unsigned int)iovs[i].iov_len,nshead_error(res));
			return res;
		}
	}
	return NSHEAD_RET_SUCCESS;
}


//下面的接口都是为纯C程序提供的接口

const char* nshead_error_c(int ret)
{
	return nshead_error(ret);
}

int nshead_read_c(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, size_t buf_size, int timeout,
		unsigned flags)
{
	return nshead_read(sock, head, req, req_size, buf, buf_size, timeout, flags);
}

int nshead_readv_c(int sock, 
		nshead_t *head, 
		iovec_t *iovs, int iov_count, 
		int timeout, unsigned flags)
{
	return nshead_readv(sock, head, iovs, iov_count, timeout, flags);
}

int nshead_write_c(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, int timeout, unsigned flags)
{
	return nshead_write(sock, head, req, req_size, buf, timeout, flags);	
}


int nshead_writev_c(int sock, 
		nshead_t *head, 
		iovec_t *iovs, int iov_count, 
		int timeout, unsigned flags)
{
	return nshead_writev(sock, head, iovs, iov_count, timeout, flags);
}

int nshead_read_ex_c(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, size_t buf_size, int timeout,
		unsigned flags)
{
	return nshead_read_ex(sock, head, req, req_size, buf, buf_size, timeout, flags);
}

int nshead_readv_ex_c(int sock, 
		nshead_t *head, 
		iovec_t *iovs, int iov_count, 
		int timeout, unsigned flags)
{
	return nshead_readv_ex(sock, head, iovs, iov_count, timeout, flags);
}

int nshead_write_ex_c(int sock, nshead_t *head, void *req, size_t req_size,
		void *buf, int timeout, unsigned flags)
{
	return nshead_write_ex(sock, head, req, req_size, buf, timeout, flags);	
}


int nshead_writev_ex_c(int sock, 
		nshead_t *head, 
		iovec_t *iovs, int iov_count, 
		int timeout, unsigned flags)
{
	return nshead_writev_ex(sock, head, iovs, iov_count, timeout, flags);
}


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
