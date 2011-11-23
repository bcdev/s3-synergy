/*
 * syl2.cpp
 *
 *  Created on: Nov 22, 2011
 *      Author: ralf
 */

#include "../modules/Aer.h"
#include "../modules/Aei.h"
#include "../modules/Ave.h"
#include "../modules/Aco.h"
#include "../modules/Col.h"
#include "../modules/Pcl.h"
#include "../reader/SynL1Reader.h"
#include "../writer/SynL2Writer.h"

#include "../util/BasicTask.h"

int main(int argc, char* argv[]) {
	BasicTask task("SYL2");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
	shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
	shared_ptr<Module> aei = shared_ptr<Module>(new Aei());
	shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(ave);
	task.getContext().addModule(aer);
	task.getContext().addModule(aei);
	task.getContext().addModule(aco);
	task.getContext().addModule(writer);

	return task.execute(argc, argv);
}
