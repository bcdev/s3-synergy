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

    void addVariables(Context& context);
    void averageVariables(Logging& logging, long firstTargetL, long lastTargetL);
    void averageLatLon(long targetL);
    void averageFlags(long targetL);
    void averageSolarIrradiances(long targetL);

    static void accumulateLatLon(double lat, double lon, double& x, double& y, double& z);
    static bool isRadianceName(const string& variableName);
    static void toLatLon(double x, double y, double z, double& lat, double& lon);
    void processLine(const Grid & targetGrid, long  targetL, const Grid & sourceGrid, const Accessor & sourceFlagsAccessor);

    const Segment* geoSegment;
    const Segment* sourceSegment;
    Segment* targetSegment;
    vector<string> variableNames;
    uint8_t averagingFactor;

    static const double D2R = 3.14159265358979323846 / 180.0;
    static const double R2D = 180.0 / 3.14159265358979323846;
};

#endif /* AVE_H_ */
