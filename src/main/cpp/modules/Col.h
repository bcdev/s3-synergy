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

	void addTargetSegment(Context& context) const;
	void addOlciVariables(Context& context);
	void addVariable(Context& context, const ProductDescriptor& productDescriptor, const string& sourceName, const string& targetName, const string& sourceSegmentId);
	void addSlstrVariables(Context& context);
	void addVariableAlias(Context& context, const string& targetName, const string& sourceName);
	void collocateOlci(Context& context, Accessor& sourceAccessor, const int sourceType, string& sourceName);
	void collocateSlstr(Context& context, Accessor& sourceAccessor, const int sourceType, const Grid& sourceGrid, string& sourceName);
	string retrievePositionVariableName(const string& targetName, const string& axis);
	string retrieveDeltaVariableName(const string& targetName, const string& axis);

	vector<string> targetVariables;
	map<string, string> segmentIds;
	map<string, string> variables;

	static const string SLO_CONFIDENCE_FLAG_VARIABLE;
	static const string SLN_CONFIDENCE_FLAG_VARIABLE;
	static const string AXIS_ROW;
	static const string AXIS_COL;
};

#endif /* COL_H_ */
