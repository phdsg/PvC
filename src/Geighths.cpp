/*

Geighths

cv input selects one out of eight outputs to fire a gate from.

assumed input range is 0-10V. full range is split into 8 segments.
with a clock signal connected, the unit goes into sample and hold mode.

PLANS:
	- option to use 8bit conversion to break up signal
	- internal clock (maybe)
	- random/stepped mode(s) with no cvIN but clockIN
	- trigger sum bus

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"

#include "dsp/digital.hpp" // SchmittTrigger PulseGenerator

struct Geighths : Module {
	enum ParamIds {
		INPUT_GAIN,
		INPUT_OFFSET,

		GATE1_LENGTH,

		NUM_PARAMS = GATE1_LENGTH + 8
	};
	enum InputIds {
		CV_IN,
		CLOCK_IN,

		NUM_INPUTS
	};
	enum OutputIds {
		GATE1_OUT,

		NUM_OUTPUTS = GATE1_OUT + 8
	};
	enum LightIds {
		GATE1_LIGHT,

		NUM_LIGHTS = GATE1_LIGHT + 8
	};

	SchmittTrigger clockTrigger;

	SchmittTrigger gateTrigger[8];
	PulseGenerator gatePulse[8];

	bool gateOn[8] {};

	float inVal = 0.0f;
	float sample = 0.0f;

	Geighths() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
};


void Geighths::step() {
	float input = inputs[CV_IN].normalize(0.0f) * params[INPUT_GAIN].value + params[INPUT_OFFSET].value;

	input = clampf(input, 0.0f, 10.0f);
	input = rescalef(input, 0.0f, 10.0f, 0, 8);
	
	// SnH when clocked
	if (inputs[CLOCK_IN].active) { 
		if (clockTrigger.process(inputs[CLOCK_IN].value)){
			sample = input; 
		}
		inVal = sample;
	}
	else {
		inVal = input;
	}
	// fire pulse on selected out
	for (int i = 0; i < 8; i++) {
		gateOn[i] = ((inVal > i) && (inVal < i+1)) ? true : false;

		if (gateTrigger[i].process(gateOn[i])) {
			gatePulse[i].trigger(params[GATE1_LENGTH + i].value);
		}

		outputs[GATE1_OUT + i].value = gatePulse[i].process(1.0/engineGetSampleRate()) ? 10.0f : 0.0f;
		lights[GATE1_LIGHT + i].value = ((gateOn[i]) || (gatePulse[i].process(1.0/engineGetSampleRate()))) ? 1.0f : 0.0f;
	}
}

GeighthsWidget::GeighthsWidget() {
	Geighths *module = new Geighths();
	setModule(module);
	box.size = Vec(15*4, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/Geighths.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22),module,Geighths::CV_IN));
	addParam(createParam<PvCKnob>(Vec(34, 22),module,Geighths::INPUT_GAIN , -2.0f, 2.0f, 1.0f));
	addParam(createParam<PvCKnob>(Vec(34, 58),module,Geighths::INPUT_OFFSET, -10.0f, 10.0f, 0.0f));

	addInput(createInput<InPortBin>(Vec(4,84),module,Geighths::CLOCK_IN));

	for (int i = 0; i < 8; i++)
	{
		addChild(createLight<PvCBigLED<WhiteLight>>(Vec(4,336 - 28*i),module,Geighths::GATE1_LIGHT + i));
		addParam(createParam<PvCLEDKnob>(Vec(4, 336 - 28*i),module,Geighths::GATE1_LENGTH + i, 0.002f, 2.0f, 0.025f));
		addOutput(createOutput<OutPortBin>(Vec(34, 336 - 28*i),module,Geighths::GATE1_OUT + i));
	}
}