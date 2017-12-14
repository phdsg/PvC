/*

COMPAIR

Dual Window Comparator (inspired by Joranalogue Compare2)

checks if an input voltage is between two values.
compare window is set by two parameters: position and width.
top knob on each channel (position) sets a center voltage of +/-5V.
the other one changes the width around the center from near zero to 10V.
so with both knobs at center the window is set at [-2.5..2.5].
both parameters are also controllable via cv input.
cv and knob values are added together when computing the window.
whenever the input signal is within that window the GATE output on that
channel will go from 0V to +5V and the NOT output will do the opposite.
logic outputs at the bottom compare the output values of both channels.
AND output goes high if A is high and B is high
OR output goes high if A or B is high
XOR output goes high if either A or B ar high and the other one is low.
FLIP output changes whenever the XOR out goes high.
channel B inputs are normalized to channel A inputs.
channel output to the logic section can be inverted.
all outputs are 0V-5V.

TODO:
 -more sanity and functionality tests!
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
		INVERT_A_PARAM,
		INVERT_B_PARAM,
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
		OVER_A_LED,
		BELOW_A_LED,
		OVER_B_LED,
		BELOW_B_LED,
		NUM_LIGHTS
	};

	bool flip = false;
	SchmittTrigger flipTrigger;
	bool outA = false;
	bool outB = false;

	Compair() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		// reset();
	}

	void step() override;
	// void reset() override;
	// void randomize() override;
};

void Compair::step(){

	float inputA = inputs[AUDIO_A_IN].value;
	float inputB = inputs[AUDIO_B_IN].normalize(inputA);

	float posA = params[POS_A_PARAM].value;
	float widthA = params[WIDTH_A_PARAM].value;
	float posB = params[POS_B_PARAM].value;
	float widthB = params[WIDTH_B_PARAM].value;;

	// A Controls	
	if (inputs[POS_A_IN].active)
		posA = (params[POS_A_PARAM].value + inputs[POS_A_IN].value);
		
	if (inputs[WIDTH_A_IN].active)
		widthA = (params[WIDTH_A_PARAM].value + inputs[WIDTH_A_IN].value);
	
	// A Window
	float upperThreshA = posA + widthA*0.5f;
	float lowerThreshA = posA - widthA*0.5f;
	
	if ( inputA < upperThreshA && inputA > lowerThreshA) {
		outA = true;
	} else {
		outA = false;
	}
		
	// B Controls	
	if (inputs[POS_B_IN].active || inputs[POS_A_IN].active)
		posB = (params[POS_B_PARAM].value + inputs[POS_B_IN].normalize(inputs[POS_A_IN].value));
	
	if (inputs[WIDTH_B_IN].active || inputs[WIDTH_B_IN].active)
		widthB = (params[WIDTH_B_PARAM].value + inputs[WIDTH_B_IN].normalize(inputs[WIDTH_A_IN].value));

	// B Window
	float upperThreshB = posB + widthB*0.5f;
	float lowerThreshB = posB - widthB*0.5f;
	
	if ( inputB < upperThreshB && inputB > lowerThreshB) {
		outB = true;
	} else {
		outB = false;
	}
	
	// Lights and Outputs
	lights[OVER_A_LED].value = (inputA > upperThreshA) ? (inputA - upperThreshA) : 0.0f;
	lights[BELOW_A_LED].value = (inputA < lowerThreshA) ? (lowerThreshA - inputA) : 0.0f;

	lights[OVER_B_LED].value = (inputB > upperThreshB) ? (inputB - upperThreshB) : 0.0f;
	lights[BELOW_B_LED].value = (inputB < lowerThreshB) ? (lowerThreshB - inputB) : 0.0f;

	outputs[GATE_A_OUT].value = lights[GATE_A_LED].value = outA ? 5.0f : 0.0f;
	outputs[NOT_A_OUT].value = lights[NOT_A_LED].value = !outA ? 5.0f : 0.0f;

	outputs[GATE_B_OUT].value = lights[GATE_B_LED].value = outB ? 5.0f : 0.0f;
	outputs[NOT_B_OUT].value = lights[NOT_B_LED].value = !outB ? 5.0f : 0.0f;

	if (params[INVERT_A_PARAM].value)
		outA = !outA;
	if (params[INVERT_B_PARAM].value)
		outB = !outB;
		
	outputs[AND_OUT].value = lights[AND_LED].value = (outA && outB) ? 5.0f : 0.0f;
	outputs[OR_OUT].value = lights[OR_LED].value = (outA || outB) ? 5.0f : 0.0f;
	outputs[XOR_OUT].value = lights[XOR_LED].value = (outA != outB) ? 5.0f : 0.0f;
	if (flipTrigger.process(outputs[XOR_OUT].value))
		flip = !flip;
	outputs[FLIP_OUT].value = lights[FLIP_LED].value = flip ? 5.0f : 0.0f;
}

CompairWidget::CompairWidget(){
	Compair *module = new Compair();
	setModule(module);
	box.size = Vec(8 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/Compair.svg")));
		addChild(panel);
	}
	// SCREWS
	addChild(createScrew<ScrewHead1>(Vec(15, 0)));
	//addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(15, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 30, 365)));

	// A
	addInput(createInput<InPort>(Vec(21,230),module,Compair::AUDIO_A_IN));
	addParam(createParam<PvCBigKnob>(Vec(10,60), module, Compair::POS_A_PARAM, -5.f , 5.f, 0));
	addInput(createInput<ModInPort>(Vec(7,190),module,Compair::POS_A_IN));
	addParam(createParam<PvCBigKnob>(Vec(10,128), module, Compair::WIDTH_A_PARAM, 0.01f , 10.f, 5.f));
	addInput(createInput<ModInPort>(Vec(35,190),module,Compair::WIDTH_A_IN));
	addOutput(createOutput<OutPort>(Vec(7,274),module,Compair::GATE_A_OUT));
	addChild(createLight<TinyLight<CyanLight>>(Vec(14,239),module,Compair::BELOW_A_LED));
	addChild(createLight<TinyLight<YellowLight>>(Vec(16,269),module,Compair::GATE_A_LED));
	addChild(createLight<TinyLight<BlueLight>>(Vec(46,239),module,Compair::OVER_A_LED));
	addOutput(createOutput<OutPort>(Vec(35,274),module,Compair::NOT_A_OUT));
	addChild(createLight<TinyLight<RedLight>>(Vec(44,269),module,Compair::NOT_A_LED));
	// B
	addInput(createInput<InPort>(Vec(77,230),module,Compair::AUDIO_B_IN));
	addParam(createParam<PvCBigKnob>(Vec(66,60), module, Compair::POS_B_PARAM, -5.f , 5.f, 0));
	addInput(createInput<ModInPort>(Vec(90,190),module,Compair::POS_B_IN));
	addParam(createParam<PvCBigKnob>(Vec(66,128), module, Compair::WIDTH_B_PARAM, 0.01f , 10.f, 5.f));
	addInput(createInput<ModInPort>(Vec(63,190),module,Compair::WIDTH_B_IN));
	addOutput(createOutput<OutPort>(Vec(90,274),module,Compair::GATE_B_OUT));
	addChild(createLight<TinyLight<CyanLight>>(Vec(70,239),module,Compair::BELOW_B_LED));
	addChild(createLight<TinyLight<YellowLight>>(Vec(100,269),module,Compair::GATE_B_LED));
	addChild(createLight<TinyLight<BlueLight>>(Vec(102,239),module,Compair::OVER_B_LED));
	addOutput(createOutput<OutPort>(Vec(63,274),module,Compair::NOT_B_OUT));
	addChild(createLight<TinyLight<RedLight>>(Vec(72,269),module,Compair::NOT_B_LED));

	// LOGIC
	addParam(createParam<PvCToggle>(Vec(26,254),module,Compair::INVERT_A_PARAM, 0, 1, 0));
	addParam(createParam<PvCToggle>(Vec(81,254),module,Compair::INVERT_B_PARAM, 0, 1, 0));
	addOutput(createOutput<OutPort>(Vec(7,324),module,Compair::AND_OUT));
	addChild(createLight<TinyLight<WhiteLight>>(Vec(16,319),module,Compair::AND_LED));
	addOutput(createOutput<OutPort>(Vec(35,324),module,Compair::OR_OUT));
	addChild(createLight<TinyLight<OrangeLight>>(Vec(44,319),module,Compair::OR_LED));
	addOutput(createOutput<OutPort>(Vec(63,324),module,Compair::XOR_OUT));
	addChild(createLight<TinyLight<YellowLight>>(Vec(72,319),module,Compair::XOR_LED));
	addOutput(createOutput<OutPort>(Vec(90,324),module,Compair::FLIP_OUT));
	addChild(createLight<TinyLight<GreenLight>>(Vec(100,319),module,Compair::FLIP_LED));
}