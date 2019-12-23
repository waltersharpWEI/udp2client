/*
 * mpcb.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: waltersharp
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <sys/time.h>
#include "Segment.h"
#include "mpcb.h"


namespace mp {

mpcb::mpcb() {
	mode = 0;
	segment_size = SEGMENT_SIZE;
	dss_root = 100;
	balance_factor = 50;
	flipflop = false;
}

mpcb::~mpcb() {
	delete tcpCS;
	delete udpCS;
	delete tcpCS2;
	delete udpCS2;
}

void mpcb::init(char* TCPIP,char* UDPIP) {
	strcpy(TCPIPs[0],TCPIP);
	strcpy(TCPports[0],"12346");
	//192.168.184.128
	strcpy(UDPIPs[0],UDPIP);
	strcpy(UDPports[0],"12345");
	//192.168.32.129
	//use the other link as for multi TCP / UDP
	//and use odd ports for UDP
	strcpy(TCPIPs[1],UDPIP);
	strcpy(TCPports[1],"12348");
	strcpy(UDPIPs[1],TCPIP);
	strcpy(UDPports[1],"12347");


	tcpCS = new TCPClientSuper(TCPIPs[0], TCPports[0]);
	udpCS = new UDPClientSuper(UDPIPs[0], UDPports[0]);
	tcpCS2 = new TCPClientSuper(TCPIPs[1], TCPports[1]);
	udpCS2 = new UDPClientSuper(UDPIPs[1], UDPports[1]);

}

//TODO: lots of work to be done
int mpcb::recv(void * outBuffer1, int BUFFER_SIZE){
	char* outBuffer = (char*)outBuffer1;
	int outputSize = fmin(BUFFER_SIZE,pipe.size());
	if (outputSize < 1) {
		return 1;
	}
	//printf("buffer size %d, pipe size %d, output size %d:", BUFFER_SIZE,pipe.size(),outputSize);
	int counter = 0;
	int offset = 0;
	while (counter < outputSize) {
		char pos = pipe.wait_and_pop();
		memcpy(outBuffer+offset,&pos,sizeof(char));
		offset += sizeof(char);
		counter += 1;
	}
	//TODO: synchronization error, needs lock.
	//printf("The size of pipe is %d\n", pipe.size());
	return 0;
}

//exp: 0 udp tcp, 1 udp udp, 2 tcp tcp, 3 tcp udp
//TODO: thread_pool management, main problem now
int mpcb::send_basic(void* content1, int content_size) {
	char buffer[MAXLINE];
	bzero(buffer,MAXLINE * sizeof(char));
	sprintf(buffer, "%d:%s;", ++dss_root, (char*)content1);
	int dice = rand() % 100;
	if (dice <= balance_factor) {
		//TODO: finish the segment cutting code
		switch (mode){
		case 0:
			udpCS->move(buffer, strlen(buffer));
			break;
		case 1:
			tcpCS->move(buffer,strlen(buffer));
			break;
		default:
			printf("unknown main_mode!");
		}
	}
	bzero(buffer,MAXLINE* sizeof(char));
	sprintf(buffer, "%d:%s;", ++dss_root, (char*)content1);
	if (flipflop) {
		exp_mode = 0;
	} else {
		exp_mode = 1;
	}
	if (dice > balance_factor) {
		switch (exp_mode){
		case 0:
			tcpCS2->move(buffer, strlen(buffer));
			break;
		case 1:
			udpCS2->move(buffer, strlen(buffer));
			break;
		default:
			printf("unknown exp_mode!");
		}
	}

	return 0;
}


int mpcb::send(void* content1, int content_size) {
	//TODO: finish this method using TCPClient and UDPClient

	//This is just a example, segmenting and scheduling needs to be done here
	//like MPClient
	//Create a UDPClient, give it target IP, port and the content for it to se

	int len=content_size;
	char content[MAXLINE];
	((char*)content1)[content_size]='\0';
	strcpy(content, (char*)content1);
	char h1[len/2+1];
	char h2[len-len/2];
	for(int i=0;i<len/2;i++){
		h1[i]=content[i];
	}
	h1[len/2]='\0';
	for(int i=0;i<len-len/2;i++){
		h2[i]=content[i+len/2];
	}
	int pn = 0;
	char* ct[2];
	for(int i=0; i< 2; i++){
		if (pn<1){
			ct[pn]=h1;
		}else ct[pn]=h2;
		int index=dss_root+pn;
		char *buffer;
		sprintf(buffer, "%d:%s", index, ct[pn]);
		ct[pn]=buffer;
		if (mode == 0){
			if (pn % 2 == 0){
				TCPClient* tcpC = new TCPClient(TCPIPs[0], ports[i], ct[pn], strlen(ct[pn]));
				tcpC->start();
				printf("sent ", ct[pn], ' using TCP.');
			}
			else{
				UDPClient *udpC = new UDPClient(UDPIPs[0], ports[i], ct[pn], strlen(ct[pn]));
				printf("sent ", ct[pn], ' using UDP.');
			}
		}
		else if (mode == 1){
			TCPClient* tcpC = new TCPClient(TCPIPs[0], TCPports[0], ct[pn], strlen(ct[pn]));
			printf("sent ", ct[pn], ' using TCP.');
		}
		else if  (mode == 2){
			UDPClient *udpC = new UDPClient(UDPIPs[0], UDPports[0], ct[pn], strlen(ct[pn]));
			printf("sent ", ct[pn], ' using UDP.');
		}
		pn += 1;
	}
	int a = rand()%100000+1;
	set_dss_root(a);
	//Start UDPClient , let it send the content then it will automatically shutdown
	tcpC->start();
	udpC->start();
	return 0;
}

void mpcb::switchmode(int target_mode){
	this->mode = target_mode;
}

void mpcb::switch_exp_mode(int target_mode){
	this->exp_mode = target_mode;
}

void mpcb::flip_exp_mode(){
	flipflop = !flipflop;
}


int mpcb::recv(void * outBuffer){
	int BUFFER_SIZE = 128;
	//TODO: this introduce the correctness problem for last 5 segment in stream.
	if (buffer.size() < 5) {
		return 1;
	}
	if (!buffer.empty()) {
		memcpy(outBuffer,(void*)buffer.top().content,BUFFER_SIZE);
		//std::cout << buffer.size() << std::endl;
		buffer.pop();
	} else {
		return 1;
	}
	return 0;
}

} /* namespace mp */
