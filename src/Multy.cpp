/*

Multy

 1 to 6 Multiple with mutes for each output

 modified tutorial.cpp with snippets from fundamental/mutes by andrew belt 


now deprecated and replaced with ShutIt

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"
#include "dsp/digital.hpp"

#define MULTCOUNT 6

struct Multy : Module {
	enum ParamIds {
		MUTE_PARAM,
		NUM_PARAMS = MUTE_PARAM + MULTCOUNT
	};
	
	enum InputIds {
		MULT_INPUT,
		NUM_INPUTS
	};
	
	enum OutputIds {
		MULT_OUTPUT,
		NUM_OUTPUTS = MULT_OUTPUT + MULTCOUNT
	};
	
	enum LightIds {
		MUTEOFF_LIGHT,
		NUM_LIGHTS = MUTEOFF_LIGHT + MULTCOUNT
	};

	bool state[MULTCOUNT] {};
	SchmittTrigger muteTrigger[MULTCOUNT];

	Multy() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		reset();
	}
	void step() override;

	void reset() override {
		for (int i = 0; i < MULTCOUNT; i++) {
			state[i] = true;
		}
	}
	
	void randomize() override {
		for (int i = 0; i < MULTCOUNT; i++) {
			state[i] = (randomf() < 0.5);
		}
	}
	// MUTE states
	json_t *toJson() override {
		json_t *rootJ = json_object();
		// states
		json_t *statesJ = json_array();
		for (int i = 0; i < MULTCOUNT; i++) {
			json_t *stateJ = json_boolean(state[i]);
			json_array_append_new(statesJ, stateJ);
		}
		json_object_set_new(rootJ, "states", statesJ);
		return rootJ;
	}
	void fromJson(json_t *rootJ) override {
		// states
		json_t *statesJ = json_object_get(rootJ, "states");
		if (statesJ) {
			for (int i = 0; i < MULTCOUNT; i++) {
				json_t *stateJ = json_array_get(statesJ, i);
				if (stateJ)
					state[i] = json_boolean_value(stateJ);
			}
		}
	}

};


void Multy::step() {
	for (int i = 0; i < MULTCOUNT; i++) {
		float in = inputs[MULT_INPUT].value;
		if (muteTrigger[i].process(params[MUTE_PARAM + i].value))
			state[i] ^= true;
		
		outputs[MULT_OUTPUT + i].value = state[i] ? in : 0.0;
		lights[MUTEOFF_LIGHT + i].setBrightness(state[i] ? 1.0 : 0.0);
	}

}

template <typename BASE>
 struct MuteLight : BASE {
 	MuteLight() {
 		this->box.size = mm2px(Vec(1.8, 1.8));
		this->bgColor = nvgRGBAf(0.998, 0.0133, 0.0133, 0.75);
 	}
 };

struct SquareButton : SVGSwitch, MomentarySwitch {
	SquareButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/SquareButton.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/SquareButtonD.svg")));
	}
};


MultyWidget::MultyWidget() {
	Multy *module = new Multy();
	setModule(module);
	box.size = Vec(2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel2HE.svg")));
		addChild(panel);
	}
	
	// SCREWS
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	
	// OUTPUTS, BUTTONS, LIGHTS
	int y_pad = 51;
	for (int i = 0; i < MULTCOUNT ; i++) {  
    	addOutput(createOutput<OutPortVal>(Vec(4, 22 + y_pad * i), module, Multy::MULT_OUTPUT + i));
    	addParam(createParam<SquareButton>(Vec(6, 47 + y_pad * i), module, Multy::MUTE_PARAM + i, 0.0, 1.0, 0.0));
    	addChild(createLight<MuteLight<WhiteLight>>(Vec(9, 50 + y_pad * i), module, Multy::MUTEOFF_LIGHT + i));
  	}
	// INPUT
	addInput(createInput<InPortAud>(Vec(4, box.size.y - 44), module, Multy::MULT_INPUT));
}