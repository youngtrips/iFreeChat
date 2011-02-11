#include <stdlib.h>
#include <string.h>

#include "ifreechat.h"
#include "udp_socket.h"
#include "msg.h"

#define BROADCAST_ADDR	"255.255.255.255"

void online_broadcast(ifreechat_t *ifc) {
	char buf[8192];
	char *base;
	size_t size;
	msg_t *msg;

	printf("post online message...\n");
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s", "1_lbt4_13#128#0016D31F56A6#0#0#0");
	sprintf(buf + strlen(buf), ":%lu", time(NULL));
	sprintf(buf + strlen(buf), ":%s", ifc->username);
	sprintf(buf + strlen(buf), ":%s", ifc->hostname);
	sprintf(buf + strlen(buf), ":%u", 0x101);
	sprintf(buf + strlen(buf), ":%s\0", ifc->nickname);

	size = 0;
	size += 1 + strlen(BROADCAST_ADDR);
	size += 1 + strlen(buf) + 1;
	size += sizeof(msg_t);
	base = (char*)malloc(size);

	msg 		= (msg_t*)base; base += sizeof(msg_t);
	msg->ip 	= base; 		base += 1 + strlen(BROADCAST_ADDR);
	msg->buf 	= base; 		base += 1 + strlen(buf);

	strcpy(msg->ip, BROADCAST_ADDR);
	msg->port = ifc->port; 
	msg->buf_size = strlen(buf);
	memcpy(msg->buf, buf, strlen(buf));

	if (udp_send(ifc, msg) < 0) {
		printf("broadcast online message error.\n");
	}
}
