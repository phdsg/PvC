/*

PvCBlank

*/////////////////////////////////////////////////////////////////////////////


#include "pvc.hpp"

struct PvCBlank : Module {
	enum ParamIds {
		

		NUM_PARAMS
	};
	enum InputIds {
		

		NUM_INPUTS
	};
	enum OutputIds {
		

		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	PvCBlank() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
};

void PvCBlank::step() {
 // TODO: make cursor blink...
}


PvCBlankWidget::PvCBlankWidget() {
	PvCBlank *module = new PvCBlank();
	setModule(module);
	box.size = Vec(15*34, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/PvC64.svg")));
		addChild(panel);
	}
	// // screws
	// addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	// addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	// addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	// addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	// addInput(createInput<>(Vec(,),module,PvCBlank::));
	// addParam(createParam<>(Vec(,),module,PvCBlank::));
	// addOutput(createOutput<>(Vec(,),module,PvCBlank::));

}