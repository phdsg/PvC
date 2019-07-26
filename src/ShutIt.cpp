/*
ShutIt

*/////////////////////////////////////////////////////////////////////////////
#include "pvc.hpp"

#define CHANCOUNT 8

struct ShutIt : Module {
	enum ParamIds {
		A_MUTE,
		SHUT_ALL = A_MUTE + CHANCOUNT,
		OPEN_ALL,
		FLIP_ALL,
		NUM_PARAMS
	};
	enum InputIds {
		A_IN,
		A_TRIG = A_IN + CHANCOUNT,
		SHUT_ALL_TRIG = A_TRIG + CHANCOUNT,
		OPEN_ALL_TRIG,
		FLIP_ALL_TRIG,

		NUM_INPUTS
	};
	enum OutputIds {
		A_OUT,
		NUM_OUTPUTS = A_OUT + CHANCOUNT
	};
	enum LightIds {
		A_STATE,
		NUM_LIGHTS = A_STATE + CHANCOUNT*2
	};

	bool muteState[CHANCOUNT] {};
	dsp::SchmittTrigger cvTrigger[CHANCOUNT];
	dsp::SchmittTrigger buttonTrigger[CHANCOUNT];
	dsp::SchmittTrigger unmuteAllTrig;
	dsp::SchmittTrigger muteAllTrig;
	dsp::SchmittTrigger flipAllTrig;
	
	ShutIt() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		for (int i = 0; i < CHANCOUNT; i++) {
			configParam(ShutIt::A_MUTE + i, 0, 1, 0);
		}
		configParam(SHUT_ALL, 0, 1, 0);
		configParam(FLIP_ALL, 0, 1, 0);
		configParam(OPEN_ALL, 0, 1, 0);
	}

	void process(const ProcessArgs &args) override;

	void onReset() override {
		for (int i = 0; i < CHANCOUNT; i++) {
			muteState[i] = false;
		}
	}
	void onRandomize() override {
		for (int i = 0; i < CHANCOUNT; i++) {
			muteState[i] = (random::uniform() < 0.5);
		}
	}
	// MUTE states
	json_t *dataToJson() override {
		json_t *rootJ = json_object();
		// states
		json_t *muteStatesJ = json_array();
		for (int i = 0; i < CHANCOUNT; i++) {
			json_t *muteStateJ = json_boolean(muteState[i]);
			json_array_append_new(muteStatesJ, muteStateJ);
		}
		json_object_set_new(rootJ, "muteStates", muteStatesJ);
		return rootJ;
	}
	void dataFromJson(json_t *rootJ) override {
		// states
		json_t *muteStatesJ = json_object_get(rootJ, "muteStates");
		if (muteStatesJ) {
			for (int i = 0; i < CHANCOUNT; i++) {
				json_t *muteStateJ = json_array_get(muteStatesJ, i);
				if (muteStateJ)
					muteState[i] = json_boolean_value(muteStateJ);
			}
		}
	}
};

void ShutIt::process(const ProcessArgs &args) {
//  do stuff
	float out = 0.0f;
	float triggerIn = 0.0f;
	for (int i = 0; i < CHANCOUNT; i++) {

		if (inputs[A_TRIG + i].isConnected())
			triggerIn = inputs[A_TRIG + i].getVoltage();
		
		if (cvTrigger[i].process(triggerIn))
			muteState[i] = !muteState[i];
		
		if (buttonTrigger[i].process(params[A_MUTE + i].getValue()))
			muteState[i] = !muteState[i];
		
		if (inputs[A_IN + i].isConnected())
			out = inputs[A_IN + i].getVoltage();

		outputs[A_OUT + i].setVoltage(muteState[i] ? 0.0f : out);
		lights[A_STATE + 2*i].value = muteState[i] ? 0 : 1;
		lights[A_STATE+1 + 2*i].value = muteState[i] ? 1 : 0;
	}
	if (muteAllTrig.process(inputs[SHUT_ALL_TRIG].getVoltage() + params[SHUT_ALL].getValue())) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = true;
		}
	}
	if (unmuteAllTrig.process(inputs[OPEN_ALL_TRIG].getVoltage() + params[OPEN_ALL].getValue())) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = false;
		}
	}
	if (flipAllTrig.process(inputs[FLIP_ALL_TRIG].getVoltage() + params[FLIP_ALL].getValue())) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = !muteState[i];
		}
	}
}

// ugh
 struct EmptyButton : SVGSwitch {
	EmptyButton() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/empty.svg")));
		box.size = Vec(86,36);
	}
};


struct ShutItWidget : ModuleWidget {
	ShutItWidget(ShutIt *module);
};

ShutItWidget::ShutItWidget(ShutIt *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/ShutIt.svg")));
	
	// screws
	addChild(createWidget<ScrewHead1>(Vec(0, 0)));
	addChild(createWidget<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createWidget<ScrewHead3>(Vec(0, 365)));
	addChild(createWidget<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// channels
	for (int i = 0; i < CHANCOUNT; i++) {
		float top = 36.0f;

		addChild(createLight<FourPixLight<WhiteRedLED>>(Vec(79,27 + top*i), module, ShutIt::A_STATE + 2*i));
		addParam(createParam<EmptyButton>(Vec(2,19 + top*i), module, ShutIt::A_MUTE + i));
		
		addInput(createInput<InPortAud>(Vec(4,26 + top*i), module, ShutIt::A_IN + i));
		addInput(createInput<InPortBin>(Vec(28,26 + top*i), module, ShutIt::A_TRIG + i));
		addOutput(createOutput<OutPortVal>(Vec(52,26 + top*i), module, ShutIt::A_OUT + i));
	}
	addInput(createInput<InPortBin>(Vec(4,324), module, ShutIt::SHUT_ALL_TRIG));
	addParam(createParam<LabelButtonS>(Vec(3,349), module, ShutIt::SHUT_ALL));
	
	addInput(createInput<InPortBin>(Vec(34,324), module, ShutIt::FLIP_ALL_TRIG));
 	addParam(createParam<LabelButtonS>(Vec(33,349), module, ShutIt::FLIP_ALL));
 	
	addInput(createInput<InPortBin>(Vec(64,324), module, ShutIt::OPEN_ALL_TRIG));
	addParam(createParam<LabelButtonS>(Vec(63,349), module, ShutIt::OPEN_ALL));

}

Model *modelShutIt = createModel<ShutIt, ShutItWidget>("ShutIt");

