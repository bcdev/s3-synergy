/*
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * File:   ErrorHandler.cpp
 * Author: thomass
 *
 * Created on January 13, 2010, 5:17 PM
 */

#include <iostream>

#include "Boost.h"
#include "Context.h"
#include "ExitCode.h"

#include "ErrorHandler.h"

ErrorHandler::ErrorHandler() {
}

ErrorHandler::~ErrorHandler() {
}

void ErrorHandler::handleError(Context& context, exception& e) const {
	const string processorVersion =
			context.getJobOrder() != 0 ?
					context.getJobOrder()->getIpfConfiguration().getVersion() :
					Constants::PROCESSOR_VERSION;
	const string diagnostics = diagnostic_information(e);

	const string moduleName = extractModuleName(diagnostics);
	const string functionName = extractFunctionName(diagnostics);
	const string lineNumber = extractLineNumber(diagnostics);
	const string exceptionMessage = string(e.what());

	const string errorMessage = createMessage(moduleName, functionName,
			lineNumber, exceptionMessage);

	context.getLogging()->error(errorMessage, moduleName, processorVersion);
	exit(ExitCode::FAILURE);
}

string ErrorHandler::extractLineNumber(const string& diagnostics) const {
	string firstLine = splitIntoLines(diagnostics)[0];
	vector<string> temp;
	boost::iter_split(temp, firstLine, boost::first_finder("("));
	boost::iter_split(temp, temp[1], boost::first_finder("):"));
	return temp[0];
}

string ErrorHandler::extractFunctionName(const string& diagnostics) const {
	string firstLine = splitIntoLines(diagnostics)[0];
	vector<string> temp;
	boost::iter_split(temp, firstLine, boost::first_finder("function"));
	boost::iter_split(temp, temp[1], boost::first_finder(" "));
	boost::iter_split(temp, firstLine, boost::first_finder("::"));
	boost::iter_split(temp, temp[1], boost::first_finder("("));
	return temp[0];
}

string ErrorHandler::extractModuleName(const string& diagnostics) const {
	string firstLine = splitIntoLines(diagnostics)[0];
	vector<string> temp;
	boost::iter_split(temp, firstLine, boost::first_finder("function"));
	boost::iter_split(temp, temp[1], boost::first_finder("::"));
	boost::iter_split(temp, temp[0], boost::first_finder(" "));
	return temp[temp.size() - 1];
}

vector<string> ErrorHandler::splitIntoLines(const string& toSplit) const {
	vector<string> lines;
	boost::iter_split(lines, toSplit, boost::first_finder("\n"));
	return lines;
}

string ErrorHandler::createMessage(const string& moduleName,
		const string& functionName, const string& lineNumber,
		const string& exceptionMessage) const {

	string infoString;
	if (!moduleName.empty() && !functionName.empty() && !lineNumber.empty()) {
		infoString.append(
				" at: " + moduleName + "::" + functionName + " (l. "
						+ lineNumber + ").");
	}

	string message;
	message.append(exceptionMessage);
	message.append(infoString);

	return message;
}
