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
 * File:   VariableImpl.h
 * Author: thomass
 *
 * Created on November 11, 2010, 11:33 AM
 */

#ifndef VARIABLEIMPL_H
#define	VARIABLEIMPL_H

#include "Variable.h"
#include <string>

class VariableImpl : public Variable {
public:
    VariableImpl();
    VariableImpl(const VariableImpl& orig);
    virtual ~VariableImpl();

    std::string getName();
    std::string getUnit();

};

#endif	/* VARIABLEIMPL_H */

