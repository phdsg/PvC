/*

VuBar

12 LED VU Meter using rack's vumeter.hpp

- top led selects db interval: 1-[2]-3-4
- knob sets brightness

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"
#include "dsp/vumeter.hpp"

struct Vubar : Module {
	enum ParamIds {
		DIM_PARAM,
		RANGE_PARAM,
	 	NUM_PARAMS
	};

	enum InputIds {
		METER_INPUT,
		NUM_INPUTS
	};

	enum OutputIds {
		METER_THRU_OUT,
	 	NUM_OUTPUTS
	};
	
	enum LightIds {
		METER_LIGHT,
		NUM_LIGHTS = METER_LIGHT + 12
	};

	Vubar() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		// reset();
	}
	void step() override;
	// void reset() override;
	// void randomize() override;
};

void Vubar::step(){

	if (inputs[METER_INPUT].active) {
		float signal_in = inputs[METER_INPUT].value;
		outputs[METER_THRU_OUT].value = signal_in;
// Lights
		VUMeter vuBar;
		vuBar.dBInterval = params[RANGE_PARAM].value;
		vuBar.setValue(signal_in * 0.1f);
		for (int i = 0; i < 12; i++) {
			lights[METER_LIGHT + i].setBrightnessSmooth(vuBar.getBrightness(i) * (params[DIM_PARAM].value) + clampf(fabs(signal_in*0.01f),0.00f,0.1f));
		}
	}
}

template <typename BASE>
 struct MeterLight : BASE {
 	MeterLight() {
 		this->box.size = Vec(20, 20);
 	}
 };

struct VuLED1 : ModuleLightWidget {
	VuLED1() {	addBaseColor(nvgRGB(0xff, 0x00, 0x28));	}
};
struct VuLED2 : ModuleLightWidget {
	VuLED2() {	addBaseColor(nvgRGB(0xff, 0x55, 0x29));	}
};
struct VuLED3 : ModuleLightWidget {
	VuLED3() {	addBaseColor(nvgRGB(0xdd, 0x66, 0x2a));	}
};
struct VuLED4 : ModuleLightWidget {
	VuLED4() {	addBaseColor(nvgRGB(0xcc, 0x77, 0x2b));	}
};
struct VuLED5 : ModuleLightWidget {
	VuLED5() {	addBaseColor(nvgRGB(0xbb, 0x88, 0x2c));	}
};
struct VuLED6 : ModuleLightWidget {
	VuLED6() {	addBaseColor(nvgRGB(0xaa, 0x99, 0x2d));	}
};
struct VuLED7 : ModuleLightWidget {
	VuLED7() {	addBaseColor(nvgRGB(0x99, 0xaa, 0x2d));	}
};
struct VuLED8 : ModuleLightWidget {
	VuLED8() {	addBaseColor(nvgRGB(0x88, 0xbb, 0x2c));	}
};
struct VuLED9 : ModuleLightWidget {
	VuLED9() {	addBaseColor(nvgRGB(0x77, 0xcc, 0x2b));	}
};
struct VuLED10 : ModuleLightWidget {
	VuLED10() {	addBaseColor(nvgRGB(0x66, 0xdd, 0x2a));	}
};
struct VuLED11 : ModuleLightWidget {
	VuLED11() {	addBaseColor(nvgRGB(0x55, 0xee, 0x29));	}
};
struct VuLED12 : ModuleLightWidget {
	VuLED12() {	addBaseColor(nvgRGB(0x44, 0xff, 0x28));	}
};

struct RangeToggle : SVGSwitch, ToggleSwitch {
	RangeToggle() {
		addFrame(SVG::load(assetPlugin(plugin,"res/components/VubarScaleToggle1.svg")));
		addFrame(SVG::load(assetPlugin(plugin,"res/components/VubarScaleToggle2.svg")));
		addFrame(SVG::load(assetPlugin(plugin,"res/components/VubarScaleToggle3.svg")));
		addFrame(SVG::load(assetPlugin(plugin,"res/components/VubarScaleToggle4.svg")));
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

	// LEDs
	addChild(createLight<MeterLight<VuLED1>>	(Vec(5, 22 + 22 * 0), module, Vubar::METER_LIGHT + 0));
	addChild(createLight<MeterLight<VuLED2>>	(Vec(5, 22 + 22 * 1), module, Vubar::METER_LIGHT + 1));
	addChild(createLight<MeterLight<VuLED3>>	(Vec(5, 22 + 22 * 2), module, Vubar::METER_LIGHT + 2));
	addChild(createLight<MeterLight<VuLED4>>	(Vec(5, 22 + 22 * 3), module, Vubar::METER_LIGHT + 3));
	addChild(createLight<MeterLight<VuLED5>>	(Vec(5, 22 + 22 * 4), module, Vubar::METER_LIGHT + 4));
	addChild(createLight<MeterLight<VuLED6>>	(Vec(5, 22 + 22 * 5), module, Vubar::METER_LIGHT + 5));
	addChild(createLight<MeterLight<VuLED7>>	(Vec(5, 22 + 22 * 6), module, Vubar::METER_LIGHT + 6));
	addChild(createLight<MeterLight<VuLED8>>	(Vec(5, 22 + 22 * 7), module, Vubar::METER_LIGHT + 7));
	addChild(createLight<MeterLight<VuLED9>>	(Vec(5, 22 + 22 * 8), module, Vubar::METER_LIGHT + 8));
	addChild(createLight<MeterLight<VuLED10>>	(Vec(5, 22 + 22 * 9), module, Vubar::METER_LIGHT + 9));
	addChild(createLight<MeterLight<VuLED11>>	(Vec(5, 22 + 22 * 10), module, Vubar::METER_LIGHT + 10));
	addChild(createLight<MeterLight<VuLED12>>	(Vec(5, 22 + 22 * 11), module, Vubar::METER_LIGHT + 11));
	
	// Dim the lights
	addParam(createParam<PvCKnob>(Vec(4, 286),module,Vubar::DIM_PARAM, 0.5f, 0.9f, 0.7f));
	// dB interval toggle 
	addParam(createParam<RangeToggle>(Vec(7, 24),module,Vubar::RANGE_PARAM, 1, 4, 2));
	// INPUT
	addInput(createInput<InPort>(Vec(4, 336), module, Vubar::METER_INPUT));
	//thru out
	addOutput(createOutput<OutPort>(Vec(4,312), module, Vubar::METER_THRU_OUT));
}