/*
 * LookupTableReader.h
 *
 *  Created on: Aug 18, 2011
 *      Author: ralf
 */

#ifndef LOOKUPTABLEREADER_H_
#define LOOKUPTABLEREADER_H_

#include "../core/LookupTable.h"

class LookupTableReader {
public:
	LookupTableReader();
	~LookupTableReader();

	LookupTable<float>* readFloat(const string& path, const string& variableName) const;
};

#endif /* LOOKUPTABLEREADER_H_ */
