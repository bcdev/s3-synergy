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
 * File:   ErrorHandler.h
 * Author: thomass
 *
 * Created on January 13, 2010, 5:17 PM
 */

#ifndef ERRORHANDLER_H
#define	ERRORHANDLER_H

#include <stdexcept>
#include <string>

#include "../core/Context.h"

using std::exception;
using std::string;

class Context;

class ErrorHandler {
public:
    ErrorHandler();
    ~ErrorHandler();
    void handleError(Context& context, exception &e) const;
};

#endif	/* ERRORHANDLER_H */

