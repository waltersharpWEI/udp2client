
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

int main() {
	srand(time(NULL));
	UDPClient udpC1;
	udpC1.init("172.31.43.94", 12345);
	char buffer[MAX_MSG_LEN];
	char content[MAX_MSG_LEN];
	bzero(content,MAX_MSG_LEN);
	init_content(content, 1400);
	while (true) {
		++dss;
		int packet_length = update_buffer(buffer, gettimestamp(), dss, content);
		int len = udpC1.sendto_x(buffer,packet_length,0);
		if (len < 0) {
			perror("sendto");
			exit(0);
		} else {
			//printf("%d bytes sent, %s\n",len,content);
		}
		usleep(30);
	}
	return 0;
}
