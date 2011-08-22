/*
 * Array.h
 *
 *  Created on: Aug 18, 2011
 *      Author: ralf
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

template<class W>
class Array {
public:
	virtual ~Array() {
	}

	virtual W get(size_t i) const = 0;
	virtual size_t size() const = 0;
	virtual W getScaleFactor() const = 0;
	virtual W getAddOffset() const = 0;

    virtual void* getUntypedData() const = 0;

    virtual void resize(size_t n) = 0;
	virtual void set(size_t i, W value) = 0;

	template<class T>
	static Array<W>* newArray(const valarray<T>& data, W scaleFactor = W(1),
			W addOffset = W(0)) {
		return new ArrayImpl<T, W>(data, scaleFactor, addOffset);
	}
};

template<class T, class W>
class ArrayImpl: public Array<W> {
public:
	ArrayImpl(const valarray<T>& data, W scaleFactor = 1.0, W addOffset = 0.0) :
			data(data), scaleFactor(scaleFactor), addOffset(addOffset) {
	}

	virtual ~ArrayImpl() {
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
