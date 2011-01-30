/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-28
 * File Name: ui_update_thread.c
 * Description: 
 *
 */

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#include "ipmsg.h"
#include "udp_socket.h"
#include "gtk_common.h"
#include "recv_msg.h"
#include "msg.h"

#include "ifreechat.h"

#include "user.h"
#include "group.h"

void handle_message(struct msg_t *msg, struct ifreechat_t *ifc);
void online_broadcast(struct udp_socket_t *usock);

void *recv_msg_loop(void *arg) {
	struct ifreechat_t *ifreechat;
	struct udp_socket_t *usock;
	struct msg_t *msg;

	ifreechat = (struct ifreechat_t*)arg;
	usock = (struct udp_socket_t*)(ifreechat->usock);

	online_broadcast(usock);

	for(;;) {
		pthread_mutex_lock(&(usock->mutex));
		pthread_cond_wait(&(usock->cond), &(usock->mutex));
		pthread_mutex_unlock(&(usock->mutex));

		if (usock->shutdown == 1) {
			break;
		}

		if (udp_recv(usock, &msg) < 0) {
			continue;
		}
		
//		printf("from: %s:%u\n", msg->ip, msg->port);
//		printf("msg_size: %u\n", msg->size);
//		printf("msg_data: %s\n", msg->data);
//		printf("\n");

		handle_message(msg, ifreechat);
//		free(msg->data);
//		free(msg);
	}
	pthread_exit(0);
}


void start_recv_msg(struct ifreechat_t *ifc) {
	
	pthread_create(&(ifc->recv_msg_tid), NULL,
			recv_msg_loop, (void*)ifc);
}

void stop_recv_msg(struct ifreechat_t *ifc) {
	
	pthread_join(ifc->recv_msg_tid, NULL);
}



void online_broadcast(struct udp_socket_t *usock) {
	struct msg_t *msg;
	char buf[8192];

	printf("post online message...\n");

	msg = (struct msg_t*)malloc(sizeof(struct msg_t));
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", 1);
	sprintf(buf + strlen(buf), ":%lu", time(NULL));
	sprintf(buf + strlen(buf), ":%s", "pc119");
	sprintf(buf + strlen(buf), ":%s", "pc119");
	sprintf(buf + strlen(buf), ":%u", 0x101);
	sprintf(buf + strlen(buf), ":");

	msg->size = strlen(buf);
	msg->data = (char*)malloc(msg->size + 1);
	memcpy(msg->data, buf, msg->size);

	strcpy(msg->ip, "255.255.255.255");
	msg->port = 2425;

	udp_send(usock, msg);
}


const char *skip_section(const char *string, char ch, unsigned char times)
{
	const char *ptr;
	unsigned char count;

	ptr = string;
	count = 0;
	while (count < times) {
		if (!(ptr = strchr(ptr, ch)))
			break;
		ptr++;
		count++;
	}

	return ptr;
}

unsigned int get_dec_number(const char *msg, char ch, unsigned char times)
{
	const char *ptr;
	unsigned int number;

	if (!(ptr = skip_section(msg, ch, times)))
		return 0;
	if (sscanf(ptr, "%u" , &number) == 1)
		return number;
	return 0;
}

char *get_section_string(const char *msg, char ch, unsigned char times)
{
	const char *pptr, *ptr;
	char *string;
	size_t len;

	if (!(pptr = skip_section(msg, ch, times)))
		return NULL;
	ptr = strchr(pptr, ch);
	if ((len = ptr ? ptr - pptr : strlen(pptr)) == 0)
		return NULL;
	string = g_strndup(pptr, len);

	return string;
}

void update_contact_treeview(struct ifreechat_t *ifc, 
								char *username, 
								char *groupname,
								char *ip_addr,
								char *mac,
								int avatar) {
	struct user_t *user;
	char avatar_file[128];
	
	if (avatar == 0) {
		strcpy(avatar_file, "pixmaps/avatar/default.png");
	} else {
		sprintf(avatar_file, "pixmaps/avatar/%d.bmp", avatar);
	}

	user = add_user(ifc->ulist, username, avatar_file,
			ip_addr, mac, 
			"test", groupname);
	gdk_threads_enter();
	group_add_user(ifc, ifc->glist, user);
	gdk_threads_leave();
}


int get_avatar_no(const char *version) {

	char *p;
	char *q;
	int res;

	p = strchr(version, '_'); p++;
	q = strchr(p, '_'); q++;
	for(res = 0;*q != '#'; q++) {
		res = res * 10 + (*q - '0');
	}
	return res;
}

char *get_mac(const char *version, char *buf) {
	char *p;
	char *q;
	char *o;
	p = strchr(version, '#'); p++;
	q = strchr(p, '#'); q++;

	for(o = buf;*q != '#'; o++, q++)
		*o = *q;
	*o = '\0';
	return buf;
}



void handle_message(struct msg_t *msg, struct ifreechat_t *ifc) {
	
	unsigned int cmd;
	unsigned int pno;
	char *version;
	char *username;
	char *nickname;
	char *hostname;
	char *buf;
	char *encode;
	char *groupname;
	char *nickname_buf;
	char *groupname_buf;
	char mac[20];
	int avatar;
	int i;
	int j;

	buf = msg->data;
	version = get_section_string(buf, ':', 0);
	avatar = get_avatar_no(version);
	get_mac(version, mac);
	printf("mac: %s\n", mac);
	
	pno = get_dec_number(buf, ':', 1);
	username = get_section_string(buf, ':', 2);
	hostname = get_section_string(buf, ':', 3);
	cmd = get_dec_number(buf, ':', 4);

	nickname_buf = get_section_string(buf, ':', 5);
	groupname_buf = strrchr(msg->data, ':') + strlen(nickname_buf) + 2;

	nickname = string_validate(nickname_buf, "gbk", &encode);
	if (nickname == NULL) 
		nickname = nickname_buf;

	groupname = string_validate(groupname_buf, "gbk", &encode);
	if (groupname == NULL)
		groupname = groupname_buf;

	if (cmd & 0x3) {

		printf("nickanme: %s\n", nickname);
		printf("groupname: %s\n", groupname);
		update_contact_treeview(ifc, 
				nickname, 
				strlen(groupname) == 0 ? "No Group" : groupname,
				msg->ip, 
				mac,
				avatar);
	}
}




