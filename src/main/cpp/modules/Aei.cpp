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
using std::max;
using std::min;
using std::numeric_limits;

Aei::Aei() :
        BasicModule("AEI") {
}

Aei::~Aei() {
}

void Aei::start(Context& context) {
    sourceSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
    sourceGrid = &sourceSegment->getGrid();
    targetSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    targetGrid = &targetSegment->getGrid();

    AuxdataProvider& configurationAuxdataProvider = (AuxdataProvider&)context.getObject(Constants::AUX_ID_SYCPAX);
    averagingFactor = configurationAuxdataProvider.getShort("ave_square");

    const SegmentDescriptor& targetSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
    targetSegment->addVariable(targetSegmentDescriptor.getVariableDescriptor("T550"));
    targetSegment->addVariable(targetSegmentDescriptor.getVariableDescriptor("T550_er"));
    targetSegment->addVariable(targetSegmentDescriptor.getVariableDescriptor("A550"));
    targetSegment->addVariable(targetSegmentDescriptor.getVariableDescriptor("AMIN"));
}

void Aei::process(Context& context) {
    const Accessor& aotSourceAccessor = sourceSegment->getAccessor("T550");
    const Accessor& aotErrorSourceAccessor = sourceSegment->getAccessor("T550_er");
    const Accessor& angstromExponentSourceAccessor = sourceSegment->getAccessor("A550");
    const Accessor& aerosolModelSourceAccessor = sourceSegment->getAccessor("AMIN");
    const Accessor& flagsSourceAccessor = sourceSegment->getAccessor("SYN_flags");

    Accessor& aotTargetAccessor = targetSegment->getAccessor("T550");
    Accessor& aotErrorTargetAccessor = targetSegment->getAccessor("T550_er");
    Accessor& angstromExponentTargetAccessor = targetSegment->getAccessor("A550");
    Accessor& aerosolModelTargetAccessor = targetSegment->getAccessor("AMIN");
    Accessor& flagsTargetAccessor = targetSegment->getAccessor("SYN_flags");

	const long lastSourceL = context.getLastComputableL(*sourceSegment, *this);
    const long firstTargetL = context.getFirstComputableL(*targetSegment, *this);
	context.getLogging().debug("Segment [" + targetSegment->toString() + "]: firstComputableL = " + lexical_cast<string>(firstTargetL), getId());
    long lastTargetL = context.getLastComputableL(*targetSegment, *this);
	context.getLogging().debug("Segment [" + targetSegment->toString() + "]: lastComputableL = " + lexical_cast<string>(lastTargetL), getId());

    for (long targetL = firstTargetL; targetL <= lastTargetL; targetL++) {
        context.getLogging().progress("Interpolating line l = " + lexical_cast<string>(targetL), getId());

        const long sourceL0 = min(max(0L, (long) floor((targetL - averagingFactor / 2) / averagingFactor)), sourceGrid->getMaxL() - 1L);
    	context.getLogging().debug("Segment [" + sourceSegment->toString() + "]: sourceL0 = " + lexical_cast<string>(sourceL0), getId());
        const long sourceL1 = sourceL0 + 1;

        if (sourceL1 > lastSourceL) {
        	lastTargetL = targetL - 1;
        	break;
        }

        const double targetL0 = sourceL0 * averagingFactor + averagingFactor / 2.0;
        const double targetL1 = sourceL1 + averagingFactor;

        for (long k = targetGrid->getFirstK(); k < targetGrid->getMaxK(); k++) {
			for (long targetM = targetGrid->getFirstM(); targetM < targetGrid->getMaxM(); targetM++) {
				const long sourceM0 = min(max(0L, (long) floor((targetM - averagingFactor / 2) / averagingFactor)), sourceGrid->getMaxM() - 1L);
				const long sourceM1 = sourceM0 + 1;

				const double targetM0 = sourceM0 * averagingFactor + averagingFactor / 2.0;
				const double targetM1 = targetM0 + averagingFactor;

				const double wl = (targetL + 0.5 - targetL0) / averagingFactor;
				const double wm = (targetM + 0.5 - targetM0) / averagingFactor;

				const double aot = interpolation(aotSourceAccessor, k, sourceL0, sourceL1, sourceM0, sourceM1, wl, wm);
				const double aotError = interpolation(aotErrorSourceAccessor, k, sourceL0, sourceL1, sourceM0, sourceM1, wl, wm);
				const double angstromExponent = interpolation(angstromExponentSourceAccessor, k, sourceL0, sourceL1, sourceM0, sourceM1, wl, wm);

				const size_t targetIndex = targetGrid->getIndex(k, targetL, targetM);
				if (aot == Constants::FILL_VALUE_DOUBLE) {
					aotTargetAccessor.setFillValue(targetIndex);
				} else {
					aotTargetAccessor.setDouble(targetIndex, aot);
				}
				if (aotError == Constants::FILL_VALUE_DOUBLE) {
					aotErrorTargetAccessor.setFillValue(targetIndex);
				} else {
					aotErrorTargetAccessor.setDouble(targetIndex, aotError);
				}
				if (angstromExponent == Constants::FILL_VALUE_DOUBLE) {
					angstromExponentTargetAccessor.setFillValue(targetIndex);
				} else {
					angstromExponentTargetAccessor.setDouble(targetIndex, angstromExponent);
				}

				size_t sourceIndex;
				if (abs(targetL0 - targetL) <= (averagingFactor / 2) && abs(targetM0 - targetM) <= (averagingFactor / 2)) {
					sourceIndex = sourceGrid->getIndex(k, sourceL0, sourceM0);
				} else if (abs(targetL0 - targetL) <= (averagingFactor / 2) && abs(targetM1 - targetM) <= (averagingFactor / 2)) {
					sourceIndex = sourceGrid->getIndex(k, sourceL0, sourceM1);
				} else if (abs(targetL1 - targetL) <= (averagingFactor / 2) && abs(targetM0 - targetM) <= (averagingFactor / 2)) {
					sourceIndex = sourceGrid->getIndex(k, sourceL1, sourceM0);
				} else if (abs(targetL1 - targetL) <= (averagingFactor / 2) && abs(targetM1 - targetM) <= (averagingFactor / 2)) {
					sourceIndex = sourceGrid->getIndex(k, sourceL1, sourceM1);
				}
				aerosolModelTargetAccessor.setUByte(targetIndex, aerosolModelSourceAccessor.getUByte(sourceIndex));
				flagsTargetAccessor.setUShort(targetIndex, flagsSourceAccessor.getUShort(sourceIndex));
			}
		}
	}

    context.setLastComputedL(*targetSegment, *this, lastTargetL);
    if (lastTargetL < targetGrid->getMaxL()) {
    	context.setFirstRequiredL(*sourceSegment, *this, lastSourceL);
    } else {
    	context.setFirstRequiredL(*sourceSegment, *this, -1);
    }
}

