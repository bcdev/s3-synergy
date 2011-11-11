/*
 * Aei.cpp
 *
 *  Created on: Oct 24, 2011
 *      Author: thomasstorm
 */

#include <algorithm>
#include <cmath>

#include "Aei.h"

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

        long sourceL0 = min(0.0, floor((targetL - averagingFactor / 2) / averagingFactor));
        long sourceL1 = sourceL0 + 1;

        long targetL0 = sourceL0 * averagingFactor + averagingFactor / 2;
        long targetL1 = sourceL1 * averagingFactor + averagingFactor / 2;

        for (long m = collocatedGrid->getFirstM(); m < collocatedGrid->getMaxM(); m++) {
            long averagedM0 = min(0.0, floor((m - averagingFactor / 2) / averagingFactor));
            long averagedM1 = averagedM0 + 1;

            double m0 = averagedM0 * averagingFactor + averagingFactor / 2;
            double m1 = averagedM1 * averagingFactor + averagingFactor / 2;

            computeWeights(targetL, targetL0, targetL1, m, m0, m1, weights);

            for (long k = collocatedGrid->getFirstK(); k < collocatedGrid->getMaxK(); k++) {

                double tau550 = 0.0;
                double tau550err = 0.0;
                double alpha550 = 0.0;

                getAveragedTaus(k, sourceL0, sourceL1, averagedM0, averagedM1, averagedTaus);
                getAveragedTauErrors(k, sourceL0, sourceL1, averagedM0, averagedM1, averagedTauErrors);
                getAveragedAlphas(k, sourceL0, sourceL1, averagedM0, averagedM1, averagedAlphas);

                for (size_t i = 0; i < weights.size(); i++) {
                    tau550 += weights[i] * averagedTaus[i];
                    tau550err += weights[i] * averagedTauErrors[i];
                    alpha550 += weights[i] * averagedAlphas[i];
                }

                const size_t collocatedIndex = collocatedGrid->getIndex(k, targetL, m);
                collocatedAccessorAlpha550->setDouble(collocatedIndex, alpha550);
                collocatedAccessorTau550->setDouble(collocatedIndex, tau550);
                collocatedAccessorTau550err->setDouble(collocatedIndex, tau550err);

                size_t averagedIndex;
                if ((abs(targetL0 - targetL) <= (averagingFactor / 2)) && (abs(m0 - m) <= (averagingFactor / 2))) {
                    averagedIndex = averagedGrid->getIndex(k, sourceL0, averagedM0);
                } else if ((abs(targetL0 - targetL) <= (averagingFactor / 2)) && (abs(m1 - m) <= (averagingFactor / 2))) {
                    averagedIndex = averagedGrid->getIndex(k, sourceL0, averagedM1);
                } else if ((abs(targetL1 - targetL) <= (averagingFactor / 2)) && (abs(m0 - m) <= (averagingFactor / 2))) {
                    averagedIndex = averagedGrid->getIndex(k, sourceL1, averagedM0);
                } else if ((abs(targetL1 - targetL) <= (averagingFactor / 2)) && (abs(m1 - m) <= (averagingFactor / 2))) {
                    averagedIndex = averagedGrid->getIndex(k, sourceL1, averagedM1);
                } else {
                    BOOST_THROW_EXCEPTION(logic_error("no averaged index found for collocated index k=" + lexical_cast<string>(k) + ",l=" + lexical_cast<string>(targetL) + ",m=" + lexical_cast<string>(m) ));
                }
                collocatedAccessorAmin->setShort(collocatedIndex, accessorAmin.getShort(averagedIndex));
                collocatedAccessorFlags.setUShort(collocatedIndex, accessorFlags.getUShort(averagedIndex));
            }
        }
    }
}

void Aei::getAveragedTaus(long k, long averagedL0, long averagedL1, long averagedM0, long averagedM1, valarray<double>& averagedTaus) const {
    averagedTaus[0] = accessorTau->getDouble(averagedGrid->getIndex(k, averagedL0, averagedM0));
    averagedTaus[1] = accessorTau->getDouble(averagedGrid->getIndex(k, averagedL0, averagedM1));
    averagedTaus[2] = accessorTau->getDouble(averagedGrid->getIndex(k, averagedL1, averagedM0));
    averagedTaus[3] = accessorTau->getDouble(averagedGrid->getIndex(k, averagedL1, averagedM1));
}

void Aei::getAveragedTauErrors(long k, long averagedL0, long averagedL1, long averagedM0, long averagedM1, valarray<double>& averagedTauErrors) const {
    averagedTauErrors[0] = accessorTauError->getDouble(averagedGrid->getIndex(k, averagedL0, averagedM0));
    averagedTauErrors[1] = accessorTauError->getDouble(averagedGrid->getIndex(k, averagedL0, averagedM1));
    averagedTauErrors[2] = accessorTauError->getDouble(averagedGrid->getIndex(k, averagedL1, averagedM0));
    averagedTauErrors[3] = accessorTauError->getDouble(averagedGrid->getIndex(k, averagedL1, averagedM1));
}

void Aei::getAveragedAlphas(long k, long averagedL0, long averagedL1, long averagedM0, long averagedM1, valarray<double>& averagedAlphas) const {
    averagedAlphas[0] = accessorAlpha->getDouble(averagedGrid->getIndex(k, averagedL0, averagedM0));
    averagedAlphas[1] = accessorAlpha->getDouble(averagedGrid->getIndex(k, averagedL0, averagedM1));
    averagedAlphas[2] = accessorAlpha->getDouble(averagedGrid->getIndex(k, averagedL1, averagedM0));
    averagedAlphas[3] = accessorAlpha->getDouble(averagedGrid->getIndex(k, averagedL1, averagedM1));
}

void Aei::computeWeights(long l, long l0, long l1, long m, long m0, long m1, valarray<double>& weights) const {
    weights[0] = (l + 0.5 - l0) * (m + 0.5 - m0) / (averagingFactor * averagingFactor);
    weights[1] = (l + 0.5 - l0) * (m1 - m - 0.5) / (averagingFactor * averagingFactor);
    weights[2] = (l1 - l - 0.5) * (m + 0.5 - m0) / (averagingFactor * averagingFactor);
    weights[3] = (l1 - l - 0.5) * (m1 - m - 0.5) / (averagingFactor * averagingFactor);
}

