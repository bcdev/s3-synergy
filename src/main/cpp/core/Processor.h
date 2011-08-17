/* 
 * File:   Processor.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "Context.h"
#include "Module.h"
#include "ModuleException.h"

using std::vector;

class Processor {
public:

    Processor();
    ~Processor();

    void process(Context& context);
    bool isCompleted() const;

private:
    void setCompleted(bool completed);
    ModuleException wrapException(exception& e, const string& moduleName, const string& sourceMethod) const;

    bool completed;
};

#endif	/* PROCESSOR_H */

