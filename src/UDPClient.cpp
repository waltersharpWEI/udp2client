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
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include "UDPClient.h"

UDPClient::UDPClient() {
	 if(sockfd = socket(AF_INET, SOCK_DGRAM, 0) < 0)
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
	return sendto(sockfd, msg, strlen((char*)msg), 0, (struct sockaddr*)&addr,sizeof(addr));
}

//clear all UDP options,
//call before set the UDP-mode
void UDPClient::optclear() {

}
//set to UDP-Default
void UDPClient::setOptDefault() {

}
//set to UDP-Lite
void UDPClient::setOptLite() {

}
//set to UDP-Soomro
void UDPClient::setOptSoomro(){

}
//set the buffer size to size in bytes
void UDPClient::setBuffer(int size){

}
