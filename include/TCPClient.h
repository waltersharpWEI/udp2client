/*
 * TCPServer.h
 *
 *  Created on: Sep 4, 2019
 *      Author: ubuntu
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

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

#define MAX_MSG_SIZE 256
#define SERVER_PORT  9987

namespace mp
{

class TCPClient
{
private:
	char                   IP[MAXLINE];
	struct sockaddr_in     serv_addr;
	char                   buf[MAXLINE];
	char                   port[MAXLINE];
	int                    sock_id;
	int                    send_len;
	socklen_t              serv_addr_len;
	char				   content[MAXLINE];
	int                    content_size;
public:
	TCPClient();
	virtual ~TCPClient();
	//TCPClient(threadsafe_queue<Segment> *syncQueue);
	TCPClient(const char* IP, const char* ports,void* content, int content_size);
	int move();
	int start();
	void run();
};

} /* namespace mp */

#endif /* TCPCLIENT_H_ */
