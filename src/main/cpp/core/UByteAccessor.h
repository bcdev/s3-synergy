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
 * File:   UByteAccessor.h
 * Author: ralf
 *
 * Created on December 18, 2010, 5:22 PM
 */

#ifndef UBYTEACCESSOR_H
#define	UBYTEACCESSOR_H

#include "AbstractAccessor.h"

class UByteAccessor: public virtual AbstractAccessor<uint8_t> {
public:

	UByteAccessor(size_t n, double scaleFactor = 1.0, double addOffset = 0.0) :
			AbstractAccessor<uint8_t>(n, scaleFactor, addOffset) {
	}

	virtual ~UByteAccessor() {
	}

	valarray<uint8_t>& getUByteData() const throw (bad_cast) {
		return getTypedData();
	}
};

#endif	/* UBYTEACCESSOR_H */

