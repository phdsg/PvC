// VuBar
//
// 10 stop shift register
//
// I'm not a programmer so it's kinda hacky, 
// but I hope that you find it useful.
/////////////////////////////////////////////////////

#include "pvc.hpp"
#include "dsp/vumeter.hpp"
#include "dsp/digital.hpp"

int ertek = 0;
//Érték is value in Hungarian :)

struct Shiftie : Module {
	enum ParamIds {
	 	NUM_PARAMS
	};
	enum InputIds {
		POS_INPUT,
		NEG_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		VAL_OUT,
	 	NUM_OUTPUTS
	};
	enum LightIds {
		METER_LIGHT,
		NUM_LIGHTS = METER_LIGHT + 10
	};

	Shiftie() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		// reset();
	}
	void step() override;
	// void reset() override;
	// void randomize() override;
	SchmittTrigger upTrigger, downTrigger, setTrigger;
};

void Shiftie::step(){
	
// Lights
	if( inputs[POS_INPUT].active ) {

		if( setTrigger.process(inputs[POS_INPUT].value) ) {

			if(ertek < 10)
			{
				ertek += 1;
				lights[METER_LIGHT + ((10-ertek))].setBrightness(1.0);

			}
			else
			{
				ertek = 10;
			}

		};

	};
	if( inputs[NEG_INPUT].active ) {

		if( setTrigger.process(inputs[NEG_INPUT].value) ) {

			if(ertek > 0)
			{
				ertek -= 1;
				lights[METER_LIGHT + ((10-ertek))-1].setBrightness(0.0);

			}
			else
			{
				ertek = 0;
			}

		};

	};

	outputs[VAL_OUT].value = ertek;
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

ShiftieWidget::ShiftieWidget(){
	Shiftie *module = new Shiftie();
	setModule(module);
	box.size = Vec(2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/Shiftie.svg")));
		addChild(panel);
	}
		// panel screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	//addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	//addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));
	
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 0)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 1)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 2)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 3)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 4)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 5)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 6)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 7)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 8)));
	addChild(createScrew<LEDback>(Vec(7, 42 + 23 * 9)));
	// meter leds
	addChild(createLight<MeterLight<RedLight>>(Vec(8, 43), module, Shiftie::METER_LIGHT + 0));
	addChild(createLight<MeterLight<RedLight>>(Vec(8, 43 + 23), module, Shiftie::METER_LIGHT + 1));
	addChild(createLight<MeterLight<RedLight>>(Vec(8, 43 + 23 * 2), module, Shiftie::METER_LIGHT + 2));
	addChild(createLight<MeterLight<OrangeLight>>(Vec(8, 43 + 23 * 3), module, Shiftie::METER_LIGHT + 3));
	addChild(createLight<MeterLight<OrangeLight>>(Vec(8, 43 + 23 * 4), module, Shiftie::METER_LIGHT + 4));
	addChild(createLight<MeterLight<OrangeLight>>(Vec(8, 43 + 23 * 5), module, Shiftie::METER_LIGHT + 5));
	addChild(createLight<MeterLight<YellowLight>>(Vec(8, 43 + 23 * 6), module, Shiftie::METER_LIGHT + 6));
	addChild(createLight<MeterLight<YellowLight>>(Vec(8, 43 + 23 * 7), module, Shiftie::METER_LIGHT + 7));
	addChild(createLight<MeterLight<YellowLight>>(Vec(8, 43 + 23 * 8), module, Shiftie::METER_LIGHT + 8));
	addChild(createLight<MeterLight<GreenLight>>(Vec(8, 43 + 23 * 9), module, Shiftie::METER_LIGHT + 9));
	// inputs
	addInput(createInput<InPort>(Vec(3, box.size.y - 44), module, Shiftie::NEG_INPUT));
	addInput(createInput<InPort>(Vec(3, box.size.y - 74), module, Shiftie::POS_INPUT));
	// output
	addOutput(createOutput<OutPort>(Vec(3, box.size.y - 104), module, Shiftie::VAL_OUT));
}