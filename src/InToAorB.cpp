/*

InToAorB

*/////////////////////////////////////////////////////////////////////////////



#include "pvc.hpp"
#include "dsp/digital.hpp"


struct InToAorB : Module {
	enum ParamIds {
		PROB_UI,

		NUM_PARAMS
	};
	enum InputIds {
		SIGNAL_IN,
		GATE_IN,
		FLIP_IN,
		TOSS_IN,
		PROB_CV,

		NUM_INPUTS
	};
	enum OutputIds {
		SIGNAL_HI_OUT,
		SIGNAL_LO_OUT,
		
		GATE_HI_OUT,
		GATE_LO_OUT,
		
		NUM_OUTPUTS
	};
	enum LightIds {
		HI_LED,
		LO_LED,

		NUM_LIGHTS
	};

	bool gate = false;
	bool flipped = false;
	
	SchmittTrigger tossTrigger;
	SchmittTrigger flipTrigger;

	InToAorB() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
};



void InToAorB::step() {

	gate = (inputs[GATE_IN].normalize(false) > 0) ? true : false;

	if (flipTrigger.process(inputs[FLIP_IN].value)) {
		flipped = !flipped;
	}

	if (inputs[TOSS_IN].active) {
		if (tossTrigger.process(inputs[TOSS_IN].value)) {
			flipped = (randomf() < params[PROB_UI].value + inputs[PROB_CV].value);
			// gate = (randomf() < params[PROB_UI].value + inputs[PROB_CV].value) ? !gate : gate;
		}
	}

	gate = (flipped) ? !gate : gate;

	outputs[SIGNAL_HI_OUT].value = gate * inputs[SIGNAL_IN].value;
	outputs[GATE_HI_OUT].value = gate * 10.0f;
	outputs[SIGNAL_LO_OUT].value = !gate * inputs[SIGNAL_IN].value;
	outputs[GATE_LO_OUT].value = !gate * 10.0f;
	lights[HI_LED].value = gate;
	lights[LO_LED].value = !gate;
}

template <typename BASE>
 struct FourPixLight : BASE {
 	FourPixLight() {
 		this->box.size = Vec(4, 4);
 	}
 };

InToAorBWidget::InToAorBWidget() {
	InToAorB *module = new InToAorB();
	setModule(module);
	box.size = Vec(15*2, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/InToAorB.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22),module, InToAorB::SIGNAL_IN));

	addParam(createParam<PvCKnob>(Vec(4,60),module, InToAorB::PROB_UI, 0.0f, 1.0f, 0.5f));
	addInput(createInput<InPortCtrl>(Vec(4,84),module, InToAorB::PROB_CV));
	addInput(createInput<InPortBin>(Vec(4,120),module, InToAorB::TOSS_IN));
	addInput(createInput<InPortBin>(Vec(4,158),module, InToAorB::FLIP_IN));
	addInput(createInput<InPortBin>(Vec(4,196),module, InToAorB::GATE_IN));

	addChild(createLight<FourPixLight<WhiteLight>>(Vec(13,243),module, InToAorB::HI_LED));
	addOutput(createOutput<OutPortVal>(Vec(4,248),module, InToAorB::SIGNAL_HI_OUT));
	addOutput(createOutput<OutPortBin>(Vec(4,272),module, InToAorB::GATE_HI_OUT));

	addChild(createLight<FourPixLight<WhiteLight>>(Vec(13,307),module, InToAorB::LO_LED));
	addOutput(createOutput<OutPortVal>(Vec(4,312),module, InToAorB::SIGNAL_LO_OUT));
	addOutput(createOutput<OutPortBin>(Vec(4,336),module, InToAorB::GATE_LO_OUT));
}