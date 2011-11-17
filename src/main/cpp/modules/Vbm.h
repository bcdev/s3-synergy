/*
 * Vbm.h
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#ifndef VBM_H_
#define VBM_H_

#include "../modules/BasicModule.h"

class Vbm: public BasicModule {
public:
	Vbm();
	virtual ~Vbm();

	void start(Context& context);
	void stop(Context& context);
	void process(Context& context);

private:
	friend class VbmTest;
};

#endif /* VBM_H_ */
