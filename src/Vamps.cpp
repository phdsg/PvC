// Vamps
//
// a 2 RackUnit mod of Andrew Belt's Fundamental VCA
// 	
// 
// dsp is all as in original, only widgets and layout changed.
// plus my hacky c/p job to give each unit a 2nd audio channel.
//
////////////////////////////////////////////////////////////////



#include "pvc.hpp"


struct Vamps : Module {
	enum ParamIds {
		LEVEL1_PARAM,
		LEVEL2_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CH1EXP_INPUT,
		CH1LIN_INPUT,
		CH1IN1_INPUT,
		CH1IN2_INPUT,
		CH2EXP_INPUT,
		CH2LIN_INPUT,
		CH2IN1_INPUT,
		CH2IN2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		CH1OUT1_OUTPUT,
		CH1OUT2_OUTPUT,
		CH2OUT1_OUTPUT,
		CH2OUT2_OUTPUT,
		NUM_OUTPUTS
	};

	Vamps() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};


static void stepChannel(Input &in1, Input &in2, Param &level, Input &lin, Input &exp, Output &out1, Output &out2) {
	float v1 = in1.value * level.value;
	float v2 = in2.value * level.value;
	const float expBase = 50.0;

	if (lin.active) {
		v1 *= clampf(lin.value / 10.0, 0.0, 1.0);
		v2 *= clampf(lin.value / 10.0, 0.0, 1.0);
	}
	if (exp.active) {
		v1 *= rescalef(powf(expBase, clampf(exp.value / 10.0, 0.0, 1.0)), 1.0, expBase, 0.0, 1.0);
		v2 *= rescalef(powf(expBase, clampf(exp.value / 10.0, 0.0, 1.0)), 1.0, expBase, 0.0, 1.0);
	}
	out1.value = v1;
	out2.value = v2;
}

void Vamps::step() {
	stepChannel(inputs[CH1IN1_INPUT], inputs[CH1IN2_INPUT], params[LEVEL1_PARAM], inputs[CH1LIN_INPUT], inputs[CH1EXP_INPUT], outputs[CH1OUT1_OUTPUT], outputs[CH1OUT2_OUTPUT]);
	stepChannel(inputs[CH2IN1_INPUT], inputs[CH2IN2_INPUT], params[LEVEL2_PARAM], inputs[CH2LIN_INPUT], inputs[CH2EXP_INPUT], outputs[CH2OUT1_OUTPUT], outputs[CH2OUT2_OUTPUT]);
}


VampsWidget::VampsWidget() {
	Vamps *module = new Vamps();
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
	
	// channel one
	addInput(createInput<InPort>(Vec(4, 22), module, Vamps::CH1IN1_INPUT));
	addInput(createInput<InPort>(Vec(4, 44), module, Vamps::CH1IN2_INPUT));

	addParam(createParam<PvCKnob>(Vec(4, 70), module, Vamps::LEVEL1_PARAM, 0.0, 1.0, 0.5));
	addInput(createInput<ModInPort>(Vec(4, 94), module, Vamps::CH1EXP_INPUT));
	addInput(createInput<ModInPort>(Vec(4, 116), module, Vamps::CH1LIN_INPUT));

	addOutput(createOutput<OutPort>(Vec(4, 142), module, Vamps::CH1OUT1_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(4, 164), module, Vamps::CH1OUT2_OUTPUT));

	//channel two
	addInput(createInput<InPort>(Vec(4, 22 + 172), module, Vamps::CH2IN1_INPUT));
	addInput(createInput<InPort>(Vec(4, 44 + 172), module, Vamps::CH2IN2_INPUT));

	addParam(createParam<PvCKnob>(Vec(4, 70 + 172), module, Vamps::LEVEL2_PARAM, 0.0, 1.0, 0.5));
	addInput(createInput<ModInPort>(Vec(4, 94 + 172), module, Vamps::CH2EXP_INPUT));
	addInput(createInput<ModInPort>(Vec(4, 116 + 172), module, Vamps::CH2LIN_INPUT));

	addOutput(createOutput<OutPort>(Vec(4, 142 + 172), module, Vamps::CH2OUT1_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(4, 164 + 172), module, Vamps::CH2OUT2_OUTPUT));
}
