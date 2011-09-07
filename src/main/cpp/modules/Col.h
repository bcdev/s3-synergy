/*
 * Col.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef COL_H_
#define COL_H_

#include "../core/BasicModule.h"

/**
 * COL - the SYN L2 collocation module.
 *
 * @author Ralf Quast
 */
class Col: public BasicModule {
public:
	Col();

	void start(Context& context);

	void stop(Context& context);

	void process(Context& context);

	virtual ~Col();
};

#endif /* COL_H_ */
