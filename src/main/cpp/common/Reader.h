/* 
 * File:   Reader.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:26 PM
 */

#ifndef READER_H
#define	READER_H

#include "Segment.h"

class Reader {
public:

    Reader();
    ~Reader();

    bool hasNextSegment();
    Segment& getNextSegment();
};

#endif	/* READER_H */

