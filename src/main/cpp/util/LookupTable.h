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
#include <iostream>
#include <valarray>

using namespace std;

template<class Wp>
class LookupTable {
public:
    typedef valarray<Wp> Axis;

    LookupTable();
    LookupTable(const Axis axes[], size_t numAxes);
    LookupTable(const Axis axes[], size_t numAxes, const Wp values[]);
    LookupTable(const Axis* axes[], size_t numAxes, const Wp values[]);
    ~LookupTable();

    Wp operator()(const Wp coordinates[]) const;

    Wp maxCoordinate(size_t axisIndex) const;
    Wp minCoordinate(size_t axisIndex) const;
    bool isValidCoordinate(size_t axisIndex, Wp coordinate) const;

    void degrade(size_t axisIndex, Wp coordinate);

    void reset(const Axis axes[], size_t numAxes);
    void reset(const Axis axes[], size_t numAxes, const Wp values[]);
    void reset(const Axis* axes[], size_t numAxes, const Wp values[]);

    istream& read(istream& is);
    ostream& write(ostream& os) const;
    istream& readValues(istream& is);
    ostream& writeValues(ostream& os) const;

private:
    void updateSizesAndStrides();
    void getVertexes(const Wp coordinates[], size_t vertexes[]) const;
    void interpolate(Wp values[], const Wp values2[], size_t numValues,
            Wp interpolationFactor) const;

    Wp interpolationFactor(size_t axisIndex, Wp coordinate, size_t vertex) const;

    size_t valueIndex(const size_t vertexes[]) const;
    size_t vertex(size_t axisIndex, Wp coordinate) const;

    static size_t numValues(const size_t sizes[], size_t numAxes);
    static size_t numValues(const Axis axes[], size_t numAxes);
    static size_t numValues(const Axis* axes[], size_t numAxes);
    static bool isBigEndian();

    template<class T>
    static istream& read(istream& is, T* first, size_t n);

    template<class T>
    static ostream& write(ostream& is, const T* first, size_t n);

    valarray<Axis> _x;
        // coordinate axes = vertex coordinates
    valarray<Wp> _y;
        // tabulated values

    valarray<size_t> _sizes;
    valarray<size_t> _strides;
    valarray<size_t> _indexes;

    size_t _n;
        // table dimension = number of coordinate axes
};

template<class Wp>
LookupTable<Wp>::LookupTable()
: _x(), _y(), _sizes(), _strides(), _indexes(), _n(0) {
}

template<class Wp>
LookupTable<Wp>::LookupTable(const Axis axes[], size_t numAxes)
: _x(), _y(), _sizes(), _strides(), _indexes(), _n(0) {
    reset(axes, numAxes);
}

template<class Wp>
LookupTable<Wp>::LookupTable(const Axis axes[], size_t numAxes, const Wp values[])
: _x(), _y(), _sizes(), _strides(), _indexes(), _n(0) {
    reset(axes, numAxes, values);
}

template<class Wp>
LookupTable<Wp>::LookupTable(const Axis* axes[], size_t numAxes, const Wp values[])
: _x(), _y(), _sizes(), _strides(), _indexes(), _n(0) {
    reset(axes, numAxes, values);
}

template<class Wp>
LookupTable<Wp>::~LookupTable() {
}

template<class Wp>
Wp
LookupTable<Wp>::operator()(const Wp coordinates[]) const {
    using std::valarray;

    valarray<size_t> v(_n);
    getVertexes(coordinates, &v[0]);

    valarray<Wp> values = _y[_indexes + valueIndex(&v[0])];
    // extract the y-values at the vertexes of the smallest n-cube
    // containing the interpolation point
    for (size_t i = 0, j = values.size(); j >>= 1 != 0; ++i)
        interpolate(&values[0], &values[j], j,
            interpolationFactor(i, coordinates[i], v[i]));

    return values[0];
}

template<class Wp>
inline
Wp
LookupTable<Wp>::maxCoordinate(size_t axisIndex) const {
    assert(axisIndex < _n and _sizes[axisIndex] > 0);
    return _x[axisIndex][_sizes[axisIndex] - 1];
}

template<class Wp>
inline
Wp
LookupTable<Wp>::minCoordinate(size_t axisIndex) const {
    assert(axisIndex < _n and _sizes[axisIndex] > 0);
    return _x[axisIndex][0];
}

template<class Wp>
bool
LookupTable<Wp>::isValidCoordinate(size_t axisIndex, Wp coordinate) const {
    return coordinate >= minCoordinate(axisIndex) &&
            coordinate <= maxCoordinate(axisIndex);
}

