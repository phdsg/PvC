/*

InToAorB

TODO: toss mode

*/////////////////////////////////////////////////////////////////////////////



#include "pvc.hpp"
#include "dsp/digital.hpp"


struct InToAorB : Module {
	enum ParamIds {
		PROB_UI,
		TOSS_MODE,
		TOSS_UI,
		FLIP_UI,
		SET_A_UI,
		SET_B_UI,

		NUM_PARAMS
	};
	enum InputIds {
		SIG_IN,
		PROB_CV,
		TOSS_IN,
		FLIP_IN,
		SET_A_IN,
		SET_B_IN,

		NUM_INPUTS
	};
	enum OutputIds {
		SIG_A_OUT,
		SIG_B_OUT,
		GATE_A_OUT,
		GATE_B_OUT,
		TRIG_A_OUT,
		TRIG_B_OUT,
		
		NUM_OUTPUTS
	};
	enum LightIds {
		A_LED,
		B_LED,

		DIR_LED,
		FLP_LED,

		NUM_LIGHTS
	};

	bool gate = false;
	bool flipMode = false;
	
	SchmittTrigger tossTrigger;
	SchmittTrigger flipTrigger;
	SchmittTrigger resetTrigger;
	PulseGenerator gatePulse;
	
	InToAorB() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;

	void reset() override {
		gate = false;
		flipMode = false;
	}

	json_t *toJson() override {
		json_t *rootJ = json_object();
		
		json_t *gateStateJ = json_boolean(gate);
		json_object_set_new(rootJ, "gateState", gateStateJ);

		return rootJ;
	}
	
	void fromJson(json_t *rootJ) override {
		json_t *gateStateJ = json_object_get(rootJ, "gateState");
		if (gateStateJ)
			gate = json_boolean_value(gateStateJ);
	}
};



void InToAorB::step() {
	flipMode = params[TOSS_MODE].value;

	if (tossTrigger.process(inputs[TOSS_IN].value + params[TOSS_UI].value)) {
			gatePulse.trigger(0.01f);
			if (flipMode)
				gate = (randomf() < params[PROB_UI].value + inputs[PROB_CV].value) ? !gate : gate; //
			else
				gate = (randomf() < (params[PROB_UI].value + inputs[PROB_CV].value*0.1f));
			
	}


	if (flipTrigger.process(inputs[FLIP_IN].value + params[FLIP_UI].value)) {
		gatePulse.trigger(0.01f);
		gate = !gate;
	}

	if (resetTrigger.process(inputs[SET_A_IN].value + params[SET_A_UI].value)) {
		gatePulse.trigger(0.01f);
		gate = false;
	}

	if (resetTrigger.process(inputs[SET_B_IN].value + params[SET_B_UI].value)) {
		gatePulse.trigger(0.01f);
		gate = true;
	}

	outputs[SIG_A_OUT].value = !gate * inputs[SIG_IN].value;
	outputs[GATE_A_OUT].value = !gate * 10.0f;
	outputs[TRIG_A_OUT].value = !gate * gatePulse.process(1.0/engineGetSampleRate()) * 10.0f;
	outputs[SIG_B_OUT].value = gate * inputs[SIG_IN].value;
	outputs[GATE_B_OUT].value = gate * 10.0f;
	outputs[TRIG_B_OUT].value = gate * gatePulse.process(1.0/engineGetSampleRate()) * 10.0f;
	lights[A_LED].value = !gate;
	lights[B_LED].value = gate;
	lights[DIR_LED].value = !flipMode;
	lights[FLP_LED].value = flipMode;
}

struct ModeToggle : SVGSwitch, ToggleSwitch {
	ModeToggle() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(12,6);
	}
};
struct LabelButtonL : SVGSwitch, MomentarySwitch {
	LabelButtonL() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/LabelButtonL_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/LabelButtonL_1.svg")));
		box.size = Vec(36,12);
	}
};
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


InToAorBWidget::InToAorBWidget() {
	InToAorB *module = new InToAorB();
	setModule(module);
	box.size = Vec(15*4, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/InToAorB.svg")));
		addChild(panel);
	}
	// screws
	// addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 30, 0)));
	addChild(createScrew<ScrewHead3>(Vec(15, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(19,22), module, InToAorB::SIG_IN));

	addParam(createParam<PvCKnob>(Vec(19,64), module, InToAorB::PROB_UI, 0.0f, 1.0f, 0.5f));
	addInput(createInput<InPortCtrl>(Vec(19,88), module, InToAorB::PROB_CV));
	addInput(createInput<InPortBin>(Vec(19,124), module, InToAorB::TOSS_IN));
	addParam(createParam<LabelButtonL>(Vec(12,149), module, InToAorB::TOSS_UI, 0, 1, 0));
	addChild(createLight<FourPixLight<OrangeLight>>(Vec(25,165),module, InToAorB::DIR_LED));
	addChild(createLight<FourPixLight<BlueLight>>(Vec(31,165),module, InToAorB::FLP_LED));
	addParam(createParam<ModeToggle>(Vec(24,164), module, InToAorB::TOSS_MODE, 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(19,180), module, InToAorB::FLIP_IN));
	addParam(createParam<LabelButtonL>(Vec(12,205), module, InToAorB::FLIP_UI, 0, 1, 0));
	
	addInput(createInput<InPortBin>(Vec(4,222),module, InToAorB::SET_A_IN));
	addParam(createParam<LabelButtonS>(Vec(3,247), module, InToAorB::SET_A_UI, 0, 1, 0));
	addChild(createLight<FourPixLight<CyanLight>>(Vec(13,267),module, InToAorB::A_LED));
	addOutput(createOutput<OutPortVal>(Vec(4,276),module, InToAorB::SIG_A_OUT));
	addOutput(createOutput<OutPortBin>(Vec(4,312),module, InToAorB::GATE_A_OUT));
	addOutput(createOutput<OutPortBin>(Vec(4,336),module, InToAorB::TRIG_A_OUT));

	addInput(createInput<InPortBin>(Vec(34,222),module, InToAorB::SET_B_IN));
	addParam(createParam<LabelButtonS>(Vec(33,247), module, InToAorB::SET_B_UI, 0, 1, 0));
	addChild(createLight<FourPixLight<PurpleLight>>(Vec(43,267),module, InToAorB::B_LED));
	addOutput(createOutput<OutPortVal>(Vec(34,276),module, InToAorB::SIG_B_OUT));
	addOutput(createOutput<OutPortBin>(Vec(34,312),module, InToAorB::GATE_B_OUT));
	addOutput(createOutput<OutPortBin>(Vec(34,336),module, InToAorB::TRIG_B_OUT));
}