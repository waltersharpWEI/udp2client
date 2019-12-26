/*
 * UDPClient.h
 *
 *  Created on: Dec 23, 2019
 *      Author: ubuntu
 */

#ifndef SRC_UDPCLIENT_H_
#define SRC_UDPCLIENT_H_

#define ADDR_LEN 100

class UDPClient {
	int send_buffer_size;
	int mss;
	int current_delay;
	double current_loss_event_rate;
	int current_througput;
	int current_delay_jitter;
	double current_psnr;
	int current_rtt;
	int smoothed_rtt;
	char IP[ADDR_LEN];
	int port;
	int sockfd, len;
	struct sockaddr_in addr;
public:
	UDPClient();
	virtual ~UDPClient();
	//initialize the IP and port
	int init(char *IP, int port);
	//sendtox sendto wrapper of UDP sendto
	int sendto_x(const void * msg, int len, unsigned int flags);
	//clear all UDP options,
	//call before set the UDP-mode
	void optclear();
	//set to UDP-Default
	void setOptDefault();
	//set to UDP-Lite
	void setOptLite();
	//set to UDP-Soomro
	void setOptSoomro();
	//set the buffer size to size in bytes
	void setBuffer(int size);
};

#endif /* SRC_UDPCLIENT_H_ */
