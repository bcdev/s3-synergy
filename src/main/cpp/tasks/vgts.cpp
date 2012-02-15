/*
 * vgts.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: thomasstorm
 */

#include "../modules/Col.h"
#include "../modules/Pcl.h"
#include "../modules/Vbm.h"
#include "../modules/Vfl.h"
#include "../modules/Vac.h"
#include "../modules/Vco.h"
#include "../reader/SynL1Reader.h"
#include "../writer/VgtWriter.h"
#include "../writer/ManifestWriter.h"

#include "../util/BasicTask.h"

int main(int argc, char* argv[]) {
	BasicTask task("VGTS");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vfl = shared_ptr<Module>(new Vfl());
	shared_ptr<Module> vac = shared_ptr<Module>(new Vac());
	shared_ptr<Module> vco = shared_ptr<Module>(new Vco());
	shared_ptr<Module> writer = shared_ptr<Module>(new VgtWriter(Constants::PRODUCT_VGS));
	shared_ptr<Module> manifestWriter = shared_ptr<Module>(new ManifestWriter(Constants::PRODUCT_VGS));

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vfl);
	task.getContext().addModule(vac);
	task.getContext().addModule(vco);
	task.getContext().addModule(writer);
	task.getContext().addModule(manifestWriter);

	return task.execute(argc, argv);
}
