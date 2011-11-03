/* 
 * Copyright (C) 2011 by Brockmann Consult (info@brockmann-consult.de)
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
 * File:   LookupTable.h
 * Author: ralf
 *
 * Created on January 25, 2011, 4:50 PM
 */

#ifndef LOOKUPTABLE_H
#define	LOOKUPTABLE_H

#include <valarray>
#include <vector>

#include "Boost.h"
#include "Identifiable.h"
#include "LookupTableImpl.h"

using std::valarray;
using std::vector;

/**
 * A lookup table.
 *
 * @author Ralf Quast
 */
template<class W>
class LookupTable: public Identifiable {
public:
	typedef valarray<W> Dimension;
	typedef valarray<W> Vector;
	typedef valarray<valarray<W> > Matrix;

	virtual ~LookupTable() {
	}

	virtual valarray<W>& getTable(const W coordinates[], size_t dimIndex, valarray<W>& tableValues) const = 0;

	virtual W getScalar(const W coordinates[], valarray<W>& f, valarray<W>& w) const = 0;
	virtual size_t getScalarWorkspaceSize() const = 0;
	virtual W getScalar(const W coordinates[], size_t dimIndex, const valarray<W>& tableValues, valarray<W>& w) const = 0;

	virtual matrix<W>& getMatrix(const W coordinates[], matrix<W>& matrix, valarray<W>& f, valarray<W>& w) const = 0;
	virtual size_t getMatrixColCount() const = 0;
	virtual size_t getMatrixRowCount() const = 0;
	virtual size_t getMatrixWorkspaceSize() const = 0;

	virtual valarray<W>& getVector(const W coordinates[], valarray<W>& vector, valarray<W>& f, valarray<W>& w) const = 0;
	virtual size_t getVectorDimensionCount() const = 0;
	virtual size_t getVectorWorkspaceSize() const = 0;

	virtual size_t getDimensionCount() const = 0;
	virtual size_t getDimensionLength(size_t dimIndex) const = 0;
	virtual size_t getStride(size_t dimIndex) const = 0;

	virtual W getScaleFactor() const = 0;
	virtual W getAddOffset() const = 0;
	virtual W getMaxCoordinate(size_t dimIndex) const = 0;
	virtual W getMinCoordinate(size_t dimIndex) const = 0;

	virtual W getValue(size_t i) const = 0;

	template<class T>
	static shared_ptr<LookupTable<W> > newLookupTable(const string& id, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor = W(1), W addOffset = W(0)) {
		return shared_ptr<LookupTable<W> >(new LookupTableImpl<T, W>(id, dims, values, scaleFactor, addOffset));
	}
};

#endif	/* LOOKUPTABLE_H */

