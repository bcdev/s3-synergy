/*
 * AuxdataProvider.h
 *
 *  Created on: 11.10.2011
 *      Author: thomasstorm
 */

#ifndef AUXDATAPROVIDER_H_
#define AUXDATAPROVIDER_H_

#include <map>
#include <string>
#include <valarray>

#include "../core/Boost.h"
#include "../core/Identifiable.h"

using std::map;
using std::string;
using std::valarray;

class AuxdataProvider: public Identifiable {
public:
	AuxdataProvider(const string& id, string auxdataPath);
	virtual ~AuxdataProvider();

	const string& getId() const;

	void getUByte(const string& varName, uint8_t& data) const;
	void getUShort(const string& varName, uint16_t& data) const;
	void getDouble(const string& varName, double& data) const;
	void getShort(const string& varName, int16_t& data) const;

	void getVectorDouble(const string& varName, valarray<double>& data) const;
	void getVectorShort(const string& varName, valarray<int16_t>& data) const;

	void getMatrixDouble(const string& varName, matrix<double>& data) const;

private:
	const string id;
	const int fileId;
};

#endif /* AUXDATAPROVIDER_H_ */
