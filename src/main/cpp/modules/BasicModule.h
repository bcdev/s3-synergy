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

#ifndef BASICMODULE_H
#define	BASICMODULE_H

#include <algorithm>
#include <set>

#include "../core/AbstractModule.h"
#include "../util/AuxdataProvider.h"
#include "../util/LookupTableReader.h"

using std::set;

/**
 * A basic module, by default doing nothing.
 */
class BasicModule: public AbstractModule {
public:

	/**
	 * Constructs a new instance of this class.
	 * @param moduleId The module ID.
	 * @param moduleVersion The module version.
	 */
	BasicModule(const string& moduleId, const string& moduleVersion = Constants::PROCESSOR_VERSION) : AbstractModule(moduleId, moduleVersion) {

	}

	virtual ~BasicModule() {
	}

protected:
    template<class T>
    static void copy(const valarray<T>& s, valarray<T>& t);

    static string getAuxdataPath() {
		return Constants::S3_SYNERGY_HOME + "/files/";
	}

    template<class K, class V>
	static bool contains(const map<K, V>& map, const K& key) {
		return map.find(key) != map.end();
	}

    template<class K>
    static bool contains(const set<K>& set, const K& key) {
        return set.find(key) != set.end();
    }

    template<class T>
    static bool contains(const vector<T>& v, const T& t) {
        return std::find(v.begin(), v.end(), t) != v.end();
    }

    template<class T>
    static bool isSet(T flags, T flagMasks) {
        return (flags & flagMasks) == flagMasks;
    }

    void addAccessor(Context& context, Segment& s, const VariableDescriptor& varDescriptor) const;

    AuxdataProvider& getAuxdataProvider(Context& context, const string& auxId) const;

    LookupTable<double>& getLookupTable(Context& context, const string& auxId, const string& varName) const;
};

template<class T>
void BasicModule::copy(const std::valarray<T>& s, std::valarray<T>& t) {
	using std::copy;

	if (t.size() != s.size()) {
		t.resize(s.size());
	}
    copy(&s[0], &s[s.size()], &t[0]);
}

#endif	/* BASICMODULE_H */

