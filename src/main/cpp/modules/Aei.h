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
    const Grid* averagedGrid;
    const Grid* collocatedGrid;

    Segment* averagedSegment;
    Segment* collocatedSegment;

    int16_t averagingFactor;

    Accessor* accessorTau;
    Accessor* accessorTauError;
    Accessor* accessorAlpha;

    Accessor* collocatedAccessorTau550;
    Accessor* collocatedAccessorTau550err;
    Accessor* collocatedAccessorAlpha550;
    Accessor* collocatedAccessorAmin;

    void computeWeights(long l, long l0, long l1, long m, long m0, long m1, valarray<double>& weights) const;
    void getVertexes(const Accessor& accessor, long k, long l0, long l1, long m0, long m1, valarray<double>& vertexes) const;
};

#endif /* AEI_H_ */
