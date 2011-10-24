/*
 * Aei.h
 *
 *  Created on: Oct 24, 2011
 *      Author: thomasstorm
 */

#ifndef AEI_H_
#define AEI_H_

#include "BasicModule.h"

class Aei : public BasicModule {
public:
	Aei();
	virtual ~Aei();

    void start(Context& context);
    void process(Context& context);
private:
    friend class AeiTest;
    const Grid* averagedGrid;
    const Grid* collocatedGrid;

    Segment* averagedSegment;
    Segment* collocatedSegment;

    int16_t averagingFactor;
};

#endif /* AEI_H_ */
