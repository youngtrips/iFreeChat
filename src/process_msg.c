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
#include <pthread.h>
#include <time.h>

#include "gtk_common.h"
#include "dlist.h"
#include "process_msg.h"
#include "user.h"
#include "blowfish.h"

handle_msg_func handle_msg_func_table[MAXN_FUNC];

int get_avatar_id_from_version(const char *version) {
	char *p;
	if (0 != strncmp(version, "1_lbt", 5)) {
		return 0;
	}
	p = strrchr(version, '_'); p++;
	return atoi(p);
}

void get_mac_from_version(const char *version, char *mac) {
	char *p;
	char *q;
	if (0 != strncmp(version, "1_lbt", 5)) {
		strcpy(mac, "000000000000");
		return;
	}
	p = strchr(version, '#'); p++;
	q = strchr(p, '#'); q++;
	for(p = mac;*q != '#'; p++, q++) {
		*p = *q;
	}
	*p = '\0';
}

int on_buddy_entry(ifreechat_t *ifc, msg_t *msg) {
	const char *no_group = "no group";
	const char *my_group = "my self";
	char avatar[64];
	char macaddr[20];
	char *encode;
	char *nickname;
	char *category;
	char *p;
	int avatar_id;
	uint32_t cmd;
	user_t *user;
	char buf[1024];
	dlist_t *plist;

	if (!strncmp(msg->version, "1_iptux",7))
		return;
	nickname = (char*)string_validate(msg->data, "gbk", &encode);
	if (strlen(msg->data) == 0) {
		nickname = msg->username;
	} else {
		nickname = (char*)string_validate(msg->data, "gbk", &encode);
		if (nickname == NULL)
			nickname = msg->data;
	}

	p = strchr(msg->data, '\0'); p++;
	if (strlen(p) == 0) {
		category = (char*)no_group;
	} else {
		category = (char*)string_validate(p, "gbk", &encode);
		if (category == NULL)
			category = p;
	}
	cmd = atoi(msg->cmd);

	avatar_id = get_avatar_id_from_version(msg->version);
	get_mac_from_version(msg->version, macaddr);

	if (avatar_id == 0) {
		strcpy(avatar, "pixmaps/avatar/default.png");
	} else {
		sprintf(avatar, "pixmaps/avatar/%d.bmp", avatar_id);
	}

	pthread_mutex_lock(&(ifc->ulist_lock));
	dlist_foreach(plist, &(ifc->ulist)) {
		user = (user_t*)dlist_entry(plist, user_t, unode);
		if (!strcmp(user->ipaddr, msg->ip)) {
			/* update user info */
			strcpy(user->nickname, nickname);
			strcpy(user->username, msg->username);
			strcpy(user->hostname, msg->hostname);
			strcpy(user->macaddr,  macaddr);
			strcpy(user->category, category);
			strcpy(user->avatar, avatar);
			break;
		}
	}
	pthread_mutex_unlock(&(ifc->ulist_lock));
	if (plist != &(ifc->ulist)) {
		printf("update...\n");
//		update_user_to_treeview((ifc->main_window).contact_treeview, user);
		return;
	}

	user = new_user(
			nickname, 		msg->username, 
			msg->hostname, 	avatar,
			msg->ip, 		macaddr,
			"hello", 		category, 
			"GBK"
			);

	pthread_mutex_lock(&(ifc->ulist_lock));
	dlist_add_tail(&(user->unode), &(ifc->ulist));
	pthread_mutex_unlock(&(ifc->ulist_lock));

	gdk_threads_enter();
	add_user_to_treeview(ifc, (ifc->main_window).contact_treeview, user);
	gdk_threads_leave();

	if ((cmd & CMD_BR_ENTRY) && strcmp(msg->username, ifc->username) != 0) {
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "1_lbt4_%d#128#%s#0#0#0:%lu:%s:%s:%u:%s",
				ifc->avatar_id,
				ifc->macaddr,
				time(NULL),
				ifc->username,
				ifc->hostname,
				0x103,
				ifc->nickname);
		udp_send_msg(ifc, msg->ip, msg->port, buf, strlen(buf));
	}
	return 0;
}

int on_buddy_exit(ifreechat_t *ifc, msg_t *msg) {

	return 0;
}

void send_reply_msg(ifreechat_t *ifc, msg_t *msg) {
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "1_lbt4_%d#128#%s#0#0#0:%lu:%s:%s:%u:%s",
			ifc->avatar_id,
			ifc->macaddr,
			time(NULL),
			msg->username,
			msg->hostname,
			CMD_RECVMSG,
			msg->packet_no);
	udp_send_msg(ifc, msg->ip, msg->port, buf, strlen(buf));
}

int on_buddy_sendmsg(ifreechat_t *ifc, msg_t *msg) {
	uint32_t cmd;
	char *encode;
	char *data;
	dlist_t *p;
	pchatbox_t *chatbox;
	user_t *user;
	time_t tm;

	printf("data: %s\n", msg->data);

	msg->gpid = 0;
	pthread_mutex_lock(&(ifc->mlist_lock));
	dlist_add_tail(&(msg->node), &(ifc->mlist));
	pthread_mutex_unlock(&(ifc->mlist_lock));

	data = (char*)string_validate(msg->data, "gbk", &encode);
	if (data) 
		strcpy(msg->data, data);

	gtk_status_icon_set_blinking(((ifc->main_window).icon), TRUE);
	pthread_mutex_lock(&(ifc->ulist_lock));
	user = NULL;
	dlist_foreach(p, &(ifc->ulist)) {
		user = (user_t*)dlist_entry(p, user_t, unode);
		if (!strcmp(user->ipaddr, msg->ip)) {
			msg->user = (void*)user;
			break;
		}
	}
	pthread_mutex_unlock(&(ifc->ulist_lock));
	if (user == NULL) {
		printf("no such user...\n");
		free(msg);
		return;
	}

	tm = (time_t)atoi(msg->packet_no);
	pthread_mutex_lock(&(ifc->pchatbox_lock));
	dlist_foreach(p, &(ifc->pchatbox)) {
		chatbox = (pchatbox_t*)dlist_entry(p, pchatbox_t, chatbox_node);
		user = (user_t*)chatbox->data;
		if (!strcmp(user->ipaddr, msg->ip)) {
			gdk_threads_enter();
			chatbox_insert_msg(chatbox, user->nickname, &tm, msg->data);
			gdk_threads_leave();
			break;
		}
	}
	pthread_mutex_unlock(&(ifc->pchatbox_lock));

	cmd = atoi(msg->cmd);
	if (cmd & OPT_SENDCHECK) {
		send_reply_msg(ifc, msg);
	}
	return 0;
}

