/*
 * ConfigParser.h
 *
 *  Created on: Nov 10, 2010
 *      Author: thomass
 */

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_

#include "../common/XmlParser.h"


class ConfigParser : public XmlParser {
public:
	ConfigParser( std::string path );
	virtual ~ConfigParser();
	Configuration parseConfiguration();
private:
	void parseSubConfigs( Configuration * config, DOMElement * node, bool isSyn );
};

#endif /* CONFIGPARSER_H_ */
