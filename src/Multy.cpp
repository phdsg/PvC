
// MU[L]TY
// 1 to 10 Multiple with mutes for each output
// modified tutorial.cpp 
// with stolen snippets from fundamental/mutes

#include "pvc.hpp"
#include "dsp/digital.hpp"

struct Multy : Module {
	enum ParamIds {
		MUTE_PARAM,
		NUM_PARAMS = MUTE_PARAM + 10
	};
	enum InputIds {
		MULT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		MULT_OUTPUT,
		NUM_OUTPUTS = MULT_OUTPUT + 10
	};
	enum LightIds {
		MUTE_LIGHT,
		NUM_LIGHTS = MUTE_LIGHT + 10
	};

	bool state[10];
	SchmittTrigger muteTrigger[10];

	Multy() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		reset();
	}
	void step() override;

	void reset() override {
		for (int i = 0; i < 10; i++) {
			state[i] = true;
		}
	}
	void randomize() override {
		for (int i = 0; i < 10; i++) {
			state[i] = (randomf() < 0.5);
		}
	}

	// MUTE LED STATES (stolen from fundamental/mutes)
	json_t *toJson() override {
		json_t *rootJ = json_object();
		// states
		json_t *statesJ = json_array();
		for (int i = 0; i < 10; i++) {
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
			for (int i = 0; i < 10; i++) {
				json_t *stateJ = json_array_get(statesJ, i);
				if (stateJ)
					state[i] = json_boolean_value(stateJ);
			}
		}
	}

};


void Multy::step() {
	// also taken from fundamental/mutes
	for (int i = 0; i < 10; i++) {
		if (muteTrigger[i].process(params[MUTE_PARAM + i].value))
			state[i] ^= true;
		float in = inputs[MULT_INPUT].value;
		outputs[MULT_OUTPUT + i].value = state[i] ? in : 0.0;
		lights[MUTE_LIGHT + i].setBrightness(state[i] ? 0.9 : 0.0);
	}

}

template <typename BASE>
struct MuteLight : BASE {
	MuteLight() {
		this->box.size = Vec(16.0, 16.0);
	}
};

MultyWidget::MultyWidget() {
	Multy *module = new Multy();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Multy.svg")));
		addChild(panel);
	}

	// panel screws
	addChild(createScrew<ScrewSilver>(Vec(0, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(0, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 15, 365)));

	// add 10 Mute Buttons
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 67), module, Multy::MUTE_PARAM + 0, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 97), module, Multy::MUTE_PARAM + 1, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 127), module, Multy::MUTE_PARAM + 2, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 157), module, Multy::MUTE_PARAM + 3, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 187), module, Multy::MUTE_PARAM + 4, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 217), module, Multy::MUTE_PARAM + 5, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 247), module, Multy::MUTE_PARAM + 6, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 277), module, Multy::MUTE_PARAM + 7, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 307), module, Multy::MUTE_PARAM + 8, 0.0, 1.0, 0.0));
	addParam(createParam<LEDBezel>(Vec(box.size.x - 55, 337), module, Multy::MUTE_PARAM + 9, 0.0, 1.0, 0.0));

	// add one input
	//TODO: (visual) create colored ports
	addInput(createInput<PJ301MPort>(Vec(box.size.x - 56, 36), module, Multy::MULT_INPUT));

	// add 10 outs
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 65), module, Multy::MULT_OUTPUT + 0));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 95), module, Multy::MULT_OUTPUT + 1));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 125), module, Multy::MULT_OUTPUT + 2));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 155), module, Multy::MULT_OUTPUT + 3));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 185), module, Multy::MULT_OUTPUT + 4));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 215), module, Multy::MULT_OUTPUT + 5));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 245), module, Multy::MULT_OUTPUT + 6));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 275), module, Multy::MULT_OUTPUT + 7));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 305), module, Multy::MULT_OUTPUT + 8));
	addOutput(createOutput<PJ301MPort>(Vec(box.size.x - 30, 335), module, Multy::MULT_OUTPUT + 9));

	// add the mute lights
	// TODO: switching from red to green instead of just green on/off
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 70), module, Multy::MUTE_LIGHT + 0));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 100), module, Multy::MUTE_LIGHT + 1));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 130), module, Multy::MUTE_LIGHT + 2));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 160), module, Multy::MUTE_LIGHT + 3));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 190), module, Multy::MUTE_LIGHT + 4));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 220), module, Multy::MUTE_LIGHT + 5));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 250), module, Multy::MUTE_LIGHT + 6));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 280), module, Multy::MUTE_LIGHT + 7));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 310), module, Multy::MUTE_LIGHT + 8));
	addChild(createLight<MuteLight<GreenLight>>(Vec(box.size.x - 52, 340), module, Multy::MUTE_LIGHT + 9));

}
