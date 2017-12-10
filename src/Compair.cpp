// Dual Window Comparator
//
// inspired by Joranalogue Compare2
// 
//////////////////////////////////////////////////////////////////////////////

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

	outputs[GATE_A_OUT].value = outA ? 5.0f : 0.0f;
	outputs[NOT_A_OUT].value = !outA ? 5.0f : 0.0f;

	outputs[GATE_B_OUT].value = outB ? 5.0f : 0.0f;
	outputs[NOT_B_OUT].value = !outB ? 5.0f : 0.0f;
		
	outputs[AND_OUT].value = (outA && outB) ? 5.0f : 0.0f;
	outputs[OR_OUT].value = (outA || outB) ? 5.0f : 0.0f;
	outputs[XOR_OUT].value = (outA != outB) ? 5.0f : 0.0f;
	if (flipTrigger.process(outputs[XOR_OUT].value))
		flip = !flip;
	outputs[FLIP_OUT].value = flip ? 5.0f : 0.0f;
//	

}

CompairWidget::CompairWidget(){
	Compair *module = new Compair();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel4HE.svg")));
		addChild(panel);
	}
	// SCREWS
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// A
	addInput(createInput<InPort>(Vec(6,22),module,Compair::AUDIO_A_IN));
	addParam(createParam<PvCKnob>(Vec(6,48), module, Compair::POS_A_PARAM, -5.f , 5.f, 0));
	addInput(createInput<ModInPort>(Vec(6,74),module,Compair::POS_A_IN));
	addParam(createParam<PvCKnob>(Vec(6,104), module, Compair::WIDTH_A_PARAM, 0.01f , 10.f, 5.f));
	addInput(createInput<ModInPort>(Vec(6,130),module,Compair::WIDTH_A_IN));
	addOutput(createOutput<OutPort>(Vec(6,170),module,Compair::GATE_A_OUT));
	addOutput(createOutput<OutPort>(Vec(6,200),module,Compair::NOT_A_OUT));
	// B
	addInput(createInput<InPort>(Vec(32,22),module,Compair::AUDIO_B_IN));
	addParam(createParam<PvCKnob>(Vec(32,48), module, Compair::POS_B_PARAM, -5.f , 5.f, 0));
	addInput(createInput<ModInPort>(Vec(32,74),module,Compair::POS_B_IN));
	addParam(createParam<PvCKnob>(Vec(32,104), module, Compair::WIDTH_B_PARAM, 0.01f , 10.f, 5.f));
	addInput(createInput<ModInPort>(Vec(32,130),module,Compair::WIDTH_B_IN));
	addOutput(createOutput<OutPort>(Vec(32,170),module,Compair::GATE_B_OUT));
	addOutput(createOutput<OutPort>(Vec(32,200),module,Compair::NOT_B_OUT));
	// LOGIC
	addOutput(createOutput<OutPort>(Vec(19,240),module,Compair::AND_OUT));
	addOutput(createOutput<OutPort>(Vec(19,270),module,Compair::OR_OUT));
	addOutput(createOutput<OutPort>(Vec(19,300),module,Compair::XOR_OUT));
	addOutput(createOutput<OutPort>(Vec(19,330),module,Compair::FLIP_OUT));
}