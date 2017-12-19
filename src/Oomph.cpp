/*

Oomph
 trying some waveshaping functions from musicdsp.org

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"

struct Oomph : Module {
	enum ParamIds {
		AMOUNT_PARAM,
		NUM_PARAMS
	};

	enum InputIds {
		AUDIO_IN,
		MOD_IN,
		NUM_INPUTS
	};

	enum OutputIds {
		AUDIO_OUT,		
		NUM_OUTPUTS
	};

	enum LightIds {
		NUM_LIGHTS
	};

	Oomph() : Module( NUM_PARAMS , NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
	// reset();
	}
	
	void step() override;
};

void Oomph::step() {
	float input = clampf(inputs[AUDIO_IN].value*0.1f,-1.0f,1.0f);

	float mod = 11.0f; // goes to ELEVEN!
	if (inputs[MOD_IN].active)
		mod *= inputs[MOD_IN].value*0.1f;

	float squash = fabs(params[AMOUNT_PARAM].value * mod);

	float output = input * (1.0f + squash) / (1.0f + squash * fabs(input*2.0f)); // OOMPH!
	
	outputs[AUDIO_OUT].value = output * 10.0f; 
}


struct PvCFader : SVGSlider {
	PvCFader() {
		maxHandlePos = Vec(1, 1);
		minHandlePos = Vec(1, 225);
		background->svg = SVG::load(assetPlugin(plugin, "res/components/PvCFader.svg"));
		background->wrap();
		box.size = background->box.size;
		handle->svg = SVG::load(assetPlugin(plugin, "res/components/PvCFaderCap.svg"));
		handle->wrap();
	}
};


OomphWidget::OomphWidget() {
	Oomph *module = new Oomph();
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
	
	//addParam(createParam<PvCKnob>(Vec(4, 44), module, Oomph::AMOUNT, 0.0, 1.0, 0.0));

	// big fat fader until space is needed
	addParam(createParam<PvCFader>(Vec(7, 22), module, Oomph::AMOUNT_PARAM, 0.0f, 1.0f, 0.0f));
	addInput(createInput<ModInPort>(Vec(4, 272), module, Oomph::MOD_IN));
	addOutput(createOutput<OutPort>(Vec(4, 312), module, Oomph::AUDIO_OUT));
	addInput(createInput<InPort>(Vec(4, 336), module, Oomph::AUDIO_IN));
}