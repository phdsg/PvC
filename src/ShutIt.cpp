/*
ShutIt

quad channel trigger mutes

- LEDs are manual nute triggers
- inputs normalized to last in, so also works as a mult

*/////////////////////////////////////////////////////////////////////////////
#include "pvc.hpp"

#include "dsp/digital.hpp"

#define CHANCOUNT 4

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
		NUM_LIGHTS = A_STATE + CHANCOUNT
	};

	bool muteState[CHANCOUNT] {};
	SchmittTrigger cvTrigger[CHANCOUNT];
	SchmittTrigger buttonTrigger[CHANCOUNT];

	ShutIt() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;

		void reset() override {
		for (int i = 0; i < CHANCOUNT; i++) {
			muteState[i] = true;
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
	float out = 0.0;
	for (int i = 0; i < CHANCOUNT; i++) {
		if (cvTrigger[i].process(inputs[A_TRIG + i].value))
			muteState[i] ^= true;
		if (buttonTrigger[i].process(params[A_MUTE + i].value))
			muteState[i] ^= true;
		if (inputs[A_IN + i].active)
			out = inputs[A_IN + i].value;
		outputs[A_OUT + i].value = muteState[i] ? out : 0.0;
		lights[A_STATE + i].value = muteState[i] ? 0:0.75;
	}
}

template <typename BASE>
 struct MuteLight : BASE {
 	MuteLight() {
 		this->box.size = Vec(8, 8);
 	}
 };

 struct EmptyButton : SVGSwitch, MomentarySwitch {
	EmptyButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(8,8);
	}
};


ShutItWidget::ShutItWidget() {
	ShutIt *module = new ShutIt();
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
	// channels
	float top = 85.25;
	for (int i = 0; i < CHANCOUNT; i++) {
		addInput(createInput<ModInPort>(Vec(4,22 + top*i),module, ShutIt::A_TRIG + i));
		addChild(createLight<MuteLight<RedLight>>(Vec(11,46 + top*i), module, ShutIt::A_STATE + i));
		addParam(createParam<EmptyButton>(Vec(11,46 + top*i),module, ShutIt::A_MUTE + i, 0, 1 , 0));
		addOutput(createOutput<OutPort>(Vec(4,56 + top*i),module, ShutIt::A_OUT + i));
		addInput(createInput<InPort>(Vec(4,80 + top*i),module, ShutIt::A_IN + i));
	}
}