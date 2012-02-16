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

#ifndef LONGACCESSOR_H
#define	LONGACCESSOR_H

#include "AbstractAccessor.h"

class LongAccessor: public virtual AbstractAccessor<int64_t, Constants::TYPE_LONG> {
public:

    /**
     * Constructs a new instance of this class.
     * @param n The size of the underlying array.
     * @param fillValue The fill value used for the variable.
     */
	LongAccessor(size_t n, int64_t fillValue = numeric_limits<uint64_t>::min()) :
			AbstractAccessor<int64_t, Constants::TYPE_LONG>(n, fillValue) {
	}

	virtual ~LongAccessor() {
	}

	valarray<int64_t>& getLongData() const throw (bad_cast) {
		return getTypedData();
	}
};

#endif	/* LONGACCESSOR_H */

