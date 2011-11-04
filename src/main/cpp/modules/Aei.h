/*
 * Aei.h
 *
 *  Created on: Oct 24, 2011
 *      Author: thomasstorm
 */

#ifndef AEI_H_
#define AEI_H_

#include "BasicModule.h"
#include "../core/Segment.h"

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
    void getAveragedTaus(long k, long averagedL0, long averagedL1, long averagedM0, long averagedM1, valarray<double>& averagedTaus) const;
    void getAveragedTauErrors(long k, long averagedL0, long averagedL1, long averagedM0, long averagedM1, valarray<double>& averagedTauErrors) const;
    void getAveragedAlphas(long k, long averagedL0, long averagedL1, long averagedM0, long averagedM1, valarray<double>& averagedAlpha) const;
};

#endif /* AEI_H_ */
