/*

SumIt

adds 12 inputs together and divides sum by number of active inputs

- volume knob controls output gain [0..2]V
- output clamped [-10..10]

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"


struct SumIt : Module {

	enum ParamIds {
		VOLUME,
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
	
	// sum and count inputs
	for (int i = 0; i < 12; i++) {
		mix += inputs[INPUT + i].value;
		if (inputs[INPUT + i].active)
			count++;
	}
	// divide sum by number of active inputs	
	if (count > 0)
		mix /= count;

	// out gain and clamp
	mix *= params[VOLUME].value;
	outputs[OUTPUT].value = clampf(mix, -10.0f, 10.0f);
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
	// inputs
	for (int i = 0; i < 12; i++) {
		addInput(createInput<InPortAud>(Vec(4,22 + 24*i), module, SumIt::INPUT + i));
	}
	// gain and out
	addParam(createParam<PvCKnob>(Vec(4,24 + 24*12), module, SumIt::VOLUME, 0.0f,2.0f,1.0f));
	addOutput(createOutput<OutPortVal>(Vec(4,24 + 24*13), module, SumIt::OUTPUT));
}