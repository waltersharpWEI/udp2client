#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <chrono>
#include "threadsafe_queue.h"
#include "mpcb.h"

long timestamp() {
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
			        std::chrono::system_clock::now().time_since_epoch()
	);
	return ms.count();
}

int mode_seq[10] = {0,1,0,1,0,1,0,1,0,1};

int main(int argc, char* argv[]) {
	const char* optstring="s:k:m:c:p:q:w:t:u:f:h:e:b:i:";
	char o;
	int segment_size = 512;
	int time_before_kill = -1;
	int mode_of_running = 0;
	int exp_mode = 0;
	int content_size = 512;
	int packet_before_switch_protocol = 10000;
	int protocol1 = 0;
	int protocol2 = 0;
	int handover_num = 1;
	char outBuffer[2048];
	char TCPIP[128],UDPIP[128];
	int flow_size = -1;
	int balance_factor = 0;
	int switch_interval = 1000;

	srand(time(NULL));

	while ((o = getopt(argc, argv, optstring)) != -1) {
		switch (o) {
		    case 's':
		        printf("opt is segment size, oprarg is: %s\n", optarg);
		        segment_size = atoi(optarg);
		        break;
		    case 'k':
		        printf("opt is time before kill, oprarg is: %s\n", optarg);
		        time_before_kill = atoi(optarg);
		        break;
		    case 'm':
		        printf("opt is mode of running, oprarg is: %s\n", optarg);
		        //0 is client, 1 is server
		        mode_of_running = atoi(optarg);
		        break;
		    case 'c':
		    	printf("opt is content size, value is %s\n", optarg);
		    	content_size = atoi(optarg);
		    	break;
		    case 'p':
		    	printf("protocol1 is %s (1 for TCP, 2 for UDP) \n", optarg);
		    	protocol1 = atoi(optarg);
		    	break;
		    case 'q':
		    	printf("protocol2 is %s (1 for TCP, 2 for UDP) \n", optarg);
		    	protocol2 = atoi(optarg);
		    	break;
		    case 'w':
				printf("number of segment before switching protocol is %s\n", optarg);
				packet_before_switch_protocol = atoi(optarg);
		    	break;
		    case 't':
		    	printf("TCP channel IP is %s\n", optarg);
		    	strcpy(TCPIP,optarg);
		    	break;
		    case 'u':
		    	printf("UDP channel IP is %s\n", optarg);
		    	strcpy(UDPIP,optarg);
		    	break;
		    case 'f':
		    	//if flow size is given then
		    	//time to kill is disabled
		    	printf("Flow size in packets %s\n", optarg);
		    	flow_size = atoi(optarg);
		    	break;
		    case 'h':
		    	printf("number of handovers %s\n",optarg);
		    	handover_num = atoi(optarg);
		    	break;
		    case 'e':
		    	printf("expriment mode %s\n",optarg);
		    	exp_mode = atoi(optarg);
		    	break;
		    case 'b':
		    	printf("Balance Factor %s\n",optarg);
		    	balance_factor = atoi(optarg);
		    	break;
		    case 'i':
		    	printf("Switch interval %s\n",optarg);
		    	switch_interval = atoi(optarg);
		    case '?':
		        printf("error optopt: %c\n", optopt);
		        printf("error opterr: %d\n", opterr);
		        break;
		}
	}
	bzero(outBuffer,2048);
	for (int i = 0; i < content_size;i++) {
		outBuffer[i] = char(i % 10 + '0');
	}

	mp::mpcb m1;
	m1.init(TCPIP,UDPIP);
	//change the exp_mode
	//m1.switch_exp_mode(exp_mode);
	//we always use mutipath mode 0
	m1.switchmode(0);
	m1.set_balance_factor(balance_factor);
	int counter = 0;
	time_t start_time = time(NULL);
	clock_t start_clock = clock();
	long tp0 = timestamp();
	printf("%ld\n", tp0);
	while (true){
		++counter;
		//printf("Started to send!\n");
		int x = m1.send_basic(outBuffer,strlen(outBuffer));
		if (counter % switch_interval == 0) {
			m1.flip_exp_mode();
		}
		if (counter == flow_size) {
			long tp2 = timestamp();
			printf("%ld\n", tp2);
			printf("%ld\n", tp2 - tp0);
			break;
		}
		time_t current_time = time(NULL);
		if (flow_size < 0 && time_before_kill >=0 && current_time - start_time > time_before_kill) {
			break;
		}
	}
	return 0;
}

