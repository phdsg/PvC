// meter
#include "pvc.hpp"
#include "dsp/vumeter.hpp"

struct Vubar : Module {
	enum ParamIds {
	 	NUM_PARAMS
	};
	enum InputIds {
		METER_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
	 	NUM_OUTPUTS
	};
	enum LightIds {
		METER_LIGHT,
		NUM_LIGHTS = METER_LIGHT + 15
	};

	
	Vubar() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		// reset();
	}
	void step() override;
	// void reset() override;
	// void randomize() override;
};

void Vubar::step(){
	float signal_in = inputs[METER_INPUT].value;
// Lights
	VUMeter vuMeter;
	vuMeter.dBInterval = 3.0;
	vuMeter.setValue(signal_in / 10.0);
	for (int i = 0; i < 15; i++) {
		lights[METER_LIGHT + i].setBrightnessSmooth(vuMeter.getBrightness(i));
	}
}

template <typename BASE>
 struct MeterLight : BASE {
 	MeterLight() {
 		this->box.size = Vec(16.25, 16.25);
// 		this->box.size = mm2px(Vec(1.088, 1.088));
// 		this->bgColor = nvgRGBf(0.698, 0.133, 0.133);
// 		this->color = nvgRGBf(1, 1, 1);
 	}
 };

VubarWidget::VubarWidget(){
	Vubar *module = new Vubar();
	setModule(module);
	box.size = Vec(2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/Vubar.svg")));
		addChild(panel);
	}
		// panel screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	//addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	//addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPort>(Vec(3, box.size.y - 18 - 26), module, Vubar::METER_INPUT));
	addChild(createLight<MeterLight<RedLight>>(Vec(7, 36), module, Vubar::METER_LIGHT + 0));
	addChild(createLight<MeterLight<YellowLight>>(Vec(7, 36 + 20), module, Vubar::METER_LIGHT + 1));
	addChild(createLight<MeterLight<YellowLight>>(Vec(7, 36 + 20 * 2), module, Vubar::METER_LIGHT + 2));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 3), module, Vubar::METER_LIGHT + 3));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 4), module, Vubar::METER_LIGHT + 4));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 5), module, Vubar::METER_LIGHT + 5));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 6), module, Vubar::METER_LIGHT + 6));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 7), module, Vubar::METER_LIGHT + 7));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 8), module, Vubar::METER_LIGHT + 8));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 9), module, Vubar::METER_LIGHT + 9));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 10), module, Vubar::METER_LIGHT + 10));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 11), module, Vubar::METER_LIGHT + 11));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 12), module, Vubar::METER_LIGHT + 12));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 13), module, Vubar::METER_LIGHT + 13));
	addChild(createLight<MeterLight<GreenLight>>(Vec(7, 36 + 20 * 14), module, Vubar::METER_LIGHT + 14));

}