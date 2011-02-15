/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: utils.c
 * Description: 
 */

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "utils.h"

int enable_socket_reuse(int sock)
{
	socklen_t len;
	int optval;

	optval = 1;
	len = sizeof(optval);
	return setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, len);
}

int enable_socket_broadcast(int sock) {
	socklen_t len;
	int optval;

	optval = 1;
	len = sizeof(optval);

	return setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, len);
}

int enable_socket_multicast(int sock) {
	struct ip_mreq mreq;
	memset(&mreq, 0, sizeof(struct ip_mreq));
	mreq.imr_multiaddr.s_addr = inet_addr("226.81.9.8");
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		fprintf(stderr, "setsockopt() IP_ADD_MEMBERSHIP error.\n");
		return -1;
	}
	return 0;
}

int setnonblocking(int sock)
{
    int opts;

	opts = fcntl(sock,F_GETFL);
    if(opts < 0)
    {
        printf("setnoblocking error...\n");
        perror("fcntl(sock,GETFL)");
		return -1;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
		return -1;
    }   
	return 0;
}

int modify_recv_buf(int sock, int recv_buf_size)
{
	return 0;
	return setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&recv_buf_size, sizeof(int));
}

int modify_send_buf(int sock, int send_buf_size)
{
	return 0;
	return setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char*)&send_buf_size, sizeof(int));
}

int get_cwd(char *buf, size_t size)
{
	int ret;
	char *p;
	bzero(buf, size);
	if((ret = readlink("/proc/self/exe", buf, size)) < 0)
		return -1;
	p = strrchr(buf, '/');
	if(!p)
		return -1;
	*p = '\0';
	return strlen(buf);
}


int lockfile(int fd)
{
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        return (fcntl(fd,F_SETLK,&fl));
}

#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
int try_save_pid(const char *filename)
{
	char buf[16];
	int fd;
	fd = open(filename, O_RDWR | O_CREAT, LOCKMODE);
	if(fd < 0)
	{
//		syslog("Can't open %s: %s", filename, strerror(errno));
		return -1;
	}
	if(lockfile(fd) < 0)
	{
		if(errno == EACCES || errno == EAGAIN)
		{
			//someone have lock this file
			bzero(buf, sizeof(buf));
			read(fd, buf, sizeof(buf));
			return atoi(buf);
		}
	}
	ftruncate(fd, 0);
	sprintf(buf, "%d", getpid());
	write(fd, buf, strlen(buf) + 1);	
	return 0;
}

uint32_t str2ip (const char *ip)
{
    uint32_t res;
    uint32_t p1;
    uint32_t p2;
    uint32_t p3;
    uint32_t p4;
    
    if(ip == NULL)
        return 0;
    sscanf (ip, "%u.%u.%u.%u", &p1, &p2, &p3, &p4);
    #ifdef DEBUG_UTILS
    printf ("%u %u %u %u\n", p1, p2, p3, p4);
    #endif
    res = (p1 << 24) | (p2 << 16) | (p3 << 8) | p4;
     return res;
}

char *ip2str (uint32_t ip, char *buf)
{
    if(buf == NULL)
    {
        return NULL;
    }
    uint8_t a = (ip & 0xff000000) >> 24;
    uint8_t b = (ip & 0x00ff0000) >> 16;
    uint8_t c = (ip & 0x0000ff00) >> 8;
    uint8_t d = (ip & 0x000000ff);
    sprintf(buf, "%u.%u.%u.%u", a, b, c, d);
    return buf;
}


char *my_ctime(time_t *tm) {
	char *p;
	char *q;

	p = ctime(tm);
	q = strchr(p, '\n');
	*q = '\0';
	return p;
}
