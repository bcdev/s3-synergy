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

	virtual ~Col();

	void start(Context& context);
	void stop(Context& context);
	void process(Context& context);

private:
	friend class ColTest;

	void addVariable(Context& context, Segment& targetSegment, const string& targetName, const Segment& sourceSegment, const string& sourceName, const ProductDescriptor& sourceProductDescriptor);
	void addVariableAlias(Context& context, Segment& targetSegment, const string& targetName, const Segment& sourceSegment, const string& sourceName) const;
	void addSlstrVariables(Context& context);

	vector<string> targetNames;
	map<string, string> collocationXMap;
	map<string, string> collocationYMap;
	map<string, string> sourceNameMap;
	map<string, const Segment*> sourceSegmentMap;

	static const string SLO_CONFIDENCE_FLAG_VARIABLE;
	static const string SLN_CONFIDENCE_FLAG_VARIABLE;
};

#endif /* COL_H_ */
