/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-17
 * File Name: feiq.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "encoding.h"
#include "protocol.h"
#include "utils.h"
#include "feiq.h"

int feiq_build_packet(packet_t **pkt, const msg_t *msg) {

	mem_pool_t *pool;
	char buf[65535];
	size_t size;

	pool = (mem_pool_t*)msg->user_data;
	memset(buf, 0, sizeof(buf));

	sprintf(buf, "1_lbt4_%d#131#%s#0#0#%d:%u:%s:%s:%u:",
			msg->avatar_id,
			msg->macaddr,
			msg->gpmsg_size,
			msg->packet_id,
			msg->username,
			msg->hostname,
			msg->command);
	size = strlen(buf);
	memcpy(buf + size, msg->data, msg->data_size);
	size += msg->data_size;

	*pkt = new_packet(pool, str2ip(msg->ip), msg->port, buf, size);
	if (*pkt == NULL)
		return -1;
	return 0;
}

static int get_avatar_id_from_version(const char *version) {
	char *p;
	if (0 != strncmp(version, "1_lbt", 5)) {
		return 0;
	}
	p = strrchr(version, '_'); p++;
	return atoi(p);
}

static void get_mac_from_version(const char *version, char *mac) {
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

static int get_gpmsg_size_from_version(const char *version) {
	char *p;
	char *q;
	p = (char*)version;
	q = strchr(p, '#'); p = ++q;
	q = strchr(p, '#'); p = ++q;
	q = strchr(p, '#'); p = ++q;
	q = strchr(p, '#'); p = ++q;
	q = strchr(p, '#'); p = ++q;

	return atoi(p);
}

int feiq_parse_packet(const packet_t *pkt, msg_t *msg) {

	size_t size;
	char *p;
	char *q;

	size = pkt->size;
	msg->mtime = pkt->mtime;

	p = pkt->data;
	/* get version */
	q = strchr(p, ':'); *q++ = '\0';
	strcpy(msg->version, p);
	size -= strlen(p) + 1;

	/* get avatar id */
	msg->avatar_id = get_avatar_id_from_version(p);

	/* get macaddr */
	get_mac_from_version(p, msg->macaddr);

	/* get group message size */
	msg->gpmsg_size = get_gpmsg_size_from_version(p);

	/* get ip */
	ip2str(pkt->ip, msg->ip);

	/* get port */
	msg->port = pkt->port;

	/* get packet id */
	p = q;
	q = strchr(p, ':'); *q++ = '\0';
	msg->packet_id = atoi(p);
	size -= strlen(p) + 1;

	/* get username */
	p = q;
	q = strchr(p, ':'); *q++ = '\0';
	strcpy(msg->username, p);
	size -= strlen(p) + 1;

	/* gethostname */
	p = q;
	q = strchr(p, ':'); *q++ = '\0';
	strcpy(msg->hostname, p);
	size -= strlen(p) + 1;

	/* get command */
	p = q;
	q = strchr(p, ':'); *q++ = '\0';
	msg->command = atoi(p);
	size -= strlen(p) + 1;

	/* get data */
	p = q;
	memset(msg->data, 0, sizeof(msg->data));
	memcpy(msg->data, p, size);
	msg->data_size = size;

	msg->user_data = NULL;
	init_dlist_node(&(msg->list_node));
	return 0;
}

void feiq_on_entry(msg_t *msg) {
	char *p;

	msg->avatar_id = get_avatar_id_from_version(msg->version);
	if (msg->avatar_id == 0)
		sprintf(msg->avatar, "../pixmaps/avatar/default.png");
	else
		sprintf(msg->avatar, "../pixmaps/avatar/%d.bmp", msg->avatar_id);

	p = strchr(msg->data, '\0'); *p++ = '\0';
	if (strlen(msg->data) == 0) 
		strcpy(msg->nickname, msg->username);
	else
		strcpy(msg->nickname, msg->data);
	if (strlen(p) == 0)
		strcpy(msg->category, "no group");
	else
		strcpy(msg->category, p);
}

void feiq_on_pchat(msg_t *msg) {
	char *encode;
	char *plain;

	plain = string_validate(msg->data, "GBK", &encode);
	if (plain) {
		memcpy(msg->data, plain, strlen(plain));
		msg->data_size = strlen(plain);
	}
}

int feiq_handle_msg(const msg_t *msg, void *user_data) {
	protocol_t *proto;

	proto = (protocol_t*)(msg->user_data);
	switch(msg->command & 0x000000FF) {
		case CMD_BR_ENTRY:
			feiq_on_entry(msg);
			proto->on_entry(user_data, msg);
			break;
		case CMD_BR_EXIT:
			proto->on_exit(user_data, msg);
			break;
		case CMD_SENDMSG:
			feiq_on_pchat(msg);
			proto->on_pchat(user_data, msg);
			break;
	}
	return 0;
}

