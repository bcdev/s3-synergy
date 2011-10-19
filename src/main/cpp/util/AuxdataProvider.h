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

using std::string;
using std::valarray;

class AuxdataProvider {
public:
    AuxdataProvider(string auxdataPath);
    virtual ~AuxdataProvider();

    float getFloat(const string& varName) const;
    valarray<float> getFloatArray(const string& varName) const;
    valarray<int16_t> getInt16TArray(const string& varName) const;

private:
    int fileId;
};

#endif /* AUXDATAPROVIDER_H_ */
