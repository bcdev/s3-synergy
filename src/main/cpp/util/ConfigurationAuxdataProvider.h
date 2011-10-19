/*
 * ConfigurationAuxdataProvider.h
 *
 *  Created on: 11.10.2011
 *      Author: thomasstorm
 */

#ifndef CONFIGURATIONAUXDATAPROVIDER_H_
#define CONFIGURATIONAUXDATAPROVIDER_H_

#include <string>
#include <valarray>

using std::string;
using std::valarray;

class ConfigurationAuxdataProvider {
public:
    ConfigurationAuxdataProvider(string auxdataPath);
    virtual ~ConfigurationAuxdataProvider();

    float getFloat(const string& varName) const;
    valarray<float> getFloatArray(const string& varName) const;
    valarray<int16_t> getInt16TArray(const string& varName) const;

private:
    int fileId;
};

#endif /* CONFIGURATIONAUXDATAPROVIDER_H_ */
