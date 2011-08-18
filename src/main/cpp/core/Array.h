/*
 * Array.h
 *
 *  Created on: Aug 18, 2011
 *      Author: ralf
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <valarray>

#include "Boost.h"

using std::valarray;

template<class T>
class Array {
public:
	Array(const valarray<T>& data, double scaleFactor = 1.0, double addOffset =
			0.0) :
			data(data), scaleFactor(scaleFactor), addOffset(addOffset) {
	}

	~Array() {
	}

	double get(size_t i) {
		return boost::numeric_cast<double>(data[i]) * scaleFactor + addOffset;
	}

	double getScaleFactor() {
		return scaleFactor;
	}

	double getAddOffset() {
		return addOffset;
	}
private:
	const valarray<T> data;

	const double scaleFactor;
	const double addOffset;
};

#endif /* ARRAY_H_ */
