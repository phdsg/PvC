/*
ShutIt

8 channel mute gate switch multiple

- invisible manual mute triggers around each port group
- inputs normalized to last connected above, so also works as a flexible mult
- trigger inputs are normalized in the same way allowing for a variety of
  combinations, as the manual triggers only affect their own channel.

                          based on code from fundamental mutes by andrew belt
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
		NUM_INPUTS = A_TRIG + CHANCOUNT
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
}

template <typename BASE>
 struct SixPixLight : BASE {
 	SixPixLight() {
 		this->box.size = Vec(6, 6);
 	}
 };
// ugh
 struct EmptyButton : SVGSwitch, MomentarySwitch {
	EmptyButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(56,40);
	}
};

ShutItWidget::ShutItWidget() {
	ShutIt *module = new ShutIt();
	setModule(module);
	box.size = Vec(15*4, 380);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel4HEocta.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// channels
	for (int i = 0; i < CHANCOUNT; i++) {
		float top = 42.25;

		addChild(createLight<SixPixLight<GreenRedLight>>(Vec(27,27 + top*i), module, ShutIt::A_STATE + 2*i));
		addParam(createParam<EmptyButton>(Vec(2,22 + top*i),module, ShutIt::A_MUTE + i, 0, 1 , 0));
		addInput(createInput<InPortBin>(Vec(19,39 + top*i),module, ShutIt::A_TRIG + i));
		addInput(createInput<InPortAud>(Vec(4,22 + top*i),module, ShutIt::A_IN + i));
		addOutput(createOutput<OutPortVal>(Vec(34,22 + top*i),module, ShutIt::A_OUT + i));
	}
}