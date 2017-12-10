/*

Dual Window Comparator (inspired by Joranalogue Compare2)

checks if an input voltage is between two values.
compare window is set by two parameters: position and width.
top knob on each channel (position) sets a center voltage of +/-5V.
the other one changes the width around the center from near zero to 10V.
so with both knobs at center the window is set at [-2.5..2.5].
both parameters are also controllable with a cv input.
cv and knob values are added together when computing the window.

whenever the input signal is within that window the GATE output on that
channel will go from 0V to +5V and the NOT output will do the opposite.

logic outputs at the bottom compare the output values of both channels.
AND output goes high if A is high and B is high
OR output goes high if A or B is high
XOR output goes high if either A or B ar high and the other one is low.
FLIP output changes whenever the XOR out goes high

channel B inputs are normalized to channel A inputs.

all outputs are 0V-5V


TODO:
 -more sanity and functionality tests!
 -labels
 -more documentation?
 -set up demo patch ideas
 -code clean-up, optimization, simplification

*/

////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"
#include "dsp/digital.hpp"

struct Compair : Module {
	enum ParamIds {
		POS_A_PARAM,
		WIDTH_A_PARAM,
		POS_B_PARAM,
		WIDTH_B_PARAM,
	 	NUM_PARAMS
	};
	enum InputIds {
		AUDIO_A_IN,
		POS_A_IN,
		WIDTH_A_IN,
		AUDIO_B_IN,
		POS_B_IN,
		WIDTH_B_IN,
		NUM_INPUTS
	};
	enum OutputIds {
		GATE_A_OUT,
		NOT_A_OUT,
	 	GATE_B_OUT,
		NOT_B_OUT,
		AND_OUT,
		OR_OUT,
		XOR_OUT,
		FLIP_OUT,
	 	NUM_OUTPUTS
	};
	enum LightIds {
		GATE_A_LED,
		NOT_A_LED,
		GATE_B_LED,
		NOT_B_LED,
		AND_LED,
		OR_LED,
		XOR_LED,
		FLIP_LED,
		NUM_LIGHTS
	};

	bool flip = 0;
	SchmittTrigger flipTrigger;
	bool outA = 0;
	bool outB = 0;

	Compair() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		// reset();
	}
	void step() override;
	// void reset() override;
	// void randomize() override;
};

void Compair::step(){

	float inputA = inputs[AUDIO_A_IN].value;
	float posA_cv = inputs[POS_B_IN].value;
	float widthA_cv = inputs[WIDTH_B_IN].value;

	float inputB = inputs[AUDIO_B_IN].normalize(inputA);
	float posB_cv = inputs[POS_B_IN].normalize(posA_cv);
	float widthB_cv = inputs[WIDTH_B_IN].normalize(widthA_cv);
	
	// compute Controls	
	float posA = params[POS_A_PARAM].value;
	if (inputs[POS_A_IN].active)
		posA = (posA + posA_cv);

	float widthA = params[WIDTH_A_PARAM].value;
	if (inputs[WIDTH_A_IN].active)
		widthA = (widthA + widthA_cv);
	
	// compute Compare Window
	float upperThreshA = posA + widthA*0.5f;
	float lowerThreshA = posA - widthA*0.5f;
	
	if ( inputA < upperThreshA && inputA > lowerThreshA) {
		outA = 1;
	} else {
		outA = 0;
	}
	
	// compute Controls	
	float posB = params[POS_B_PARAM].value;
	if (inputs[POS_B_IN].active)
		posB = (posB + posB_cv);

	float widthB = params[WIDTH_B_PARAM].value;
	if (inputs[WIDTH_B_IN].active)
		widthB = (widthB + widthB_cv);
	
	// compute Compare Window
	float upperThreshB = posB + widthB*0.5f;
	float lowerThreshB = posB - widthB*0.5f;
	
	if ( inputB < upperThreshB && inputB > lowerThreshB) {
		outB = 1;
	} else {
		outB = 0;
	}

	outputs[GATE_A_OUT].value = lights[GATE_A_LED].value = outA ? 5.0f : 0.0f;
	outputs[NOT_A_OUT].value = lights[NOT_A_LED].value = !outA ? 5.0f : 0.0f;

	outputs[GATE_B_OUT].value = lights[GATE_B_LED].value = outB ? 5.0f : 0.0f;
	outputs[NOT_B_OUT].value = lights[NOT_B_LED].value = !outB ? 5.0f : 0.0f;
		
	outputs[AND_OUT].value = lights[AND_LED].value = (outA && outB) ? 5.0f : 0.0f;
	outputs[OR_OUT].value = lights[OR_LED].value = (outA || outB) ? 5.0f : 0.0f;
	outputs[XOR_OUT].value = lights[XOR_LED].value = (outA != outB) ? 5.0f : 0.0f;
	if (flipTrigger.process(outputs[XOR_OUT].value))
		flip = !flip;
	outputs[FLIP_OUT].value = lights[FLIP_LED].value = flip ? 5.0f : 0.0f;
//	

}