int get_gpmsg_size(const char *version) {
	char *p;
	char *q;

	p = version;
	q = strchr(p, '#'); p = ++q;
	q = strchr(p, '#'); p = ++q;
	q = strchr(p, '#'); p = ++q;
	q = strchr(p, '#'); p = ++q;
	q = strchr(p, '#'); p = ++q;
	return atoi(p);
}

uint32_t get_group_id(const char *msg) {
	uint32_t id;
	char *p;
	char *q;
	
	printf("msg: [%s]\n", msg);
	p = strchr(msg, '#'); p++;
	q = strchr(p, '#'); *q = '\0';

	printf("[%s]\n", p);
	id = strtoul(p, NULL, 16);
	*q = '#';
	return id;
}


int on_buddy_sendgpmsg(ifreechat_t *ifc, msg_t *msg) {
	uint32_t cmd;
	uint32_t gpid;
	char *encode;
	char *data;
	char *plain;
	char *pmsg;
	size_t gpmsg_size;

	dlist_t *p;
	gchatbox_t *chatbox;
	group_t *group;
	user_t *user;
	time_t tm;
	CBlowFish *bf;
	
	if (!strcmp(msg->username, ifc->username)) {
		return;
	}

	tm = time(NULL);
	pthread_mutex_lock(&(ifc->ulist_lock));
	dlist_foreach(p, &(ifc->ulist)) {
		user = (user_t*)dlist_entry(p, user_t, unode);
		if (!strcmp(user->ipaddr, msg->ip)) {
			break;
		}
	}
	pthread_mutex_unlock(&(ifc->ulist_lock));
	if (p == &(ifc->ulist)) {
		printf("no such user(ip=%s)\n", msg->ip);
		free(msg);
		return -1;
	}

	gpmsg_size = get_gpmsg_size(msg->version);

	bf = CreateBlowFish(user->macaddr, strlen(user->macaddr));
	plain = (char*)malloc(gpmsg_size + 1);

	gpmsg_size = BlowFish_Decrypt(bf, msg->data, plain, gpmsg_size);
	data = (char*)string_validate(plain, "gbk", &encode);
	if (data == NULL)
		data = plain;
	gpid = get_group_id(data);

	pthread_mutex_lock(&(ifc->glist_lock));
	dlist_foreach(p, &(ifc->glist)) {
		group = (group_t*)dlist_entry(p, group_t, gnode);
		if (group->group_id == gpid)
			break;
	}
	pthread_mutex_unlock(&(ifc->glist_lock));
	if (p == &(ifc->glist)) {
		printf("no such group...\n");
	} else {
		pmsg = strchr(data, '#'); data = pmsg + 1;
		pmsg = strchr(data, '#'); pmsg++;
		msg->user = user;
		msg->gpid = gpid;
		strcpy(msg->data, pmsg);

		pthread_mutex_lock(&(ifc->mlist_lock));
		dlist_add_tail(&(msg->node), &(ifc->mlist));
		pthread_mutex_unlock(&(ifc->mlist_lock));
		gtk_status_icon_set_blinking(((ifc->main_window).icon), TRUE);
		
		pthread_mutex_lock(&(ifc->gchatbox_lock));
		dlist_foreach(p, &(ifc->gchatbox)) {
			chatbox = (gchatbox_t*)dlist_entry(p, gchatbox_t, chatbox_node);
			if (((group_t*)chatbox->data)->group_id == gpid) {
				gdk_threads_enter();
				chatbox_insert_msg(chatbox, user->nickname, &tm, pmsg);
				gdk_threads_leave();
				break;
			}
		}
		pthread_mutex_unlock(&(ifc->gchatbox_lock));
	}
	free(plain);
	DestroyBlowFish(bf);
	return 0;
}


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
	memset(base, 0, cap);

	p = (char*)data;
	msg = (msg_t*)base; base += sizeof(msg_t);

	init_dlist_node(&(msg->node));

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
	memcpy(msg->data, p, size - (p - (char*)data));

	return msg;
}


void process_message(ifreechat_t *ifc, char *ip, uint16_t port, 
		void *data, size_t size) {
	msg_t *msg;
	uint32_t cmd;

	msg = parse_message(data, size);
	if (msg == NULL) {
		fprintf(stderr, "parse_message() error...\n");
		return;
	}
	strcpy(msg->ip, ip);
	msg->port = port;

	cmd = atoi(msg->cmd);
	switch(cmd & 0x000000ff) {
		case CMD_BR_ENTRY:
		case CMD_ANSENTRY:
			on_buddy_entry(ifc, msg);
			free(msg);
			break;
		case CMD_SENDMSG:
			on_buddy_sendmsg(ifc, msg);
			break;
		case CMD_GPMSG:
			on_buddy_sendgpmsg(ifc, msg);
			break;
	}
}

