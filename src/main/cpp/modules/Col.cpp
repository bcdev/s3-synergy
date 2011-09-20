/*
 * Col.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#include "Col.h"

const string Col::SLO_CONFIDENCE_FLAG_VARIABLE = "SLO_confidence";
const string Col::SLN_CONFIDENCE_FLAG_VARIABLE = "SLN_confidence";
const string Col::AXIS_ROW = "y";
const string Col::AXIS_COL = "x";

Col::Col() :
		BasicModule("COL") {
}

Col::~Col() {
}

void Col::start(Context& context) {
	addTargetSegment(context);

	addOlciVariables(context);
	addSlstrVariables(context);
}

void Col::addTargetSegment(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_OLC);
	const Grid& g = s.getGrid();

	context.addSegment(Constants::SEGMENT_SYN_COLLOCATED, g.getSizeL(), g.getSizeM(), g.getSizeK(), g.getMinL(), g.getMaxL());
}

void Col::addOlciVariables(Context& context) {
	const ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor("SY1");
	// TODO - read mapping from auxiliary data
	const size_t mapping[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 17, 18, 19, 21 };
	for (size_t i = 0; i < 18; i++) {
		const string sourceName = "L_" + lexical_cast<string>(mapping[i]);
		const string targetName = "L_" + lexical_cast<string>(i + 1);
		if (mapping[i] == 17) {
			addVariableAlias(context, targetName, sourceName);
		} else {
			addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_OLC);
		}
	}

	for (size_t i = 0; i < 18; i++) {
		const string sourceName = "L_" + lexical_cast<string>(mapping[i]) + "_er";
		const string targetName = "L_" + lexical_cast<string>(i + 1) + "_er";
		if (mapping[i] == 17) {
			addVariableAlias(context, targetName, sourceName);
		} else {
			addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_OLC);
		}
	}

	addVariableAlias(context, "longitude", "longitude");
	addVariableAlias(context, "latitude", "latitude");
	addVariableAlias(context, "altitude", "altitude");
}

void Col::addSlstrVariables(Context& context) {
	const ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor("SY1");
	for (size_t i = 1; i < 7; i++) {
		const string sourceName = "L_" + lexical_cast<string>(i);
		const string targetName = "L_" + lexical_cast<string>(i + 18);
		addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_SLN);
	}
	for (size_t i = 1; i < 7; i++) {
		const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
		const string targetName = "L_" + lexical_cast<string>(i + 18) + "_exception";
		addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_SLN);
	}
	for (size_t i = 1; i < 7; i++) {
		const string sourceName = "L_" + lexical_cast<string>(i);
		const string targetName = "L_" + lexical_cast<string>(i + 24);
		addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_SLO);
	}
	for (size_t i = 1; i < 7; i++) {
		const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
		const string targetName = "L_" + lexical_cast<string>(i + 24) + "_exception";
		addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_SLO);
	}

	addVariable(context, productDescriptor, SLN_CONFIDENCE_FLAG_VARIABLE, SLN_CONFIDENCE_FLAG_VARIABLE, Constants::SEGMENT_SLN);
	addVariable(context, productDescriptor, SLO_CONFIDENCE_FLAG_VARIABLE, SLO_CONFIDENCE_FLAG_VARIABLE, Constants::SEGMENT_SLO);
}

void Col::addVariable(Context& context, const ProductDescriptor& productDescriptor, const string& sourceName, const string& targetName, const string& sourceSegmentId) {
	const VariableDescriptor& descriptor = productDescriptor.getSegmentDescriptor(sourceSegmentId).getVariableDescriptor(sourceName);
	Segment& s = context.getSegment(sourceSegmentId);
	Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	context.getLogging()->progress("adding variable '" + targetName + "' to segment '" + t.getId() + "'", getId());
	t.addVariable(targetName, descriptor.getType(), descriptor.getScaleFactor(), descriptor.getAddOffset());
	variables[targetName] = sourceName;
	segmentIds[targetName] = sourceSegmentId;
	targetVariables.push_back(targetName);
}

void Col::addVariableAlias(Context& context, const string& targetName, const string& sourceName) {
	Segment& s = context.getSegment(Constants::SEGMENT_OLC);
	Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	context.getLogging()->progress("adding alias '" + targetName + "' to segment '" + t.getId() + "'", getId());
	t.addVariableAlias(targetName, s, sourceName);
	variables[targetName] = sourceName;
	segmentIds[targetName] = s.getId();
	targetVariables.push_back(targetName);
}

void Col::stop(Context& context) {
}

void Col::process(Context& context) {

	/*
	 * for each variable v in collocatedSegment:
	 *
	 *      get corresponding misregistration accessors row, col
	 *      get row, col for index k, l, m
	 *
	 *      get accessor n for v from non-collocated segment (OLCI, SLN, SLO)
	 *      get value val from n for position row, col
	 *
	 *      get accessor c for v from collocated segment
	 *      set c at position k, l, m to val
	 */

	foreach(string& targetName, targetVariables)
			{

				context.getLogging()->progress("collocating variable '" + targetName + "'.", getId());
				const string& sourceVariableName = variables[targetName];

				const string& sourceSegmentName = segmentIds[targetName];
				Segment& sourceSegment = context.getSegment(sourceSegmentName);
				const Grid& sourceGrid = sourceSegment.getGrid();
				Accessor& sourceAccessor = sourceSegment.getAccessor(sourceVariableName);
				const int sourceType =
						context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SY1).getSegmentDescriptor(sourceSegmentName).getVariableDescriptor(sourceVariableName).getType();

				if (sourceSegmentName.compare(Constants::SEGMENT_OLC) == 0) {
					collocateOlci(context, sourceAccessor, sourceType, targetName);
				} else if (sourceSegmentName.compare(Constants::SEGMENT_SLN) == 0 || sourceSegmentName.compare(Constants::SEGMENT_SLO) == 0) {
					collocateSlstr(context, sourceAccessor, sourceType, sourceGrid, targetName);
				} else {
					BOOST_THROW_EXCEPTION(logic_error("invalid source segment '" + sourceSegmentName + "'."));
				}
			}
}

