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

#ifndef SHORTACCESSOR_H
#define	SHORTACCESSOR_H

#include "AbstractAccessor.h"

class ShortAccessor: public virtual AbstractAccessor<int16_t, Constants::TYPE_SHORT> {
public:

	ShortAccessor(size_t n, int16_t fillValue = numeric_limits<uint16_t>::min(), double scaleFactor = 1.0, double addOffset = 0.0) :
			AbstractAccessor<int16_t, Constants::TYPE_SHORT>(n, fillValue, scaleFactor, addOffset) {
	}

	virtual ~ShortAccessor() {
	}

	valarray<int16_t>& getShortData() const throw (bad_cast) {
		return getTypedData();
	}
};

#endif	/* SHORTACCESSOR_H */

