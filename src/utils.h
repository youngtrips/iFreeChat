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

int enable_socket_broadcast(int s);

int enable_socket_multicast(int s);

int setnonblocking(int s);


#endif
