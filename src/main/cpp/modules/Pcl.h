/*
 * Pcl.h
 *
 *  Created on: Sep 14, 2011
 *      Author: thomasstorm
 */

#ifndef PCL_H_
#define PCL_H_

#include "../core/BasicModule.h"

class Pcl: public BasicModule {
public:
	Pcl();
	virtual ~Pcl();

	void start(Context& context);
	void stop(Context& context);
	void process(Context& context);


private:
	friend class PclTest;

	Segment* collocatedSegment;
    const Accessor* olcFlagsAccessor;
    const Accessor* slnFlagsAccessor;
    const Accessor* sloFlagsAccessor;
    string targetVariableName;

	size_t getIndex(long k, long l, long m) const;
	uint16_t getValue(size_t index, uint32_t olcFlags, uint8_t slnFlags, uint8_t sloFlags) const;
	const Accessor& getSourceAccessor(Context& context, string variableName, string sourceSegmentId);
	void setUpSourceAccessors(Context & context);
    void setUpSegment(Context & context);
};

#endif /* PCL_H_ */
