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
 * File:   MatrixLookupTableImpl.h
 * Author: ralf
 *
 * Created on January 25, 2011, 4:50 PM
 */

#ifndef MATRIXLOOKUPTABLEIMPL_H
#define	MATRIXLOOKUPTABLEIMPL_H

#include <algorithm>
#include <cassert>
#include <valarray>
#include <vector>

using std::valarray;
using std::vector;

template<class W>
class MatrixLookupTable;

template<class T, class W>
class MatrixLookupTableImpl: public MatrixLookupTable<W> {
public:
	typedef valarray<W> Dimension;

	MatrixLookupTableImpl(const string& id, size_t rowCount, size_t colCount, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor = W(1), W addOffset = W(0));
	virtual ~MatrixLookupTableImpl();

	const string& getId() const;

	matrix<W>& getValues(const W coordinates[], matrix<W>& values) const;

	size_t getDimensionCount() const;
	size_t getDimensionLength(size_t dimIndex) const;
	size_t getStride(size_t dimIndex) const;

	W getScaleFactor() const;
	W getAddOffset() const;
	W getMaxCoordinate(size_t dimIndex) const;
	W getMinCoordinate(size_t dimIndex) const;

	W getValue(size_t i) const {
		return boost::numeric_cast<W>(y[i]) * scaleFactor + addOffset;
	}

private:
	size_t getIndex(size_t dimIndex, W coordinate, W& fraction) const;

	const string id;
	const size_t rowCount;
	const size_t colCount;

	const W scaleFactor;
	const W addOffset;

	const vector<Dimension> x;
	// coordinate dimensions = vertex coordinates
	const shared_array<T> y;
	// tabulated values

	valarray<size_t> sizes;
	valarray<size_t> strides;
	valarray<size_t> offsets;

	const size_t n;
	// table dimension = number of coordinate axes
};

template<class T, class W>
MatrixLookupTableImpl<T, W>::MatrixLookupTableImpl(const string& id, size_t rowCount, size_t colCount, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor, W addOffset) :
		id(id), rowCount(rowCount), colCount(colCount), scaleFactor(scaleFactor), addOffset(addOffset), x(dims), y(values), sizes(dims.size()), strides(dims.size()), offsets(1 << dims.size()), n(
				x.size()) {
	for (size_t i = 0; i < n; ++i) {
		assert(x[i].size() > 0);
		sizes[i] = x[i].size();
	}
	for (size_t i = n, stride = rowCount * colCount; i-- > 0;) {
		if (sizes[i] > 1) {
			strides[i] = stride;
		} else {
			strides[i] = 0;
		}
		stride *= sizes[i];
	}
	for (size_t i = 0; i < n; ++i) {
		const size_t k = 1 << i;

		for (size_t j = 0; j < k; ++j) {
			offsets[k + j] = offsets[j] + strides[i];
		}
	}
}

template<class T, class W>
MatrixLookupTableImpl<T, W>::~MatrixLookupTableImpl() {
}

template<class T, class W>
matrix<W>& MatrixLookupTableImpl<T, W>::getValues(const W coordinates[], matrix<W>& matrix) const {
	assert(matrix.size1() >= rowCount);
	assert(matrix.size2() >= colCount);

	const size_t elementCount = rowCount * colCount;

	valarray<W> f(n);
	valarray<W> v(offsets.size() * elementCount);

	size_t origin = 0;
	for (size_t i = 0; i < n; ++i) {
		origin += getIndex(i, coordinates[i], f[i]) * strides[i];
	}
	for (size_t i = 0; i < offsets.size(); ++i) {
		for (size_t j = 0; j < elementCount; ++j) {
			v[i * elementCount + j] = boost::numeric_cast<W>(y[origin + offsets[i] + j]);
		}
	}
	for (size_t i = n; i-- > 0;) {
		const size_t m = 1 << i;

		for (size_t j = 0; j < m; ++j) {
			for (size_t k = 0; k < elementCount; ++k) {
				v[j * elementCount + k] += f[i] * (v[(m + j) * elementCount + k] - v[j * elementCount + k]);
			}
		}
	}
	for (size_t i = 0, k = 0; i < rowCount; ++i) {
		for (size_t j = 0; j < colCount; ++j, ++k) {
			matrix(i, j) = v[k] * scaleFactor + addOffset;
		}
	}

	return matrix;
}

template<class T, class W>
inline const string& MatrixLookupTableImpl<T, W>::getId() const {
	return id;
}

template<class T, class W>
inline size_t MatrixLookupTableImpl<T, W>::getDimensionCount() const {
	return n;
}

template<class T, class W>
inline size_t MatrixLookupTableImpl<T, W>::getDimensionLength(size_t dimIndex) const {
	assert(dimIndex < n);
	return sizes[dimIndex];
}

template<class T, class W>
inline size_t MatrixLookupTableImpl<T, W>::getStride(size_t dimIndex) const {
	assert(dimIndex < n);
	return strides[dimIndex];
}

template<class T, class W>
inline W MatrixLookupTableImpl<T, W>::getScaleFactor() const {
	return scaleFactor;
}

template<class T, class W>
inline W MatrixLookupTableImpl<T, W>::getAddOffset() const {
	return addOffset;
}

template<class T, class W>
inline W MatrixLookupTableImpl<T, W>::getMaxCoordinate(size_t dimIndex) const {
	assert(dimIndex < n);
	assert(sizes[dimIndex] > 0);
	return x[dimIndex][sizes[dimIndex] - 1];
}

template<class T, class W>
inline W MatrixLookupTableImpl<T, W>::getMinCoordinate(size_t dimIndex) const {
	assert(dimIndex < n and sizes[dimIndex] > 0);
	return x[dimIndex][0];
}

template<class T, class W>
size_t MatrixLookupTableImpl<T, W>::getIndex(size_t dimIndex, W coordinate, W& f) const {
	assert(dimIndex < n);
	assert(sizes[dimIndex] > 0);

	size_t lo = 0;
	size_t hi = sizes[dimIndex] - 1;

	while (hi > lo + 1) {
		const size_t m = (lo + hi) >> 1;

		if (coordinate < x[dimIndex][m])
			hi = m;
		else
			lo = m;
	}
	if (sizes[dimIndex] > 1) {
		f = (coordinate - x[dimIndex][lo]) / (x[dimIndex][hi] - x[dimIndex][lo]);
		if (f < W(0.0)) {
			f = W(0.0);
		} else if (f > W(1.0)) {
			f = W(1.0);
		}
	} else {
		f = 0.0;
	}

	return lo;
}

#endif	/* MATRIXLOOKUPTABLEIMPL_H */

