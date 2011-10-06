/*
 * Aco.h
 *
 *  Created on: Aug 24, 2011
 *      Author: ralf
 */

#ifndef ACO_H_
#define ACO_H_

#include "../core/BasicModule.h"
#include "../core/LookupTable.h"

class Aco : public BasicModule {
public:
	Aco();
	virtual ~Aco();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);
private:
    shared_ptr<MatrixLookupTable<double> > lutSlnRatm;
    shared_ptr<MatrixLookupTable<double> > lutSloRatm;
    shared_ptr<MatrixLookupTable<double> > lutT;
    shared_ptr<MatrixLookupTable<double> > lutRhoAtm;
    shared_ptr<ScalarLookupTable<double> > lutCO3;

	static const double PI = 3.14159265358979323846;
	static const double D2R = 3.14159265358979323846 / 180.0;
};

#endif /* ACO_H_ */
