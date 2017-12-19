/*

Atze

a slim Dual Voltage Controlled Attenuverter

- knob 1: scale [-2..*0*..2]
- knob 2: offset [-10..*0*..10]


*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"


struct Atze : Module {
	enum ParamIds {
		A_SCALE,
		B_SCALE,
		A_OFFSET,
		B_OFFSET,
		
		NUM_PARAMS
	};
	enum InputIds {
		A_IN,
		B_IN,
		A_SCL_CV,
		B_SCL_CV,
		A_OFF_CV,
		B_OFF_CV,
		NUM_INPUTS
	};
	enum OutputIds {
		A_OUT,
		B_OUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Atze() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
};



void Atze::step() {
//  do stuff
	float out = 0;
	float scale = 0;
	float offset = 0;
	for (int i = 0; i < 2; i++) {
		scale = (inputs[A_SCL_CV + i].active) ? inputs[A_SCL_CV + i].value * 0.1f * params[A_SCALE + i].value : params[A_SCALE + i].value;
		offset = (inputs[A_OFF_CV + i].active) ? inputs[A_OFF_CV + i].value * 0.1f * params[A_OFFSET + i].value : params[A_OFFSET + i].value;
		if (inputs[A_IN + i].active)
			out = inputs[A_IN + i].value;
		outputs[A_OUT + i].value = clampf(out * scale + offset,-10,10);
	}
}


AtzeWidget::AtzeWidget() {
	Atze *module = new Atze();
	setModule(module);
	box.size = Vec(15*2, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel2HE.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	// input
	float top = 174;
	for (int i = 0; i < 2; i++) {
		
		addOutput(createOutput<OutPort>(Vec(4,22 + top*i), module, Atze::A_OUT + i));
		addParam(createParam<PvCKnob>(Vec(4,52 + top*i), module, Atze::A_SCALE + i, -2, 2, 0));
		addInput(createInput<ModInPort>(Vec(4,78 + top*i), module, Atze::A_SCL_CV + i));
		addParam(createParam<PvCKnob>(Vec(4,108 + top*i), module, Atze::A_OFFSET + i, -10, 10, 0));
		addInput(createInput<ModInPort>(Vec(4,134 + top*i), module, Atze::A_OFF_CV + i));
		
		addInput(createInput<InPort>(Vec(4,162 + top*i), module, Atze::A_IN + i));
	}
	
}