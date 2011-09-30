/*
 * Ave.h
 *
 *  Created on: Sep 22, 2011
 *      Author: thomasstorm
 */

#ifndef AVE_H_
#define AVE_H_

#include <stdexcept>
#include <vector>

#include "../core/Boost.h"
#include "../core/BasicModule.h"

using std::logic_error;

/**
 * AVE - the SYN L2 averaging module.
 *
 * @author Thomas Storm
 */
class Ave: public BasicModule {
public:
	Ave();

	virtual ~Ave();

	void start(Context& context);
	void stop(Context& context);
	void process(Context& context);

private:
	friend class AveTest;

    const Grid* averagedGrid;
    const Segment* collocatedSegment;
    Segment* averagedSegment;
    vector<VariableDescriptor*> variables;
    const Accessor* synFlags;

	static const int8_t AVERAGING_FACTOR;

    bool isValidPosition(const Grid* grid, long k, long l, long m) const;
    bool isFillValue(const string& variableName, const long index) const;
    double getValue(const string& variableName, const long index) const;
};

#endif /* AVE_H_ */
