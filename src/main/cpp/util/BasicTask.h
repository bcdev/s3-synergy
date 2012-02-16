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

#ifndef BASICTASK_H_
#define BASICTASK_H_

#include "../core/Context.h"

#include "XPathInitializer.h"

class BasicTask {
public:
	BasicTask(const string& name);
	~BasicTask();

	int execute(int argc, char* argv[]);
	int execute(const string& jobOrderPath);

	Context& getContext() {
		return context;
	}

private:
	void logIoParameters(JobOrder& jobOrder, Logging& logging) const;

	const string name;

	XPathInitializer init;
	Context context;
};

#endif /* BASICTASK_H_ */
