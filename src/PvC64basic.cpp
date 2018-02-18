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

struct PvCBlankWidget : ModuleWidget {
	PvCBlankWidget(PvCBlank *module);
};

PvCBlankWidget::PvCBlankWidget(PvCBlank *module) : ModuleWidget(module) {

	box.size = Vec(15*34, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/PvC64.svg")));
		addChild(panel);
	}
	// // screws
	// addChild(Widget::create<ScrewHead1>(Vec(0, 0)));
	// addChild(Widget::create<ScrewHead2>(Vec(box.size.x - 15, 0)));
	// addChild(Widget::create<ScrewHead3>(Vec(0, 365)));
	// addChild(Widget::create<ScrewHead4>(Vec(box.size.x - 15, 365)));

	// addInput(Port::create<>(Vec(,), Port::INPUT, module,PvCBlank::));
	// addParam(ParamWidget::create<>(Vec(,),module,PvCBlank::));
	// addOutput(Port::create<>(Vec(,), Port::OUTPUT, module,PvCBlank::));

}

Model *modelPvCBlank = Model::create<PvCBlank, PvCBlankWidget>(
	"PvC", "PvCBlank", "PvCBlank", BLANK_TAG);