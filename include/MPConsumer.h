/*
 * MPConsumer.h
 *
 *  Created on: Sep 2, 2019
 *      Author: ubuntu
 */

#ifndef MPCONSUMER_H_
#define MPCONSUMER_H_

#include "Segment.h"
#include "threadsafe_queue.h"
#include <algorithm>
#include <queue>
#include <thread>

namespace mp {
class MPConsumer {
private:
	std::priority_queue<Segment> *buffer;
	threadsafe_queue<Segment> *syncQueue;
	threadsafe_queue<char> *pipe;
public:
	MPConsumer();
	MPConsumer(std::priority_queue<Segment> *buffer, threadsafe_queue<Segment> *syncQueue, threadsafe_queue<char> *pipe);
	virtual ~MPConsumer();
	void push_pipe();
	int move();
	int start();
	void run();
};
}

#endif /* MPCONSUMER_H_ */
