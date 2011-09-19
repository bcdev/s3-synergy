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
 * File:   VectorLookupTableImpl.h
 * Author: ralf
 *
 * Created on January 25, 2011, 4:50 PM
 */

#ifndef VECTORLOOKUPTABLEIMPL_H
#define	VECTORLOOKUPTABLEIMPL_H

#include <algorithm>
#include <cassert>
#include <valarray>
#include <vector>

using std::valarray;
using std::vector;

template<class W>
class VectorLookupTable;

template<class T, class W>
class VectorLookupTableImpl: public VectorLookupTable<W> {
public:
	typedef valarray<W> Dimension;

	VectorLookupTableImpl(const string& id, size_t length, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor = W(1), W addOffset = W(0));
	virtual ~VectorLookupTableImpl();

	const string& getId() const;

	valarray<W>& getValues(const W coordinates[], valarray<W>& values) const;

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
	const size_t length;
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
VectorLookupTableImpl<T, W>::VectorLookupTableImpl(const string& id, size_t length, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor, W addOffset) :
		id(id), length(length), scaleFactor(scaleFactor), addOffset(addOffset), x(dims), y(values), sizes(dims.size()), strides(dims.size()), offsets(1 << dims.size()), n(x.size()) {
	for (size_t i = 0; i < n; ++i) {
		assert(x[i].size() > 0);
		sizes[i] = x[i].size();
	}
	for (size_t i = n, stride = length; i-- > 0;) {
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
VectorLookupTableImpl<T, W>::~VectorLookupTableImpl() {
}

template<class T, class W>
valarray<W>& VectorLookupTableImpl<T, W>::getValues(const W coordinates[], valarray<W>& values) const {
	assert(values.size() >= length);

	valarray<W> f(n);
	valarray<W> v(offsets.size() * length);

	size_t origin = 0;
	for (size_t i = 0; i < n; ++i) {
		origin += getIndex(i, coordinates[i], f[i]) * strides[i];
	}
	for (size_t i = 0; i < offsets.size(); ++i) {
		for (size_t j = 0; j < length; ++j) {
			v[i * length + j] = boost::numeric_cast<W>(y[origin + offsets[i] + j]);
		}
	}
	for (size_t i = n; i-- > 0;) {
		const size_t m = 1 << i;

		for (size_t j = 0; j < m; ++j) {
			for (size_t k = 0; k < length; ++k) {
				v[j * length + k] += f[i] * (v[(m + j) * length + k] - v[j * length + k]);
			}
		}
	}
	for (size_t k = 0; k < length; ++k) {
		values[k] = v[k] * scaleFactor + addOffset;
	}

	return values;
}

template<class T, class W>
inline const string& VectorLookupTableImpl<T, W>::getId() const {
	return id;
}

template<class T, class W>
inline size_t VectorLookupTableImpl<T, W>::getDimensionCount() const {
	return n;
}

template<class T, class W>
inline size_t VectorLookupTableImpl<T, W>::getDimensionLength(size_t dimIndex) const {
	assert(dimIndex < n);
	return sizes[dimIndex];
}

template<class T, class W>
inline size_t VectorLookupTableImpl<T, W>::getStride(size_t dimIndex) const {
	assert(dimIndex < n);
	return strides[dimIndex];
}

template<class T, class W>
inline W VectorLookupTableImpl<T, W>::getScaleFactor() const {
	return scaleFactor;
}

template<class T, class W>
inline W VectorLookupTableImpl<T, W>::getAddOffset() const {
	return addOffset;
}

template<class T, class W>
inline W VectorLookupTableImpl<T, W>::getMaxCoordinate(size_t dimIndex) const {
	assert(dimIndex < n);
	assert(sizes[dimIndex] > 0);
	return x[dimIndex][sizes[dimIndex] - 1];
}

template<class T, class W>
inline W VectorLookupTableImpl<T, W>::getMinCoordinate(size_t dimIndex) const {
	assert(dimIndex < n and sizes[dimIndex] > 0);
	return x[dimIndex][0];
}

template<class T, class W>
size_t VectorLookupTableImpl<T, W>::getIndex(size_t dimIndex, W coordinate, W& f) const {
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

	f = (coordinate - x[dimIndex][lo]) / (x[dimIndex][hi] - x[dimIndex][lo]);
	if (f < W(0.0)) {
		f = W(0.0);
	} else if (f > W(1.0)) {
		f = W(1.0);
	}

	return lo;
}

#endif	/* VECTORLOOKUPTABLEIMPL_H */

