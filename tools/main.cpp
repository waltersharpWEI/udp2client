
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include "UDPClient.h"

#define MAX_MSG_LEN 1000

int main() {
	UDPClient udpC1;
	udpC1.init("127.0.0.1", 12345);
	char buffer[MAX_MSG_LEN];
	strcpy(buffer,"1234567890");
	while (true) {
		udpC1.sendto_x(buffer,strlen(buffer),0);
		usleep(1000);
	}
	return 0;
}
