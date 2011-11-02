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
#include "ScalarLookupTableImpl.h"
#include "VectorLookupTableImpl.h"
#include "MatrixLookupTableImpl.h"

using std::valarray;
using std::vector;

/**
 * A scalar lookup table.
 *
 * @author Ralf Quast
 */
template<class W>
class ScalarLookupTable: public Identifiable {
public:
	typedef valarray<W> Dimension;

	virtual ~ScalarLookupTable() {
	}

	virtual valarray<W>& getTable(const W coordinates[], size_t dimIndex, valarray<W>& tableValues) const = 0;
	virtual W getValue(const W coordinates[]) const = 0;
	virtual W getValue(const W coordinates[], size_t dimIndex, const valarray<W>& tableValues, valarray<W>& w) const = 0;

	virtual size_t getDimensionCount() const = 0;
	virtual size_t getDimensionLength(size_t dimIndex) const = 0;
	virtual size_t getStride(size_t dimIndex) const = 0;

	virtual W getScaleFactor() const = 0;
	virtual W getAddOffset() const = 0;
	virtual W getMaxCoordinate(size_t dimIndex) const = 0;
	virtual W getMinCoordinate(size_t dimIndex) const = 0;

	virtual W getValue(size_t i) const = 0;

	template<class T>
	static shared_ptr<ScalarLookupTable<W> > newScalarLookupTable(const string& id,
			const vector<Dimension>& dims, const shared_array<T>& values,
			W scaleFactor = W(1), W addOffset = W(0)) {
		return shared_ptr<ScalarLookupTable<W> >(
				new ScalarLookupTableImpl<T, W>(id, dims, values, scaleFactor,
						addOffset));
	}
};

/**
 * A vector lookup table.
 *
 * @author Ralf Quast
 */
template<class W>
class VectorLookupTable: public Identifiable {
public:
	typedef valarray<W> Dimension;

	virtual ~VectorLookupTable() {
	}

	virtual valarray<W>& getValues(const W coordinates[], valarray<W>& values) const = 0;

	virtual size_t getDimensionCount() const = 0;
	virtual size_t getDimensionLength(size_t dimIndex) const = 0;
	virtual size_t getStride(size_t dimIndex) const = 0;

	virtual W getScaleFactor() const = 0;
	virtual W getAddOffset() const = 0;
	virtual W getMaxCoordinate(size_t dimIndex) const = 0;
	virtual W getMinCoordinate(size_t dimIndex) const = 0;

	virtual W getValue(size_t i) const = 0;

	template<class T>
	static shared_ptr<VectorLookupTable<W> > newVectorLookupTable(const string& id, size_t length, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor = W(1),
			W addOffset = W(0)) {
		return shared_ptr<VectorLookupTable<W> >(new VectorLookupTableImpl<T, W>(id, length, dims, values, scaleFactor, addOffset));
	}
};

/**
 * A matrix lookup table.
 *
 * @author Ralf Quast
 */
template<class W>
class MatrixLookupTable: public Identifiable {
public:
	typedef valarray<W> Dimension;

	virtual ~MatrixLookupTable() {
	}

	virtual matrix<W>& getValues(const W coordinates[], matrix<W>& matrix) const = 0;
	virtual matrix<W>& getValues(const W coordinates[], matrix<W>& matrix, valarray<W>& f, valarray<W>& w) const = 0;

	virtual size_t getDimensionCount() const = 0;
	virtual size_t getDimensionLength(size_t dimIndex) const = 0;
	virtual size_t getStride(size_t dimIndex) const = 0;
	virtual size_t getWorkspaceSize() const = 0;
	virtual W getScaleFactor() const = 0;
	virtual W getAddOffset() const = 0;
	virtual W getMaxCoordinate(size_t dimIndex) const = 0;
	virtual W getMinCoordinate(size_t dimIndex) const = 0;

	virtual W getValue(size_t i) const = 0;

	template<class T>
	static shared_ptr<MatrixLookupTable<W> > newMatrixLookupTable(const string& id, size_t rowCount, size_t colCount, const vector<Dimension>& dims, const shared_array<T>& values,
			W scaleFactor = W(1), W addOffset = W(0)) {
		return shared_ptr<MatrixLookupTable<W> >(new MatrixLookupTableImpl<T, W>(id, rowCount, colCount, dims, values, scaleFactor, addOffset));
	}
};

#endif	/* LOOKUPTABLE_H */

