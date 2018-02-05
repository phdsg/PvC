/*
ShutItXL

12 channel mute gate switch multiple

- invisible manual mute triggers around each port group
- inputs normalized to last connected above, so also works as a flexible mult
- trigger inputs are normalized in the same way allowing for a variety of
  combinations, as the manual triggers only affect their own channel.

                          based on code from fundamental mutes by andrew belt
*/////////////////////////////////////////////////////////////////////////////
#include "pvc.hpp"

#include "dsp/digital.hpp"

#define CHANCOUNT 12

struct ShutItXL : Module {
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
	
	ShutItXL() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {	}

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

void ShutItXL::step() {
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


// ugh
struct EmptyButton : SVGSwitch, MomentarySwitch {
	EmptyButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(86,26);
	}
};
struct WhiteRedLight : ModuleLightWidget {
	WhiteRedLight() {
		addBaseColor(COLOR_WHITE);
		addBaseColor(COLOR_RED);
	}
};

template <typename BASE>
 struct FivePixLight : BASE {
 	FivePixLight() {
 		this->box.size = Vec(5, 5);
 	}
 };

ShutItXLWidget::ShutItXLWidget() {
	ShutItXL *module = new ShutItXL();
	setModule(module);
	box.size = Vec(15*6, 380);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel6HE.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(15, 0)));
	//addChild(createScrew<ScrewHead4>(Vec(box.size.x - 30, 0)));
	//addChild(createScrew<ScrewHead2>(Vec(15, 365)));
	addChild(createScrew<ScrewHead3>(Vec(box.size.x - 30, 365)));
	// channels
	for (int i = 0; i < CHANCOUNT; i++) {
		float top = 28;
		
		addParam(createParam<EmptyButton>(Vec(2,20 + top*i),module, ShutItXL::A_MUTE + i, 0, 1 , 0));
		addInput(createInput<InPortAud>(Vec(4,24 + top*i),module, ShutItXL::A_IN + i));
		addInput(createInput<InPortBin>(Vec(28,24 + top*i),module, ShutItXL::A_TRIG + i));
		addChild(createLight<FivePixLight<WhiteRedLight>>(Vec(54.5,32.5 + top*i), module, ShutItXL::A_STATE + 2*i));
		addOutput(createOutput<OutPortVal>(Vec(64,24 + top*i),module, ShutItXL::A_OUT + i));
	}
}