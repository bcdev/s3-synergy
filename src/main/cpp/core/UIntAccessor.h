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
 * File:   UIntAccessor.h
 * Author: ralf
 *
 * Created on December 18, 2010, 5:23 PM
 */

#ifndef UINTACCESSOR_H
#define	UINTACCESSOR_H

class UIntAccessor : public virtual AbstractAccessor<uint32_t> {
public:

    UIntAccessor(size_t n) : AbstractAccessor<uint32_t>(n) {
    }

    virtual ~UIntAccessor() {
    }

    valarray<uint32_t>& getUIntData() const throw (bad_cast) {
        return getData();
    }
};

#endif	/* UINTACCESSOR_H */

