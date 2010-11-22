/* 
 * File:   Processor.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "Module.h"

class Processor {
public:

    Processor();
    ~Processor();

    void process(ProcessorContext& context);

};

#endif	/* PROCESSOR_H */

