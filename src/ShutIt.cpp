/*
ShutIt

*/////////////////////////////////////////////////////////////////////////////
#include "pvc.hpp"

#include "dsp/digital.hpp"

#define CHANCOUNT 8

struct ShutIt : Module {
	enum ParamIds {
		A_MUTE,
		NUM_PARAMS = A_MUTE + CHANCOUNT
	};
	enum InputIds {
		A_IN,
		A_TRIG = A_IN + CHANCOUNT,
		MUTE_ALL_TRIG = A_TRIG + CHANCOUNT,
		UNMUTE_ALL_TRIG,
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
	SchmittTrigger cvTrigger[CHANCOUNT];
	SchmittTrigger buttonTrigger[CHANCOUNT];
	SchmittTrigger unmuteAllTrig;
	SchmittTrigger muteAllTrig;
	SchmittTrigger flipAllTrig;
	
	ShutIt() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {	}

	void step() override;

	void reset() override {
		for (int i = 0; i < CHANCOUNT; i++) {
			muteState[i] = false;
		}
	}
	
	void randomize() override {
		for (int i = 0; i < CHANCOUNT; i++) {
			muteState[i] = (randomf() < 0.5);
		}
	}
	// MUTE states
	json_t *toJson() override {
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
	void fromJson(json_t *rootJ) override {
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

void ShutIt::step() {
//  do stuff
	float out = 0.0f;
	float triggerIn = 0.0f;
	for (int i = 0; i < CHANCOUNT; i++) {

		if (inputs[A_TRIG + i].active)
			triggerIn = inputs[A_TRIG + i].value;
		
		if (cvTrigger[i].process(triggerIn))
			muteState[i] = !muteState[i];
		
		if (buttonTrigger[i].process(params[A_MUTE + i].value))
			muteState[i] = !muteState[i];
		
		if (inputs[A_IN + i].active)
			out = inputs[A_IN + i].value;

		outputs[A_OUT + i].value = muteState[i] ? 0.0f : out;
		lights[A_STATE + 2*i].value = muteState[i] ? 0 : 1;
		lights[A_STATE+1 + 2*i].value = muteState[i] ? 1 : 0;
	}
	if (muteAllTrig.process(inputs[MUTE_ALL_TRIG].value)) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = true;
		}
	}
	if (unmuteAllTrig.process(inputs[UNMUTE_ALL_TRIG].value)) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = false;
		}
	}
	if (flipAllTrig.process(inputs[FLIP_ALL_TRIG].value)) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = !muteState[i];
		}
	}
}

template <typename BASE>
 struct FourPixLight : BASE {
 	FourPixLight() {
 		this->box.size = Vec(4, 4);
 	}
 };
// ugh
 struct EmptyButton : SVGSwitch, MomentarySwitch {
	EmptyButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(86,36);
	}
};
struct WhiteRedLight : ModuleLightWidget {
	WhiteRedLight() {
		addBaseColor(COLOR_WHITE);
		addBaseColor(COLOR_RED);
	}
};

ShutItWidget::ShutItWidget() {
	ShutIt *module = new ShutIt();
	setModule(module);
	box.size = Vec(15*6, 380);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/ShutIt.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// channels
	for (int i = 0; i < CHANCOUNT; i++) {
		float top = 38.0f;

		addChild(createLight<FourPixLight<WhiteRedLight>>(Vec(73,25 + top*i), module, ShutIt::A_STATE + 2*i));
		addParam(createParam<EmptyButton>(Vec(2,20 + top*i),module, ShutIt::A_MUTE + i, 0, 1 , 0));
		addInput(createInput<InPortAud>(Vec(4,22 + top*i),module, ShutIt::A_IN + i));
		addInput(createInput<InPortBin>(Vec(34,22 + top*i),module, ShutIt::A_TRIG + i));
		addOutput(createOutput<OutPortVal>(Vec(64,30 + top*i),module, ShutIt::A_OUT + i));
	}
	addInput(createInput<InPortBin>(Vec(4,336),module, ShutIt::MUTE_ALL_TRIG));
	addInput(createInput<InPortBin>(Vec(34,336),module, ShutIt::FLIP_ALL_TRIG));
	addInput(createInput<InPortBin>(Vec(64,336),module, ShutIt::UNMUTE_ALL_TRIG));
}