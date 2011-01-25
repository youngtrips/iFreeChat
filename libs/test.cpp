#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "udp_socket.h"

int main() {
	udp_socket udp("0.0.0.0", 2425, 
			65535, 65535, 
			65535, 65535,
			UDP_SRV);
	udp.init();
	udp.start_listen();

	for(;;) {
		sleep(2);
	}

	return 0;
}
