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
 * File:   DebugLogging.h
 * Author: thomass
 *
 * Created on September 26, 2011
 */

#ifndef DEBUGLOGGING_H
#define	DEBUGLOGGING_H

#include "InfoLogging.h"

class DebugLogging: public InfoLogging {
public:
    DebugLogging(const string& logFileName);

	virtual ~DebugLogging();

	void debug(const string& message, const string& moduleName) {
	    logToStdout(message, moduleName, PROCESSOR_VERSION, "[D]");
	}

protected:
    DebugLogging() {};
};

#endif	/* DEBUGLOGGING_H */