CompairWidget::CompairWidget(){
	Compair *module = new Compair();
	setModule(module);
	box.size = Vec(8 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel8HE.svg")));
		addChild(panel);
	}
	// SCREWS
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// A
	addInput(createInput<InPort>(Vec(21,22),module,Compair::AUDIO_A_IN));

	addParam(createParam<PvCBigKnob>(Vec(10,50), module, Compair::POS_A_PARAM, -5.f , 5.f, 0));
	addInput(createInput<ModInPort>(Vec(21,96),module,Compair::POS_A_IN));
	addParam(createParam<PvCBigKnob>(Vec(10,124), module, Compair::WIDTH_A_PARAM, 0.01f , 10.f, 5.f));
	addInput(createInput<ModInPort>(Vec(21,170),module,Compair::WIDTH_A_IN));

	addOutput(createOutput<OutPort>(Vec(8,206),module,Compair::GATE_A_OUT));
	addChild(createLight<TinyLight<YellowLight>>(Vec(17.5,202),module,Compair::GATE_A_LED));
	addOutput(createOutput<OutPort>(Vec(34,206),module,Compair::NOT_A_OUT));
	addChild(createLight<TinyLight<RedLight>>(Vec(43.5,202),module,Compair::NOT_A_LED));
	// B
	addInput(createInput<InPort>(Vec(77,22),module,Compair::AUDIO_B_IN));

	addParam(createParam<PvCBigKnob>(Vec(66,50), module, Compair::POS_B_PARAM, -5.f , 5.f, 0));
	addInput(createInput<ModInPort>(Vec(77,96),module,Compair::POS_B_IN));
	addParam(createParam<PvCBigKnob>(Vec(66,124), module, Compair::WIDTH_B_PARAM, 0.01f , 10.f, 5.f));
	addInput(createInput<ModInPort>(Vec(77,170),module,Compair::WIDTH_B_IN));

	addOutput(createOutput<OutPort>(Vec(64,206),module,Compair::GATE_B_OUT));
	addChild(createLight<TinyLight<YellowLight>>(Vec(73.5,202),module,Compair::GATE_B_LED));
	addOutput(createOutput<OutPort>(Vec(90,206),module,Compair::NOT_B_OUT));
	addChild(createLight<TinyLight<RedLight>>(Vec(99.5,202),module,Compair::NOT_B_LED));
	// LOGIC
	addOutput(createOutput<OutPort>(Vec(49,246),module,Compair::AND_OUT));
	addChild(createLight<TinyLight<WhiteLight>>(Vec(58.5,242),module,Compair::AND_LED));
	addOutput(createOutput<OutPort>(Vec(49,276),module,Compair::OR_OUT));
	addChild(createLight<TinyLight<OrangeLight>>(Vec(58.5,272),module,Compair::OR_LED));
	addOutput(createOutput<OutPort>(Vec(49,306),module,Compair::XOR_OUT));
	addChild(createLight<TinyLight<YellowLight>>(Vec(58.5,302),module,Compair::XOR_LED));
	addOutput(createOutput<OutPort>(Vec(49,336),module,Compair::FLIP_OUT));
	addChild(createLight<TinyLight<GreenLight>>(Vec(58.5,332),module,Compair::FLIP_LED));
}