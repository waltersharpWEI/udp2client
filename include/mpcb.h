/*
 * mpcb.h
 *
 *  Created on: Sep 1, 2019
 *      Author: waltersharp
 */

#ifndef MPCB_H_
#define MPCB_H_
#include "Segment.h"
#include "MPConsumer.h"
#include "TCPServer.h"
#include "UDPServer.h"
#include "TCPClient.h"
#include "TCPClientSuper.h"
#include "UDPClient.h"
#include "UDPClientSuper.h"
#include "mpparas.h"
#include <queue>
#include <algorithm>
#include <functional>
#include "threadsafe_queue.h"

//TODO: handle the heap and buffer explosion by check the status of the heap and drop the new comer, or block them
namespace mp {

class mpcb {
private:
	MPConsumer *mpc;
	TCPServer *tcpS;
	UDPServer *udpS;
	TCPClient *tcpC;
	UDPClient *udpC;
	TCPClientSuper *tcpCS,*tcpCS2;
	UDPClientSuper *udpCS,*udpCS2;
    std::priority_queue<Segment> buffer;
	threadsafe_queue<Segment> syncQueue;
	threadsafe_queue<char> pipe;
	int dss_root;
	int mode;
	int exp_mode;
	int segment_size;
	bool flipflop;
	char TCPports[20][MAXLINE];
	char UDPports[20][MAXLINE];
	char TCPIPs[20][MAXLINE];
	char UDPIPs[20][MAXLINE];
	char ports[100][MAXLINE];
public:
	int tcp_thread_count;
	int udp_thread_count;
	int send_count;
	int balance_factor;
	mpcb();
	virtual ~mpcb();
	void init(char* TCPIP,char* UDPIP);
	int recv(void * outBuffer, int BUFFER_SIZE);
	int recv(void * outBuffer);
	int send_basic(void* content, int content_size);
	int send(void * content, int content_size);
	void switchmode(int target_mode);
	void switch_exp_mode(int target_mode);
	void flip_exp_mode();
	void set_dss_root(int a){
		dss_root=a;
	}
	void set_segment_size(int a) {
		segment_size = a;
	}
	void set_balance_factor(int f) {
		balance_factor = f;
	}
};

} /* namespace mp */

#endif /* MPCB_H_ */
