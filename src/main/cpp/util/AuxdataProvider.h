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

#ifndef AUXDATAPROVIDER_H_
#define AUXDATAPROVIDER_H_

#include <map>
#include <string>
#include <valarray>

#include "../core/Boost.h"
#include "../core/Identifiable.h"

using std::map;
using std::string;
using std::valarray;

using boost::numeric::ublas::matrix;

class AuxdataProvider: public Identifiable {
public:
	AuxdataProvider(const string& id, string auxdataPath);
	virtual ~AuxdataProvider();

	const string& getId() const;

	void getUByte(const string& varName, uint8_t& data) const;
	void getUShort(const string& varName, uint16_t& data) const;
	void getDouble(const string& varName, double& data) const;
	void getShort(const string& varName, int16_t& data) const;

	void getVectorDouble(const string& varName, valarray<double>& data) const;
	void getVectorShort(const string& varName, valarray<int16_t>& data) const;

	void getMatrixDouble(const string& varName, matrix<double>& data) const;

private:
	const string id;
	const int fileId;
};

#endif /* AUXDATAPROVIDER_H_ */
