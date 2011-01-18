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

#ifndef DEFAULTMODULE_H
#define	DEFAULTMODULE_H

#include "Context.h"
#include "Module.h"

using std::max;

/**
 * A default module, doing nothing.
 */
class DefaultModule : public Module {
public:

    /**
     * Constructs a new instance of this class.
     * @param moduleId The module ID.
     * @param moduleVersion The module version.
     */
    DefaultModule(const string& moduleId, const string& moduleVersion = "") : id(moduleId), version(moduleVersion) {
    };

    virtual ~DefaultModule() {
    };

    const string& getId() const {
        return id;
    };

    const string& getVersion() const {
        return version;
    }

    virtual void start(Context& context) {
    };

    virtual void stop(Context& context) {
    };

    virtual void process(Context& context) {
    };

    virtual size_t getMinLRequired(const Segment& segment, size_t line) const {
        return line;
    }

protected:

    /**
     * Returns the index of the row where this module shall start the processing
     * of segment data.
     * @param context The context of this module.
     * @param segment The segment data.
     * @return The index of the row where this module shall start the processing
     *         of the {@code segment}.
     */
    size_t getStartL(const Context& context, const Segment& segment) const {
        if (context.hasMaxLComputed(segment, *this)) {
            return max(segment.getGrid().getStartL(),
                    context.getMaxLComputed(segment, *this) + 1);
        } else {
            return segment.getGrid().getStartL();
        }
    }

    /**
     * Returns the index of the row where this module shall stop the processing
     * of segment data.
     * @param startLine The start line to compute the end line for.
     * @param grid The segment's grid.
     * @return The index of the row were this module shall stop the processing
     *         of segment data.
     */
    size_t getDefaultEndL(const size_t startLine, const Grid& grid) const {
        if (startLine + grid.getSizeL() - 1 > grid.getMaxL() - 1) {
            return grid.getMaxL();
        } else {
            return startLine + grid.getSizeL() - 1;
        }
    }

protected:

    template<class K, class V>
    bool contains(const map<K, V>& map, const K& key) const {
        return map.find(key) != map.end();
    }

private:
    const string id;
    const string version;
};

#endif	/* DEFAULTMODULE_H */

