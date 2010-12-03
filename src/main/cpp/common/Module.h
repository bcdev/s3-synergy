/* 
 * File:   Module.h
 * Author: thomass
 *
 * Created on November 17, 2010, 3:35 PM
 */

#ifndef MODULE_H
#define	MODULE_H

#include "Segment.h"

class ProcessorContext;

class Module {
public:

    virtual ~Module() {
    };

    virtual const string& getModuleId() const = 0;
    virtual const string& getVersion() const = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    
    /**
     * Returns the minimum line required for computing a segment starting
     * with the line given as argument.
     * @param line The line.
     * @return the minimum line required for computing a segment starting
     * with {@code line}.
     */
    virtual size_t getMinLineRequired(size_t line) const = 0;
    virtual Segment* processSegment(ProcessorContext& context) = 0;
};

#endif	/* MODULE_H */

