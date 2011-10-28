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
 * File:   ScalarLookupTableImpl.h
 * Author: ralf
 *
 * Created on January 25, 2011, 4:50 PM
 */

#ifndef SCALARLOOKUPTABLEIMPL_H
#define	SCALARLOOKUPTABLEIMPL_H

#include <algorithm>
#include <cassert>
#include <valarray>
#include <vector>

using std::valarray;
using std::vector;

template<class W>
class ScalarLookupTable;

template<class T, class W>
class ScalarLookupTableImpl: public ScalarLookupTable<W> {
public:
	typedef valarray<W> Dimension;

	ScalarLookupTableImpl(const string& id, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor = W(1), W addOffset = W(0));
	virtual ~ScalarLookupTableImpl();

	const string& getId() const;

	W getValue(const W coordinates[]) const;

	size_t getDimensionCount() const;
	size_t getDimensionLength(size_t dimIndex) const;
	size_t getStride(size_t dimIndex) const;

	W getScaleFactor() const;
	W getAddOffset() const;
	W getMaxCoordinate(size_t dimIndex) const;
	W getMinCoordinate(size_t dimIndex) const;

	W getValue(size_t i) const;

private:
	size_t getIndex(size_t dimIndex, W coordinate, W& fraction) const;

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
ScalarLookupTableImpl<T, W>::ScalarLookupTableImpl(const string& id, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor, W addOffset) :
		id(id), scaleFactor(scaleFactor), addOffset(addOffset), x(dims), y(values), sizes(dims.size()), strides(dims.size()), offsets(1 << dims.size()), n(x.size()) {
	for (size_t i = 0; i < n; ++i) {
		assert(x[i].size() > 0);
		sizes[i] = x[i].size();
	}
	for (size_t i = n, stride = 1; i-- > 0;) {
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
ScalarLookupTableImpl<T, W>::~ScalarLookupTableImpl() {
}

template<class T, class W>
W ScalarLookupTableImpl<T, W>::getValue(const W coordinates[]) const {
	valarray<W> f(n);
	valarray<W> v(offsets.size());

	size_t origin = 0;
#pragma omp parallel for reduction(+ : origin)
	for (size_t i = 0; i < n; ++i) {
		origin += getIndex(i, coordinates[i], f[i]) * strides[i];
	}
#pragma omp parallel for
	for (size_t i = 0; i < offsets.size(); ++i) {
		v[i] = boost::numeric_cast<W>(y[origin + offsets[i]]);
	}
	for (size_t i = n; i-- > 0;) {
		const size_t m = 1 << i;
#pragma omp parallel for
		for (size_t j = 0; j < m; ++j) {
			v[j] += f[i] * (v[m + j] - v[j]);
		}
	}

	return v[0] * scaleFactor + addOffset;
}

template<class T, class W>
inline const string& ScalarLookupTableImpl<T, W>::getId() const {
	return id;
}

template<class T, class W>
inline size_t ScalarLookupTableImpl<T, W>::getDimensionCount() const {
	return n;
}

template<class T, class W>
inline size_t ScalarLookupTableImpl<T, W>::getDimensionLength(size_t dimIndex) const {
	assert(dimIndex < n);
	return sizes[dimIndex];
}

template<class T, class W>
inline size_t ScalarLookupTableImpl<T, W>::getStride(size_t dimIndex) const {
	assert(dimIndex < n);
	return strides[dimIndex];
}

template<class T, class W>
inline W ScalarLookupTableImpl<T, W>::getValue(size_t i) const {
	return boost::numeric_cast<W>(y[i]) * scaleFactor + addOffset;
}

template<class T, class W>
inline W ScalarLookupTableImpl<T, W>::getScaleFactor() const {
	return scaleFactor;
}

template<class T, class W>
inline W ScalarLookupTableImpl<T, W>::getAddOffset() const {
	return addOffset;
}

template<class T, class W>
inline W ScalarLookupTableImpl<T, W>::getMaxCoordinate(size_t dimIndex) const {
	assert(dimIndex < n);
	assert(sizes[dimIndex] > 0);
	return x[dimIndex][sizes[dimIndex] - 1];
}

template<class T, class W>
inline W ScalarLookupTableImpl<T, W>::getMinCoordinate(size_t dimIndex) const {
	assert(dimIndex < n and sizes[dimIndex] > 0);
	return x[dimIndex][0];
}

template<class T, class W>
size_t ScalarLookupTableImpl<T, W>::getIndex(size_t dimIndex, W coordinate, W& f) const {
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

#endif	/* SCALARLOOKUPTABLEIMPL_H */

