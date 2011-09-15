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

protected:
	size_t getIndex(size_t k, size_t l, size_t m) const;
	uint16_t getValue(size_t index, long olcFlags, short slnFlags, short sloFlags) const;
	const Accessor& getSourceAccessor(Context& context, string variableName);

private:
	Segment* collocatedSegment;
    const Accessor* olcFlagsAccessor;
    const Accessor* slnFlagsAccessor;
    const Accessor* sloFlagsAccessor;

	void setUpSourceAccessors(Context & context);
    void setUpSegment(Context & context);
};

#endif /* PCL_H_ */