template<class Wp>
void
LookupTable<Wp>::degrade(size_t axisIndex, Wp coordinate) {
    using std::gslice;
    using std::valarray;

    if (_n > 1) {
        const size_t v = vertex(axisIndex, coordinate);
        const size_t start = v * _strides[axisIndex];

        _sizes[axisIndex] = 1;

        valarray<Wp> values = _y[gslice(start, _sizes, _strides)];
        const valarray<Wp> values2 = _y[gslice(start + _strides[axisIndex], _sizes, _strides)];
        valarray<Axis> axes(_n - 1);

        for (size_t i = 0; i < axisIndex; ++i) {
            axes[i].resize(_sizes[i]);
            axes[i] = _x[i];
        }
        for (size_t i = axisIndex; i < _n - 1; ++i) {
            axes[i].resize(_sizes[i + 1]);
            axes[i] = _x[i + 1];
        }
        interpolate(&values[0], &values2[0], values.size(),
                interpolationFactor(axisIndex, coordinate, v));

        reset(&axes[0], _n - 1, &values[0]);
    }
}

template<class Wp>
std::istream&
LookupTable<Wp>::read(std::istream& is) {
    using std::valarray;

    if (is) {
        size_t n;
        read(is, &n, 1);

        valarray<size_t> sizes(n);
        valarray<Axis> axes(n);

        for (size_t i = 0; i < n; ++i) {
            read(is, &sizes[i], 1);
            axes[i].resize(sizes[i]);
            read(is, &axes[i][0], sizes[i]);
        }

        valarray<Wp> values(numValues(&sizes[0], n));
        read(is, &values[0], values.size());

        if (is) {
            reset(&axes[0], n, &values[0]);
        }
    }

    return is;
}

template<class Wp>
ostream&
LookupTable<Wp>::write(ostream& os) const {
    if (os) {
        write(os, &_n, 1);
        for (size_t i = 0; i < _n; ++i) {
            write(os, &_sizes[i], 1);
            write(os, &_x[i][0], _sizes[i]);
        }
        write(os, &_y[0], _y.size());
    }

    return os;
}

template<class Wp>
ostream&
LookupTable<Wp>::writeValues(ostream& os) const {
    if (os) {
        write(os, &_y[0], _y.size());
    }

    return os;
}

template<class Wp>
inline
Wp
LookupTable<Wp>::interpolationFactor(size_t axisIndex,
Wp coordinate, size_t vertex) const {
    return (coordinate - _x[axisIndex][vertex]) / (_x[axisIndex][vertex + 1] -
            _x[axisIndex][vertex]);
}

template<class Wp>
size_t
LookupTable<Wp>::numValues(const size_t sizes[], size_t numAxes) {
    size_t numValues = 1;

    for (size_t i = 0; i < numAxes; ++i) {
        assert(sizes[i] > 0);
        numValues *= sizes[i];
    }

    return numValues;
}

template<class Wp>
size_t
LookupTable<Wp>::numValues(const Axis axes[], size_t numAxes) {
    size_t numValues = 1;

    for (size_t i = 0; i < numAxes; ++i) {
        assert(axes[i].size() > 0);
        numValues *= axes[i].size();
    }

    return numValues;
}

template<class Wp>
size_t
LookupTable<Wp>::numValues(const Axis* axes[], size_t numAxes) {
    size_t numValues = 1;

    for (size_t i = 0; i < numAxes; ++i) {
        assert(axes[i]->size() > 0);
        numValues *= axes[i]->size();
    }

    return numValues;
}

template<class Wp>
size_t
LookupTable<Wp>::valueIndex(const size_t vertexes[]) const {
    size_t index = 0;

    for (size_t i = 0; i < _n; ++i) {
        assert(vertexes[i] < _sizes[i]);

        index += vertexes[i] * _strides[i];
    }

    return index;
}

template<class Wp>
size_t
LookupTable<Wp>::vertex(size_t axisIndex, Wp coordinate) const {
    assert(axisIndex < _n and _sizes[axisIndex] > 0);

    if (!isValidCoordinate(axisIndex, coordinate)) {
        cout << "Table dimension  = " << _n << endl;
        cout << "Axis index       = " << axisIndex << endl;
        cout << "Coordinate value = " << coordinate << endl;
        cout << "Minimum value    = " << minCoordinate(axisIndex) << endl;
        cout << "Maximum value    = " << maxCoordinate(axisIndex) << endl;
    }
    assert(coordinate >= _x[axisIndex][0] and coordinate <= _x[axisIndex][_sizes[axisIndex] - 1]);

    size_t i = 0;
    size_t k = _sizes[axisIndex] - 1;

    while (k > i + 1) {
        const size_t j = (i + k) >> 1;

        if (coordinate > _x[axisIndex][j])
            i = j;
        else
            k = j;
    }

    return i;
}

