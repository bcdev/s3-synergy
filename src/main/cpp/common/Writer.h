/* 
 * File:   Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#ifndef WRITER_H
#define	WRITER_H

#include "Module.h"

class Writer : public Module{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual Segment* processSegment(ProcessorContext& context) = 0;
private:

};

#endif	/* WRITER_H */

