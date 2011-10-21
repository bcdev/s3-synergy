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

class AuxdataProvider : public Identifiable {
public:
    AuxdataProvider(const string& id, string auxdataPath);
    virtual ~AuxdataProvider();

    double getDouble(const string& varName);
    valarray<double>& getDoubleArray(const string& varName);
    matrix<double>& getDoubleMatrix(const string& varName);
    valarray<int16_t>& getShortArray(const string& varName);
    const string& getId() const;

private:
    const string id;
    int fileId;
    map<string, double> doubles;
    map<string, valarray<double> > doubleArrays;
    map<string, matrix<double> > doubleMatrices;
    map<string, valarray<int16_t> > shortArrays;

    template<class K, class V>
    static bool contains(const map<K, V>& map, const K& key) {
        return map.find(key) != map.end();
    }
};

#endif /* AUXDATAPROVIDER_H_ */
