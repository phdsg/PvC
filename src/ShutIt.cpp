/*
ShutIt

*/////////////////////////////////////////////////////////////////////////////
#include "pvc.hpp"

#include "dsp/digital.hpp"

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
			muteState[i] = (randomUniform() < 0.5);
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
	if (muteAllTrig.process(inputs[SHUT_ALL_TRIG].value + params[SHUT_ALL].value)) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = true;
		}
	}
	if (unmuteAllTrig.process(inputs[OPEN_ALL_TRIG].value + params[OPEN_ALL].value)) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = false;
		}
	}
	if (flipAllTrig.process(inputs[FLIP_ALL_TRIG].value + params[FLIP_ALL].value)) {
		for (int i = 0; i < CHANCOUNT; i++)	{
			muteState[i] = !muteState[i];
		}
	}
}

struct LabelButtonS : SVGSwitch, MomentarySwitch {
	LabelButtonS() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/LabelButtonS_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/LabelButtonS_1.svg")));

		box.size = Vec(24,12);
	}
};
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

struct ShutItWidget : ModuleWidget {
	ShutItWidget(ShutIt *module);
};

ShutItWidget::ShutItWidget(ShutIt *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/panels/ShutIt.svg")));
	
	// screws
	addChild(Widget::create<ScrewHead1>(Vec(0, 0)));
	addChild(Widget::create<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(Widget::create<ScrewHead3>(Vec(0, 365)));
	addChild(Widget::create<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// channels
	for (int i = 0; i < CHANCOUNT; i++) {
		float top = 36.0f;

		addChild(ModuleLightWidget::create<FourPixLight<WhiteRedLight>>(Vec(79,27 + top*i), module, ShutIt::A_STATE + 2*i));
		addParam(ParamWidget::create<EmptyButton>(Vec(2,19 + top*i), module, ShutIt::A_MUTE + i, 0, 1, 0));
		addInput(Port::create<InPortAud>(Vec(4,26 + top*i), Port::INPUT, module, ShutIt::A_IN + i));
		addInput(Port::create<InPortBin>(Vec(28,26 + top*i), Port::INPUT, module, ShutIt::A_TRIG + i));
		addOutput(Port::create<OutPortVal>(Vec(52,26 + top*i), Port::OUTPUT, module, ShutIt::A_OUT + i));
	}
	addInput(Port::create<InPortBin>(Vec(4,322), Port::INPUT, module, ShutIt::SHUT_ALL_TRIG));
  addParam(ParamWidget::create<LabelButtonS>(Vec(3,347), module, ShutIt::SHUT_ALL, 0, 1, 0));
	addInput(Port::create<InPortBin>(Vec(34,322), Port::INPUT, module, ShutIt::FLIP_ALL_TRIG));
  addParam(ParamWidget::create<LabelButtonS>(Vec(33,347), module, ShutIt::FLIP_ALL, 0, 1, 0));
	addInput(Port::create<InPortBin>(Vec(64,322), Port::INPUT, module, ShutIt::OPEN_ALL_TRIG));
  addParam(ParamWidget::create<LabelButtonS>(Vec(63,347), module, ShutIt::OPEN_ALL, 0, 1, 0));
  
}

Model *modelShutIt = Model::create<ShutIt, ShutItWidget>("PvC", "ShutIt", "ShutIt", LOGIC_TAG, SWITCH_TAG);

