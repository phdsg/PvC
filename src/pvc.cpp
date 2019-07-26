#include "pvc.hpp"


Plugin *pluginInstance;

void init(rack::Plugin *p) {
	pluginInstance = p;

	p->addModel(modelHeads);
	p->addModel(modelTails);
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
	// p->addModel(modelPvCBlank);
	
}

