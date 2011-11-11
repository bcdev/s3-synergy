/*
 * Aei.cpp
 *
 *  Created on: Oct 24, 2011
 *      Author: thomasstorm
 */

#include <algorithm>
#include <cmath>

#include "Aei.h"

using std::abs;
using std::floor;
using std::min;
using std::numeric_limits;

Aei::Aei() :
        BasicModule("AEI") {
}

Aei::~Aei() {
}

void Aei::start(Context& context) {
    averagedSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
    averagedGrid = &averagedSegment->getGrid();
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    collocatedGrid = &collocatedSegment->getGrid();
    AuxdataProvider& configurationAuxdataProvider = (AuxdataProvider&)context.getObject(Constants::AUX_ID_SYCPAX);
    averagingFactor = configurationAuxdataProvider.getShort("ave_square");

    accessorTau = &averagedSegment->getAccessor("T550");
    accessorTauError = &averagedSegment->getAccessor("T550_er");
    accessorAlpha = &averagedSegment->getAccessor("A550");

    const SegmentDescriptor& collocatedSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);

    collocatedAccessorTau550 = &collocatedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550"));
    collocatedAccessorTau550err = &collocatedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550_er"));
    collocatedAccessorAlpha550 = &collocatedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("A550"));
    collocatedAccessorAmin = &collocatedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("AMIN"));
}

void Aei::process(Context& context) {
    context.getLogging().progress("Performing aerosol interpolation...", getId());
    const Accessor& accessorAmin = averagedSegment->getAccessor("AMIN");
    const Accessor& accessorFlags = averagedSegment->getAccessor("SYN_flags");

    Accessor& collocatedAccessorFlags = collocatedSegment->getAccessor("SYN_flags");

    long firstTargetL = context.getFirstComputableL(*collocatedSegment, *this);
    long lastTargetL = context.getLastComputableL(*collocatedSegment, *this);

    valarray<double> weights(4);
    valarray<double> averagedTaus(4);
    valarray<double> averagedTauErrors(4);
    valarray<double> averagedAlphas(4);

    for (long targetL = firstTargetL; targetL <= lastTargetL; targetL++) {
        context.getLogging().progress("Interpolating line l = " + lexical_cast<string>(targetL), getId());

        const long sourceL0 = min(0.0, floor((targetL - averagingFactor / 2) / averagingFactor));
        const long sourceL1 = sourceL0 + 1;

        const long targetL0 = sourceL0 * averagingFactor + averagingFactor / 2;
        const long targetL1 = sourceL1 * averagingFactor + averagingFactor / 2;

        for (long k = collocatedGrid->getFirstK(); k < collocatedGrid->getMaxK(); k++) {
			for (long targetM = collocatedGrid->getFirstM(); targetM < collocatedGrid->getMaxM(); targetM++) {
				const long sourceM0 = min(0.0, floor((targetM - averagingFactor / 2) / averagingFactor));
				const long sourceM1 = sourceM0 + 1;

				const long targetM0 = sourceM0 * averagingFactor + averagingFactor / 2;
				const long targetM1 = sourceM1 * averagingFactor + averagingFactor / 2;

				computeWeights(targetL, targetL0, targetL1, targetM, targetM0, targetM1, weights);

				double tau550 = 0.0;
				double tau550err = 0.0;
				double alpha550 = 0.0;

				getVertexes(*accessorTau, k, sourceL0, sourceL1, sourceM0, sourceM1, averagedTaus);
				getVertexes(*accessorTauError, k, sourceL0, sourceL1, sourceM0, sourceM1, averagedTauErrors);
				getVertexes(*accessorAlpha, k, sourceL0, sourceL1, sourceM0, sourceM1, averagedAlphas);

				for (size_t i = 0; i < weights.size(); i++) {
					tau550 += weights[i] * averagedTaus[i];
					tau550err += weights[i] * averagedTauErrors[i];
					alpha550 += weights[i] * averagedAlphas[i];
				}

				const size_t targetIndex = collocatedGrid->getIndex(k, targetL, targetM);
				collocatedAccessorAlpha550->setDouble(targetIndex, alpha550);
				collocatedAccessorTau550->setDouble(targetIndex, tau550);
				collocatedAccessorTau550err->setDouble(targetIndex, tau550err);

				size_t sourceIndex;
				if (abs(targetL0 - targetL) <= (averagingFactor / 2) && abs(targetM0 - targetM) <= (averagingFactor / 2)) {
					sourceIndex = averagedGrid->getIndex(k, sourceL0, sourceM0);
				} else if (abs(targetL0 - targetL) <= (averagingFactor / 2) && abs(targetM1 - targetM) <= (averagingFactor / 2)) {
					sourceIndex = averagedGrid->getIndex(k, sourceL0, sourceM1);
				} else if (abs(targetL1 - targetL) <= (averagingFactor / 2) && abs(targetM0 - targetM) <= (averagingFactor / 2)) {
					sourceIndex = averagedGrid->getIndex(k, sourceL1, sourceM0);
				} else if (abs(targetL1 - targetL) <= (averagingFactor / 2) && abs(targetM1 - targetM) <= (averagingFactor / 2)) {
					sourceIndex = averagedGrid->getIndex(k, sourceL1, sourceM1);
				}
				collocatedAccessorAmin->setUByte(targetIndex, accessorAmin.getUByte(sourceIndex));
				collocatedAccessorFlags.setUShort(targetIndex, accessorFlags.getUShort(sourceIndex));
			}
		}
	}

    context.setLastComputedL(*collocatedSegment, *this, lastTargetL);
    context.setFirstRequiredL(*averagedSegment, *this, 0);
}

void Aei::getVertexes(const Accessor& accessor, long k, long l0, long l1, long m0, long m1, valarray<double>& values) const {
    values[0] = accessor.getDouble(averagedGrid->getIndex(k, l0, m0));
    values[1] = accessor.getDouble(averagedGrid->getIndex(k, l0, m1));
    values[2] = accessor.getDouble(averagedGrid->getIndex(k, l1, m0));
    values[3] = accessor.getDouble(averagedGrid->getIndex(k, l1, m1));
}

void Aei::computeWeights(long l, long l0, long l1, long m, long m0, long m1, valarray<double>& weights) const {
    weights[0] = (l + 0.5 - l0) * (m + 0.5 - m0) / (averagingFactor * averagingFactor);
    weights[1] = (l + 0.5 - l0) * (m1 - m - 0.5) / (averagingFactor * averagingFactor);
    weights[2] = (l1 - l - 0.5) * (m + 0.5 - m0) / (averagingFactor * averagingFactor);
    weights[3] = (l1 - l - 0.5) * (m1 - m - 0.5) / (averagingFactor * averagingFactor);
}

