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

	vector<string> targetNames;
	map<string, size_t> sourceChannelMap;
	map<string, string> sourceNameMap;
};

#endif /* COL_H_ */
