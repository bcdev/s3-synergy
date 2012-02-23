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

#ifndef ERRORHANDLER_H
#define	ERRORHANDLER_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Context;

/**
 * An error handler.
 */
class ErrorHandler {
public:
	/**
	 * Constructor.
	 */
	ErrorHandler();

	/**
	 * Destructor.
	 */
	~ErrorHandler();

	/**
	 * Handles the exception supplied as argument (logs an error message and
	 * terminates).
	 * @param context The context.
	 * @param e The exception.
	 */
	void handleError(Context& context, std::exception& e) const;

private:
	string extractFunctionName(const string& info) const;
	string extractClassName(const string& info) const;
	string extractLineNumber(const string& info) const;
	vector<string> splitIntoLines(const string& text) const;
	string createMessage(const string& moduleName, const string& methodName,
			const string& module, const string& functionName,
			const string& lineNumber, const string& exceptionMessage) const;
};

#endif	/* ERRORHANDLER_H */

