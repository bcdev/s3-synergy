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

#ifndef VPR_H_
#define VPR_H_

#include "../modules/BasicModule.h"
#include "../util/PixelFinder.h"

/**
 * The VGT-P projection module (VPR).
 */
class Vpr : public BasicModule, private GeoLocation {
public:
	Vpr();
	virtual ~Vpr();

	void start(Context& context);
	void stop(Context& context);
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
