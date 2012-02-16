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
