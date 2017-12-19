/*

SumIt

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"


struct SumIt : Module {

	enum ParamIds {
		NUM_PARAMS
	};

	enum InputIds {
		INPUT,
		NUM_INPUTS = INPUT + 12
	};

	enum OutputIds {
		OUTPUT,
		NUM_OUTPUTS
	};

	enum LightIds {
		NUM_LIGHTS
	};

	SumIt() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
};



void SumIt::step() {
	
	float mix = 0;
	int count = 0;
	
	// Inputs
	for (int i = 0; i < 12; i++) {
		mix += inputs[INPUT + i].value;
		if (inputs[INPUT + i].active)
			count++;
	}
	// mix	
	if (count > 0)
		mix /= count;

	// Outputs
	outputs[OUTPUT].value = mix;
}


SumItWidget::SumItWidget() {
	SumIt *module = new SumIt();
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

	for (int i = 0; i < 12; i++) {
		addInput(createInput<InPort>(Vec(4,22 + 26*i), module, SumIt::INPUT + i));
	}
	addOutput(createOutput<OutPort>(Vec(4,24 + 26*12), module, SumIt::OUTPUT));
}