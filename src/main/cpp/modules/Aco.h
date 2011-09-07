/*
 * Aco.h
 *
 *  Created on: Aug 24, 2011
 *      Author: ralf
 */

#ifndef ACO_H_
#define ACO_H_

#include "../core/BasicModule.h"

class Aco : public BasicModule {
public:
	Aco();
	virtual ~Aco();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);
private:
    shared_ptr<LookupTable<double> > olcRatm;
    shared_ptr<LookupTable<double> > t;
    shared_ptr<LookupTable<double> > rhoAtm;
    shared_ptr<LookupTable<double> > co3;
};

#endif /* ACO_H_ */
