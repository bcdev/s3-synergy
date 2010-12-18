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
 * File:   IntAccessor.h
 * Author: ralf
 *
 * Created on December 17, 2010, 5:11 PM
 */

#ifndef INTACCESSOR_H
#define	INTACCESSOR_H

class IntAccessor : public virtual AbstractAccessor<int32_t> {
public:

    IntAccessor(size_t n) : AbstractAccessor<int32_t>(n) {
    }

    virtual ~IntAccessor() {
    }

    valarray<int32_t>& getIntData() const throw (bad_cast) {
        return getData();
    }
};

#endif	/* INTACCESSOR_H */

