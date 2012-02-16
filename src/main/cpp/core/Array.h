/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <valarray>

#include <boost/cstdint.hpp>

#include "Boost.h"

using std::valarray;

using boost::int8_t;
using boost::int16_t;
using boost::int32_t;
using boost::int64_t;
using boost::uint8_t;
using boost::uint16_t;
using boost::uint32_t;
using boost::uint64_t;

template<class T, class W>
class ArrayImpl;

/**
 * Represents an array of values.
 *
 * NOTE: This class is not used yet.
 *
 * @author Ralf Quast
 */
template<class W>
class Array {
public:
	virtual ~Array() {
	}

	virtual W get(size_t i) const = 0;
	virtual size_t size() const = 0;
	virtual W getScaleFactor() const = 0;
	virtual W getAddOffset() const = 0;
	virtual valarray<W>& getData(valarray<W>& target) const = 0;
    virtual void* getUntypedData() const = 0;

    virtual void resize(size_t n) = 0;
	virtual void set(size_t i, W value) = 0;

	template<class T>
	static shared_ptr<Array<W> > newArray(const valarray<T>& data, W scaleFactor = W(1),
			W addOffset = W(0)) {
		return shared_ptr<Array<W> >(new ArrayImpl<T, W>(data, scaleFactor, addOffset));
	}

	template<class T>
	static shared_ptr<Array<W> > newArray(const T* data, size_t size, W scaleFactor = W(1),
			W addOffset = W(0)) {
		return shared_ptr<Array<W> >(new ArrayImpl<T, W>(valarray<T>(data, size), scaleFactor, addOffset));
	}
};

template<class T, class W>
class ArrayImpl: public Array<W> {
public:
	ArrayImpl(const size_t size, W scaleFactor = 1.0, W addOffset = 0.0) :
			data(size), scaleFactor(scaleFactor), addOffset(addOffset) {
	}

	ArrayImpl(const valarray<T>& data, W scaleFactor = 1.0, W addOffset = 0.0) :
			data(data), scaleFactor(scaleFactor), addOffset(addOffset) {
	}

	virtual ~ArrayImpl() {
	}

	valarray<W>& getData(valarray<W>& target) const {
		for (size_t i = 0; i < data.size(); i++) {
			target[i] = get(i);
		}
		return target;
	}

	W get(size_t i) const {
		return boost::numeric_cast<W>(data[i]) * scaleFactor + addOffset;
	}

	size_t size() const {
		return data.size();
	}

	W getScaleFactor() const {
		return scaleFactor;
	}

	W getAddOffset() const {
		return addOffset;
	}

	void* getUntypedData() const {
		return (void*) &data[0];
	}

	void resize(size_t n) {
		data.resize(n);
	}

	void set(size_t i, W value) {
		data[i] = boost::numeric_cast<T>((value - addOffset) / scaleFactor);
	}

private:
	valarray<T> data;

	const W scaleFactor;
	const W addOffset;
};

#endif /* ARRAY_H_ */