template<class Wp>
void
LookupTable<Wp>::getVertexes(const Wp coordinates[], size_t vertexes[]) const {
    for (size_t i = 0; i < _n; ++i)
        vertexes[i] = vertex(i, coordinates[i]);
}

template<class Wp>
void
LookupTable<Wp>::interpolate(Wp values[], const Wp values2[], size_t numValues,
Wp interpolationFactor) const {
    for (size_t i = 0; i < numValues; ++i)
        values[i] = (Wp(1) - interpolationFactor) * values[i] +
            interpolationFactor * values2[i];
}

template<class Wp>
void
LookupTable<Wp>::updateSizesAndStrides() {
    _sizes.resize(_x.size());
    _strides.resize(_x.size());

    for (size_t i = 0; i < _x.size(); ++i)
        _sizes[i] = _x[i].size();
    for (size_t i = _x.size(), stride = 1; i-- > 0;) {
        _strides[i] = stride;
        stride *= _sizes[i];
    }

    _n = _x.size();
}

template<class Wp>
void
LookupTable<Wp>::reset(const Axis axes[], size_t numAxes) {
    const size_t numValues = this->numValues(axes, numAxes);

    _x.resize(numAxes);
    _y.resize(numValues);

    for (size_t i = 0; i < numAxes; ++i) {
        const Axis& axis = axes[i];

        _x[i].resize(axis.size());
        copy(&axis[0], &axis[axis.size()], &_x[i][0]);
    }

    updateSizesAndStrides();

    valarray<size_t> numbers(numValues);
    for (size_t i = 0; i < numValues; ++i)
        numbers[i] = i;
    _indexes.resize(1 << _n);
    _indexes = numbers[gslice(0, valarray<size_t > (2, _n), _strides)];
}

template<class Wp>
void
LookupTable<Wp>::reset(const Axis axes[], size_t numAxes, const Wp values[]) {
    const size_t numValues = this->numValues(axes, numAxes);

    _x.resize(numAxes);
    _y.resize(numValues);

    for (size_t i = 0; i < numAxes; ++i) {
        const Axis& axis = axes[i];

        _x[i].resize(axis.size());
        copy(&axis[0], &axis[axis.size()], &_x[i][0]);
    }
    copy(&values[0], &values[numValues], &_y[0]);

    updateSizesAndStrides();

    valarray<size_t> numbers(numValues);
    for (size_t i = 0; i < numValues; ++i)
        numbers[i] = i;
    _indexes.resize(1 << _n);
    _indexes = numbers[gslice(0, valarray<size_t > (2, _n), _strides)];
}

template<class Wp>
void
LookupTable<Wp>::reset(const Axis* axes[], size_t numAxes, const Wp values[]) {
    const size_t numValues = this->numValues(axes, numAxes);

    _x.resize(numAxes);
    _y.resize(numValues);

    for (size_t i = 0; i < numAxes; ++i) {
        const Axis& axis = *axes[i];

        _x[i].resize(axis.size());
        copy(&axis[0], &axis[axis.size()], &_x[i][0]);
    }
    copy(&values[0], &values[numValues], &_y[0]);

    updateSizesAndStrides();

    valarray<size_t> numbers(numValues);
    for (size_t i = 0; i < numValues; ++i)
        numbers[i] = i;
    _indexes.resize(1 << _n);
    _indexes = numbers[gslice(0, valarray<size_t > (2, _n), _strides)];
}

template<class Wp>
bool
LookupTable<Wp>::isBigEndian() {
    const unsigned long test = 1;

    return *reinterpret_cast<const unsigned char*> (&test) == 0;
}

template<class Wp>
template<class T>
istream&
LookupTable<Wp>::read(istream& is, T* first, size_t n) {
    if (isBigEndian()) {
        is.read(reinterpret_cast<char*> (first), sizeof (T) * n);
    } else {
        char bytes[sizeof (T)];

        for (size_t i = 0; i < n; ++i, ++first) {
            is.read(bytes, sizeof (T));
            reverse(bytes, bytes + sizeof (T));
            *first = *reinterpret_cast<T*> (bytes);
        }
    }

    return is;
}

template<class Wp>
template<class T>
ostream&
LookupTable<Wp>::write(ostream& os, const T* first, size_t n) {
    if (isBigEndian()) {
        os.write(reinterpret_cast<const char*> (first), sizeof (T) * n);
    } else {
        char bytes[sizeof (T)];

        for (size_t i = 0; i < n; ++i, ++first) {
            reverse_copy(reinterpret_cast<const char*> (first),
                    reinterpret_cast<const char*> (first) + sizeof (T), bytes);
            os.write(bytes, sizeof (T));
        }
    }

    return os;
}

#endif	/* LOOKUPTABLE_H */

