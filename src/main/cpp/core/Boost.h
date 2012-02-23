/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 */

#ifndef BOOST_H
#define	BOOST_H

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/exception/all.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/regex.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#define foreach         BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

using boost::error_info;
using boost::shared_array;
using boost::shared_ptr;

using boost::numeric::ublas::matrix;

typedef error_info<struct errinfo_exit_code_, int> errinfo_exit_code;
typedef error_info<struct errinfo_nc_status_, int> errinfo_nc_status;
typedef error_info<struct errinfo_module_name_, std::string> errinfo_module_name;
typedef error_info<struct errinfo_method_name_, std::string> errinfo_method_name;

#endif	/* BOOST_H */
