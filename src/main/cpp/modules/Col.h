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

#include "../modules/BasicModule.h"
#include "../core/Boost.h"

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
	void process(Context& context);

private:
	friend class ColTest;

	void addOlciVariables(Context& context);
	void addSlstrVariables(Context& context);
	void addVariable(Context& context, Segment& targetSegment, const string& targetName, const Segment& sourceSegment, const string& sourceName, const ProductDescriptor& sourceProductDescriptor);
	void addVariableAlias(Context& context, Segment& targetSegment, const string& targetName, const Segment& sourceSegment, const string& sourceName) const;

	vector<string> targetNames;
	// TODO - improvement: use accessor instead of name mappings
	map<string, string> collocationNameMapX;
	map<string, string> collocationNameMapY;
	map<string, string> sourceNameMap;
	map<string, const Segment*> sourceSegmentMap;

	vector<Accessor*> solarIrradianceAccessors;
	map<Accessor*, const Accessor*> sourceAccessorMap;
	map<Accessor*, const Accessor*> xCollocationAccessorMap;
	map<Accessor*, const Accessor*> yCollocationAccessorMap;

	static const string SLO_CONFIDENCE_FLAG_VARIABLE_NAME;
	static const string SLN_CONFIDENCE_FLAG_VARIABLE_NAME;
	static const size_t OLC_TO_SYN_CHANNEL_MAPPING[18];
	static const size_t OLC_REFERENCE_CHANNEL = 17;
};

#endif /* COL_H_ */
