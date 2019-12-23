/*
 * TCPServer.cpp
 *
 *  Created on: Sep 4, 2019
 *      Author: ubuntu
 */

#include "TCPServer.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"

#include <iostream>
namespace mp {

TCPServer::TCPServer() {
	// TODO Auto-generated constructor stub

}

TCPServer::TCPServer(threadsafe_queue<Segment> *syncQueue) {
	srand(time(NULL));
	strcpy(port,"12346");
	std::cout << "TCP Server started on port " << port << std::endl;

	if ((sock_id = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("Create socket failed\n");
		exit(0);
	}
	/*fill the server sockaddr_in struct*/
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(port));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock_id,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0 ) {
		perror("Bind socket faild\n");
		exit(0);
	}
    if (-1 == listen(sock_id, 10)) {
		perror("Listen socket failed\n");
		exit(0);
	}
    clie_addr_len = sizeof(clie_addr);
	this->syncQueue = syncQueue;
}

TCPServer::~TCPServer() {
	// TODO Auto-generated destructor stub
}

Segment TCPServer::makeSegment(void* buffer) {
	char* str = (char*)(buffer);
	Segment s;
	int len = strlen(str);
	int index = -1; //index: the position of :
	int dss=0;
	for(int i = 0; i < len; i++)
	{
	  if (str[i] == ':')
	  {
		index = i;
		break;
	  }
		dss=dss*10+(str[i]-'0');
	}
	char sdss[len - index];//dss in string format
	if (index == -1) {
	  std::cout <<"error"<< std::endl;
	  //TODO: how to detect error?
	  return s;
	}
	else {
		for(int i = 0; i < len - index; i++){
			sdss[i] = str[i+index+1];
		}
		//std::cout <<"sdss: "<< sdss << std::endl;
	}
	s.dss = dss;
	s.content = malloc(sizeof(char) * 2048);
	s.size = len - index;
	bzero(s.content,2048);
	memcpy(s.content,(void*)sdss,sizeof(char) * (len - index));
	return s;
}

int TCPServer::move() {
	int link_id = accept(sock_id, (struct sockaddr *)&clie_addr, &clie_addr_len);
	if (-1 == link_id) {
		perror("Accept socket failed\n");
		exit(0);
	}
	bzero(buf, MAXLINE);
	recv_len = recv(link_id, buf, MAXLINE, 0);
	if(recv_len < 0) {
		printf("Recieve Data From Server Failed!\n");
		return 1;
	}
	//printf("\nFinish Recieve\n");
	close(link_id);
	Segment s = makeSegment(buf);
	syncQueue->push(s);
	return 0;
}

void TCPServer::run(){
	while(true) {
		//printf("TCP echo\n");
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
		move();
	}
}

int TCPServer::start(){
	std::thread t(std::bind(&TCPServer::run, this));
	//TODO:think about the join
	t.detach();
	return 0;
}

} /* namespace mp */
