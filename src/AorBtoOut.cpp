/*

AorBtoOut


*/////////////////////////////////////////////////////////////////////////////



#include "pvc.hpp"
#include "dsp/digital.hpp"


struct AorBtoOut : Module {
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
		SIG_A_IN,
		SIG_B_IN,
		PROB_CV,
		TOSS_IN,
		FLIP_IN,
		SET_A_IN,
		SET_B_IN,

		NUM_INPUTS
	};
	enum OutputIds {
		SIG_OUT,
				
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
	
	AorBtoOut() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

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



void AorBtoOut::step() {
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

	outputs[SIG_OUT].value = !gate ? inputs[SIG_A_IN].value : inputs[SIG_B_IN].value;
	outputs[GATE_A_OUT].value = !gate * 10.0f;
	outputs[TRIG_A_OUT].value = !gate * gatePulse.process(1.0/engineGetSampleRate()) * 10.0f;
	outputs[GATE_B_OUT].value = gate * 10.0f;
	outputs[TRIG_B_OUT].value = gate * gatePulse.process(1.0/engineGetSampleRate()) * 10.0f;
	lights[A_LED].value = !gate;
	lights[B_LED].value = gate;
	lights[DIR_LED].value = !flipMode;
	lights[FLP_LED].value = flipMode;
}
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
struct ModeToggle : SVGSwitch, ToggleSwitch {
	ModeToggle() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		// addFrame(SVG::load(assetPlugin(plugin, "res/components/CompairToggleDn.svg")));
		box.size = Vec(12,6);
	}
};
template <typename BASE>
 struct FourPixLight : BASE {
 	FourPixLight() {
 		this->box.size = Vec(4, 4);
 	}
 };

AorBtoOutWidget::AorBtoOutWidget() {
	AorBtoOut *module = new AorBtoOut();
	setModule(module);
	box.size = Vec(15*4, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/AorBtoOut.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(15, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(30, 365)));
	// addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22),module, AorBtoOut::SIG_A_IN));
	addInput(createInput<InPortAud>(Vec(34,22),module, AorBtoOut::SIG_B_IN));
	
	addParam(createParam<PvCKnob>(Vec(19,64),module, AorBtoOut::PROB_UI, 0.0f, 1.0f, 0.5f));
	addInput(createInput<InPortCtrl>(Vec(19,88),module, AorBtoOut::PROB_CV));
	addInput(createInput<InPortBin>(Vec(19,124),module, AorBtoOut::TOSS_IN));
	addParam(createParam<LabelButtonL>(Vec(12,149), module, AorBtoOut::TOSS_UI, 0, 1, 0));
	addChild(createLight<FourPixLight<OrangeLight>>(Vec(25,165),module, AorBtoOut::DIR_LED));
	addChild(createLight<FourPixLight<BlueLight>>(Vec(31,165),module, AorBtoOut::FLP_LED));
	addParam(createParam<ModeToggle>(Vec(24,164), module, AorBtoOut::TOSS_MODE, 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(19,180),module, AorBtoOut::FLIP_IN));
	addParam(createParam<LabelButtonL>(Vec(12,205), module, AorBtoOut::FLIP_UI, 0, 1, 0));
		
	addInput(createInput<InPortBin>(Vec(4,222),module, AorBtoOut::SET_A_IN));
	addParam(createParam<LabelButtonS>(Vec(3,247), module, AorBtoOut::SET_A_UI, 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(34,222),module, AorBtoOut::SET_B_IN));
	addParam(createParam<LabelButtonS>(Vec(33,247), module, AorBtoOut::SET_B_UI, 0, 1, 0));

	addOutput(createOutput<OutPortVal>(Vec(19,276),module, AorBtoOut::SIG_OUT));

	addChild(createLight<FourPixLight<CyanLight>>(Vec(13,267),module, AorBtoOut::A_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,312),module, AorBtoOut::GATE_A_OUT));
	addOutput(createOutput<OutPortBin>(Vec(4,336),module, AorBtoOut::TRIG_A_OUT));

	addChild(createLight<FourPixLight<PurpleLight>>(Vec(43,267),module, AorBtoOut::B_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,312),module, AorBtoOut::GATE_B_OUT));
	addOutput(createOutput<OutPortBin>(Vec(34,336),module, AorBtoOut::TRIG_B_OUT));
}