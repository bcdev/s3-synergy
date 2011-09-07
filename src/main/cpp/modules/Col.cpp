/*
 * Col.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#include "Col.h"

Col::Col() :
		BasicModule("COL") {
}

Col::~Col() {
}

void Col::start(Context& context) {
	const Segment& s = context.getSegment(Constants::SEGMENT_OLC);
	const Grid& g = s.getGrid();

	Segment& t = context.addSegment(Constants::SEGMENT_SYN_COLLOCATED,
			g.getSizeL(), g.getSizeM(), g.getSizeK(), g.getMinL(), g.getMaxL());

	// TODO - read mapping from auxiliary data
	const size_t mapping[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16,
			17, 18, 19, 21 };

	for (size_t i = 0; i < 18; i++) {
		const string sourceName = "L_" + lexical_cast<string>(mapping[i]);
		const string targetName = "L_" + lexical_cast<string>(i + 1);
		context.getLogging()->info(
				"adding alias '" + targetName + "' to segment '" + t.getId()
						+ "'", getId());
		t.addVariableAlias(targetName, s, sourceName);
	}
	for (size_t i = 0; i < 18; i++) {
		const string sourceName = "L_" + lexical_cast<string>(mapping[i])
				+ "_er";
		const string targetName = "L_" + lexical_cast<string>(i + 1) + "_er";
		context.getLogging()->info(
				"adding alias '" + targetName + "' to segment '" + t.getId()
						+ "'", getId());
		t.addVariableAlias(targetName, s, sourceName);
	}
	for (size_t i = 0; i < 18; i++) {
		if (s.hasVariable("delta_x_" + lexical_cast<string>(mapping[i]))) {
			const string sourceName = "delta_x_"
					+ lexical_cast<string>(mapping[i]);
			const string targetName = "delta_x_" + lexical_cast<string>(i + 1);
			context.getLogging()->info(
					"adding alias '" + targetName + "' to segment '" + t.getId()
							+ "'", getId());
			t.addVariableAlias(targetName, s, sourceName);
			t.addVariableAlias("delta_y_" + lexical_cast<string>(i + 1), s,
					"delta_y_" + lexical_cast<string>(mapping[i]));
		}
	}
	for (size_t i = 0; i < 18; i++) {
		if (s.hasVariable("delta_y_" + lexical_cast<string>(mapping[i]))) {
			const string sourceName = "delta_y_"
					+ lexical_cast<string>(mapping[i]);
			const string targetName = "delta_y_" + lexical_cast<string>(i + 1);
			context.getLogging()->info(
					"adding alias '" + targetName + "' to segment '" + t.getId()
							+ "'", getId());
			t.addVariableAlias(targetName, s, sourceName);
		}
	}
	context.getLogging()->info(
			"adding alias 'longitude' to segment '" + t.getId() + "'", getId());
	t.addVariableAlias("longitude", s, "longitude");

	context.getLogging()->info(
			"adding alias 'latitude' to segment '" + t.getId() + "'", getId());
	t.addVariableAlias("latitude", s, "latitude");

	context.getLogging()->info(
			"adding alias 'altitude' to segment '" + t.getId() + "'", getId());
	t.addVariableAlias("altitude", s, "altitude");

	context.getLogging()->info(
			"adding alias 'OLC_flags' to segment '" + t.getId() + "'", getId());
	t.addVariableAlias("OLC_flags", s, "OLC_flags");
}

void Col::stop(Context& context) {
}

void Col::process(Context& context) {
}

