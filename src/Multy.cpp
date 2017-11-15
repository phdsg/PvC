
// MU[L]TY
// 1 to 10 Multiple with mutes for each output
// modified tutorial.cpp 
// with stolen snippets from fundamental/mutes

#include "pvc.hpp"
#include "dsp/digital.hpp"

#define MULTCOUNT 10

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
		MUTE_LIGHT,
		NUM_LIGHTS = MUTE_LIGHT + MULTCOUNT
	};

	bool state[MULTCOUNT];
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

	// MUTE LED STATES (stolen from fundamental/mutes)
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
	// also taken from fundamental/mutes
	for (int i = 0; i < MULTCOUNT; i++) {
		if (muteTrigger[i].process(params[MUTE_PARAM + i].value))
			state[i] ^= true;
		float in = inputs[MULT_INPUT].value;
		outputs[MULT_OUTPUT + i].value = state[i] ? in : 0.0;
		lights[MUTE_LIGHT + i].setBrightness(state[i] ? 1.0 : 0.0);
	}

}

template <typename BASE>
struct MuteLight : BASE {
	MuteLight() {
		this->box.size = Vec(16.25, 16.25);
		this->bgColor = nvgRGBf(0.698, 0.133, 0.133);
		this->color = nvgRGBf(0.498, 1, 0);
	}
};

MultyWidget::MultyWidget() {
	Multy *module = new Multy();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/Multy.svg")));
		addChild(panel);
	}
	
	// panel screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// add input
	addInput(createInput<BluePort>(Vec(box.size.x - 56, 36), module, Multy::MULT_INPUT));
	// add buttons, outputs and lights
	int y_pad = 30;
	for (int i = 0; i < MULTCOUNT ; i++) {  
    	addParam(createParam<MuteBezel>(Vec(box.size.x - 55, 66 + y_pad * i), module, Multy::MUTE_PARAM + i, 0.0, 1.0, 0.0));
    	addOutput(createOutput<RedPort>(Vec(box.size.x - 30, 65 + y_pad * i), module, Multy::MULT_OUTPUT + i));
    	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 69 + y_pad * i), module, Multy::MUTE_LIGHT + i));
  	}
	
}