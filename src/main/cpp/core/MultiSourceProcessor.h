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

#ifndef MULTISOURCEPROCESSOR_H
#define	MULTISOURCEPROCESSOR_H

#include "Context.h"
#include "Module.h"

using std::vector;

class MultiSourceProcessor {
public:

	MultiSourceProcessor();
	~MultiSourceProcessor();

	void process(Context& context);

private:
	void addErrorInfo(std::exception& e, const string& moduleName,
			const string& methodName, int exitCode) {
		boost::enable_error_info(e) << errinfo_module_name(moduleName)
				<< errinfo_method_name(methodName)
				<< errinfo_exit_code(exitCode);
	}

	void addErrorInfo(boost::exception& e, const string& moduleName,
			const string& methodName, int exitCode) {
		e << errinfo_module_name(moduleName) << errinfo_method_name(methodName)
				<< errinfo_exit_code(exitCode);
	}

	class Timer {
	public:
		Timer();
		~Timer();

		void start();
		void stop();
		string getDiffTime() const;
		time_t getStartTime() const;

	private:
		time_t startTime;
		time_t stopTime;
	};

	bool completed;
};

#endif	/* MULTISOURCEPROCESSOR_H */