void Col::collocateSlstr(Context& context, Accessor& sourceAccessor, const int sourceType, const Grid& sourceGrid, string& targetName) {
	Segment& s = context.getSegment(Constants::SEGMENT_OLC);
	Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);

	Accessor& targetAccessor = t.getAccessor(targetName);
	const string& rowVariableName = retrievePositionVariableName(targetName, AXIS_ROW);
	const string& colVariableName = retrievePositionVariableName(targetName, AXIS_COL);

	const Accessor& misregistrationRowAccessor = s.getAccessor(rowVariableName);
	const Accessor& misregistrationColAccessor = s.getAccessor(colVariableName);
	const double rowAddOffset = misregistrationRowAccessor.getAddOffset();
	const double colAddOffset = misregistrationColAccessor.getAddOffset();
	const double rowScaleFactor = misregistrationRowAccessor.getScaleFactor();
	const double colScaleFactor = misregistrationColAccessor.getScaleFactor();

	const Grid& collocatedGrid = t.getGrid();
	for (size_t k = collocatedGrid.getFirstK(); k < collocatedGrid.getFirstK() + collocatedGrid.getSizeK(); k++) {
		for (size_t l = collocatedGrid.getFirstL(); l < collocatedGrid.getFirstL() + collocatedGrid.getSizeL(); l++) {
			for (size_t m = collocatedGrid.getFirstM(); m < collocatedGrid.getFirstM() + collocatedGrid.getSizeM(); m++) {

				const size_t index = collocatedGrid.getIndex(k, l, m);
				const uint32_t unscaledRowIndex = misregistrationRowAccessor.getUInt(index);
				const uint32_t unscaledColIndex = misregistrationColAccessor.getUInt(index);
				const int64_t sourceRow = floor(unscaledRowIndex * rowScaleFactor + rowAddOffset + 0.5);
				const int64_t sourceCol = floor(unscaledColIndex * colScaleFactor + colAddOffset + 0.5);

				// todo - ts19sep11 - get fill value from product
				if (sourceRow < 0 || sourceCol < 0) {
					context.getLogging()->debug("no collocation possible for position (k,l,m): (" + lexical_cast<string>(k) + "," + lexical_cast<string>(l) + "," + lexical_cast<string>(m) + ")",
							getId());
					continue;
				}

//                try {

				switch (sourceType) {
				case Constants::TYPE_BYTE: {
					int8_t value = sourceAccessor.getByte(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setByte(index, value);
					break;
				}
				case Constants::TYPE_UBYTE: {
					uint8_t value = sourceAccessor.getUByte(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setUByte(index, value);
					break;
				}
				case Constants::TYPE_SHORT: {
					int16_t value = sourceAccessor.getShort(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setShort(index, value);
					break;
				}
				case Constants::TYPE_USHORT: {
					uint16_t value = sourceAccessor.getUShort(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setUShort(index, value);
					break;
				}
				case Constants::TYPE_INT: {
					int32_t value = sourceAccessor.getInt(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setInt(index, value);
					break;
				}
				case Constants::TYPE_UINT: {
					uint32_t value = sourceAccessor.getUInt(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setUInt(index, value);
					break;
				}
				case Constants::TYPE_LONG: {
					int64_t value = sourceAccessor.getLong(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setLong(index, value);
					break;
				}
				case Constants::TYPE_ULONG: {
					uint64_t value = sourceAccessor.getULong(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setULong(index, value);
					break;
				}
				case Constants::TYPE_FLOAT: {
					float value = sourceAccessor.getFloat(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setFloat(index, value);
					break;
				}
				case Constants::TYPE_DOUBLE: {
					double value = sourceAccessor.getDouble(sourceGrid.getIndex(0, sourceRow, sourceCol));
					targetAccessor.setDouble(index, value);
					break;
				}
				default:
					BOOST_THROW_EXCEPTION(std::invalid_argument("Error collocating variable '" + targetName + "': Unsupported data type."));
					break;
				}

//                } catch(out_of_range) {
//                    std::cout << "\nunscaledRowIndex=" + lexical_cast<string>(unscaledRowIndex) + "\n";
//                    std::cout << "scaledRowIndex=" + lexical_cast<string>(sourceRow) + "\n";
//                    std::cout << "\nunscaledColIndex=" + lexical_cast<string>(unscaledColIndex) + "\n";
//                    std::cout << "scaledColIndex=" + lexical_cast<string>(sourceCol) + "\n";
//                    std::cout << "k=" + lexical_cast<string>(k) + "\n";
//                    std::cout << "l=" + lexical_cast<string>(l) + "\n";
//                    std::cout << "m=" + lexical_cast<string>(m) + "\n";
//                    exit(1);
//                }
			}
		}
	}
}

void Col::collocateOlci(Context& context, Accessor& sourceAccessor, const int sourceType, string& targetName) {
	Segment& s = context.getSegment(Constants::SEGMENT_OLC);
	Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	Accessor& targetAccessor = t.getAccessor(targetName);
	const string& rowVariableName = retrieveDeltaVariableName(targetName, AXIS_ROW);
	const string& colVariableName = retrieveDeltaVariableName(targetName, AXIS_COL);

	const Accessor& misregistrationRowAccessor = s.getAccessor(rowVariableName);
	const Accessor& misregistrationColAccessor = s.getAccessor(colVariableName);
	const double rowAddOffset = misregistrationRowAccessor.getAddOffset();
	const double colAddOffset = misregistrationColAccessor.getAddOffset();
	const double rowScaleFactor = misregistrationRowAccessor.getScaleFactor();
	const double colScaleFactor = misregistrationColAccessor.getScaleFactor();

	const Grid& collocatedGrid = t.getGrid();
	for (size_t k = collocatedGrid.getFirstK(); k < collocatedGrid.getFirstK() + collocatedGrid.getSizeK(); k++) {
		for (size_t l = collocatedGrid.getFirstL(); l < collocatedGrid.getFirstL() + collocatedGrid.getSizeL(); l++) {
			for (size_t m = collocatedGrid.getFirstM(); m < collocatedGrid.getFirstM() + collocatedGrid.getSizeM(); m++) {

				const size_t index = collocatedGrid.getIndex(k, l, m);
				const int16_t unscaledRowIndex = misregistrationRowAccessor.getShort(index);
				const int16_t unscaledColIndex = misregistrationColAccessor.getShort(index);

				// todo - ts19sep11 - get fill value from product
				if (unscaledRowIndex == -32768 || unscaledColIndex == -32768) {
					context.getLogging()->debug("no collocation possible for position (k,l,m): (" + lexical_cast<string>(k) + "," + lexical_cast<string>(l) + "," + lexical_cast<string>(m) + ")",
							getId());
					continue;
				}

				const size_t sourceRowIndex = floor(index - unscaledRowIndex * rowScaleFactor + rowAddOffset + 0.5);
				const size_t sourceColIndex = floor(index - unscaledColIndex * colScaleFactor + colAddOffset + 0.5);

				const Grid& sourceGrid = s.getGrid();
//                try {

				switch (sourceType) {
				case Constants::TYPE_BYTE: {
					int8_t value = sourceAccessor.getByte(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setByte(index, value);
					break;
				}
				case Constants::TYPE_UBYTE: {
					uint8_t value = sourceAccessor.getUByte(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setUByte(index, value);
					break;
				}
				case Constants::TYPE_SHORT: {
					int16_t value = sourceAccessor.getShort(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setShort(index, value);
					break;
				}
				case Constants::TYPE_USHORT: {
					uint16_t value = sourceAccessor.getUShort(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setUShort(index, value);
					break;
				}
				case Constants::TYPE_INT: {
					int32_t value = sourceAccessor.getInt(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setInt(index, value);
					break;
				}
				case Constants::TYPE_UINT: {
					uint32_t value = sourceAccessor.getUInt(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setUInt(index, value);
					break;
				}
				case Constants::TYPE_LONG: {
					int64_t value = sourceAccessor.getLong(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setLong(index, value);
					break;
				}
				case Constants::TYPE_ULONG: {
					uint64_t value = sourceAccessor.getULong(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setULong(index, value);
					break;
				}
				case Constants::TYPE_FLOAT: {
					float value = sourceAccessor.getFloat(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setFloat(index, value);
					break;
				}
				case Constants::TYPE_DOUBLE: {
					double value = sourceAccessor.getDouble(sourceGrid.getIndex(0, sourceRowIndex, sourceColIndex));
					targetAccessor.setDouble(index, value);
					break;
				}
				default:
					BOOST_THROW_EXCEPTION(std::invalid_argument("Error collocating variable '" + targetName + "': Unsupported data type."));
					break;
				}

//                } catch(out_of_range) {
//                    std::cout << "\nunscaledRowIndex=" + lexical_cast<string>(unscaledRowIndex) + "\n";
//                    std::cout << "scaledRowIndex=" + lexical_cast<string>(sourceRow) + "\n";
//                    std::cout << "\nunscaledColIndex=" + lexical_cast<string>(unscaledColIndex) + "\n";
//                    std::cout << "scaledColIndex=" + lexical_cast<string>(sourceCol) + "\n";
//                    std::cout << "k=" + lexical_cast<string>(k) + "\n";
//                    std::cout << "l=" + lexical_cast<string>(l) + "\n";
//                    std::cout << "m=" + lexical_cast<string>(m) + "\n";
//                    exit(1);
//                }
			}
		}
	}
}

string Col::retrievePositionVariableName(const string& targetName, const string& axis) {
	regex sloMatcher("L_([2][5]|[2][6]|[2][7]|[2][8]|[2][9]|[3][0]).*");
	if (regex_match(targetName, sloMatcher) || targetName.compare(SLO_CONFIDENCE_FLAG_VARIABLE) == 0) {
		return axis + "_corr_o";
	}
	if (targetName.compare(SLN_CONFIDENCE_FLAG_VARIABLE) == 0) {
		return axis + "_corr_1";
	}
	regex slnMatcher("L_([1][9]|[2][0]|[2][1]|[2][2]|[2][3]|[2][4]).*");
	if (regex_match(targetName, slnMatcher)) {
		size_t index = lexical_cast<size_t>(targetName.substr(2, 2));
		index -= 18;
		return axis + "_corr_" + lexical_cast<string>(index);
	}

	BOOST_THROW_EXCEPTION(logic_error("invalid target variable '" + targetName + "'."));
}

string Col::retrieveDeltaVariableName(const string& targetName, const string& axis) {
	if (targetName.compare("OLC_flags") == 0 || targetName.compare("longitude") == 0 || targetName.compare("latitude") == 0 || targetName.compare("altitude") == 0) {
		return "delta_" + axis + "_1";
	}
	regex secondMatcher("L_[10-18].*");
	if (regex_match(targetName, secondMatcher)) {
		size_t index = lexical_cast<size_t>(targetName.substr(2, 2));
		return "delta_" + axis + "_" + lexical_cast<string>(index);
	}
	regex firstMatcher("L_[1-9].*");
	if (regex_match(targetName, firstMatcher)) {
		size_t index = lexical_cast<size_t>(targetName.substr(2, 1));
		return "delta_" + axis + "_" + lexical_cast<string>(index);
	}

	BOOST_THROW_EXCEPTION(logic_error("invalid target variable '" + targetName + "'."));
}
