/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: utils.h
 * Description: 
 *
 */

#ifndef __UTILS_H
#define __UTILS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "socket.h"

int enable_socket_reuse(int sock);

int enable_socket_broadcast(int s);

int enable_socket_multicast(int s);

int setnonblocking(int s);

uint32_t str2ip (const char *ip);

char *ip2str (uint32_t ip, char *buf);

char *my_ctime(time_t *tm);
	
#endif
