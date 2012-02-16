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

/**
 * A utility class for creating an IPF task.
 */
class BasicTask {
public:
    /**
     * Constructs a new instance of this class.
     * @param name The name of the task created.
     */
	BasicTask(const string& name);

	/**
	 * Destructor.
	 */
	~BasicTask();

    /**
     * Executes the task, i.e. executes all modules added to the task's context.
     * @param argc The number of command line arguments.
     * @param argv The command line arguments.
     * @return the exit code.
     */
	int execute(int argc, char* argv[]);

    /**
     * Executes the task, i.e. executes all modules added to the task's context.
     * @param jobOrderPath The path to the IPF Job Order file.
     * @return the exit code.
     */
	int execute(const string& jobOrderPath);

    /**
     * Returns the task's context.
     * @return the task's context.
     */
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
