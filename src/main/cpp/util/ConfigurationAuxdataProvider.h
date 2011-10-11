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

    float getInitialTau550();
    valarray<float> getInitialNus();
    valarray<float> getInitialOmegas();
    valarray<int16_t> getAMINs();
    valarray<int16_t> getNdviIndices();

private:
    int fileId;
    float initialTau550;
    valarray<float> initialNus;
    valarray<float> initialOmegas;
    valarray<int16_t> AMINs;
    valarray<int16_t> ndviIndices;
};

#endif /* CONFIGURATIONAUXDATAPROVIDER_H_ */
