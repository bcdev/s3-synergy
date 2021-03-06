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

#ifndef VFL_H_
#define VFL_H_

#include "../modules/BasicModule.h"

/**
 * The VGT pixel flagging module (VFL).
 */
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
