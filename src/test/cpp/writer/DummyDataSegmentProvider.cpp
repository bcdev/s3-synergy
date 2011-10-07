/*
 * Copyright (C) 2011 by Brockmann Consult (info@brockmann-consult.de)
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
 *
 * File:   DummyDataSegmentProvider.cpp
 * Author: thomasstorm
 *
 * Created on September 29, 2011
 */

#include <algorithm>

#include "../../../main/cpp/util/IOUtils.h"

#include "DummyDataSegmentProvider.h"

using std::min;

DummyDataSegmentProvider::DummyDataSegmentProvider() :
		BasicModule("DUMMY_DATA_SEGMENT_PROVIDER") {
}

DummyDataSegmentProvider::~DummyDataSegmentProvider() {
}

void DummyDataSegmentProvider::start(Context& context) {
	size_t segmentLineCount = 400;
	const string segmentLineCountString = context.getJobOrder().getIpfConfiguration().getDynamicProcessingParameter("Segment_Line_Count");
	if (!segmentLineCountString.empty()) {
		segmentLineCount = lexical_cast<size_t>(segmentLineCountString);
	}
	context.getLogging()->info("segment line count is " + lexical_cast<string>(segmentLineCount), getId());

	vector<SegmentDescriptor*> segmentDescriptors = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptors();

	foreach(const SegmentDescriptor* segDesc, segmentDescriptors)
			{
				vector<VariableDescriptor*> variableDescriptors = segDesc->getVariableDescriptors();

				foreach(VariableDescriptor* varDesc, variableDescriptors)
						{
							const string& segmentName = segDesc->getName();
							if (!context.hasSegment(segmentName)) {
								valarray<size_t> dimensionSizes = IOUtils::getDimensionSizes(varDesc);
								context.addSegment(segmentName, min(segmentLineCount, dimensionSizes[1]), dimensionSizes[2], dimensionSizes[0], 0, dimensionSizes[1] - 1);
							}
							Segment& segment = context.getSegment(segmentName);
							if (!segment.hasVariable(varDesc->getName())) {
								segment.addVariable(*varDesc);
							}
						}
			}
}

void DummyDataSegmentProvider::stop(Context& context) {
	reverse_foreach(const string id, context.getSegmentIds()) {
	    context.removeSegment(id);
	}
}

void DummyDataSegmentProvider::process(Context& context) {
	vector<SegmentDescriptor*> segmentDescriptors = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptors();
	foreach(const SegmentDescriptor* segDesc, segmentDescriptors) {
	    Segment& segment = context.getSegment(segDesc->getName());
	    foreach(VariableDescriptor* vd, segDesc->getVariableDescriptors()) {
	        Accessor& acc = segment.getAccessor(vd->getName());
	        const Grid& grid = segment.getGrid();
	        for(long k = grid.getFirstK(); k < grid.getSizeK(); k++) {
	            for(long l = grid.getFirstL(); l < min(grid.getSizeL(), grid.getMaxL()); l++) {
	                for(long m = grid.getFirstM(); m < grid.getSizeM(); m++) {
	                    size_t index = grid.getIndex(k, l, m);
	                    int type = acc.getType();
	                    switch(type) {
	                        case Constants::TYPE_BYTE: {
	                            acc.setByte(index, m % 127);
	                            break;
	                        }
                            case Constants::TYPE_UBYTE: {
                                acc.setUByte(index, m % 255);
                                break;
                            }
                            case Constants::TYPE_CHAR: {
                                acc.setFillValue(index);
                                break;
                            }
                            case Constants::TYPE_SHORT: {
                                acc.setShort(index, m % 32767);
                                break;
                            }
                            case Constants::TYPE_USHORT: {
                                acc.setUShort(index, m % 65535);
                                break;
                            }
                            case Constants::TYPE_INT: {
                                acc.setInt(index, m % 65535);
                                break;
                            }
                            case Constants::TYPE_UINT: {
                                acc.setUInt(index, m % 65535);
                                break;
                            }
                            case Constants::TYPE_LONG: {
                                acc.setLong(index, m % 65535);
                                break;
                            }
                            case Constants::TYPE_ULONG: {
                                acc.setULong(index, m % 65535);
                                break;
                            }
                            case Constants::TYPE_FLOAT: {
                                acc.setFloat(index, m % 65535);
                                break;
                            }
                            case Constants::TYPE_DOUBLE: {
                                acc.setDouble(index, m % 65535);
                                break;
                            }
	                    }
	                }
	            }
	        }
	    }
	    context.setLastComputedL(segment, *this, segment.getGrid().getLastL());
	}
}
