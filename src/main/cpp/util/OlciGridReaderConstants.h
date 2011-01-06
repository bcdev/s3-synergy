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
 * File:   ReaderConstants.h
 * Author: thomass
 *
 * Created on January 3, 2011, 1:57 PM
 */

#ifndef OLCIGRIDREADERCONSTANTS_H
#define	OLCIGRIDREADERCONSTANTS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * This class merely contains constants used for reading input data.
 */
class OlciGridReaderConstants {
public:
    OlciGridReaderConstants();
    virtual ~OlciGridReaderConstants();

    static const string L_1;
    static const string L_2;
    static const string L_3;
    static const string L_4;
    static const string L_5;
    static const string L_6;
    static const string L_7;
    static const string L_8;
    static const string L_9;
    static const string L_10;
    static const string L_11;
    static const string L_12;
    static const string L_13;
    static const string L_14;
    static const string L_15;
    static const string L_16;
    static const string L_17;
    static const string L_18;
    static const string L_19;
    static const string L_20;
    static const string L_21;
    static const string L_22;
    static const string L_23;
    static const string L_24;
    static const string L_25;
    static const string L_26;
    static const string L_27;
    static const string L_28;
    static const string L_29;
    static const string L_30;

    static const vector<string> getVariablesToRead();
private:

};

#endif	/* OLCIGRIDREADERCONSTANTS_H */

