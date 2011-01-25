/*
 * Author: youngtrips
 * Created Time:  Tue 25 Jan 2011 11:12:05 AM CST
 * File Name: utils.h
 * Description: 
 */



#ifndef __UTILS_H
#define __UTILS_H


int modify_send_buf(int sock, int recv_buf_size);

int modify_recv_buf(int sock, int recv_buf_size);

uint32_t str2ip(const char *ip);

char *ip2str(uint32_t ip, char *buf);

#endif
