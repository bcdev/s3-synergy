/*
 * Pcl.h
 *
 *  Created on: Sep 14, 2011
 *      Author: thomasstorm
 */

#ifndef PCL_H_
#define PCL_H_

#include "../modules/BasicModule.h"

class Pcl: public BasicModule {
public:
	Pcl();
	virtual ~Pcl();

	void start(Context& context);
	void process(Context& context);

private:
	friend class PclTest;

	Segment* collocatedSegment;
    const Accessor* olcFlagsAccessor;
    const Accessor* slnFlagsAccessor;
    const Accessor* sloFlagsAccessor;
	vector<Accessor*> radianceAccessors;
    string targetVariableName;

	size_t getIndex(long k, long l, long m) const;
	uint16_t getValue(uint32_t olcFlags, uint8_t slnFlags, uint8_t sloFlags) const;
	const Accessor& getSourceAccessor(Context& context, string variableName, string sourceSegmentId);
	void setUpSourceAccessors(Context & context);
    void setUpSegment(Context & context);

	static const uint32_t SY1_OLCI_LAND_FLAG = 2147483648U;
	static const uint8_t SY1_SLSTR_CLOUD_FLAG = 64U;

	static const uint16_t SY2_CLOUD_FLAG = 1U;
	static const uint16_t SY2_LAND_FLAG = 32U;
	static const uint16_t SY2_NO_SLN_FLAG = 64U;
	static const uint16_t SY2_NO_SLO_FLAG = 128U;
};

#endif /* PCL_H_ */
