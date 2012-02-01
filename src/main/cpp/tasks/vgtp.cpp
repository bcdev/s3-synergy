/*
 * vgtp.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: thomasstorm
 */

#include "../modules/Col.h"
#include "../modules/Pcl.h"
#include "../modules/Vbm.h"
#include "../modules/Vfl.h"
#include "../modules/Vpr.h"
#include "../reader/SynL1Reader.h"
#include "../writer/VgtWriter.h"

#include "../util/BasicTask.h"

int main(int argc, char* argv[]) {
	BasicTask task("VGTP");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vfl = shared_ptr<Module>(new Vfl());
	shared_ptr<Module> vpr = shared_ptr<Module>(new Vpr());
	shared_ptr<Module> writer = shared_ptr<Module>(new VgtWriter());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vfl);
	task.getContext().addModule(vpr);
	task.getContext().addModule(writer);

	return task.execute(argc, argv);
}
