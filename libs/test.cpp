#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "network.h"
#include "protocol.h"

int main() {
	network net(2425, 2425);
	net.start();

	msg_t msg;
	msg.buf = malloc(65535);
	msg.buf_cap = 65535;
	if (msg.buf == NULL) {
		printf("err...\n");
		return 1;
	}

	for(;;) {
		memset(msg.buf, 0, msg.buf_cap);
		msg.buf_size = 0;
		net.recv_msg(&msg);
		if (msg.buf_size > 0) {
			printf("recv message from :%s:%u\n", msg.ip, msg.port);
			printf("%s\n", (char*)msg.buf);
			printf("++++++++++++++++++++++++++++++++++++++++++\n");
		}
		sleep(1);
	}

	return 0;
}
