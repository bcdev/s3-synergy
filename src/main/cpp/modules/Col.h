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


private:
    friend class ColTest;
    void addOlciVariables(Context& context, Segment& collocatedSegment, const Segment& olciSegment);
    void addVariable(ProductDescriptor& productDescriptor, const string& sourceName, const string& targetName, Context& context, Segment& collocatedSegment);
    void addSlstrVariables(Context& context, Segment& collocatedSegment);
    void addVariableAlias(Context & context, const string & targetName, Segment & collocatedSegment, const Segment & olciSegment, const string & sourceName);
    shared_ptr<Segment> collocatedSegment;

};

#endif /* COL_H_ */
