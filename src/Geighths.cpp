/*

Geighths

cv input selects one out of eight outputs to fire a gate from.

assumed input range is 0-10V. full range is split into 8 segments.
with a clock signal connected, the unit goes into sample and hold mode.

PLANS/IDEAS:
	- option to use 8bit conversion to break up signal
	- internal clock (maybe)
	- random/stepped mode(s) with no cvIN but clockIN
	- trigger sum bus

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"

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

	dsp::SchmittTrigger clockTrigger;

	dsp::SchmittTrigger gateTrigger[8];
	dsp::PulseGenerator gatePulse[8];

	bool gateOn[8] {};

	float inVal = 0.0f;
	float sample = 0.0f;

	Geighths() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(INPUT_GAIN, -2.0f, 2.0f, 1.0f);
		configParam(INPUT_OFFSET, -10.0f, 10.0f, 0.0f);
		for (int i = 0; i < 8; i++)	{	
			configParam(GATE1_LENGTH + i, 0.002f, 2.0f, 0.02f);
		}
	}

	void process(const ProcessArgs &args) override;
	void reset() {
		inVal = sample = 0.0f;

		for (int i = 0; i < 8; i++)	{
			gateOn[i] = false;
		}
	}
};


void Geighths::process(const ProcessArgs &args) {
	float input = inputs[CV_IN].getNormalVoltage(0.0f) * params[INPUT_GAIN].getValue() + params[INPUT_OFFSET].getValue();
	 // TODO: nicer input window scaling
	input = clamp(input, 0.0f, 10.0f);
	input = rescale(input, 0.0f, 10.0f, 0, 8);
		
	sample = clockTrigger.process(inputs[CLOCK_IN].getVoltage()) ? input : sample;
	
	inVal = (inputs[CLOCK_IN].isConnected()) ? sample : input;

	// fire pulse on selected out
	for (int i = 0; i < 8; i++) {
		gateOn[i] = ((inVal > i) && (inVal < i+1)) ? true : false;

		if (gateTrigger[i].process(gateOn[i])) {
			gatePulse[i].trigger(params[GATE1_LENGTH + i].getValue());
		}

		outputs[GATE1_OUT + i].setVoltage(gatePulse[i].process(1.0/args.sampleRate) * 10.0f);
		// lights[GATE1_LIGHT + i].value = ((gateOn[i]) || (gatePulse[i].process(1.0/args.sampleRate))) ? 1.0f : 0.0f;
		lights[GATE1_LIGHT + i].value = gateOn[i] * 0.75f + gatePulse[i].process(1.0/args.sampleRate) * 0.25f;
	}
}

struct GeighthsWidget : ModuleWidget {
	GeighthsWidget(Geighths *module);
};

GeighthsWidget::GeighthsWidget(Geighths *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/Geighths.svg")));
	
	// screws
	addChild(createWidget<ScrewHead1>(Vec(0, 0)));
	addChild(createWidget<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createWidget<ScrewHead3>(Vec(0, 365)));
	addChild(createWidget<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22), module,Geighths::CV_IN));
	addInput(createInput<InPortBin>(Vec(34,22), module,Geighths::CLOCK_IN));
	addParam(createParam<PvCKnob>(Vec(4, 64),module,Geighths::INPUT_GAIN));
	addParam(createParam<PvCKnob>(Vec(34, 64),module,Geighths::INPUT_OFFSET));
	
	for (int i = 0; i < 8; i++)
	{
		addChild(createLight<PvCBigLED<BlueLED>>(Vec(4,318 - 30*i),module,Geighths::GATE1_LIGHT + i));
		addParam(createParam<PvCLEDKnob>(Vec(4, 318 - 30*i),module,Geighths::GATE1_LENGTH + i));
		addOutput(createOutput<OutPortBin>(Vec(34, 318 - 30*i), module,Geighths::GATE1_OUT + i));
	}
}

Model *modelGeighths = createModel<Geighths, GeighthsWidget>("Geighths");