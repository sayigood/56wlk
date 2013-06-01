/*
 */


#ifndef _MYNET_H
#define _MYNET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ul_log.h"

/**
 *@file
 *@brief 网络处理函数封装
 */

/**
 * @brief 带超时（微秒级）的tcp连接函数 
 * @param host : 连接主机名，不能为NULL
 * @param port : 连接主机监听端口
 * @param timeout : 最大超时时间（微妙）的指针，不能为NULL，连接成功timeout将被设置为剩余的时间
 * @param name : 连接模块名
 * @return -1 连接失败或超时
 * @return socket 连接成功的socket
 */
int MyConnectO(const char *host, int port, int *timeout, const char *name);

/**
 * @brief 带超时（微秒级）的socket读操作 
 * @param sock : 已连接的socket
 * @param buf  : 读入的空间
 * @param len  : 读入的最大长度
 * @param timeout  : 最大超时时间（微妙）的指针，不能为NULL，函数返回后timeout将被设置为剩余的时间
 * @param name : 连接模块名
 * @return -1 读失败
 * @return 实际读到的字符数
 */
int MyReadO(int sock, void *buf, ssize_t len, int *timeout, const char *name);

/**
* @brief 带超时（微秒级）的socket读操作，直到读到的内容中出现了stop串，或者读到len长度，或超时。 
* @param sock : 已连接的socket
* @param buf  : 读入的空间
* @param len  : 读入的最大长度
* @param timeout  : 最大超时时间（微妙）的指针，不能为NULL，函数返回后timeout将被设置为剩余的时间
* @param name : 连接模块名
* @param stop : 当读到内容中出现stop字串时，停止读
* @return -1 读失败
* @return 实际读到的字符数
*/
int MyReadOS(int sock, void *buf, ssize_t len, int *timeout, const char *name,
	     const char *stop);

/**
* @brief 带超时（微秒级）的socket写操作 
* @param sock : 已连接的socket
* @param buf  : 要写出的空间
* @param len  : 要写出的最大长度
* @param timeout  : 最大超时时间（微妙）的指针，不能为NULL，函数返回后timeout将被设置为剩余的时间
* @param name : 连接模块名
* @return -1 读失败
* @return 实际写出的字符数
*/
int MyWriteO(int sock, void *buf, ssize_t len, int *timeout,
	     const char *name);

/**
* @brief 关闭socket 
* @param fd : 已连接的socket
* @return -1 关闭失败
* @return 0 成功关闭
*/
int MyClose(int fd);

/**
* @brief 开启tcp监听端口 
* @param port : 端口
* @param queue  : listen等待队列长度
* @return -1 失败
* @return 成功返回开启监听的fd
*/
int MyTcpListen(int port, int queue);

#endif // _MYNET_H
/* vim: set ts=8 sw=4 sts=4 tw=78 noet: */
