/*
 * MPConsumer.cpp
 *
 *  Created on: Sep 2, 2019
 *      Author: ubuntu
 */
#include "MPConsumer.h"
#include "stdio.h"
#include "mpparas.h"

namespace mp {
MPConsumer::MPConsumer() {
	// TODO Auto-generated constructor stub

}

MPConsumer::MPConsumer(std::priority_queue<Segment> *buffer, threadsafe_queue<Segment> *syncQueue, threadsafe_queue<char> * pipe) {
	// TODO Auto-generated constructor stub
	this->buffer = buffer;
	this->syncQueue = syncQueue;
	this->pipe = pipe;

}

MPConsumer::~MPConsumer() {
	// TODO Auto-generated destructor stub
}
void MPConsumer::push_pipe() {
	while (!buffer->empty()) {
		Segment tempS = buffer->top();
		buffer->pop();
		char* temp = (char*)tempS.content;
		//printf("tempS.size:%d\n",tempS.size);
		for (int i = 0;i < tempS.size;i++) {
			if (temp[i] == '\0') {
				continue;
			}
			pipe->push(temp[i]);
			//printf("%c",temp[i]);
		}
	}
}

int MPConsumer::move() {
	//printf("waiting for the producers.....\n");
	auto tempS = syncQueue->wait_and_pop();
	//printf("consuming once.....\n");
	buffer->push(tempS);
	//TODO::It is too frequent
	if (buffer->size() > MAX_RECV_BUFFER) {
		push_pipe();
	}
	return 0;
}

void MPConsumer::run(){
	while(true) {
		//printf("MPC echo\n");
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		move();
	}
}

int MPConsumer::start(){
	std::thread t(std::bind(&MPConsumer::run, this));
	//TODO:think about the join
	t.detach();
	return 0;
}

}

