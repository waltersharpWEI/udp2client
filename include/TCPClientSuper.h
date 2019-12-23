/*
 * TCPClientSuper.h
 *
 *  Created on: Oct 20, 2019
 *      Author: walter
 */

#ifndef TCPCLIENTSUPER_H_
#define TCPCLIENTSUPER_H_

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

class TCPClientSuper
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
	TCPClientSuper();
	virtual ~TCPClientSuper();
	TCPClientSuper(const char* IP, const char* ports);
	int move(void* content, int content_size);
};

} /* namespace mp */

#endif /* TCPCLIENTSUPER_H_ */