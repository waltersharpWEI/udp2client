/*
 * TCPClientSuperSuper.cpp
 *
 *  Created on: Oct 20, 2019
 *      Author: waltersharp
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#include "TCPClientSuper.h"

namespace mp
{

TCPClientSuper::TCPClientSuper() {
	// TODO Auto-generated constructor stub

}

// C++ also use this to use the object itself created

TCPClientSuper::TCPClientSuper(const char* IP, const char* port) // threadsafe_queue<Segment> *syncQueue
{
	strcpy(this->port,port);
	strcpy(this->IP, IP);
	if ((sock_id = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("Create socket failed\n");
		exit(0);
	}
	/*fill the server sockaddr_in struct*/
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(port));
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr_len = sizeof(serv_addr);
	if ((sock_id = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("Create socket failed\n");
		exit(0);
	}
	if (connect(sock_id, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Failed to connect with server");
		exit(0);
	}
	//std::cout << "TCP Client started on socket: " << IP << "-" << port << std::endl;
	//printf("content of TCPClientSuper is %d with size %d", this->content, this->content_size);

}

TCPClientSuper::~TCPClientSuper() {
	close(sock_id);
}


int TCPClientSuper::move(void* content,int content_size) {
	memcpy(this->content, content, content_size * sizeof(char));
	bzero(buf, MAXLINE);
	memcpy(buf, content, content_size);
	//printf("send segment:%s",buf);
	send_len = send(sock_id, buf, content_size, 0);
	if(send_len < 0) {
		perror("Fail to send data");
		//printf("Send Data To Server Failed!\n");
		return 1;
	}
	//close(sock_id);
	return 0;
}


} /* namespace mp */
