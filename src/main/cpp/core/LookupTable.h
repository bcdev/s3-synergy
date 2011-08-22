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

#include <algorithm>
#include <cassert>
#include <valarray>
#include <vector>

#include "Boost.h"
#include "Identifiable.h"

using std::valarray;
using std::vector;

template<class T, class W>
class LookupTableImpl;

template<class W>
class LookupTable: public Identifiable {
public:
	typedef valarray<W> Dimension;

	virtual ~LookupTable() {
	}

	virtual W operator()(const W coordinates[]) const = 0;

	template<class T>
	static LookupTable<W>* newLookupTable(const string& id,
			const vector<Dimension>& dims, const valarray<T>& values,
			W scaleFactor = W(1), W addOffset = W(0)) {
		return new LookupTableImpl<T, W>(id, &dims[0], dims.size(), &values[0],
				scaleFactor, addOffset);
	}
};

template<class T, class W>
class LookupTableImpl: public LookupTable<W> {
public:
	typedef valarray<W> Dimension;

	LookupTableImpl(const string& id, W scaleFactor = W(1), W addOffset = W(0));
	LookupTableImpl(const string& id, const Dimension dims[], size_t dimCount,
			W scaleFactor = W(1), W addOffset = W(0));
	LookupTableImpl(const string& id, const Dimension dims[], size_t dimCount,
			const T values[], W scaleFactor = W(1), W addOffset = W(0));
	LookupTableImpl(const string& id, const Dimension* dims[], size_t dimCount,
			const T values[], W scaleFactor = W(1), W addOffset = W(0));
	virtual ~LookupTableImpl();

	W operator()(const W coordinates[]) const;

	const string& getId() const;

	W maxCoordinate(size_t dimIndex) const;
	W minCoordinate(size_t dimIndex) const;
	bool isValidCoordinate(size_t dimIndex, W coordinate) const;

	void reset(const Dimension dims[], size_t dimCount);
	void reset(const Dimension dims[], size_t dimCount, const T values[]);
	void reset(const Dimension* dims[], size_t dimCount, const T values[]);

private:
	void updateSizesAndStrides();
	void getVertexes(const W coordinates[], size_t vertexes[]) const;
	void interpolate(W values[], const W values2[], size_t numValues,
			W interpolationFactor) const;

	W interpolationFactor(size_t dimIndex, W coordinate, size_t vertex) const;

	size_t valueIndex(const size_t vertexes[]) const;
	size_t vertex(size_t dimIndex, W coordinate) const;

	static size_t valueCount(const size_t sizes[], size_t dimCount);
	static size_t valueCount(const Dimension dims[], size_t dimCount);
	static size_t valueCount(const Dimension* dims[], size_t dimCount);

	const string id;
	const W scaleFactor;
	const W addOffset;

	valarray<Dimension> x;
	// coordinate dimensions = vertex coordinates
	valarray<T> y;
	// tabulated values

	valarray<size_t> sizes;
	valarray<size_t> strides;
	valarray<size_t> indexes;

	size_t n;
	// table dimension = number of coordinate axes
};

template<class T, class W>
LookupTableImpl<T, W>::LookupTableImpl(const string& id, W scaleFactor,
		W addOffset) :
		id(id), scaleFactor(scaleFactor), addOffset(addOffset), x(), y(), sizes(), strides(), indexes(), n(
				0) {
}

template<class T, class W>
LookupTableImpl<T, W>::LookupTableImpl(const string& id, const Dimension dims[],
		size_t dimCount, W scaleFactor, W addOffset) :
		id(id), scaleFactor(scaleFactor), addOffset(addOffset), x(), y(), sizes(), strides(), indexes(), n(
				0) {
	reset(dims, dimCount);
}

template<class T, class W>
LookupTableImpl<T, W>::LookupTableImpl(const string& id, const Dimension dims[],
		size_t dimCount, const T values[], W scaleFactor, W addOffset) :
		id(id), scaleFactor(scaleFactor), addOffset(addOffset), x(), y(), sizes(), strides(), indexes(), n(
				0) {
	reset(dims, dimCount, values);
}

