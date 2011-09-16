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
    const Segment& olciSegment = context.getSegment(Constants::SEGMENT_OLC);
    const Grid& olciGrid = olciSegment.getGrid();

    collocatedSegment = shared_ptr<Segment>(&context.addSegment(Constants::SEGMENT_SYN_COLLOCATED, olciGrid.getSizeL(), olciGrid.getSizeM(), olciGrid.getSizeK(), olciGrid.getMinL(), olciGrid.getMaxL()));

    addOlciVariables(context, *collocatedSegment, olciSegment);
    addSlstrVariables(context, *collocatedSegment);
}

void Col::addOlciVariables(Context& context, Segment& collocatedSegment, const Segment& olciSegment) {
    ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor("SY1");
    // TODO - read mapping from auxiliary data
    const size_t mapping[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 17, 18, 19, 21 };
    for (size_t i = 0; i < 18; i++) {
        const string sourceName = "L_" + lexical_cast<string>(mapping[i]);
        const string targetName = "L_" + lexical_cast<string>(i + 1);
        if (mapping[i] == 17) {
            addVariableAlias(context, targetName, collocatedSegment, olciSegment, sourceName);
        } else {
            addVariable(productDescriptor, sourceName, targetName, context, collocatedSegment);
        }
    }
    for (size_t i = 0; i < 18; i++) {
        const string sourceName = "L_" + lexical_cast<string>(mapping[i]) + "_er";
        const string targetName = "L_" + lexical_cast<string>(i + 1) + "_er";
        if (mapping[i] == 17) {
            addVariableAlias(context, targetName, collocatedSegment, olciSegment, sourceName);
        } else {
            addVariable(productDescriptor, sourceName, targetName, context, collocatedSegment);
        }
    }
    context.getLogging()->info("adding alias 'longitude' to segment '" + collocatedSegment.getId() + "'", getId());
    collocatedSegment.addVariableAlias("longitude", olciSegment, "longitude");
    context.getLogging()->info("adding alias 'latitude' to segment '" + collocatedSegment.getId() + "'", getId());
    collocatedSegment.addVariableAlias("latitude", olciSegment, "latitude");
    context.getLogging()->info("adding alias 'altitude' to segment '" + collocatedSegment.getId() + "'", getId());
    collocatedSegment.addVariableAlias("altitude", olciSegment, "altitude");
}

void Col::addSlstrVariables(Context& context, Segment& collocatedSegment) {
    ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor("SY1");
    for (size_t i = 1; i < 7; i++) {
        const string sourceName = "L_" + lexical_cast<string>(i);
        const string targetName = "L_" + lexical_cast<string>(i + 18);
        addVariable(productDescriptor, sourceName, targetName, context, collocatedSegment);
    }
    for (size_t i = 1; i < 7; i++) {
        const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
        const string targetName = "L_" + lexical_cast<string>(i + 18) + "_exception";
        addVariable(productDescriptor, sourceName, targetName, context, collocatedSegment);
    }
    for (size_t i = 1; i < 7; i++) {
        const string sourceName = "L_" + lexical_cast<string>(i);
        const string targetName = "L_" + lexical_cast<string>(i + 24);
        addVariable(productDescriptor, sourceName, targetName, context, collocatedSegment);
    }
    for (size_t i = 1; i < 7; i++) {
        const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
        const string targetName = "L_" + lexical_cast<string>(i + 24) + "_exception";
        addVariable(productDescriptor, sourceName, targetName, context, collocatedSegment);
    }

    addVariable(productDescriptor, "SLN_confidence", "SLN_confidence", context, collocatedSegment);
    addVariable(productDescriptor, "SLO_confidence", "SLO_confidence", context, collocatedSegment);
}

void Col::addVariable(ProductDescriptor& productDescriptor, const string& sourceName, const string& targetName, Context& context, Segment& collocatedSegment) {
    const VariableDescriptor *descriptor = productDescriptor.getVariableDescriptor(sourceName);
    context.getLogging()->info("adding variable '" + targetName + "' to segment '" + collocatedSegment.getId() + "'", getId());
    collocatedSegment.addVariable(targetName, descriptor->getType(), descriptor->getScaleFactor(), descriptor->getAddOffset());
}

void Col::addVariableAlias(Context & context, const string & targetName, Segment & collocatedSegment, const Segment & olciSegment, const string & sourceName) {
    context.getLogging()->info("adding alias '" + targetName + "' to segment '" + collocatedSegment.getId() + "'", getId());
    collocatedSegment.addVariableAlias(targetName, olciSegment, sourceName);
}

void Col::stop(Context& context) {
}

void Col::process(Context& context) {

    const Grid& collocatedGrid = collocatedSegment->getGrid();
    for (size_t k = collocatedGrid.getFirstK(); k < collocatedGrid.getFirstK() + collocatedGrid.getSizeK(); k++) {
        for (size_t l = collocatedGrid.getFirstL(); l < collocatedGrid.getFirstL() + collocatedGrid.getSizeL(); l++) {
            for (size_t m = collocatedGrid.getFirstM(); m < collocatedGrid.getFirstM() + collocatedGrid.getSizeM(); m++) {

                /*
                 * for each variable v in collocatedSegment:
                 *
                 *      get corresponding misregistration accessor m
                 *      get accessor n for v from non-collocated segment (OLCI, SLN, SLO)
                 *      get value pos from m for index k, l, m
                 *      get value val from n for position pos
                 *
                 *      get accessor c for v from collocated segment
                 *      set c at position pos to val
                 */

            }
        }
    }
}

