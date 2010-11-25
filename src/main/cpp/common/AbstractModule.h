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
 * File:   AbstractModule.h
 * Author: ralf
 *
 * Created on November 24, 2010, 8:53 AM
 */

#ifndef ABSTRACTMODULE_H
#define	ABSTRACTMODULE_H

#include "Module.h"

class AbstractModule : public Module {
public:
    AbstractModule(const string& moduleId) : id(moduleId), version("1.0-SNAPSHOT") {
    };

    AbstractModule(const string& moduleId, string version) : id(moduleId), version(version) {
    };

    virtual ~AbstractModule() {
    };

    const string& getId() {
        return id;
    };

    const string& getVersion() {
        return version;
    }

    virtual void start() {

    };

    virtual void stop() {
    };

private:
    const string id;
    const string version;
};

#endif	/* ABSTRACTMODULE_H */

