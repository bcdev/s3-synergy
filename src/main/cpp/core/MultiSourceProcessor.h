/* 
 * File:   MultiSourceProcessor.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
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

