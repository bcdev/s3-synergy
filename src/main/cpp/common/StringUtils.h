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
 * File:   StringUtils.h
 * Author: thomass
 *
 * Created on November 24, 2010, 5:33 PM
 */

#ifndef STRINGUTILS_H
#define	STRINGUTILS_H


#include <iostream>
#include <string>
#include <sstream>

using std::string;
using std::stringstream;

class StringUtils {
public:
    StringUtils();
    virtual ~StringUtils();

    static bool stringToBool(string in) {
        if (in.compare("true") == 0 || in.compare("True") == 0
                || in.compare("TRUE")) {
            return true;
        }
        return false;
    }

    static string intToString(long toConvert) {
        stringstream temp;
        temp << toConvert;
        return temp.str();
    }
private:

};

#endif	/* STRINGUTILS_H */

