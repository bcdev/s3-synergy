/*
 * XmlParser.h
 *
 *  Created on: Nov 8, 2010
 *      Author: thomass
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <string>
#include <exception>
#include <vector>

#include "XPathInitializer.h"

using std::invalid_argument;
using std::string;
using std::vector;

class XmlParser {
public:
	XmlParser();
	~XmlParser();

	const string evaluateToString(const string& path,
			const char* expression) const throw (invalid_argument);
	const string evaluateToString(const string& path,
			const string& expression) const throw (invalid_argument);
	const vector<string> evaluateToStringList(const string& path,
			const string& expression) const throw (invalid_argument);
	const vector<string> evaluateToStringList(const string& path,
			const char* expression) const throw (invalid_argument);
};

#endif /* XMLPARSER_H_ */
