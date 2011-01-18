/* 
 * Copyright (C) 2011 by Brockmann Consult (info@brockmann-consult.de)
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
 * File:   Boost.h
 * Author: ralf
 *
 * Created on January 14, 2011, 12:29 PM
 */

#ifndef BOOST_H
#define	BOOST_H

#include <boost/algorithm/string.hpp>
#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

#define foreach         BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

using boost::algorithm::ends_with;
using boost::algorithm::starts_with;
using boost::algorithm::trim_copy;
using boost::filesystem::directory_iterator;
using boost::filesystem::is_directory;
using boost::filesystem::path;
//using boost::exception;
using boost::lexical_cast;
using boost::numeric_cast;

#endif	/* BOOST_H */

