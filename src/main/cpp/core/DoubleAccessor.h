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

#ifndef DOUBLEACCESSOR_H
#define	DOUBLEACCESSOR_H

#include "AbstractAccessor.h"

/**
 * A concrete implementation of the AbstractAccessor for the 'double' type.
 */
class DoubleAccessor : public virtual AbstractAccessor<double, Constants::TYPE_DOUBLE> {
public:

    /**
     * Constructs a new instance of this class.
     * @param n The size of the underlying array.
     * @param fillValue The fill value used for the variable.
     */
    DoubleAccessor(size_t n, double fillValue = -numeric_limits<double>::max()) : AbstractAccessor<double, Constants::TYPE_DOUBLE>(n, fillValue) {
    }

    virtual ~DoubleAccessor() {
    }

	double getDouble(size_t i) const throw (bad_cast, out_of_range) {
		return getDoubleData()[at(i)];
	}

	void setDouble(size_t i, double value) throw (bad_cast, out_of_range) {
		getDoubleData()[at(i)] = value;
	}

	float getFloat(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<float>(getDoubleData()[at(i)]);
	}

	void setFloat(size_t i, float value) throw (bad_cast, out_of_range) {
		getDoubleData()[at(i)] = boost::numeric_cast<double>(value);
	}

	valarray<double>& getDoubleData() const throw (bad_cast) {
        return getTypedData();
    }
};


#endif	/* DOUBLEACCESSOR_H */

