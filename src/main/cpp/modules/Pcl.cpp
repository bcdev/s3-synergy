/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#include "Pcl.h"

Pcl::Pcl() :
		BasicModule("PCL") {
}

Pcl::~Pcl() {
}

void Pcl::start(Context& context) {
	Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);

	const ProductDescriptor& productDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2);
	const SegmentDescriptor& segmentDescriptor = productDescriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);

	context.getLogging().info("Adding variable 'SYN_flags' to segment '" + collocatedSegment.getId() + "'.", getId());
	collocatedSegment.addVariable(segmentDescriptor.getVariableDescriptor("SYN_flags"));

	context.getLogging().info("Adding variable 'OLC_flags' to segment '" + collocatedSegment.getId() + "'.", getId());
	collocatedSegment.addVariable(segmentDescriptor.getVariableDescriptor("OLC_flags"));

	context.getLogging().info("Adding variable 'SLN_flags' to segment '" + collocatedSegment.getId() + "'.", getId());
	collocatedSegment.addVariable(segmentDescriptor.getVariableDescriptor("SLN_flags"));

	context.getLogging().info("Adding variable 'SLO_flags' to segment '" + collocatedSegment.getId() + "'.", getId());
	collocatedSegment.addVariable(segmentDescriptor.getVariableDescriptor("SLO_flags"));
}

