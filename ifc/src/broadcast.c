#include <stdlib.h>
#include <string.h>

#include "ifreechat.h"
#include "udp_socket.h"
#include "feiq.h"
#include "msg.h"
#include "packet.h"

void online_broadcast(ifreechat_t *ifc) {
	packet_t *pkt;

	feiq_entry_broadcast_pkt(&pkt, ifc->pool,
			ifc->avatar_id, ifc->macaddr,
			(uint32_t)time(NULL), 
			ifc->username,
			ifc->hostname, 
			ifc->nickname,
			ifc->category);
	
	udp_send(ifc->usock, (void*)pkt);
}

void offline_broadcast(ifreechat_t *ifc) {
}

