/*
 * Vco.h
 *
 *  Created on: Feb 6, 2012
 *      Author: ralf
 */

#ifndef VCO_H_
#define VCO_H_

#include "BasicModule.h"
#include "../util/PixelFinder.h"

class Vco: public BasicModule, private GeoLocation {
public:
	Vco();
	virtual ~Vco();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VcoTest;

	void addTargetSegments(Context& context);
	void addTargetVariables(Context& context);

	double getLat(size_t index) const {
		return latAccessor->getDouble(index);
	}

	double getLon(size_t index) const {
		return lonAccessor->getDouble(index);
	}

	const Grid& getGrid() const {
		return geoSegment->getGrid();
	}

	void getMinMaxSourceLat(double& minLat, double& maxLat) const;
	void getMinMaxTargetLat(double& minLat, double& maxLat, long firstL, long lastL) const;

	double getTargetLat(long l) const ;
	double getTargetLon(long m) const;

	void setValue(Accessor* sourceAccessor, Accessor* targetAccessor, size_t sourceIndex, size_t targetIndex) const;

	void setMapLats(Context& context) const;
    void setMapLons(Context& context) const;
    void setMapLatBounds(Context& context) const;
    void setMapLonBounds(Context& context) const;

	int maxTargetLat;
	int minTargetLat;
	int maxTargetLon;
	int minTargetLon;

	const Segment* synSegment;
	const Segment* geoSegment;

	const Accessor* latAccessor;
	const Accessor* lonAccessor;

    static const int TARGET_PIXELS_PER_DEGREE = 112;
    static const int SUBSAMPLED_TARGET_PIXELS_PER_DEGREE = 14;
	static const double DEGREES_PER_TARGET_PIXEL = 1.0 / TARGET_PIXELS_PER_DEGREE;
	static const double DEGREES_PER_SUBSAMPLED_TARGET_PIXEL = 1.0 / SUBSAMPLED_TARGET_PIXELS_PER_DEGREE;
};

#endif /* VCO_H_ */
