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

	float mix = 0.0f;
	int count = 0;

	SumIt() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;

	void reset() override {
		mix = 0.0f;
		count = 0;
	}
};


void SumIt::step() {
	mix = 0.0f;
	count = 0;
	
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
	outputs[OUTPUT].value = clamp(mix, -10.0f, 10.0f);
}

struct SumItWidget : ModuleWidget {
	SumItWidget(SumIt *module);
};

SumItWidget::SumItWidget(SumIt *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/panels/panel2HE.svg")));
	// screws
	// addChild(Widget::create<ScrewHead1>(Vec(0, 0)));
	addChild(Widget::create<ScrewHead2>(Vec(box.size.x - 15, 0)));
	// addChild(Widget::create<ScrewHead3>(Vec(0, 365)));
	addChild(Widget::create<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// inputs
	for (int i = 0; i < 12; i++) {
		addInput(Port::create<InPortAud>(Vec(4,22 + 24*i), Port::INPUT, module, SumIt::INPUT + i));
	}
	// gain and out
	addParam(ParamWidget::create<PvCKnob>(Vec(4,24 + 24*12), module, SumIt::VOLUME, 0.0f,2.0f,1.0f));
	addOutput(Port::create<OutPortVal>(Vec(4,24 + 24*13), Port::OUTPUT, module, SumIt::OUTPUT));
}

Model *modelSumIt = Model::create<SumIt, SumItWidget>(
	"PvC", "SumIt", "SumIt", ATTENUATOR_TAG, AMPLIFIER_TAG, MIXER_TAG);