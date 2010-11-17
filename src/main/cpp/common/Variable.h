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
 * File:   Variable.h
 * Author: thomass
 *
 * Created on November 11, 2010, 10:00 AM
 */

#ifndef VARIABLE_H
#define	VARIABLE_H

#include <string>

using std::string;

class Variable {
public:
    virtual string getName() = 0;
    virtual string getUnit() = 0;
    virtual ~Variable() { };

};

#endif	/* VARIABLE_H */

