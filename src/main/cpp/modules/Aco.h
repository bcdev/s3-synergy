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
    void addAccessor(Context& context, Segment& s, const VariableDescriptor& varDescriptor) const;
    void addMatrixLookupTable(Context& context, const string& fileName, const string& varName) const;
    void addScalarLookupTable(Context& context, const string& fileName, const string& varName) const;

	static const double PI = 3.14159265358979323846;
	static const double D2R = 3.14159265358979323846 / 180.0;
};

#endif /* ACO_H_ */
