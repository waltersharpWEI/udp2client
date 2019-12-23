/*
 * TCPServer.h
 *
 *  Created on: Sep 4, 2019
 *      Author: ubuntu
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include "Segment.h"
#include "mpparas.h"
#include "threadsafe_queue.h"
#include <algorithm>
#include <queue>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace mp {

class TCPServer {
private:
	threadsafe_queue<Segment> * syncQueue;
	Segment makeSegment(void* buffer);
	struct sockaddr_in     serv_addr;
	struct sockaddr_in     clie_addr;
	char                   buf[MAXLINE];
	char                   port[MAXLINE];
	int                    sock_id;
	int                    recv_len;
	socklen_t              clie_addr_len;
public:
	TCPServer();
	virtual ~TCPServer();
	TCPServer(threadsafe_queue<Segment> *syncQueue);
	int move();
	int start();
	void run();
};

} /* namespace mp */

#endif /* TCPSERVER_H_ */
