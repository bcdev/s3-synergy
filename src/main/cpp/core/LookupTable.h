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

/**
 * A lookup table.
 *
 * @author Ralf Quast
 */
template<class W>
class LookupTable: public Identifiable {
public:
	typedef valarray<W> Dimension;

	virtual ~LookupTable() {
	}

	virtual W operator()(const W coordinates[]) const = 0;

	virtual size_t getDimensionCount() const = 0;
	virtual W getScaleFactor() const = 0;
	virtual W getAddOffset() const = 0;
	virtual W maxCoordinate(size_t dimIndex) const = 0;
	virtual W minCoordinate(size_t dimIndex) const = 0;
	virtual bool isValidCoordinate(size_t dimIndex, W coordinate) const = 0;

	template<class T>
	static shared_ptr<LookupTable<W> > newLookupTable(const string& id,
			const vector<Dimension>& dims, const shared_array<T>& values,
			W scaleFactor = W(1), W addOffset = W(0)) {
		return shared_ptr<LookupTable<W> >(
				new LookupTableImpl<T, W>(id, dims, values, scaleFactor,
						addOffset));
	}
};

template<class T, class W>
class LookupTableImpl: public LookupTable<W> {
public:
	typedef valarray<W> Dimension;

	LookupTableImpl(const string& id, const vector<Dimension>& dims,
			const shared_array<T>& values, W scaleFactor = W(1), W addOffset =
					W(0));
	virtual ~LookupTableImpl();

	const string& getId() const;

	W operator()(const W coordinates[]) const;

	size_t getDimensionCount() const;
	W getScaleFactor() const;
	W getAddOffset() const;
	W maxCoordinate(size_t dimIndex) const;
	W minCoordinate(size_t dimIndex) const;
	bool isValidCoordinate(size_t dimIndex, W coordinate) const;

private:
	void getVertexes(const W coordinates[], size_t vertexes[]) const;
	void interpolate(W values[], const W values2[], size_t numValues,
			W interpolationFactor) const;

	W interpolationFactor(size_t dimIndex, W coordinate, size_t vertex) const;

	size_t getIndex(const size_t vertexes[]) const;
	size_t getVertex(size_t dimIndex, W coordinate) const;

	const string id;
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
LookupTableImpl<T, W>::LookupTableImpl(const string& id,
		const vector<Dimension>& dims, const shared_array<T>& values,
		W scaleFactor, W addOffset) :
		id(id), scaleFactor(scaleFactor), addOffset(addOffset), x(dims), y(
				values), sizes(dims.size()), strides(dims.size()), offsets(
				1 << dims.size()), n(x.size()) {
	for (size_t i = 0; i < n; ++i) {
		assert(x[i].size() > 0);
		sizes[i] = x[i].size();
	}
	for (size_t i = n, stride = 1; i-- > 0;) {
		strides[i] = stride;
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
LookupTableImpl<T, W>::~LookupTableImpl() {
}

template<class T, class W>
W LookupTableImpl<T, W>::operator()(const W coordinates[]) const {
	valarray<size_t> v(n);
	getVertexes(coordinates, &v[0]);
	const size_t origin = getIndex(&v[0]);
	valarray<W> values(offsets.size());
	// extract the y-values at the vertexes of the smallest n-cube,
	// which contains the interpolation point
	for (size_t i = 0; i < offsets.size(); ++i) {
		values[i] = boost::numeric_cast<W>(y[offsets[i] + origin]);
	}
	for (size_t i = 0, j = offsets.size(); j >>= 1 != 0; ++i) {
		interpolate(&values[0], &values[j], j,
				interpolationFactor(i, coordinates[i], v[i]));
	}

	return values[0] * scaleFactor + addOffset;
}

template<class T, class W>
inline const string& LookupTableImpl<T, W>::getId() const {
	return id;
}

template<class T, class W>
inline size_t LookupTableImpl<T, W>::getDimensionCount() const {
	return n;
}

template<class T, class W>
inline W LookupTableImpl<T, W>::getScaleFactor() const {
	return scaleFactor;
}

template<class T, class W>
inline W LookupTableImpl<T, W>::getAddOffset() const {
	return addOffset;
}

template<class T, class W>
inline W LookupTableImpl<T, W>::maxCoordinate(size_t dimIndex) const {
	assert(dimIndex < n);
	assert(sizes[dimIndex] > 0);
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
size_t LookupTableImpl<T, W>::getIndex(const size_t vertexes[]) const {
	size_t index = 0;

	for (size_t i = 0; i < n; ++i) {
		assert(vertexes[i] < sizes[i]);
		index += vertexes[i] * strides[i];
	}

	return index;
}

template<class T, class W>
size_t LookupTableImpl<T, W>::getVertex(size_t dimIndex, W coordinate) const {
	assert(dimIndex < n);
	assert(sizes[dimIndex] > 0);
	assert(coordinate >= x[dimIndex][0]);
	assert(coordinate <= x[dimIndex][sizes[dimIndex] - 1]);

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
		vertexes[i] = getVertex(i, coordinates[i]);
}

template<class T, class W>
void LookupTableImpl<T, W>::interpolate(W values[], const W values2[],
		size_t numValues, W interpolationFactor) const {
	for (size_t i = 0; i < numValues; ++i)
		values[i] = (W(1) - interpolationFactor) * values[i]
				+ interpolationFactor * values2[i];
}

#endif	/* LOOKUPTABLE_H */

