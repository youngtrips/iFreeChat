/*
 * Author: youngtrips
 * Created Time:  2011-02-10
 * File Name: process_msg.c
 * Description: 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "process_msg.h"

handle_msg_func handle_msg_func_table[MAXN_FUNC];


int buddy_entry_func(ifreechat_t *ifc, msg_t *msg) {

	return 0;
}

int buddy_exit_func(ifreechat_t *ifc, msg_t *msg) {

	return 0;
}

//1_lbt4_12#128#001EEC0E0C0B#0#0#0:1297408327:ydf:YDF-4F61952E5CE:6291459:ydf
msg_t *parse_message(void *data, size_t size) {
	char *base;
	msg_t *msg;
	size_t cap;
	char *q;
	char *p;

	cap = 0;
	cap += 1 + size + 20;
	cap += sizeof(msg_t);
	base = (char*)malloc(cap);
	if (base == NULL)
		return NULL;

	p = (char*)data;
	msg = (msg_t*)base; base += sizeof(msg_t);

	//ip
	msg->ip = base;	base += 20;

	//version
	q = strchr(p, ':'); *q++ = '\0';
	msg->version = base;	base += 1 + strlen(p);
	strcpy(msg->version, p); p = q;

	//packet no
	q = strchr(p, ':'); *q++ = '\0';
	msg->packet_no = base; 	base += 1 + strlen(p);
	strcpy(msg->packet_no, p); p = q;

	//username
	q = strchr(p, ':'); *q++ = '\0';
	msg->username = base;	base += 1 + strlen(p);
	strcpy(msg->username, p); p = q;

	//hostname
	q = strchr(p, ':'); *q++ = '\0';
	msg->hostname = base;	base += 1 + strlen(p);
	strcpy(msg->hostname, p); p = q;

	//cmd
	q = strchr(p, ':'); *q++ = '\0';
	msg->cmd = base;	base += 1 + strlen(p);
	strcpy(msg->cmd, p); p = q;

	//data
	msg->data = base;
	strcpy(msg->data, p);

	return msg;
}


void process_message(ifreechat_t *ifc, char *ip, uint16_t port, 
		void *data, size_t size) {
	msg_t *msg;

	msg = parse_message(data, size);
	if (msg == NULL) {
		fprintf(stderr, "parse_message() error...\n");
		return;
	}
	strcpy(msg->ip, ip);
	msg->port = port;

	printf("[%s]\n", msg->version);
	printf("[%s]\n", msg->packet_no);
	printf("[%s]\n", msg->username);
	printf("[%s]\n", msg->hostname);
	printf("[%s]\n", msg->cmd);
	printf("[%s]\n", msg->data);
	printf("\n");

	free(msg);
}

