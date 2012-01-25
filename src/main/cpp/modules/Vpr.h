/*
 * Vpr.h
 *
 *  Created on: Dec 01, 2011
 *      Author: thomasstorm
 */

#ifndef VPR_H_
#define VPR_H_

#include "../core/Pixel.h"
#include "../modules/BasicModule.h"
#include "../util/PixelFinder.h"

class Vpr : public BasicModule, private GeoLocation {
public:
	Vpr();
	virtual ~Vpr();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VprTest;

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
	void getMinMaxSourceLon(double& minLon, double& maxLon) const;

	double getTargetLat(long l) const ;
	double getTargetLon(long m) const;

	double getSubsampledTargetLat(long l) const ;
	double getSubsampledTargetLon(long m) const;

	void setValue(Accessor* sourceAccessor, Accessor* targetAccessor, size_t sourceIndex, size_t targetIndex) const;

	void setMapLats(Context& context) const;
    void setMapLons(Context& context) const;
    void setMapLatBounds(Context& context) const;
    void setMapLonBounds(Context& context) const;
    void setTpLats(Context& context) const;
    void setTpLons(Context& context) const;
    void setTpLatBounds(Context& context) const;
    void setTpLonBounds(Context& context) const;

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

#endif /* VPR_H_ */
