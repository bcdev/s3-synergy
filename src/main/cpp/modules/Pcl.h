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

	void setUpSourceAccessors(Context & context);
    void setUpSegment(Context & context);

	static uint16_t computeFlagValue(uint32_t olcFlags, uint8_t slnFlags, uint8_t sloFlags);
};

#endif /* PCL_H_ */
