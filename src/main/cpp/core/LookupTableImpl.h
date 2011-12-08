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
 * File:   LookupTableImpl.h
 * Author: ralf
 *
 * Created on January 25, 2011, 4:50 PM
 */

#ifndef LOOKUPTABLEIMPL_H
#define	LOOKUPTABLEIMPL_H

#include <algorithm>
#include <cassert>
#include <valarray>
#include <vector>

using std::valarray;
using std::vector;

template<class W>
class LookupTable;

template<class T, class W>
class LookupTableImpl: public LookupTable<W> {
public:
	typedef valarray<W> Dimension;

	LookupTableImpl(const string& id, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor = W(1.0), W addOffset = W(0.0));
	virtual ~LookupTableImpl();

	const string& getId() const;

	valarray<W>& getTable(const W coordinates[], size_t dimIndex, valarray<W>& tableValues) const;

	W getScalar(const W coordinates[], valarray<W>& f, valarray<W>& w) const;
	size_t getScalarWorkspaceSize() const;
	W getScalar(const W coordinates[], size_t dimIndex, const valarray<W>& tableValues, valarray<W>& w) const;

	matrix<W>& getMatrix(const W coordinates[], matrix<W>& matrix, valarray<W>& f, valarray<W>& w) const;
	size_t getMatrixColCount() const;
	size_t getMatrixRowCount() const;
	size_t getMatrixWorkspaceSize() const;
	matrix<W>& getMatrix(const W coordinates[], size_t dimIndex, matrix<W>& matrix, const valarray<W>& tableValues, valarray<W>& w) const;

