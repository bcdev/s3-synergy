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

Col::Col() : BasicModule("COL") {
}

Col::~Col() {
}

void Col::start(Context& context) {
    setUp(context);

    addOlciVariables(context);
    addSlstrVariables(context);
}

void Col::setUp(Context& context) {
    olciSegment = shared_ptr<Segment>(&context.getSegment(Constants::SEGMENT_OLC));
    const Grid& olciGrid = olciSegment->getGrid();

    collocatedSegment = shared_ptr<Segment>(&context.addSegment(Constants::SEGMENT_SYN_COLLOCATED, olciGrid.getSizeL(), olciGrid.getSizeM(), olciGrid.getSizeK(), olciGrid.getMinL(), olciGrid.getMaxL()));
}

void Col::addOlciVariables(Context& context) {
    ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor("SY1");
    // TODO - read mapping from auxiliary data
    const size_t mapping[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 17, 18, 19, 21 };
    for (size_t i = 0; i < 18; i++) {
        const string sourceName = "L_" + lexical_cast<string>(mapping[i]);
        const string targetName = "L_" + lexical_cast<string>(i + 1);
        if (mapping[i] == 17) {
            addVariableAlias(context, targetName, sourceName);
        } else {
            addVariable(productDescriptor, sourceName, targetName, context, Constants::SEGMENT_OLC);
        }
    }

    for (size_t i = 0; i < 18; i++) {
        const string sourceName = "L_" + lexical_cast<string>(mapping[i]) + "_er";
        const string targetName = "L_" + lexical_cast<string>(i + 1) + "_er";
        if (mapping[i] == 17) {
            addVariableAlias(context, targetName, sourceName);
        } else {
            addVariable(productDescriptor, sourceName, targetName, context, Constants::SEGMENT_OLC);
        }
    }

    addVariableAlias(context, "longitude", "longitude");
    addVariableAlias(context, "latitude", "latitude");
    addVariableAlias(context, "altitude", "altitude");
}

void Col::addSlstrVariables(Context& context) {
    ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor("SY1");
    for (size_t i = 1; i < 7; i++) {
        const string sourceName = "L_" + lexical_cast<string>(i);
        const string targetName = "L_" + lexical_cast<string>(i + 18);
        addVariable(productDescriptor, sourceName, targetName, context, Constants::SEGMENT_SLN);
    }
    for (size_t i = 1; i < 7; i++) {
        const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
        const string targetName = "L_" + lexical_cast<string>(i + 18) + "_exception";
        addVariable(productDescriptor, sourceName, targetName, context, Constants::SEGMENT_SLN);
    }
    for (size_t i = 1; i < 7; i++) {
        const string sourceName = "L_" + lexical_cast<string>(i);
        const string targetName = "L_" + lexical_cast<string>(i + 24);
        addVariable(productDescriptor, sourceName, targetName, context, Constants::SEGMENT_SLO);
    }
    for (size_t i = 1; i < 7; i++) {
        const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
        const string targetName = "L_" + lexical_cast<string>(i + 24) + "_exception";
        addVariable(productDescriptor, sourceName, targetName, context, Constants::SEGMENT_SLO);
    }

    addVariable(productDescriptor, SLN_CONFIDENCE_FLAG_VARIABLE, SLN_CONFIDENCE_FLAG_VARIABLE, context, Constants::SEGMENT_SLN);
    addVariable(productDescriptor, SLO_CONFIDENCE_FLAG_VARIABLE, SLO_CONFIDENCE_FLAG_VARIABLE, context, Constants::SEGMENT_SLO);
}

void Col::addVariable(ProductDescriptor& productDescriptor, const string& sourceName, const string& targetName, Context& context, const string& sourceSegmentId) {
    const VariableDescriptor& descriptor = productDescriptor.getSegmentDescriptor(sourceSegmentId).getVariableDescriptor(sourceName);
    context.getLogging()->progress("adding target variable '" + targetName + "' (source name is '" + sourceName + "') to segment '" + Constants::SEGMENT_SYN_COLLOCATED + "'", getId());
    collocatedSegment->addVariable(targetName, descriptor.getType(), descriptor.getScaleFactor(), descriptor.getAddOffset());
    variables[targetName] = sourceName;
    segmentIds[targetName] = sourceSegmentId;
    targetVariables.push_back(targetName);
}

