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

#ifndef MAPACCESSOR_H_
#define MAPACCESSOR_H_

#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>

#include <algorithm>
#include <cstring>
#include <limits>
#include <typeinfo>

#include "Accessor.h"
#include "Boost.h"
#include "Constants.h"

using std::numeric_limits;
using std::logic_error;

/**
 * An abstract implementation of the Accessor interface using memory stored in a file.
 */
template<class T, int N>
class MapAccessor: public virtual Accessor {
public:
	/**
	 * Constructs a new instance of this class.
	 * @param n The size of the underlying array.
	 * @param fillValue The fill value used for the variable.
	 * @param scaleFactor The scale factor used for the variable.
	 * @param addOffset The add-offset used for the variable.
	 */
	MapAccessor(size_t n, T fillValue = numeric_limits<T>::min(), double scaleFactor = 1.0, double addOffset = 0.0) :
			Accessor(), fillValue(fillValue), scaleFactor(scaleFactor), addOffset(addOffset), filename(Constants::S3_SYNERGY_HOME.length() + 12) {
		using std::min;
		using std::runtime_error;
		using std::strcpy;

		using boost::filesystem::create_directories;
		using boost::filesystem::exists;
		using boost::filesystem::path;

		const string tempFile = Constants::S3_SYNERGY_HOME + "/tmp/XXXXXX";

		if (!exists(path(tempFile).parent_path())) {
			if (!create_directories(path(tempFile).parent_path())) {
				BOOST_THROW_EXCEPTION( runtime_error("Cannot create directory '" + tempFile + "'."));
			}
		}

		string a;
		strcpy(&filename[0], tempFile.c_str());
		fd = mkstemp(&filename[0]);
		if (fd < 0) {
			BOOST_THROW_EXCEPTION(runtime_error("Unable to open file '" + string(&filename[0]) + "'"));
		}
		valarray<T> buffer(fillValue, 1024);
		for (size_t i = 0; i < n; i += 1024) {
			write(fd, &buffer[0], min<size_t>(1024, n - i) * sizeof(T));
		}
		length = n * sizeof(T);
		addr = mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (addr == (caddr_t) -1) {
			BOOST_THROW_EXCEPTION(runtime_error("Unable to map file '" + string(&filename[0]) + "' to memory."));
		}
		data = (T*) addr;
	}

	virtual ~MapAccessor() {
		munmap(addr, length);
		close(fd);
		remove(&filename[0]);
	}

	int8_t getByte(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int8_t>(getValue(i));
	}

	void setByte(size_t i, int8_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	double getDouble(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<double>(getValue(i)) * scaleFactor + addOffset;
	}

	void setDouble(size_t i, double value) throw (bad_cast, out_of_range) {
		value = (value - addOffset) / scaleFactor;
		if (value > numeric_limits<T>::max()) {
			data[i] = numeric_limits<T>::max();
		} else {
			data[i] = boost::numeric_cast<T>(value);
		}
	}

	float getFloat(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<float>(getValue(i)) * float(scaleFactor) + float(addOffset);
	}

	void setFloat(size_t i, float value) throw (bad_cast, out_of_range) {
		value = (value - float(addOffset)) / float(scaleFactor);
		if (value > numeric_limits<T>::max()) {
			data[i] = numeric_limits<T>::max();
		} else {
			data[i] = boost::numeric_cast<T>(value);
		}
	}

	int32_t getInt(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int32_t>(getValue(i));
	}

	void setInt(size_t i, int32_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	int64_t getLong(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int64_t>(getValue(i));
	}

	void setLong(size_t i, int64_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	int16_t getShort(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int16_t>(getValue(i));
	}

	void setShort(size_t i, int16_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<int16_t>(value);
	}

	uint8_t getUByte(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint8_t>(getValue(i));
	}

	void setUByte(size_t i, uint8_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	uint32_t getUInt(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint32_t>(getValue(i));
	}

	void setUInt(size_t i, uint32_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	uint64_t getULong(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint64_t>(getValue(i));
	}

	void setULong(size_t i, uint64_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	uint16_t getUShort(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint16_t>(getValue(i));
	}

	void setUShort(size_t i, uint16_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	const T getValue(size_t i) const throw (out_of_range) {
		return data[i];
	}

	const size_t getSampleCount() const {
		return length / sizeof(T);
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<int8_t>& getByteData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<double>& getDoubleData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<float>& getFloatData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<int32_t>& getIntData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<int64_t>& getLongData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<int16_t>& getShortData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<uint8_t>& getUByteData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<uint32_t>& getUIntData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<uint64_t>& getULongData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<uint16_t>& getUShortData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<double> getDoubles() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	/**
	 * Not implemented; always throws a logic_error.
	 */
	valarray<float> getFloats() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	void* getUntypedData() const {
		return data;
	}

	int getType() const {
		return N;
	}

	double getScaleFactor() const {
		return scaleFactor;
	}

	double getAddOffset() const {
		return addOffset;
	}

	bool isFillValue(size_t i) const throw (out_of_range) {
		return data[i] == fillValue;
	}

	void setFillValue(size_t i) throw (out_of_range) {
		data[i] = fillValue;
	}

	string getFillValue() const {
		using boost::lexical_cast;
		using boost::numeric_cast;

		if (getType() == Constants::TYPE_BYTE) {
			return lexical_cast<string>(numeric_cast<int16_t>(fillValue));
		}
		if (getType() == Constants::TYPE_UBYTE) {
			return lexical_cast<string>(numeric_cast<uint16_t>(fillValue));
		}
		return lexical_cast<string>(fillValue);
	}

	/**
	 * Intentionally does nothing.
	 */
	void shift(long n, long strideK, long strideL) {
		// intentionally doing nothing
	}

private:
	const T fillValue;
	const double scaleFactor;
	const double addOffset;

	valarray<char> filename;
	int fd;
	size_t length;
	void* addr;
	T* data;
};

#endif /* MAPACCESSOR_H_ */