	valarray<W>& getVector(const W coordinates[], valarray<W>& vector, valarray<W>& f, valarray<W>& w) const;
	size_t getVectorDimensionCount() const;
	size_t getVectorWorkspaceSize() const;

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
LookupTableImpl<T, W>::LookupTableImpl(const string& id, const vector<Dimension>& dims, const shared_array<T>& values, W scaleFactor, W addOffset) :
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
LookupTableImpl<T, W>::~LookupTableImpl() {
}

template<class T, class W>
W LookupTableImpl<T, W>::getScalar(const W coordinates[], valarray<W>& f, valarray<W>& w) const {
	assert(f.size() >= n);
	assert(w.size() >= offsets.size());

	size_t origin = 0;
//#pragma omp parallel for reduction(+ : origin)
	for (size_t i = 0; i < n; ++i) {
		origin += getIndex(i, coordinates[i], f[i]) * strides[i];
	}
//#pragma omp parallel for
	for (size_t i = 0; i < offsets.size(); ++i) {
		w[i] = (W) y[origin + offsets[i]];
	}
	for (size_t i = n; i-- > 0;) {
		const size_t m = 1 << i;
//#pragma omp parallel for
		for (size_t j = 0; j < m; ++j) {
			w[j] += f[i] * (w[m + j] - w[j]);
		}
	}

	return w[0] * scaleFactor + addOffset;
}

template<class T, class W>
valarray<W>& LookupTableImpl<T, W>::getVector(const W coordinates[], valarray<W>& vector, valarray<W>& f, valarray<W>& w) const {
	assert(n > 1);

	const size_t r = n - 1;
	const size_t length = sizes[r];
	const size_t vertexCount = 1 << r;

	assert(vector.size() >= length);
	assert(f.size() >= r);
	assert(w.size() >= vertexCount * length);

	size_t origin = 0;
//#pragma omp parallel for reduction(+ : origin)
	for (size_t i = 0; i < r; ++i) {
		origin += getIndex(i, coordinates[i], f[i]) * strides[i];
	}
//#pragma omp parallel for
	for (size_t i = 0; i < vertexCount; ++i) {
		const size_t l = i * length;
		for (size_t j = 0; j < length; ++j) {
			w[l + j] = (W) y[origin + offsets[i] + j];
		}
	}
	for (size_t i = r; i-- > 0;) {
		const size_t m = 1 << i;
		const size_t n = m * length;
//#pragma omp parallel for
		for (size_t j = 0; j < m; ++j) {
			const size_t l = j * length;
			for (size_t k = 0; k < length; ++k) {
				w[l + k] += f[i] * (w[n + l + k] - w[l + k]);
			}
		}
	}
//#pragma omp parallel for
	for (size_t k = 0; k < length; ++k) {
		vector[k] = w[k] * scaleFactor + addOffset;
	}

	return vector;
}

template<class T, class W>
matrix<W>& LookupTableImpl<T, W>::getMatrix(const W coordinates[], matrix<W>& matrix, valarray<W>& f, valarray<W>& w) const {
	assert(n > 2);

	const size_t r = n - 2;
	const size_t rowCount = getMatrixRowCount();
	const size_t colCount = getMatrixColCount();
	const size_t elementCount = rowCount * colCount;
	const size_t vertexCount = 1 << r;

	assert(matrix.size1() >= rowCount);
	assert(matrix.size2() >= colCount);
	assert(f.size() >= r);
	assert(w.size() >= vertexCount * elementCount);

	size_t origin = 0;
//#pragma omp parallel for reduction(+ : origin)
	for (size_t i = 0; i < r; ++i) {
		origin += getIndex(i, coordinates[i], f[i]) * strides[i];
	}
//#pragma omp parallel for
	for (size_t i = 0; i < vertexCount; ++i) {
		const size_t l = i * elementCount;
		for (size_t j = 0; j < elementCount; ++j) {
			w[l + j] = (W) y[origin + offsets[i] + j];
		}
	}
	for (size_t i = r; i-- > 0;) {
		const size_t m = 1 << i;
		const size_t n = m * elementCount;
//#pragma omp parallel for
		for (size_t j = 0; j < m; ++j) {
			const size_t l = j * elementCount;
			for (size_t k = 0; k < elementCount; ++k) {
				w[l + k] += f[i] * (w[n + l + k] - w[l + k]);
			}
		}
	}
//#pragma omp parallel for
	for (size_t i = 0; i < rowCount; ++i) {
		const size_t l = i * colCount;
		for (size_t k = 0; k < colCount; ++k) {
			matrix(i, k) = w[l + k] * scaleFactor + addOffset;
		}
	}

	return matrix;
}

template<class T, class W>
valarray<W>& LookupTableImpl<T, W>::getTable(const W coordinates[], size_t dimIndex, valarray<W>& tableValues) const {
	assert(tableValues.size() >= strides[0]);

	W f;

	for (size_t i = 0; i < dimIndex; ++i) {
		const size_t origin = getIndex(i, coordinates[i], f) * strides[i];
//#pragma omp parallel for
		for (size_t k = 0; k < strides[i]; ++k) {
			W a;
			W b;
			if (i == 0) {
				a = boost::numeric_cast<W>(y[origin + k]);
				b = boost::numeric_cast<W>(y[origin + k + strides[i]]);
			} else {
				a = tableValues[origin + k];
				b = tableValues[origin + k + strides[i]];
			}
			tableValues[k] = a + f * (b - a);
		}
	}

	return tableValues;
}

template<class T, class W>
W LookupTableImpl<T, W>::getScalar(const W coordinates[], size_t dimIndex, const valarray<W>& tableValues, valarray<W>& w) const {
	assert(w.size() >= strides[dimIndex]);

	W f;

	for (size_t i = dimIndex; i < n; ++i) {
		const size_t origin = getIndex(i, coordinates[i], f) * strides[i];
//#pragma omp parallel for
		for (size_t k = 0; k < strides[i]; ++k) {
			W a;
			W b;
			if (i == dimIndex) {
				a = tableValues[origin + k];
				b = tableValues[origin + k + strides[i]];
			} else {
				a = w[origin + k];
				b = w[origin + k + strides[i]];
			}
			w[k] = a + f * (b - a);
		}
	}

	return w[0] * scaleFactor + addOffset;
}

template<class T, class W>
matrix<W>& LookupTableImpl<T, W>::getMatrix(const W coordinates[], size_t dimIndex, matrix<W>& matrix, const valarray<W>& tableValues, valarray<W>& w) const {
	assert(n > 2);
	assert(w.size() >= strides[dimIndex]);

	const size_t rowCount = getMatrixRowCount();
	const size_t colCount = getMatrixColCount();

	assert(matrix.size1() >= rowCount);
	assert(matrix.size2() >= colCount);

	W f;

	for (size_t i = dimIndex; i < n - 2; ++i) {
		const size_t origin = getIndex(i, coordinates[i], f) * strides[i];
//#pragma omp parallel for
		for (size_t k = 0; k < strides[i]; ++k) {
			W a;
			W b;
			if (i == dimIndex) {
				a = tableValues[origin + k];
				b = tableValues[origin + k + strides[i]];
			} else {
				a = w[origin + k];
				b = w[origin + k + strides[i]];
			}
			w[k] = a + f * (b - a);
		}
	}
//#pragma omp parallel for
	for (size_t i = 0; i < rowCount; ++i) {
		const size_t l = i * colCount;
		for (size_t k = 0; k < colCount; ++k) {
			matrix(i, k) = w[l + k] * scaleFactor + addOffset;
		}
	}

	return matrix;
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
inline size_t LookupTableImpl<T, W>::getMatrixColCount() const {
	assert(n > 2);
	return sizes[n - 1];
}

template<class T, class W>
inline size_t LookupTableImpl<T, W>::getMatrixRowCount() const {
	assert(n > 2);
	return sizes[n - 2];
}

template<class T, class W>
inline size_t LookupTableImpl<T, W>::getMatrixWorkspaceSize() const {
	assert(n > 2);
	return (1 << (n - 2)) * sizes[n - 2] * sizes[n - 1];
}

template<class T, class W>
inline size_t LookupTableImpl<T, W>::getVectorDimensionCount() const {
	assert(n > 1);
	return sizes[n - 1];
}

template<class T, class W>
inline size_t LookupTableImpl<T, W>::getVectorWorkspaceSize() const {
	assert(n > 1);
	return (1 << (n - 1)) * sizes[n - 1];
}

template<class T, class W>
inline size_t LookupTableImpl<T, W>::getScalarWorkspaceSize() const {
	assert(n > 0);
	return 1 << n;
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

#endif	/* LOOKUPTABLEIMPL_H */


