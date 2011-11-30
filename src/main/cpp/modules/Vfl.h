/*
 * Vfl.h
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#ifndef VFL_H_
#define VFL_H_

#include "../modules/BasicModule.h"

class Vfl : public BasicModule {
public:
	Vfl();
	virtual ~Vfl();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VflTest;
	Segment* collocatedSegment;
};


#endif /* VFL_H_ */