void Pcl::process(Context& context) {
	const Segment& segment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Accessor& sourceOlcFlagsAccessor = segment.getAccessor("OLC_confidence");
	const Accessor& sourceSlnFlagsAccessor = segment.getAccessor("SLN_confidence");
	const Accessor& sourceSloFlagsAccessor = segment.getAccessor("SLO_confidence");

	vector<const Accessor*> radianceAccessors;
	for (size_t b = 1; b <= 30; b++) {
		radianceAccessors.push_back(&segment.getAccessor("L_" + lexical_cast<string>(b)));
	}

	vector<const Accessor*> slnExceptionAccessors;
	vector<const Accessor*> sloExceptionAccessors;
	for (size_t b = 1; b <= 6; b++) {
		slnExceptionAccessors.push_back(&segment.getAccessor("L_" + lexical_cast<string>(b + 18) + "_exception"));
		sloExceptionAccessors.push_back(&segment.getAccessor("L_" + lexical_cast<string>(b + 24) + "_exception"));
	}

	Accessor& targetSynFlagsAccessor = segment.getAccessor("SYN_flags");
	Accessor& targetOlcFlagsAccessor = segment.getAccessor("OLC_flags");
	Accessor& targetSlnFlagsAccessor = segment.getAccessor("SLN_flags");
	Accessor& targetSloFlagsAccessor = segment.getAccessor("SLO_flags");

	const valarray<uint32_t>& sourceOlcFlags = sourceOlcFlagsAccessor.getUIntData();
	const valarray<uint8_t>& sourceSlnFlags = sourceSlnFlagsAccessor.getUByteData();
	const valarray<uint8_t>& sourceSloFlags = sourceSloFlagsAccessor.getUByteData();

	const long firstL = context.getFirstComputableL(segment, *this);
	context.getLogging().debug("Segment [" + segment.toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	long lastL = context.getLastComputableL(segment, *this);
	context.getLogging().debug("Segment [" + segment.toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

	const Grid& collocatedGrid = segment.getGrid();

#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) > 40100
#pragma omp parallel for
#endif
	for (long l = firstL; l <= lastL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());
		for (long k = collocatedGrid.getMinK(); k <= collocatedGrid.getMaxK(); k++) {
			for (long m = collocatedGrid.getMinM(); m <= collocatedGrid.getMaxM(); m++) {
				const size_t index = collocatedGrid.getIndex(k, l, m);

				const uint32_t fOlc = sourceOlcFlags[index];
				const uint8_t fSln = sourceSlnFlags[index];
				const uint8_t fSlo = sourceSloFlags[index];

				targetOlcFlagsAccessor.setUShort(index, computeOlcSummaryFlags(fOlc));
				targetSlnFlagsAccessor.setUInt(index, computeSlsSummaryFlags(fSln, slnExceptionAccessors, index));
				targetSloFlagsAccessor.setUInt(index, computeSlsSummaryFlags(fSlo, sloExceptionAccessors, index));
				targetSynFlagsAccessor.setUShort(index, computeSynFlags(fOlc, fSln, fSlo, radianceAccessors, index));
			}
		}
	}
	context.setLastComputedL(segment, *this, lastL);
}

uint16_t Pcl::computeOlcSummaryFlags(uint32_t sourceFlags) {
	uint16_t targetFlags = 0;

	bool summarySatured = false;
	// TODO - extract constants for OLCI flags
	for (uint32_t saturatedFlag = 2097152U; saturatedFlag >= 2U; saturatedFlag = saturatedFlag >> 1) {
		summarySatured |= isSet(sourceFlags, saturatedFlag);
	}

	targetFlags |= summarySatured ? 1 : 0;
	// TODO - extract constants for OLCI flags
	targetFlags |= isSet(sourceFlags, 4194304U) ? 2 : 0;
	targetFlags |= isSet(sourceFlags, 8388608U) ? 4 : 0;
	targetFlags |= isSet(sourceFlags, 33554432U) ? 8 : 0;
	targetFlags |= isSet(sourceFlags, 67108864U) ? 16 : 0;
	targetFlags |= isSet(sourceFlags, 134217728U) ? 256 : 0;
	targetFlags |= isSet(sourceFlags, 268435456U) ? 512 : 0;
	targetFlags |= isSet(sourceFlags, 536870912U) ? 1024 : 0;
	targetFlags |= isSet(sourceFlags, 1073741824U) ? 2048 : 0;
	targetFlags |= isSet(sourceFlags, 2147483648U) ? 4096 : 0;

	return targetFlags;
}

uint32_t Pcl::computeSlsSummaryFlags(uint8_t sourceFlags, const vector<const Accessor*>& exceptionAccessors, size_t index) {
	uint32_t targetFlags = 0;

	bool summaryIspAbsent = false;
	bool summaryPixelAbsent = false;
	bool summaryNotDecompressed = false;
	bool summaryNoSignal = false;
	bool summarySaturation = false;
	bool summaryInvalidRadiance = false;
	bool summaryNoParameters = false;
	bool summaryUnfilledPixel = false;

	foreach (const Accessor* exceptionAccessor, exceptionAccessors)
			{
				const uint8_t exceptionFlag = exceptionAccessor->getUByte(index);
				// TODO - extract constants for exception flags
				summaryIspAbsent |= isSet<uint8_t>(exceptionFlag, 1);
				summaryPixelAbsent |= isSet<uint8_t>(exceptionFlag, 2);
				summaryNotDecompressed |= isSet<uint8_t>(exceptionFlag, 4);
				summaryNoSignal |= isSet<uint8_t>(exceptionFlag, 8);
				summarySaturation |= isSet<uint8_t>(exceptionFlag, 16);
				summaryInvalidRadiance |= isSet<uint8_t>(exceptionFlag, 32);
				summaryNoParameters |= isSet<uint8_t>(exceptionFlag, 64);
				summaryUnfilledPixel |= isSet<uint8_t>(exceptionFlag, 128);
			}

	targetFlags |= summaryIspAbsent ? 1 : 0;
	targetFlags |= summaryPixelAbsent ? 2 : 0;
	targetFlags |= summaryNotDecompressed ? 4 : 0;
	targetFlags |= summaryNoSignal ? 8 : 0;
	targetFlags |= summarySaturation ? 16 : 0;
	targetFlags |= summaryInvalidRadiance ? 32 : 0;
	targetFlags |= summaryNoParameters ? 64 : 0;
	targetFlags |= summaryUnfilledPixel ? 128 : 0;

	// TODO - extract constants for SLSTR source and target flags (NOTE: test product does not comply with specification)
	targetFlags |= isSet<uint8_t>(sourceFlags, 1) ? 1 << 16 : 0; // cosmetic
	targetFlags |= isSet<uint8_t>(sourceFlags, 4) ? 1 << 18 : 0; // day
	targetFlags |= isSet<uint8_t>(sourceFlags, 8) ? 1 << 19 : 0; // twilight
	targetFlags |= isSet<uint8_t>(sourceFlags, 16) ? 1 << 20 : 0; // sun_glint
	targetFlags |= isSet<uint8_t>(sourceFlags, 32) ? 1 << 21 : 0; // snow
	targetFlags |= isSet<uint8_t>(sourceFlags, 64) ? 1 << 22 : 0; // summary_cloud
	targetFlags |= isSet<uint8_t>(sourceFlags, 128) ? 1 << 23 : 0; // summary_pointing

	return targetFlags;
}

uint16_t Pcl::computeSynFlags(uint32_t olcFlags, uint8_t slnFlags, uint8_t sloFlags, const vector<const Accessor*>& radianceAccessors, size_t index) {
	const bool land = isSet(olcFlags, Constants::SY1_OLCI_LAND_FLAG);
	const bool cloud = isSet(slnFlags, Constants::SY1_SLSTR_CLOUD_FLAG) || isSet(sloFlags, Constants::SY1_SLSTR_CLOUD_FLAG);

	uint16_t synFlags = 0;
	if (land) {
		synFlags |= Constants::SY2_LAND_FLAG;
	}
	if (cloud) {
		synFlags |= Constants::SY2_CLOUD_FLAG;
	}

	bool noOlc = true;
	bool noSln = true;
	bool noSlo = true;
	for (size_t b = 0; b < 18; b++) {
		noOlc &= radianceAccessors[b]->isFillValue(index);
	}
	for (size_t b = 18; b < 24; b++) {
		noSln &= radianceAccessors[b]->isFillValue(index);
	}
	for (size_t b = 24; b < 30; b++) {
		noSlo &= radianceAccessors[b]->isFillValue(index);
	}
	if (noOlc) {
		synFlags |= Constants::SY2_NO_OLC_FLAG;
	}
	if (noSln) {
		synFlags |= Constants::SY2_NO_SLN_FLAG;
	}
	if (noSlo) {
		synFlags |= Constants::SY2_NO_SLO_FLAG;
	}

	return synFlags;
}

