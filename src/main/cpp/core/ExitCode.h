/*
 * ExitCode.h
 *
 *  Created on: Aug 16, 2011
 *      Author: ralf
 */

#ifndef EXITCODE_H_
#define EXITCODE_H_

#include "Context.h"

class ExitCode {
public:
	~ExitCode() {
	}

	const static int OK = 0;
	const static int INCOMPLETE = 1;
	const static int FAILURE = 128;

private:
	ExitCode() {
	}
};

#endif /* EXITCODE_H_ */
