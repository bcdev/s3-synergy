/*
 * ConfigParser.h
 *
 *  Created on: Nov 10, 2010
 *      Author: thomass
 */

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_

#include "XmlParser.h"
#include <iostream>

using namespace std;

class ConfigParser : public XmlParser {
public:
	ConfigParser( string path );
	virtual ~ConfigParser();
	Configuration parseConfiguration();
private:
	void parseSubConfigs( Configuration * config, DOMElement * node, bool isSyn );
};

#endif /* CONFIGPARSER_H_ */