template<class T, class W>
LookupTableImpl<T, W>::LookupTableImpl(const string& id,
		const Dimension* dims[], size_t dimCount, const T values[],
		W scaleFactor, W addOffset) :
		id(id), scaleFactor(scaleFactor), addOffset(addOffset), x(), y(), sizes(), strides(), indexes(), n(
				0) {
	reset(dims, dimCount, values);
}

template<class T, class W>
LookupTableImpl<T, W>::~LookupTableImpl() {
}

template<class T, class W>
W LookupTableImpl<T, W>::operator()(const W coordinates[]) const {
	valarray<size_t> v(n);
	getVertexes(coordinates, &v[0]);
	const size_t origin = valueIndex(&v[0]);
	valarray<W> values(indexes.size());
	for (size_t i = 0; i < indexes.size(); ++i) {
		values[i] = boost::numeric_cast<W>(y[indexes[i] + origin]) * scaleFactor
				+ addOffset;
	}
	// extract the y-values at the vertexes of the smallest n-cube,
	// which contains the interpolation point
	for (size_t i = 0, j = indexes.size(); j >>= 1 != 0; ++i) {
		interpolate(&values[0], &values[j], j,
				interpolationFactor(i, coordinates[i], v[i]));
	}

	return values[0];
}

template<class T, class W>
const string& LookupTableImpl<T, W>::getId() const {
	return id;
}

template<class T, class W>
inline W LookupTableImpl<T, W>::maxCoordinate(size_t dimIndex) const {
	assert(dimIndex < n and sizes[dimIndex] > 0);
	return x[dimIndex][sizes[dimIndex] - 1];
}

template<class T, class W>
inline W LookupTableImpl<T, W>::minCoordinate(size_t dimIndex) const {
	assert(dimIndex < n and sizes[dimIndex] > 0);
	return x[dimIndex][0];
}

template<class T, class W>
bool LookupTableImpl<T, W>::isValidCoordinate(size_t dimIndex,
		W coordinate) const {
	return coordinate >= minCoordinate(dimIndex)
			&& coordinate <= maxCoordinate(dimIndex);
}

template<class T, class W>
inline W LookupTableImpl<T, W>::interpolationFactor(size_t dimIndex,
		W coordinate, size_t vertex) const {
	return (coordinate - x[dimIndex][vertex])
			/ (x[dimIndex][vertex + 1] - x[dimIndex][vertex]);
}

template<class T, class W>
size_t LookupTableImpl<T, W>::valueCount(const size_t sizes[],
		size_t dimCount) {
	size_t valueCount = 1;

	for (size_t i = 0; i < dimCount; ++i) {
		assert(sizes[i] > 0);
		valueCount *= sizes[i];
	}

	return valueCount;
}

template<class T, class W>
size_t LookupTableImpl<T, W>::valueCount(const Dimension dims[],
		size_t dimCount) {
	size_t valueCount = 1;

	for (size_t i = 0; i < dimCount; ++i) {
		assert(dims[i].size() > 0);
		valueCount *= dims[i].size();
	}

	return valueCount;
}

template<class T, class W>
size_t LookupTableImpl<T, W>::valueCount(const Dimension* dims[],
		size_t dimCount) {
	size_t valueCount = 1;

	for (size_t i = 0; i < dimCount; ++i) {
		assert(dims[i]->size() > 0);
		valueCount *= dims[i]->size();
	}

	return valueCount;
}

template<class T, class W>
size_t LookupTableImpl<T, W>::valueIndex(const size_t vertexes[]) const {
	size_t index = 0;

	for (size_t i = 0; i < n; ++i) {
		assert(vertexes[i] < sizes[i]);
		index += vertexes[i] * strides[i];
	}

	return index;
}

