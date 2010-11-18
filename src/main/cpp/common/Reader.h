/* 
 * File:   Reader.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:26 PM
 */

#ifndef READER_H
#define	READER_H

//#include "Segment.h"

class Segment;

class Reader {
public:

    virtual ~Reader() {
    };

    virtual Segment* getNextSegment() = 0;
};

#endif	/* READER_H */

