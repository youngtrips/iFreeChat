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

void update_contact_treeview(struct ifreechat_t *ifc, char *username, char *groupname) {
	struct user_t *user;
	user = add_user(ifc->ulist, username, "pixmaps/online.png",
			"test", "test", 
			"test", groupname);
	gdk_threads_enter();
	group_add_user(ifc, ifc->glist, user);
	gdk_threads_leave();
}


void handle_message(struct msg_t *msg, struct ifreechat_t *ifc) {
	
	unsigned int cmd;
	unsigned int pno;
	char *username;
	char *nickname;
	char *hostname;
	char *buf;
	char *encode;
	char *groupname;
	char *nickname_buf;
	char *groupname_buf;
	int i;
	int j;

	buf = msg->data;
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
		update_contact_treeview(ifc, nickname, 
				strlen(groupname) == 0 ? "No Group" : groupname);
	}
}
