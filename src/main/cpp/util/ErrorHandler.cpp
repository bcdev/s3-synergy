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
 * File:   ErrorHandler.cpp
 * Author: thomass
 *
 * Created on January 13, 2010, 5:17 PM
 */

#include "../core/Boost.h"
#include "ErrorHandler.h"
#include "../core/JobOrder.h"
#include "Logger.h"
#include "../core/SynException.h"

ErrorHandler::ErrorHandler() {
}

ErrorHandler::~ErrorHandler() {
}

void ErrorHandler::handleError(Context& context, SynException& e) const {
//    string diagnostics = boost::diagnostic_information(e);
    context.getLogging()->error(e.getMessage(), e.getModuleId(), context.getJobOrder()->getConfig().getVersion());
}