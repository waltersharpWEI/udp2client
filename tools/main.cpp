
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
#include <chrono>
#include <sstream>
#include "Frame.h"

using namespace std;

#define MAX_MSG_LEN 1500

void init_content(char* content, int len) {
	for (int i = 0;i < len;i++) {
		content[i] = rand() % 10 + '0';
	}
}

int update_buffer(char* buf, long long timestamp, unsigned long long dss, char * content) {
	struct Frame fra;
	memset(&fra, 0, sizeof(fra));
	fra.timestamp = timestamp;
	fra.dss = dss;
	strcpy(fra.content,content);
	bzero(buf,MAX_MSG_LEN);
	int count = 0;
	memcpy(buf, &fra.dss, sizeof(fra.dss));
	count += sizeof(fra.dss);
	memcpy(buf + count, &fra.timestamp, sizeof(fra.timestamp));
	count += sizeof(fra.timestamp);
	memcpy(buf + count, fra.content, strlen(fra.content));
	count += strlen(fra.content);
	return count;
}

long long gettimestamp() {
	chrono::time_point<std::chrono::system_clock> now = chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return millis;
}

unsigned long long dss = 0;

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int opt;
	bool optLite = false;
	int packet_interval = 769;
	int clock_offset = -5;
	int flow_time = 20;

	while ((opt = getopt(argc, argv, "k:p:c:o")) != -1) {
		switch (opt) {
		case 'p':
			printf("packet interval: %s\n", optarg);
			packet_interval = atoi(optarg);
			break;
		case 'c':
			printf("clock offset: %s\n", optarg);
			clock_offset = atoi(optarg);
			break;
		case 'k':
			printf("flow time: %s\n", optarg);
			flow_time = atoi(optarg);
			break;
		case 'o':
			printf("Using UDP-Lite\n");
			optLite = true;
			break;
		case '?':
			printf("Unknown option: %c", optopt);
			break;
		}
	}

	printf("start the main part of the program.\n");

	UDPClient udpC1;
	if (optLite) {
		udpC1.setOptLite(10);
	}
	udpC1.init("192.168.6.136", 12345);
	char buffer[MAX_MSG_LEN];
	char content[MAX_MSG_LEN];
	bzero(content,MAX_MSG_LEN);
	init_content(content, 1400);

	//769
	long long start_time = gettimestamp();

	while (true) {
		++dss;
		long long current_time = gettimestamp();
		if (current_time - start_time >= 1000 * flow_time) {
			//exit(0);
		}
		int packet_length = update_buffer(buffer, current_time + clock_offset, dss, content);
		int len = udpC1.sendto_x(buffer,packet_length,0);
		//send error
		if (len < 0) {
			perror("sendto");
			exit(0);
		} else {
			//printf("%d bytes sent, %s\n",len,content);
		}
		//769 um = 14Mpbs CBR video stream

		//usleep(packet_interval);
	}
	return 0;
}
