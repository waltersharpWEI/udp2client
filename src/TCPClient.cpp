/*
 * TCPServer.cpp
 *
 *  Created on: Sep 4, 2019
 *      Author: ubuntu
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "TCPClient.h"

namespace mp
{

TCPClient::TCPClient() {
	// TODO Auto-generated constructor stub

}

// C++ also use this to use the object itself created

TCPClient::TCPClient(const char* IP, const char* port,void* content,int content_size) // threadsafe_queue<Segment> *syncQueue
{
	strcpy(this->port,port);
	strcpy(this->IP, IP);
	memcpy(this->content, content, content_size * sizeof(char));
	this->content_size = content_size;
	//std::cout << "TCP Client started on socket: " << IP << "-" << port << std::endl;
	//printf("content of Tcpclient is %d with size %d", this->content, this->content_size);

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
}

TCPClient::~TCPClient() {
	close(sock_id);
}

void TCPClient::run(){
	move();
}

int TCPClient::move() {
	strcpy(this->port,port);
	strcpy(this->IP, IP);
	memcpy(this->content, content, content_size * sizeof(char));
	//std::cout << "TCP Client started on socket: " << IP << "-" << port << std::endl;

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
	if (connect(sock_id, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Failed to connect with server");
		exit(0);
	}

	bzero(buf, MAXLINE);
	memcpy(buf, content, content_size);
	//printf("send segment:%s",buf);
	send_len = send(sock_id, buf, content_size, 0);
	if(send_len < 0) {
		printf("Send Data To Server Failed!\n");
		return 1;
	}
	close(sock_id);
	return 0;
}

int TCPClient::start(){
	std::thread t(std::bind(&TCPClient::run, this));
	t.join();
	return 0;
}

} /* namespace mp */
