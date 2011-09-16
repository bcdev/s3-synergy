/*
 * Col.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef COL_H_
#define COL_H_

#include <stdexcept>
#include <vector>

#include "../core/Boost.h"
#include "../core/BasicModule.h"

using std::vector;
using std::logic_error;

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
    void addVariable(ProductDescriptor& productDescriptor, const string& sourceName, const string& targetName, Context& context, Segment& collocatedSegment, const string& sourceSegmentId);
    void addSlstrVariables(Context& context, Segment& collocatedSegment);
    void addVariableAlias(Context& context, const string& targetName, Segment& collocatedSegment, const Segment& sourceSegment, const string& sourceName);
    void collocateOlci(Accessor& sourceAccessor, const Grid& sourceGrid, string& sourceName);
    void collocateSln(Accessor& sourceAccessor, const Grid& sourceGrid, const Segment& olciSegment, string& sourceName);
    void collocateSlo(Accessor& sourceAccessor, const Grid& sourceGrid, string& sourceName);
    string retrievePositionVariableName(const string& targetName, const string& axis);

    static const string SLO_CONFIDENCE_FLAG_VARIABLE;
    static const string SLN_CONFIDENCE_FLAG_VARIABLE;
    static const string AXIS_ROW;
    static const string AXIS_COL;

    shared_ptr<Segment> collocatedSegment;
    vector<string> targetVariables;
    map<string, string> segmentIds;
    map<string, string> variables;

};

#endif /* COL_H_ */
