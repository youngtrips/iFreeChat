#include <stdlib.h>
#include <string.h>
#include <openssl/blowfish.h>

#include "ifreechat.h"
#include "udp_socket.h"
#include "msg.h"
#include "process_msg.h"
#include "group.h"
#include "dlist.h"

#define BROADCAST_ADDR	"255.255.255.255"
#define MULTICAST_ADDR	"226.81.9.8"

void online_broadcast(ifreechat_t *ifc) {
	unsigned char ivec[8];
	char buf[8192];
	char plain[512];
	char crypt[512];
	size_t size;
	int len;
	int i;
	group_t *gp;
	dlist_t *p;
	BF_KEY key;

	BF_set_key(&key, 12, ifc->macaddr);

	printf("post online message...\n");
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "1_lbt4_%d#128#%s#0#0#0:%lu:%s:%s:%u:%s",
			ifc->avatar_id,
			ifc->macaddr,
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

	//post group info
	dlist_foreach(p, &(ifc->glist)) {
		gp = (group_t*)dlist_entry(p, group_t, gnode);
		sprintf(plain, "QUNMSGMARK#%lx#%s",
				gp->group_id,
				gp->group_name);
		len = strlen(plain);
		BF_cbc_encrypt(plain, crypt, len, &key, ivec, BF_ENCRYPT);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "1_lbt4_%d#128#%s#0#0#%d:%lu:%s:%s:%u:",
				ifc->avatar_id,
				ifc->macaddr,
				len,
				time(NULL),
				ifc->username,
				ifc->hostname,
				0x2000c9);
		size = strlen(buf);
		memcpy(buf + size, crypt, len + 1);
		size += len + 1;
		udp_send_msg(ifc, MULTICAST_ADDR, ifc->port, buf, size);
	}
	
}

void offline_broadcast(ifreechat_t *ifc) {
	char buf[8192];
	size_t size;
	int i;

	printf("post online message...\n");
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "1_lbt4_%d#128#%s#0#0#0:%lu:%s:%s:%u:%s",
			ifc->avatar_id,
			ifc->macaddr,
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

