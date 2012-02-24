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

#ifndef FLOATACCESSOR_H
#define	FLOATACCESSOR_H

#include "AbstractAccessor.h"

/**
 * A concrete implementation of the AbstractAccessor for the 'float' type.
 */
class FloatAccessor : public virtual AbstractAccessor<float, Constants::TYPE_FLOAT> {
public:

    /**
     * Constructs a new instance of this class.
     * @param n The size of the underlying array.
     * @param fillValue The fill value used for the variable.
     */
    FloatAccessor(size_t n, float fillValue = -numeric_limits<float>::max()) : AbstractAccessor<float, Constants::TYPE_FLOAT>(n, fillValue) {
    }

    virtual ~FloatAccessor() {
    }

    valarray<float>& getFloatData() const throw (bad_cast) {
        return getTypedData();
    }
};

#endif	/* FLOATACCESSOR_H */

