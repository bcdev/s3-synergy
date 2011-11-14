/*
 * Aei.h
 *
 *  Created on: Oct 24, 2011
 *      Author: thomasstorm
 */

#ifndef AEI_H_
#define AEI_H_

#include "../core/Segment.h"

#include "BasicModule.h"

class Aei : public BasicModule {
public:
	Aei();
	virtual ~Aei();

    void start(Context& context);
    void process(Context& context);
private:
    friend class AeiTest;

    const Grid* targetGrid;
    const Grid* sourceGrid;

    const Segment* sourceSegment;
    Segment* targetSegment;

    int16_t averagingFactor;

    double interpolation(const Accessor& accessor, long k, long l0, long l1, long m0, long m1, double wl, double wm) const;
};

#endif /* AEI_H_ */
