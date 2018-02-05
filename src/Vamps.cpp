/*
 Vamps

 a 2 RackUnit stereo mod of Andrew Belt's Fundamental VCA

 MAYBE TODO:
 - reduce to one cv in + lin/exp option 

*/////////////////////////////////////////////////////////////////////////////



#include "pvc.hpp"


struct Vamps : Module {
	enum ParamIds {
		LEVEL,
		
		NUM_PARAMS
	};
	enum InputIds {
		EXP_CV,
		LIN_CV,
		IN_L,
		IN_R,
		
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_L,
		OUT_R,
		
		NUM_OUTPUTS
	};

	Vamps() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};


void Vamps::step() {

	float left = inputs[IN_L].value * params[LEVEL].value;
	float right = inputs[IN_R].normalize(inputs[IN_L].value) * params[LEVEL].value;
	
	const float expBase = 50.0f;

	if (inputs[LIN_CV].active) {
		float linCV = clampf(inputs[LIN_CV].value * 0.1f, 0.0f, 1.0f);
		left *= linCV;
		right *= linCV;
	}
	if (inputs[EXP_CV].active) {
		float expCV = rescalef(powf(expBase, clampf(inputs[EXP_CV].value / 10.0f, 0.0f, 1.0f)), 1.0, expBase, 0.0f, 1.0f);
		left *= expCV;
		right *= expCV;
	}
	outputs[OUT_L].value = left;
	outputs[OUT_R].value = right;
	
}


VampsWidget::VampsWidget() {
	Vamps *module = new Vamps();
	setModule(module);
	box.size = Vec(15*2, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/Vamps.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	//addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	//addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	
	addInput(createInput<InPortAud>(Vec(4, 22), module, Vamps::IN_L));
	addInput(createInput<InPortAud>(Vec(4, 64), module, Vamps::IN_R));

	addParam(createParam<PvCKnob>(Vec(4, 134), module, Vamps::LEVEL, 0.0f, 1.0f, 0.5f));
	addInput(createInput<InPortCtrl>(Vec(4, 172), module, Vamps::EXP_CV));
	addInput(createInput<InPortCtrl>(Vec(4, 214), module, Vamps::LIN_CV));

	addOutput(createOutput<OutPortVal>(Vec(4, 294), module, Vamps::OUT_L));
	addOutput(createOutput<OutPortVal>(Vec(4, 336), module, Vamps::OUT_R));
}
