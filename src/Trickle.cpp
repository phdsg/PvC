/*

Trickle

trickle down

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"

#include "dsp/digital.hpp"

struct Trickle : Module {
	enum ParamIds {
		BLEND,
		NUM_PARAMS
	};
	enum InputIds {
		INPUT,
		TRIGGER,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT,
		NUM_OUTPUTS = OUTPUT + 10
	};
    enum LightIds {
    	NUM_LIGHTS
    };



	Trickle() : Module( NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS ) {
		reset();
	};


	void step() override;

	float values[10] = {};

	SchmittTrigger trickleTrigger;


	void reset() override {
		for(int i=0; i<10; i++) {
			values[i] = 0.0;
		};
	};
};




void Trickle::step() {

	if( inputs[TRIGGER].active ) {

		float input = inputs[INPUT].normalize( randomf()*10.0-5.0 );

		if( trickleTrigger.process(inputs[TRIGGER].value) ) {

			for(int i=9; i>0; i--) values[i] = (1 - params[BLEND].value) * values[i-1] + params[BLEND].value * input;
			values[0] = input;
		};
	};

	for(int i=0; i<10; i++) outputs[OUTPUT+i].value = values[i];

};



TrickleWidget::TrickleWidget() {
	Trickle *module = new Trickle();
	setModule(module);
	box.size = Vec(15*2, 380);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/panels/panel2HE.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPort>(Vec(4, 22), module, Trickle::INPUT));
	addInput(createInput<ModInPort>(Vec(4, 48), module, Trickle::TRIGGER));
	addParam(createParam<PvCKnob>(Vec(4,76), module, Trickle::BLEND, 0.0f, 1.0f, 0.0f));
	for( int i=0; i<10; i++) {
		addOutput(createOutput<OutPort>(Vec(4, 102 + 26*i  ), module, Trickle::OUTPUT+i));
	};
}
