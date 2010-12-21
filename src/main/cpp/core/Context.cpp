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
 * File:   Context.cpp
 * Author: ralf
 * 
 * Created on December 20, 2010, 12:34 PM
 */

#include "Context.h"

Context::Context() : moduleList(), objectMap(), segmentMap(), segmentList() {
    dictionary = 0;
    jobOrder = 0;
    logging = 0;
}

Context::~Context() {
}

void Context::addModule(Module& module) {
    moduleList.push_back(&module);
}

void Context::addObject(Object& object) throw (logic_error) {
    if (hasObject(object.getId())) {
        throw logic_error("an object with '" + object.getId() + "' has already been added to the context.");
    }
    objectMap[object.getId()] = &object;
}

Dictionary* Context::getDictionary() const {
    return dictionary;
}

JobOrder* Context::getJobOrder() const {
    return jobOrder;
}

Logging* Context::getLogging() const {
    return logging;
}

vector<Module*> Context::getModules() const {
    return moduleList;
}

inline
bool Context::hasObject(const string& id) const {
    return objectMap.find(id) != objectMap.end();
}