template<class T, class W>
size_t LookupTableImpl<T, W>::vertex(size_t dimIndex, W coordinate) const {
	assert(dimIndex < n and sizes[dimIndex] > 0);
	assert(
			coordinate >= x[dimIndex][0] and coordinate <= x[dimIndex][sizes[dimIndex] - 1]);

	size_t i = 0;
	size_t k = sizes[dimIndex] - 1;

	while (k > i + 1) {
		const size_t j = (i + k) >> 1;

		if (coordinate > x[dimIndex][j])
			i = j;
		else
			k = j;
	}

	return i;
}

template<class T, class W>
void LookupTableImpl<T, W>::getVertexes(const W coordinates[],
		size_t vertexes[]) const {
	for (size_t i = 0; i < n; ++i)
		vertexes[i] = vertex(i, coordinates[i]);
}

template<class T, class W>
void LookupTableImpl<T, W>::interpolate(W values[], const W values2[],
		size_t numValues, W interpolationFactor) const {
	for (size_t i = 0; i < numValues; ++i)
		values[i] = (W(1) - interpolationFactor) * values[i]
				+ interpolationFactor * values2[i];
}

template<class T, class W>
void LookupTableImpl<T, W>::updateSizesAndStrides() {
	sizes.resize(x.size());
	strides.resize(x.size());

	for (size_t i = 0; i < x.size(); ++i)
		sizes[i] = x[i].size();
	for (size_t i = x.size(), stride = 1; i-- > 0;) {
		strides[i] = stride;
		stride *= sizes[i];
	}

	n = x.size();
}

template<class T, class W>
void LookupTableImpl<T, W>::reset(const Dimension dims[], size_t dimCount) {
	using std::copy;
	using std::gslice;

	const size_t numValues = this->valueCount(dims, dimCount);

	x.resize(dimCount);
	y.resize(numValues);

	for (size_t i = 0; i < dimCount; ++i) {
		const Dimension& dim = dims[i];

		x[i].resize(dim.size());
		copy(&dim[0], &dim[dim.size()], &x[i][0]);
	}

	updateSizesAndStrides();

	valarray<size_t> numbers(numValues);
	for (size_t i = 0; i < numValues; ++i)
		numbers[i] = i;
	indexes.resize(1 << n);
	indexes = numbers[gslice(0, valarray<size_t>(2, n), strides)];
}

template<class T, class W>
void LookupTableImpl<T, W>::reset(const Dimension dims[], size_t dimCount,
		const T values[]) {
	using std::copy;
	using std::gslice;

	const size_t numValues = this->valueCount(dims, dimCount);

	x.resize(dimCount);
	y.resize(numValues);

	for (size_t i = 0; i < dimCount; ++i) {
		const Dimension& axis = dims[i];

		x[i].resize(axis.size());
		copy(&axis[0], &axis[axis.size()], &x[i][0]);
	}
	copy(&values[0], &values[numValues], &y[0]);

	updateSizesAndStrides();

	valarray<size_t> numbers(numValues);
	for (size_t i = 0; i < numValues; ++i)
		numbers[i] = i;
	indexes.resize(1 << n);
	indexes = numbers[gslice(0, valarray<size_t>(2, n), strides)];
}

template<class T, class W>
void LookupTableImpl<T, W>::reset(const Dimension* dims[], size_t dimCount,
		const T values[]) {
	using std::copy;
	using std::gslice;

	const size_t numValues = this->valueCount(dims, dimCount);

	x.resize(dimCount);
	y.resize(numValues);

	for (size_t i = 0; i < dimCount; ++i) {
		const Dimension& axis = *dims[i];

		x[i].resize(axis.size());
		copy(&axis[0], &axis[axis.size()], &x[i][0]);
	}
	copy(&values[0], &values[numValues], &y[0]);

	updateSizesAndStrides();

	valarray<size_t> numbers(numValues);
	for (size_t i = 0; i < numValues; ++i)
		numbers[i] = i;
	indexes.resize(1 << n);
	indexes = numbers[gslice(0, valarray<size_t>(2, n), strides)];
}

#endif	/* LOOKUPTABLE_H */

