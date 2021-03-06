/*
 * UDPClient.cpp
 *
 *  Created on: Dec 23, 2019
 *      Author: ubuntu
 */


#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include "UDPClient.h"

#define UDPLITE_SEND_CSCOV  10
#define UDPLITE_RECV_CSCOV  11

UDPClient::UDPClient() {
	 if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	 {
		 perror ("socket");
		 exit(1);
	 }
}

UDPClient::~UDPClient() {
	close(sockfd);
}

//initialize the IP and port
int UDPClient::init(char *IPx, int portx) {
	strcpy(IP,IPx);
	port = portx;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(IP);
}

//sendto oop version, simple wrapper
int UDPClient::sendto_x(const void * msg, int len, unsigned int flags){
	return sendto(sockfd, msg, len, 0, (struct sockaddr*)&addr,sizeof(addr));
}

//clear all UDP options,
//call before set the UDP-mode
void UDPClient::optclear() {

}
//set to UDP-Default
void UDPClient::setOptDefault() {

}
//set to UDP-Lite
void UDPClient::setOptLite(int cov) {
	close(sockfd);
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDPLITE)) < 0)
	{
		perror ("UDP-Lite socket");
		exit(1);
	}
	int val = 8;
	setsockopt(sockfd, IPPROTO_UDP, UDPLITE_SEND_CSCOV, &val, sizeof(int));
}
//set to UDP-Soomro
void UDPClient::setOptSoomro(){

}
//set the buffer size to size in bytes
void UDPClient::setBuffer(int size){

}
