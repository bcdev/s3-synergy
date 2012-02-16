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

#ifndef UINTACCESSOR_H
#define	UINTACCESSOR_H

class UIntAccessor: public virtual AbstractAccessor<uint32_t, Constants::TYPE_UINT> {
public:

    /**
     * Constructs a new instance of this class.
     * @param n The size of the underlying array.
     * @param fillValue The fill value used for the variable.
     * @param scaleFactor The scale factor used for the variable.
     * @param scaleFactor The add-offset used for the variable.
     */
	UIntAccessor(size_t n, uint32_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) :
			AbstractAccessor<uint32_t, Constants::TYPE_UINT>(n, fillValue, scaleFactor, addOffset) {
	}

	virtual ~UIntAccessor() {
	}

	valarray<uint32_t>& getUIntData() const throw (bad_cast) {
		return getTypedData();
	}
};

#endif	/* UINTACCESSOR_H */

