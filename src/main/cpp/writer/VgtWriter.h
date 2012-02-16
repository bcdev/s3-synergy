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

#ifndef VGTWRITER_H
#define	VGTWRITER_H

#include "../writer/AbstractWriter.h"

/**
 * The VGT writer module.
 */
class VgtWriter: public AbstractWriter {
public:
    VgtWriter(const string& productId = Constants::PRODUCT_VGP);
    ~VgtWriter();

protected:
	vector<SegmentDescriptor*> getSegmentDescriptors(const ProductDescriptor& productDescriptor) const;
    void defineCoordinateVariables(const Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const;
    void writeCoordinateVariables(Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const;

private:
	friend class VgtWriterTest;

	vector<VariableDescriptor*> getCoordinateVariableDescriptors(const ProductDescriptor& productDescriptor) const;
	vector<VariableDescriptor*> getTiePointCoordinateVariableDescriptors(const ProductDescriptor& productDescriptor) const;

	static bool isCoordinateSegment(const string& segmentName);
	static bool isTiePointCoordinateSegment(const string& segmentName);
	static bool isTiePointSegment(const string& segmentName);
};

#endif	/* VGTWRITER_H */

