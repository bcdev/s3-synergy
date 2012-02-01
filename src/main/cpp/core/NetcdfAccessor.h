/*
 * NetcdfAccessor.h
 *
 *  Created on: Feb 1, 2012
 *      Author: ralf
 */

#ifndef NETCDFACCESSOR_H_
#define NETCDFACCESSOR_H_

#include "Accessor.h"
#include "Boost.h"
#include "Dictionary.h"
#include "Grid.h"

using std::logic_error;

class NetcdfAccessor: public virtual Accessor {
public:
	NetcdfAccessor(const VariableDescriptor& variableDescriptor, const Grid& grid, const string& directoryPath);
	~NetcdfAccessor();

	int8_t getByte(size_t i) const throw (bad_cast, out_of_range);
	void setByte(size_t i, int8_t value) throw (bad_cast, out_of_range);
	double getDouble(size_t i) const throw (bad_cast, out_of_range);
	void setDouble(size_t i, double value) throw (bad_cast, out_of_range);
	float getFloat(size_t i) const throw (bad_cast, out_of_range);
	void setFloat(size_t i, float value) throw (bad_cast, out_of_range);
	int32_t getInt(size_t i) const throw (bad_cast, out_of_range);
	void setInt(size_t i, int32_t value) throw (bad_cast, out_of_range);
	int64_t getLong(size_t i) const throw (bad_cast, out_of_range);
	void setLong(size_t i, int64_t value) throw (bad_cast, out_of_range);
	int16_t getShort(size_t i) const throw (bad_cast, out_of_range);
	void setShort(size_t i, int16_t value) throw (bad_cast, out_of_range);
	uint8_t getUByte(size_t i) const throw (bad_cast, out_of_range);
	void setUByte(size_t i, uint8_t value) throw (bad_cast, out_of_range);
	uint32_t getUInt(size_t i) const throw (bad_cast, out_of_range);
	void setUInt(size_t i, uint32_t value) throw (bad_cast, out_of_range);
	uint64_t getULong(size_t i) const throw (bad_cast, out_of_range);
	void setULong(size_t i, uint64_t value) throw (bad_cast, out_of_range);
	uint16_t getUShort(size_t i) const throw (bad_cast, out_of_range);
	void setUShort(size_t i, uint16_t value) throw (bad_cast, out_of_range);
	const size_t getSampleCount() const;

	valarray<int8_t>& getByteData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<double>& getDoubleData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<float>& getFloatData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<int32_t>& getIntData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<int64_t>& getLongData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<int16_t>& getShortData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<uint8_t>& getUByteData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<uint32_t>& getUIntData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<uint64_t>& getULongData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<uint16_t>& getUShortData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<double> getDoubles() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<float> getFloats() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	void* getUntypedData() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	int getType() const {
		return type;
	}

	double getScaleFactor() const {
		return scaleFactor;
	}

	double getAddOffset() const {
		return addOffset;
	}

	bool isFillValue(size_t i) const throw (out_of_range);
	void setFillValue(size_t i) throw (out_of_range);
	string getFillValue() const;

	void shift(long n, long strideK, long strideL) {
		// intentionally doing nothing
	}

	bool canReturnDataPointer() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

private:

	const double scaleFactor;
	const double addOffset;
	const int type;

	int fileId;
	int varId;
};

#endif /* NETCDFACCESSOR_H_ */
