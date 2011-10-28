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

	uint8_t getUByte(const string& varName) const;
	double getDouble(const string& varName) const;
	int16_t getShort(const string& varName) const;

	const valarray<double>& getVectorDouble(const string& varName) const;
	const valarray<int16_t>& getVectorShort(const string& varName) const;

	const matrix<double>& getMatrixDouble(const string& varName) const;

private:
	const string id;
	const int fileId;

	mutable map<string, uint8_t> ubytes;
	mutable map<string, double> doubles;
	mutable map<string, int16_t> shorts;
	mutable map<string, shared_ptr<valarray<double> > > doubleArrays;
	mutable map<string, shared_ptr<valarray<int16_t> > > shortArrays;
	mutable map<string, shared_ptr<matrix<double> > > doubleMatrices;

	template<class K, class V>
	static bool contains(const map<K, V>& map, const K& key) {
		return map.find(key) != map.end();
	}
};

#endif /* AUXDATAPROVIDER_H_ */
