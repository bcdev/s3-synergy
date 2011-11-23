/*
 * ExitCode.h
 *
 *  Created on: Aug 16, 2011
 *      Author: ralf
 */

#ifndef EXITCODE_H_
#define EXITCODE_H_

class ExitCode {
public:
	~ExitCode() {
	}

	const static int OK;
	const static int INCOMPLETE;
	const static int FAILURE;

private:
	ExitCode() {
	}
};

#endif /* EXITCODE_H_ */