double Aei::interpolation(const Accessor& accessor, long k, long l0, long l1, long m0, long m1, double wl, double wm) const {
	double v00;
	double v01;
	double v10;
	double v11;
	double w00;
	double w01;
	double w10;
	double w11;

	const size_t i00 = sourceGrid->getIndex(k, l0, m0);
	if (accessor.isFillValue(i00)) {
		v00 = 0.0;
		w00 = 0.0;
	} else {
		v00 = accessor.getDouble(i00);
		w00 = (1.0 - wl) * (1.0 - wm);
	}
	const size_t i01 = sourceGrid->getIndex(k, l0, m1);
	if (accessor.isFillValue(i01)) {
		v01 = 0.0;
		w01 = 0.0;
	} else {
		v01 = accessor.getDouble(i01);
		w01 = (1.0 - wl) * wm;
	}
	const size_t i10 = sourceGrid->getIndex(k, l1, m0);
	if (accessor.isFillValue(i10)) {
		v10 = 0.0;
		w10 = 0.0;
	} else {
		v10 = accessor.getDouble(i10);
		w10 = wl * (1.0 - wm);
	}
	const size_t i11 = sourceGrid->getIndex(k, l1, m1);
	if (accessor.isFillValue(i11)) {
		v11 = 0.0;
		w11 = 0.0;
	} else {
		v11 = accessor.getDouble(i11);
		w11 = wl * wm;
	}
	if (w00 > 0.0 || w01 > 0.0 || w10 > 0.0 || w11 > 0.0) {
		return (w00 * v00 + w01 * v01 + w10 * v10 + w11 * v11) / (w00 + w01 + w10 + w11);
	}
	return Constants::FILL_VALUE_DOUBLE;
}
