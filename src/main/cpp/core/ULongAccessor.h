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

#ifndef ULONGACCESSOR_H
#define	ULONGACCESSOR_H

/**
 * A concrete implementation of the AbstractAccessor for the unsigned 'long' type.
 */
class ULongAccessor: public virtual AbstractAccessor<uint64_t, Constants::TYPE_ULONG> {
public:

    /**
     * Constructs a new instance of this class.
     * @param n The size of the underlying array.
     * @param fillValue The fill value used for the variable.
     */
	ULongAccessor(size_t n, uint64_t fillValue = 0) :
			AbstractAccessor<uint64_t, Constants::TYPE_ULONG>(n, fillValue) {
	}

	virtual ~ULongAccessor() {
	}

	valarray<uint64_t>& getULongData() const throw (bad_cast) {
		return getTypedData();
	}
};

#endif	/* ULONGACCESSOR_H */

