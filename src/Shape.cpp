// shape.cpp
//
// work-in-progress waveshaping distortion
// 
//
// trying some waveshaping functions from musicdsp.org 
//
//////////////////////////////////////////////////////

#include "pvc.hpp"

struct Shape : Module 
{
	enum ParamIds {
		AMOUNT,
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_IN,
		BLEND_CV,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO_OUT,		
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Shape() : Module( NUM_PARAMS , NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		// reset();
	}
	void step() override;
};

void Shape::step() {
	float audioIn = clampf(inputs[AUDIO_IN].value*0.1f,-1.0f,1.0f);
	float cvIn = clampf(std::abs(inputs[BLEND_CV].normalize(1.0f))*0.1f, 0.0f, 1.0f);
	float knobVal = params[AMOUNT].value;	
	
	float modAmount = knobVal;
	if (inputs[BLEND_CV].active)
		modAmount = knobVal * cvIn;

	float distort = modAmount * 111.0f;		// distortion factor
	float shaped = (1.0f + distort)*audioIn / ( 1.0f + distort*std::abs(audioIn)); // SQUASH IT!
	
	float mix = (std::abs(modAmount-1)*2.0f*audioIn + modAmount*shaped) * 0.5f; 
	outputs[AUDIO_OUT].value = clampf(mix, -1.0f, 1.0f) * 10.0f; 
}

ShapeWidget::ShapeWidget() {
	Shape *module = new Shape();
	setModule(module);
	box.size = Vec(2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel2HE.svg")));
		addChild(panel);
	}
	// SCREWS
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	
	//addParam(createParam<PvCKnob>(Vec(4, 44), module, Shape::AMOUNT, 0.0, 1.0, 0.0));
	// big fat fader until space is needed
	addParam(createParam<PvCFader>(Vec(7, 22), module, Shape::AMOUNT, 0.0f, 1.0f, 0.0f));
	addInput(createInput<ModInPort>(Vec(4, 268), module, Shape::BLEND_CV));
	addInput(createInput<InPort>(Vec(4, 310), module, Shape::AUDIO_IN));
	addOutput(createOutput<OutPort>(Vec(4, 336), module, Shape::AUDIO_OUT));
}