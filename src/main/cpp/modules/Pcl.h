/*
 * Pcl.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef PCL_H_
#define PCL_H_

#include "../core/BasicModule.h"

class Pcl: public BasicModule {
public:
	Pcl();
	virtual ~Pcl();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);
};

#endif /* PCL_H_ */
