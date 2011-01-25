#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "network.h"
#include "protocol.h"

char buf[65535];
int main() {
	network net(9091, 9091);
	net.start();

	msg_t msg;
	msg.buf = buf;
	msg.buf_cap = 65535;
	if (msg.buf == NULL) {
		printf("err...\n");
		return 1;
	}

	char ip[20];
	unsigned short port;
	int size;
	for(;;) {
		memset(buf, 0, sizeof(buf));
		size = net.recv_msg(ip, port, buf, sizeof(buf));
		if (size > 0) {
			printf("%s:%u\n", ip, port);
			printf("%s\n", buf);
		}
//		memset(msg.buf, 0, msg.buf_cap);
//		msg.buf_size = 0;
//		net.recv_msg(&msg);
//		if (msg.buf_size > 0) {
//			printf("recv message from :%s:%u\n", msg.ip, msg.port);
//			printf("%s\n", (char*)msg.buf);
//			printf("++++++++++++++++++++++++++++++++++++++++++\n");
//		}
		sleep(1);
	}

	return 0;
}
