/*
 * UDPServer.cpp
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

#include "UDPClient.h"

namespace mp
{

UDPClient::UDPClient() {
	// TODO Auto-generated constructor stub

}

// C++ also use this to use the object itself created

UDPClient::UDPClient(const char* IP, const char* port,void* content,int content_size)
{
	//printf("port cpy\n");
	strcpy(this->port,port);
	//printf("IP cpy\n");
	strcpy(this->IP, IP);
	//printf("Content cpy\n");
	memcpy(this->content, content, content_size * sizeof(char));
	this->content_size = content_size;
	//printf("%s\n",(char*)this->content);
    //std::cout << "UDP Client started on socket: " << IP << "-" << port << std::endl;

}

UDPClient::~UDPClient() {
	close(sock_id);
}

void UDPClient::run(){
	move();
}

int UDPClient::move() {
	if ((sock_id = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("Create socket failed\n");
		exit(0);
	}
	/*fill the server sockaddr_in struct*/
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(port));
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr_len = sizeof(serv_addr);
	bzero(buf, MAXLINE);
	memcpy(buf, content, content_size * sizeof(char));
	send_len = sendto(sock_id, buf, content_size, 0,(struct sockaddr*)&serv_addr,serv_addr_len);
	if(send_len < 0) {
		printf("Send Data To Server Failed!\n");
		return 1;
	}
	close(sock_id);
	return 0;
}

int UDPClient::start(){
	std::thread t(std::bind(&UDPClient::run, this));
	t.join();
	return 0;
}

} /* namespace mp */
