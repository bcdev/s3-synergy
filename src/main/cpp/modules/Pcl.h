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
	vector<Accessor*> slnExceptionAccessors;
	vector<Accessor*> sloExceptionAccessors;

	static uint16_t computeOlcFlagValue(uint32_t currentOlcFlags);

	void setUpSourceAccessors(Context & context);
    void setUpSegment(Context & context);

	uint16_t computeSynFlagValue(uint32_t olcFlags, uint8_t slnFlags, uint8_t sloFlags, size_t index);
	uint16_t computeSlnFlagValue(uint32_t currentSlnFlags, size_t index);
	uint16_t computeSloFlagValue(uint32_t currentSloFlags, size_t index);
};

#endif /* PCL_H_ */
