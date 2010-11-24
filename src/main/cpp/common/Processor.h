/* 
 * File:   Processor.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include <vector>

#include "Module.h"

using std::vector;

class Processor {
public:

    Processor();
    ~Processor();

    void addModule(Module& module);
    void process(ProcessorContext& context);
    bool isCompleted() const;

private:
    void setCompleted(bool completed);
    
    bool completed;
    vector<Module*> modules;
};

#endif	/* PROCESSOR_H */

