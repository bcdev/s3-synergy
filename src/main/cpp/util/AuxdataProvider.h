/*
 * AuxdataProvider.h
 *
 *  Created on: 11.10.2011
 *      Author: thomasstorm
 */

#ifndef AUXDATAPROVIDER_H_
#define AUXDATAPROVIDER_H_

#include <string>
#include <valarray>

#include "../core/Boost.h"

using std::string;
using std::valarray;

class AuxdataProvider {
public:
    AuxdataProvider(string auxdataPath);
    virtual ~AuxdataProvider();

    double getDouble(const string& varName) const;
    valarray<double> getDoubleArray(const string& varName) const;
    matrix<double> getDoubleMatrix(const string& varName) const;
    valarray<int16_t> getShortArray(const string& varName) const;

private:
    int fileId;
};

#endif /* AUXDATAPROVIDER_H_ */
