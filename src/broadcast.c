#include <stdlib.h>
#include <string.h>

#include "ifreechat.h"
#include "udp_socket.h"
#include "msg.h"
#include "process_msg.h"

#define BROADCAST_ADDR	"255.255.255.255"

void online_broadcast(ifreechat_t *ifc) {
	char buf[8192];
	size_t size;
	int i;

	printf("post online message...\n");
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "1_lbt4_%d#128#001A73261837#0#0#0:%lu:%s:%s:%u:%s",
			ifc->avatar_id,
			time(NULL),
			ifc->username,
			ifc->hostname,
			0x101,
			ifc->nickname);

	size = strlen(buf);
	sprintf(buf + size, "%c%s%c", '\0', ifc->category, '\0');
	size += 2 + strlen(ifc->category);

//	for(i = 0;i < size; i++) {
//		printf("%02x ", buf[i]);
//	}
//	printf("\n");
	udp_send_msg(ifc, BROADCAST_ADDR, ifc->port, buf, size);
}

void offline_broadcast(ifreechat_t *ifc) {
	char buf[8192];
	size_t size;
	int i;

	printf("post online message...\n");
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "1_lbt4_%d#128#001A73261837#0#0#0:%lu:%s:%s:%u:%s",
			ifc->avatar_id,
			time(NULL),
			ifc->username,
			ifc->hostname,
			CMD_BR_EXIT,
			ifc->nickname);
	size = 1 + strlen(buf);
	sprintf(buf + size, "%s", ifc->category);
	size += 1 + strlen(ifc->category);

//	for(i = 0;i < size; i++) {
//		printf("%02x ", buf[i]);
//	}
//	printf("\n");
	udp_send_msg(ifc, BROADCAST_ADDR, ifc->port, buf, size);
}

