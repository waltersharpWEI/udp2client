/*
 * Segment.h
 *
 *  Created on: Sep 3, 2019
 *      Author: ubuntu
 */

#ifndef SEGMENT_H_
#define SEGMENT_H_

namespace mp {

class Segment {
public:
	int dss;
	int size;
	void *content;
	Segment();
	virtual ~Segment();
	const bool operator < ( const Segment &rhs ) const;
};

} /* namespace mp */

#endif /* SEGMENT_H_ */
