/*
 * Segment.cpp
 *
 *  Created on: Sep 3, 2019
 *      Author: ubuntu
 */

#include "Segment.h"

namespace mp {

Segment::Segment() {
	// TODO Auto-generated constructor stub

}

Segment::~Segment() {
	// TODO Auto-generated destructor stub
}

const bool Segment::operator < ( const Segment &rhs ) const{
	return ( dss > rhs.dss );
}

} /* namespace mp */
