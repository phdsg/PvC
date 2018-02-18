#include "pvc.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = "PvC";
	p->version = TOSTRING(VERSION);

	p->addModel(modelAorBtoOut);
	p->addModel(modelInToAorB);
	p->addModel(modelBangDaButton);
	p->addModel(modelCompair);
	p->addModel(modelCoSuOf);
	p->addModel(modelFlipOLogic);
	p->addModel(modelGeighths);
	p->addModel(modelShutIt);
	p->addModel(modelSlimSeq);
	p->addModel(modelSumIt);
	p->addModel(modelTaHaSaHaN);
	p->addModel(modelVamps);
	p->addModel(modelVubar);
	p->addModel(modelPvCBlank);
}

