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

#ifndef INTACCESSOR_H
#define	INTACCESSOR_H

#include "AbstractAccessor.h"

class IntAccessor: public virtual AbstractAccessor<int32_t, Constants::TYPE_INT> {
public:

	IntAccessor(size_t n, int32_t fillValue = numeric_limits<uint32_t>::min(), double scaleFactor = 1.0, double addOffset = 0.0) :
			AbstractAccessor<int32_t, Constants::TYPE_INT>(n, fillValue, scaleFactor, addOffset) {
	}

	virtual ~IntAccessor() {
	}

	valarray<int32_t>& getIntData() const throw (bad_cast) {
		return getTypedData();
	}
};

#endif	/* INTACCESSOR_H */

