/*
 Vamps

 a 2 RackUnit stereo mod of Andrew Belt's Fundamental VCA

 MAYBE TODO:
 - reduce to one cv in + lin/exp option 

*/////////////////////////////////////////////////////////////////////////////



#include "pvc.hpp"


struct Vamps : Module {
	enum ParamIds {
		LEVEL1_PARAM,
		LEVEL2_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		A_EXP_CV,
		A_LIN_CV,
		A_IN_L,
		A_IN_R,
		B_EXP_CV,
		B_LIN_CV,
		B_IN_L,
		B_IN_R,
		NUM_INPUTS
	};
	enum OutputIds {
		A_OUT_L,
		A_OUT_R,
		B_OUT_L,
		B_OUT_R,
		NUM_OUTPUTS
	};

	Vamps() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};


static void stepChannel(Input &in1, Input &in2, Param &level, Input &lin, Input &exp, Output &out1, Output &out2) {
	float v1 = in1.value * level.value;
	float v2 = in2.value * level.value;
	const float expBase = 50.0;

	if (lin.active) {
		v1 *= clampf(lin.value / 10.0, 0.0, 1.0);
		v2 *= clampf(lin.value / 10.0, 0.0, 1.0);
	}
	if (exp.active) {
		v1 *= rescalef(powf(expBase, clampf(exp.value / 10.0, 0.0, 1.0)), 1.0, expBase, 0.0, 1.0);
		v2 *= rescalef(powf(expBase, clampf(exp.value / 10.0, 0.0, 1.0)), 1.0, expBase, 0.0, 1.0);
	}
	out1.value = v1;
	out2.value = v2;
}

void Vamps::step() {
	stepChannel(inputs[A_IN_L], inputs[A_IN_R], params[LEVEL1_PARAM], inputs[A_LIN_CV], inputs[A_EXP_CV], outputs[A_OUT_L], outputs[A_OUT_R]);
	stepChannel(inputs[B_IN_L], inputs[B_IN_R], params[LEVEL2_PARAM], inputs[B_LIN_CV], inputs[B_EXP_CV], outputs[B_OUT_L], outputs[B_OUT_R]);
}


VampsWidget::VampsWidget() {
	Vamps *module = new Vamps();
	setModule(module);
	box.size = Vec(15*2, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel2HEdual.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	
	// channel one
	addParam(createParam<PvCKnob>(Vec(4, 22), module, Vamps::LEVEL1_PARAM, 0.0, 1.0, 0.5));
	addInput(createInput<InPortCtrl>(Vec(4, 44), module, Vamps::A_EXP_CV));
	addInput(createInput<InPortCtrl>(Vec(4, 66), module, Vamps::A_LIN_CV));

	addInput(createInput<InPortAud>(Vec(4, 92), module, Vamps::A_IN_L));
	addOutput(createOutput<OutPortVal>(Vec(4, 115), module, Vamps::A_OUT_L));

	addInput(createInput<InPortAud>(Vec(4, 139), module, Vamps::A_IN_R));
	addOutput(createOutput<OutPortVal>(Vec(4, 162), module, Vamps::A_OUT_R));

	//channel two
	addParam(createParam<PvCKnob>(Vec(4, 22 + 174), module, Vamps::LEVEL2_PARAM, 0.0, 1.0, 0.5));
	addInput(createInput<InPortCtrl>(Vec(4, 44 + 174), module, Vamps::B_EXP_CV));
	addInput(createInput<InPortCtrl>(Vec(4, 66 + 174), module, Vamps::B_LIN_CV));

	addInput(createInput<InPortAud>(Vec(4, 92 + 174), module, Vamps::B_IN_L));
	addOutput(createOutput<OutPortVal>(Vec(4, 115 + 174), module, Vamps::B_OUT_L));

	addInput(createInput<InPortAud>(Vec(4, 139 + 174), module, Vamps::B_IN_R));
	addOutput(createOutput<OutPortVal>(Vec(4, 162 + 174), module, Vamps::B_OUT_R));
}
