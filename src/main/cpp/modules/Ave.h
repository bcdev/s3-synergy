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

#include "../modules/BasicModule.h"
#include "../core/Boost.h"

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
	void process(Context& context);

private:
	friend class AveTest;

    void addVariables(Context& context);
    void averageVariables(Logging& logging, long firstTargetL, long lastTargetL);
    void averageLatLon(long targetL);
    void averageFlags(long targetL);

    const Segment* sourceSegment;
    Segment* targetSegment;
    vector<string> variableNames;
    uint8_t averagingFactor;

    static bool isRadianceName(const string& variableName);
};

#endif /* AVE_H_ */
