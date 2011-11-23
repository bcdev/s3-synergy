/*
 * BasicTask.h
 *
 *  Created on: Nov 23, 2011
 *      Author: ralf
 */

#ifndef BASICTASK_H_
#define BASICTASK_H_

#include "../core/Context.h"
#include "../util/XPathInitializer.h"

class BasicTask {
public:
	BasicTask(const string& name);
	~BasicTask();

	int execute(int argc, char* argv[]);
	int execute(const string& jobOrderPath);

	Context& getContext() {
		return context;
	}

private:
	void logIoParameters(JobOrder& jobOrder, Logging& logging) const;

	const string name;

	XPathInitializer init;
	Context context;
};

#endif /* BASICTASK_H_ */
