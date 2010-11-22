/* 
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include "Writer.h"


class SynL2Writer : public Writer {
public:
    SynL2Writer();
    ~SynL2Writer();
    void start();
    void stop();
    Segment* processSegment(ProcessorContext& context);
};

#endif	/* SYNL2WRITER_H */

