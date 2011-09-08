/* 
 * File:   Processor.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "Context.h"
#include "Module.h"
#include "ModuleException.h"

using std::vector;

class Processor {
public:

	Processor();
	~Processor();

	void process(Context& context);

private:
	class Timer {
	public:
		Timer();
		~Timer();

		void start();
		void stop();
		string time() const;

	private:
		time_t startTime;
		time_t stopTime;
	};

	ModuleException wrapException(exception& e, const string& moduleName,
			const string& sourceMethod) const;

	bool completed;
};

#endif	/* PROCESSOR_H */

