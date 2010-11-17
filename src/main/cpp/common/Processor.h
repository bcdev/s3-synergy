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

    void process(Segment& segment);
    void addModule(Module& module);

private:
    vector<Module*> modules;
};

#endif	/* PROCESSOR_H */

