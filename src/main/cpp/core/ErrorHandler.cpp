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

#include <cstdlib>
#include <iostream>

#include "Boost.h"
#include "Context.h"
#include "ErrorHandler.h"
#include "ExitCode.h"

ErrorHandler::ErrorHandler() {
}

ErrorHandler::~ErrorHandler() {
}

void ErrorHandler::handleError(Context& context, exception& e) const {
	using std::exit;
	using boost::diagnostic_information;

	const string moduleName = boost::get_error_info<errinfo_module_name>(e) == 0 ? "Unknown" : *boost::get_error_info<errinfo_module_name>(e);
	const string methodName = boost::get_error_info<errinfo_method_name>(e) == 0 ? "Unknown" : *boost::get_error_info<errinfo_method_name>(e);
	const int exitCode = boost::get_error_info<errinfo_exit_code>(e) == 0 ? ExitCode::FAILURE : *boost::get_error_info<errinfo_exit_code>(e);
	const string info = diagnostic_information(e);
	const string className = extractClassName(info);
	const string functionName = extractFunctionName(info);
	const string lineNumber = extractLineNumber(info);
	const string exceptionMessage = e.what();
	const string errorMessage = createMessage(moduleName, methodName, className, functionName, lineNumber, exceptionMessage);

	context.getLogging().error(errorMessage, moduleName);

	exit(exitCode);
}

string ErrorHandler::extractLineNumber(const string& info) const {
	const string line = extractFirstLine(info);
	vector<string> result;

	boost::iter_split(result, line, boost::first_finder("("));
	if (result.size() > 1) {
		boost::iter_split(result, result[1], boost::first_finder("):"));
		if (result.size() > 0) {
			return result.front();
		}
	}
	return "Unknown";
}

string ErrorHandler::extractFunctionName(const string& info) const {
	const string line = extractFirstLine(info);
	vector<string> result;

	boost::iter_split(result, line, boost::first_finder("function"));
	if (result.size() > 1) {
		boost::iter_split(result, result[1], boost::first_finder(" "));
		boost::iter_split(result, line, boost::first_finder("::"));
		if (result.size() > 1) {
			boost::iter_split(result, result[1], boost::first_finder("("));
			return result.front();
		}
	}
	return "Unknown";
}

string ErrorHandler::extractClassName(const string& info) const {
	/*
	const string line = extractFirstLine(info);
	vector<string> result;

	boost::iter_split(result, line, boost::first_finder("function"));
	if (result.size() > 1) {
		boost::iter_split(result, result[1], boost::first_finder("::"));
		if (result.size() > 0) {
			boost::iter_split(result, result[0], boost::first_finder(" "));
			if (result.size() > 1) {
				return result.back();
			}
		}
	}
	*/
	return "Unknown";
}

string ErrorHandler::extractFirstLine(const string& text) const {
	return text.substr(0, text.find("\n"));
}

string ErrorHandler::createMessage(const string& moduleName, const string& methodName, const string& className, const string& functionName, const string& lineNumber,
		const string& exceptionMessage) const {

	string message;
	message.append("An exception was thrown in method '" + methodName + "' of module '" + moduleName + "':\n");
	message.append(exceptionMessage);
	message.append(" at: " + className + "::" + functionName + " (l. " + lineNumber + ").");

	return message;
}
