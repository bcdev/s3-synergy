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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * File:   ModuleException.cpp
 * Author: thomass
 * 
 * Created on January 24, 2011, 3:14 PM
 */

#include "ModuleException.h"

ModuleException::ModuleException() {
}

ModuleException::~ModuleException() throw() {
}

void ModuleException::setMessage(string message) {
    this->message = message.c_str();
}

const char* ModuleException::what() const throw () {
    return message.c_str();
}