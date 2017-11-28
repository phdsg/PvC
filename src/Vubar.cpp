// VuBar
//
// 15 LED VU Meter
//
/////////////////////////////////////////////////////

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
	VUMeter vuBar;
	vuBar.dBInterval = 2;
	vuBar.setValue(signal_in / 10.0);
	for (int i = 0; i < 15; i++) {
		lights[METER_LIGHT + i].setBrightnessSmooth(vuBar.getBrightness(i));
	}
}

template <typename BASE>
 struct MeterLight : BASE {
 	MeterLight() {
 		this->box.size = Vec(14, 14);
// 		this->box.size = mm2px(Vec(1.088, 1.088));
 		this->bgColor = nvgRGBAf(1.0, 0.0, 0.0, 0.0);
// 		this->bgColor = nvgRGBA(0x00, 0x00, 0x00, 0x00);
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
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel2HE.svg")));
		addChild(panel);
	}
	// SCREWS
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	// LED background
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 0)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 1)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 2)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 3)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 4)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 5)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 6)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 7)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 8)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 9)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 10)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 11)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 12)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 13)));
	addChild(createScrew<LEDback>(Vec(7, 24 + 20 * 14)));
	
	// LEDs
	addChild(createLight<MeterLight<PurpleLight>>(Vec(8, 25 + 20 * 0), module, Vubar::METER_LIGHT + 0));
	addChild(createLight<MeterLight<RedLight>>(Vec(8, 25 + 20 * 1), module, Vubar::METER_LIGHT + 1));
	addChild(createLight<MeterLight<RedLight>>(Vec(8, 25 + 20 * 2), module, Vubar::METER_LIGHT + 2));
	addChild(createLight<MeterLight<OrangeLight>>(Vec(8, 25 + 20 * 3), module, Vubar::METER_LIGHT + 3));
	addChild(createLight<MeterLight<OrangeLight>>(Vec(8, 25 + 20 * 4), module, Vubar::METER_LIGHT + 4));
	addChild(createLight<MeterLight<OrangeLight>>(Vec(8, 25 + 20 * 5), module, Vubar::METER_LIGHT + 5));
	addChild(createLight<MeterLight<YellowLight>>(Vec(8, 25 + 20 * 6), module, Vubar::METER_LIGHT + 6));
	addChild(createLight<MeterLight<YellowLight>>(Vec(8, 25 + 20 * 7), module, Vubar::METER_LIGHT + 7));
	addChild(createLight<MeterLight<YellowLight>>(Vec(8, 25 + 20 * 8), module, Vubar::METER_LIGHT + 8));
	addChild(createLight<MeterLight<YellowLight>>(Vec(8, 25 + 20 * 9), module, Vubar::METER_LIGHT + 9));
	addChild(createLight<MeterLight<GreenLight>>(Vec(8, 25 + 20 * 10), module, Vubar::METER_LIGHT + 10));
	addChild(createLight<MeterLight<GreenLight>>(Vec(8, 25 + 20 * 11), module, Vubar::METER_LIGHT + 11));
	addChild(createLight<MeterLight<GreenLight>>(Vec(8, 25 + 20 * 12), module, Vubar::METER_LIGHT + 12));
	addChild(createLight<MeterLight<GreenLight>>(Vec(8, 25 + 20 * 13), module, Vubar::METER_LIGHT + 13));
	addChild(createLight<MeterLight<GreenLight>>(Vec(8, 25 + 20 * 14), module, Vubar::METER_LIGHT + 14));
	
	// INPUT
	addInput(createInput<InPort>(Vec(4, box.size.y - 44), module, Vubar::METER_INPUT));
}