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
	LookupTableReader(const string& path);
	~LookupTableReader();

	template<class W>
	LookupTable<W>* readLookupTable(const string& variableName) const;

private:
	int fileId;
};

#endif /* LOOKUPTABLEREADER_H_ */
