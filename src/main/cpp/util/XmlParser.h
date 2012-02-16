/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <string>
#include <stdexcept>
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
