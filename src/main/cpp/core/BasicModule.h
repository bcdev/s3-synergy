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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 * File:   BasicModule.h
 * Author: ralf
 *
 * Created on December 21, 2010, 12:24 PM
 */

#ifndef BASICMODULE_H
#define	BASICMODULE_H

#include <cstdlib>
#include <stdexcept>

#include "Boost.h"
#include "Context.h"
#include "Module.h"

using std::getenv;
using std::max;
using std::runtime_error;

/**
 * A basic module, doing nothing.
 */
class BasicModule: public Module {
public:

	/**
	 * Constructs a new instance of this class.
	 * @param moduleId The module ID.
	 * @param moduleVersion The module version.
	 */
	BasicModule(const string& moduleId, const string& moduleVersion = Constants::PROCESSOR_VERSION) :
			id(moduleId), version(moduleVersion) {
	}

	virtual ~BasicModule() {
	}

	const string& getId() const {
		return id;
	}

	const string& getVersion() const {
		return version;
	}

	virtual void start(Context& context) {
	}

	virtual void stop(Context& context) {
	}

	virtual void process(Context& context) {
	}

	static string getInstallationPath() {
		const char* value = std::getenv("S3_SYNERGY_HOME");
		if (value != 0) {
			return string(value);
		}
		throw runtime_error("Unknown software installation path. The environment variable 'S3_SYNERGY_HOME' has not been set.");
	}

protected:

	template<class K, class V>
	bool contains(const map<K, V>& map, const K& key) const {
		return map.find(key) != map.end();
	}

private:
	const string id;
	const string version;
};

#endif	/* BASICMODULE_H */

