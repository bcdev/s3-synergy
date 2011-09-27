/* 
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 *
 * File:   ByteAccessor.h
 * Author: ralf
 *
 * Created on December 17, 2010, 1:45 PM
 */

#ifndef BYTEACCESSOR_H
#define	BYTEACCESSOR_H

#include "AbstractAccessor.h"

class ByteAccessor: public virtual AbstractAccessor<int8_t, Constants::TYPE_BYTE> {
public:

	ByteAccessor(size_t n, int8_t fillValue = numeric_limits<uint8_t>::min(), double scaleFactor = 1.0, double addOffset = 0.0) :
			AbstractAccessor<int8_t, Constants::TYPE_BYTE>(n, fillValue, scaleFactor, addOffset) {
	}

	virtual ~ByteAccessor() {
	}

	valarray<int8_t>& getByteData() const throw (bad_cast) {
		return getTypedData();
	}
};

#endif	/* BYTEACCESSOR_H */

