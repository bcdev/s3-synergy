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
 * File:   DoubleAccessor.h
 * Author: ralf
 *
 * Created on December 17, 2010, 1:19 PM
 */

#ifndef DOUBLEACCESSOR_H
#define	DOUBLEACCESSOR_H

#include "AbstractAccessor.h"

class DoubleAccessor : public virtual AbstractAccessor<double> {
public:

    DoubleAccessor(size_t n) : AbstractAccessor<double>(n) {
    }

    virtual ~DoubleAccessor() {
    }

    valarray<double>& getDoubleData() const throw (bad_cast) {
        return getTypedData();
    }
};


#endif	/* DOUBLEACCESSOR_H */

