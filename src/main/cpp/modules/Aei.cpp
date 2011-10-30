/*
 * Aei.cpp
 *
 *  Created on: Oct 24, 2011
 *      Author: thomasstorm
 */

#include <cmath>

#include "Aei.h"
#include "../core/Boost.h"
#include "../util/AuxdataProvider.h"

using std::min;
using std::floor;
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
}

void Aei::process(Context& context) {
    context.getLogging().progress("Performing aerosol interpolation...", getId());

    const Accessor& accessorTau550 = averagedSegment->getAccessor("T550");
    const Accessor& accessorTau550err = averagedSegment->getAccessor("T550err");
    const Accessor& accessorAlpha550 = averagedSegment->getAccessor("A550");
    const Accessor& accessorAmin = averagedSegment->getAccessor("AMIN");
    const Accessor& accessorFlags = averagedSegment->getAccessor("SYN_flags");

    Accessor& collocatedAccessorTau550 = collocatedSegment->getAccessor("T550");
    Accessor& collocatedAccessorTau550err = collocatedSegment->getAccessor("T550err");
    Accessor& collocatedAccessorAlpha550 = collocatedSegment->getAccessor("A550");
    Accessor& collocatedAccessorAmin = collocatedSegment->getAccessor("AMIN");
    Accessor& collocatedAccessorFlags = collocatedSegment->getAccessor("SYN_flags");

    long firstL = context.getFirstComputableL(*averagedSegment, *this);
    long lastL = context.getLastComputableL(*averagedSegment, *this);

    for (long l = firstL; l <= lastL; l++) {
        if (l % 100 == 0) {
            context.getLogging().progress("   ...in line " + lexical_cast<string>(l), getId());
        }
        for (long k = collocatedGrid->getFirstK(); k < collocatedGrid->getMaxK(); k++) {
            for (long m = collocatedGrid->getFirstM(); m < collocatedGrid->getMaxM(); m++) {

                long l_0_prime = min(0.0, floor((l - averagingFactor / 2) / averagingFactor));
                long m_0_prime = min(0.0, floor((m - averagingFactor / 2) / averagingFactor));
                long l_1_prime = l_0_prime + 1;
                long m_1_prime = m_0_prime + 1;

                double l_0 = l_0_prime * averagingFactor + averagingFactor / 2;
                double m_0 = m_0_prime * averagingFactor + averagingFactor / 2;
                double l_1 = l_1_prime * averagingFactor + averagingFactor / 2;
                double m_1 = m_1_prime * averagingFactor + averagingFactor / 2;

                double w_0_0 = (l + 0.5 - l_0) * (m + 0.5 - m_0) / (averagingFactor * averagingFactor);
                double w_0_1 = (l + 0.5 - l_0) * (m_1 - m - 0.5) / (averagingFactor * averagingFactor);
                double w_1_0 = (l_1 - l - 0.5) * (m + 0.5 - m_0) / (averagingFactor * averagingFactor);
                double w_1_1 = (l_1 - l - 0.5) * (m_1 - m - 0.5) / (averagingFactor * averagingFactor);

                double averagedTau550_0_0 = accessorTau550.getDouble(averagedGrid->getIndex(k, l_0_prime, m_0_prime));
                double averagedTau550_0_1 = accessorTau550.getDouble(averagedGrid->getIndex(k, l_0_prime, m_1_prime));
                double averagedTau550_1_0 = accessorTau550.getDouble(averagedGrid->getIndex(k, l_1_prime, m_0_prime));
                double averagedTau550_1_1 = accessorTau550.getDouble(averagedGrid->getIndex(k, l_1_prime, m_1_prime));

                double averagedTau550err_0_0 = accessorTau550err.getDouble(averagedGrid->getIndex(k, l_0_prime, m_0_prime));
                double averagedTau550err_0_1 = accessorTau550err.getDouble(averagedGrid->getIndex(k, l_0_prime, m_1_prime));
                double averagedTau550err_1_0 = accessorTau550err.getDouble(averagedGrid->getIndex(k, l_1_prime, m_0_prime));
                double averagedTau550err_1_1 = accessorTau550err.getDouble(averagedGrid->getIndex(k, l_1_prime, m_1_prime));

                double averagedAlpha550_0_0 = accessorAlpha550.getDouble(averagedGrid->getIndex(k, l_0_prime, m_0_prime));
                double averagedAlpha550_0_1 = accessorAlpha550.getDouble(averagedGrid->getIndex(k, l_0_prime, m_1_prime));
                double averagedAlpha550_1_0 = accessorAlpha550.getDouble(averagedGrid->getIndex(k, l_1_prime, m_0_prime));
                double averagedAlpha550_1_1 = accessorAlpha550.getDouble(averagedGrid->getIndex(k, l_1_prime, m_1_prime));

                double tau550 = 0.0;
                double tau550err = 0.0;
                double alpha550 = 0.0;

                tau550 += w_0_0 * averagedTau550_0_0;
                tau550 += w_0_1 * averagedTau550_0_1;
                tau550 += w_1_0 * averagedTau550_1_0;
                tau550 += w_1_1 * averagedTau550_1_1;

                tau550err += w_0_0 * averagedTau550err_0_0;
                tau550err += w_0_1 * averagedTau550err_0_1;
                tau550err += w_1_0 * averagedTau550err_1_0;
                tau550err += w_1_1 * averagedTau550err_1_1;

                alpha550 += w_0_0 * averagedAlpha550_0_0;
                alpha550 += w_0_1 * averagedAlpha550_0_1;
                alpha550 += w_1_0 * averagedAlpha550_1_0;
                alpha550 += w_1_1 * averagedAlpha550_1_1;

                const size_t collocatedIndex = collocatedGrid->getIndex(k, l, m);
                collocatedAccessorAlpha550.setDouble(collocatedIndex, alpha550);
                collocatedAccessorTau550.setDouble(collocatedIndex, tau550);
                collocatedAccessorTau550err.setDouble(collocatedIndex, tau550err);

                size_t averagedIndex;
                if((abs(l_0 - l) <= (averagingFactor / 2) ) && (abs(m_0 - m) <= (averagingFactor / 2)) ) {
                    averagedIndex = averagedGrid->getIndex(k, l_0_prime, m_0_prime);
                } else if ((abs(l_0 - l) <= (averagingFactor / 2) ) && (abs(m_1 - m) <= (averagingFactor / 2)) ) {
                    averagedIndex = averagedGrid->getIndex(k, l_0_prime, m_0_prime);
                }  else if ((abs(l_1 - l) <= (averagingFactor / 2) ) && (abs(m_0 - m) <= (averagingFactor / 2)) ) {
                    averagedIndex = averagedGrid->getIndex(k, l_0_prime, m_0_prime);
                } else if ((abs(l_1 - l) <= (averagingFactor / 2) ) && (abs(m_1 - m) <= (averagingFactor / 2)) ) {
                    averagedIndex = averagedGrid->getIndex(k, l_0_prime, m_0_prime);
                } else {
                    BOOST_THROW_EXCEPTION(logic_error("no averaged index found for collocated index k=" + lexical_cast<string>(k) + ",l=" + lexical_cast<string>(l) + ",m=" + lexical_cast<string>(m) ));
                }
                collocatedAccessorAmin.setShort(collocatedIndex, accessorAmin.getShort(averagedIndex));
                collocatedAccessorFlags.setUShort(collocatedIndex, accessorFlags.getUShort(averagedIndex));
            }
        }
    }
}

