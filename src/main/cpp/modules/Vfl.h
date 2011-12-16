/*
 * Vfl.h
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#ifndef VFL_H_
#define VFL_H_

#include "../modules/BasicModule.h"

class Vfl : public BasicModule {
public:
	Vfl();
	virtual ~Vfl();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VflTest;

	bool isClear(const uint8_t flags, const double b0, const double mir) {
		return (isSet(flags, Constants::VGT_B0_GOOD_FLAG) && b0 < thresholdsCloud[0]) || (isSet(flags, Constants::VGT_MIR_GOOD_FLAG) && mir < thresholdsCloud[1]);
	}

	bool isCloud(const uint8_t flags, const double b0, const double mir) {
		return isSet(flags, Constants::VGT_B0_GOOD_FLAG) && b0 > thresholdsCloud[2] && isSet(flags, Constants::VGT_MIR_GOOD_FLAG) && mir > thresholdsCloud[3];
	}

	bool isSnowIce1(const uint8_t flags, const double b2) const {
        return isSet(flags, Constants::VGT_B2_GOOD_FLAG) && b2 > thresholdsSnowIce[0];
    }

    bool isSnowIce2(const uint8_t flags, const double & mir) const {
        return isSet(flags, Constants::VGT_MIR_GOOD_FLAG) && mir < thresholdsSnowIce[1];
    }

    bool isSnowIce3(const uint8_t flags, const double b0, const double mir, const double b3) const {
        return isSet(flags, Constants::VGT_B0_GOOD_FLAG) && isSet(flags, Constants::VGT_B3_GOOD_FLAG) && isSet(flags, Constants::VGT_MIR_GOOD_FLAG) && (b0 - mir) / ((b0 + b3) * 1000.0) > thresholdsSnowIce[2];
    }

    bool isSnowIce4(const uint8_t flags, const double b0, const double mir) const {
        return isSet(flags, Constants::VGT_B0_GOOD_FLAG) && isSet(flags, Constants::VGT_MIR_GOOD_FLAG) && (b0 - mir) / ((b0 + mir) * 1000.0) > thresholdsSnowIce[3];
    }

    bool isSnowIce5(const uint8_t & flags, const double b0, const double b2, const double mir) {
        return isSet(flags, Constants::VGT_B0_GOOD_FLAG) && isSet(flags, Constants::VGT_B2_GOOD_FLAG) && isSet(flags, Constants::VGT_MIR_GOOD_FLAG) && (b0 + b2) / 2.0 - mir > thresholdsSnowIce[4];
    }

    valarray<double> thresholdsCloud;
	valarray<double> thresholdsSnowIce;
};


#endif /* VFL_H_ */