void Col::addVariableAlias(Context& context, const string& targetName, const string& sourceName) {
    context.getLogging()->progress("adding alias '" + targetName + "' to segment '" + Constants::SEGMENT_SYN_COLLOCATED + "'", getId());
    collocatedSegment->addVariableAlias(targetName, *olciSegment, sourceName);
    variables[targetName] = sourceName;
    segmentIds[targetName] = olciSegment->getId();
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

    foreach(string& targetName, targetVariables) {

        context.getLogging()->progress("collocating variable '" + targetName + "'.", getId());
        const string& sourceName = variables[targetName];

        const string& sourceSegmentName = segmentIds[targetName];
        Segment& sourceSegment = context.getSegment(sourceSegmentName);
        const Grid& sourceGrid = sourceSegment.getGrid();
        Accessor& sourceAccessor = sourceSegment.getAccessor(sourceName);

        if(sourceSegmentName.compare(Constants::SEGMENT_OLC) == 0) {
            collocateOlci(sourceAccessor, sourceGrid, targetName);
        } else if (sourceSegmentName.compare(Constants::SEGMENT_SLN) == 0) {
            collocateSln(sourceAccessor, sourceGrid, targetName);
        } else if (sourceSegmentName.compare(Constants::SEGMENT_SLO) == 0) {
            collocateSlo(sourceAccessor, sourceGrid, targetName);
        } else {
            BOOST_THROW_EXCEPTION(logic_error("invalid source segment '" + sourceSegmentName + "'."));
        }
    }
}

void Col::collocateSln(Accessor& sourceAccessor, const Grid& sourceGrid, string& targetName) {

    Accessor& targetAccessor = collocatedSegment->getAccessor(targetName);
    const string& rowVariableName = retrievePositionVariableName(targetName, AXIS_ROW);
    const string& colVariableName = retrievePositionVariableName(targetName, AXIS_COL);

    const Accessor& misregistrationRowAccessor = olciSegment->getAccessor(rowVariableName);
    const Accessor& misregistrationColAccessor = olciSegment->getAccessor(colVariableName);

    const Grid& collocatedGrid = collocatedSegment->getGrid();
    for (size_t k = collocatedGrid.getFirstK(); k < collocatedGrid.getFirstK() + collocatedGrid.getSizeK(); k++) {
        for (size_t l = collocatedGrid.getFirstL(); l < collocatedGrid.getFirstL() + collocatedGrid.getSizeL(); l++) {
            for (size_t m = collocatedGrid.getFirstM(); m < collocatedGrid.getFirstM() + collocatedGrid.getSizeM(); m++) {

                const size_t index = collocatedGrid.getIndex(k, l, m);
                const uint32_t sourceRow = misregistrationRowAccessor.getUInt(index);
                const uint32_t sourceCol = misregistrationColAccessor.getUInt(index);

                // todo replace by correct type
//                int8_t value = sourceAccessor.getByte(sourceGrid.getIndex(0, sourceRow, sourceCol));
//                targetAccessor.setByte(index, value);
            }
        }
    }
}

void Col::collocateSlo(Accessor& sourceAccessor, const Grid& sourceGrid, string& sourceName) {

}

void Col::collocateOlci(Accessor& sourceAccessor, const Grid& sourceGrid, string& sourceName) {

}

string Col::retrievePositionVariableName(const string& targetName, const string& axis) {
    if (targetName.compare(SLO_CONFIDENCE_FLAG_VARIABLE) == 0 || targetName.compare(SLN_CONFIDENCE_FLAG_VARIABLE) == 0) {
        return axis + "_corr_1";
    }
    regex slnMatcher("L_([1][9]|[2][0]|[2][1]|[2][2]|[2][3]|[2][4]).*");
    if(regex_match(targetName, slnMatcher)) {
        size_t index = lexical_cast<size_t>(targetName.substr(2, 2));
        index -= 18;
        return axis + "_corr_" + lexical_cast<string>(index);
    }

    BOOST_THROW_EXCEPTION(logic_error("invalid target variable '" + targetName + "'."));
}
