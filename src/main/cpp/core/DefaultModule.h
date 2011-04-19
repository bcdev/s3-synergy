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
// rename to BasicModule or AbstractModule (nf-20110419)
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

    // todo - make abstract (nf ...)
    virtual void process(Context& context) {
    };

    virtual size_t getFirstLRequired(const Segment& segment, size_t line) const {
        return line;
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

