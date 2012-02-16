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

#ifndef ABSTRACTMODULE_H
#define	ABSTRACTMODULE_H

#include <cstdlib>
#include <stdexcept>

#include "../core/Boost.h"
#include "../core/Context.h"
#include "../core/Module.h"

using std::getenv;
using std::max;
using std::runtime_error;

/**
 * An abstract implementation of the {@code Module} interface, which does nothing.
 */
class AbstractModule: virtual public Module {
public:

	/**
	 * Constructs a new instance of this class.
	 * @param moduleId The module ID.
	 * @param moduleVersion The module version.
	 */
	AbstractModule(const string& moduleId, const string& moduleVersion = Constants::PROCESSOR_VERSION) :
			id(moduleId), version(moduleVersion) {
	}

	virtual ~AbstractModule() {
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

private:
	const string id;
	const string version;
};

#endif	/* ABSTRACTMODULE_H */

