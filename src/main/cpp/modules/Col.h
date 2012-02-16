/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
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
 * The SYN L2 collocation module (COL).
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
