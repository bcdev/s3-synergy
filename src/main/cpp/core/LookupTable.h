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
	virtual size_t getDimensionLength(size_t dimIndex) const = 0;
	virtual size_t getStride(size_t dimIndex) const = 0;

	virtual W getScaleFactor() const = 0;
	virtual W getAddOffset() const = 0;
	virtual W getMaxCoordinate(size_t dimIndex) const = 0;
	virtual W getMinCoordinate(size_t dimIndex) const = 0;

	virtual W getValue(size_t i) const = 0;

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
	valarray<W> f(n);
	valarray<W> v(offsets.size());

	size_t origin = 0;
	for (size_t i = 0; i < n; ++i) {
		origin += getIndex(i, coordinates[i], f[i]) * strides[i];
	}
	for (size_t i = 0; i < offsets.size(); ++i) {
		v[i] = boost::numeric_cast<W>(y[origin + offsets[i]]);
	}
    for (int i = n; i-- > 0;) {
        const int m = 1 << i;

        for (int j = 0; j < m; ++j) {
            v[j] += f[i] * (v[m + j] - v[j]);
        }
    }

	return v[0] * scaleFactor + addOffset;
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
inline size_t LookupTableImpl<T, W>::getDimensionLength(size_t dimIndex) const {
	assert(dimIndex < n);
	return sizes[dimIndex];
}

template<class T, class W>
inline size_t LookupTableImpl<T, W>::getStride(size_t dimIndex) const {
	assert(dimIndex < n);
	return strides[dimIndex];
}

template<class T, class W>
inline W LookupTableImpl<T, W>::getValue(size_t i) const {
	return boost::numeric_cast<W>(y[i]) * scaleFactor + addOffset;
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
inline W LookupTableImpl<T, W>::getMaxCoordinate(size_t dimIndex) const {
	assert(dimIndex < n);
	assert(sizes[dimIndex] > 0);
	return x[dimIndex][sizes[dimIndex] - 1];
}

template<class T, class W>
inline W LookupTableImpl<T, W>::getMinCoordinate(size_t dimIndex) const {
	assert(dimIndex < n and sizes[dimIndex] > 0);
	return x[dimIndex][0];
}

template<class T, class W>
size_t LookupTableImpl<T, W>::getIndex(size_t dimIndex, W coordinate, W& f) const {
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

#endif	/* LOOKUPTABLE_H */

