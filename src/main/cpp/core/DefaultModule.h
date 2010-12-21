/* 
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 * File:   DefaultModule.h
 * Author: ralf
 *
 * Created on December 21, 2010, 12:24 PM
 */

#ifndef ABSTRACTMODULE_H
#define	ABSTRACTMODULE_H

#include "Context.h"
#include "Module.h"

using std::max;

/**
 * A default module, doing nothing.
 */
class DefaultModule : public virtual Module {
public:

    /**
     * Constructs a new instance of this class.
     * @param moduleId The module ID.
     * @param moduleVersion The module version.
     */
    DefaultModule(const string& moduleId, string moduleVersion = "") : id(moduleId), version(moduleVersion) {
    };

    virtual ~DefaultModule() {
    };

    const string& getId() const {
        return id;
    };

    const string& getVersion() const {
        return version;
    }

    virtual void start(Context& constext) {
    };

    virtual void stop(Context& constext) {
    };

    virtual void process(Context& constext) {
    };

    virtual size_t getMinLRequired(size_t line) const {
        return line;
    }

protected:

    /**
     * Returns the index of the row where this module shall start the processing
     * of segment data.
     * @param context The context of this module.
     * @param segment The segment data.
     * @return the index of the row where this module shall start the processing
     *         of the {@code segment}.
     */
    size_t getStartL(Context& context, Segment& segment) const {
        if (context.hasMaxLComputed(segment, *this)) {
            return max(segment.getGrid().getStartL(),
                    context.getMaxLComputed(segment, *this) + 1);
        } else {
            return segment.getGrid().getStartL();
        }
    }

private:
    const string id;
    const string version;
};

#endif	/* ABSTRACTMODULE_H */

