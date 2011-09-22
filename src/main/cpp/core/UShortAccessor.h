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
 * File:   UShortAccessor.h
 * Author: ralf
 *
 * Created on December 18, 2010, 5:25 PM
 */

#ifndef USHORTACCESSOR_H
#define	USHORTACCESSOR_H

class UShortAccessor: public virtual AbstractAccessor<uint16_t, Constants::TYPE_USHORT> {
public:

	UShortAccessor(size_t n, uint16_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) :
			AbstractAccessor<uint16_t, Constants::TYPE_USHORT>(n, fillValue, scaleFactor, addOffset) {
	}

	virtual ~UShortAccessor() {
	}

	valarray<uint16_t>& getUShortData() const throw (bad_cast) {
		return getTypedData();
	}
};

#endif	/* USHORTACCESSOR_H */

