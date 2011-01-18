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
 * File:   SynException.h
 * Author: thomass
 *
 * Created on January 18, 2011, 1:34 PM
 */

#ifndef SYNEXCEPTION_H
#define	SYNEXCEPTION_H

#include <stdexcept>
#include <string>

#include "Boost.h"

using std::string;

class SynException : public std::exception, public boost::exception {
public:
    SynException(const string& message, const string& moduleId);
    virtual ~SynException() throw();

    string& getMessage();
    string& getModuleId();
private:
    string message;
    string moduleId;
};

#endif	/* SYNEXCEPTION_H */

