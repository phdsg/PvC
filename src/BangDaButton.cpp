/*

BangDaButton

a cv sending button to trigger/switch things with a few fun extras...

main section:

 Big Momentary Button and 2 Groups of 3 outputs:
 	Outputs:
 		above: released button state
 		below: pressed button state
 	Types:
 		gate: is high as long as the state is true
 		trig: pulses once on state changes
 		flip: toggles on state changes

up section
	channel 1: 2 in 1 out, A or B into X
	channel 2: 1 in 1 out, A or MUTE into X
	channel 3: 2 in 1 out, A into X or Y
	channel 4: 1 in 1 out, A or MUTE into X 

down section
	same as up section but in mirrored order

TODO:
	ui
	toggle/momentary modes for the channels
	toggles for the flipflops

*/////////////////////////////////////////////////////////////////////////////



#include "pvc.hpp"
#include "dsp/digital.hpp" // SchmittTrigger // PulseGenerator


struct BangDaButton : Module {
	
	enum ParamIds {
		DA_BUTTON,

		NUM_PARAMS
	};
	
	enum InputIds {
		UP_SW_A_IN,
		UP_SW_B_IN,
		UP_CH1_IN,
		UP_MUX_IN,
		UP_CH2_IN,

		DOWN_SW_A_IN,
		DOWN_SW_B_IN,
		DOWN_CH1_IN,
		DOWN_MUX_IN,
		DOWN_CH2_IN,

		NUM_INPUTS
	};
	
	enum OutputIds {
		UP_SW_OUT,
		UP_CH1_OUT,
		UP_MUX_A_OUT, UP_MUX_B_OUT,
		UP_CH2_OUT,

		UP_FLIP_OUT, UP_TRIG_OUT, UP_GATE_OUT,
		DOWN_GATE_OUT, DOWN_TRIG_OUT, DOWN_FLIP_OUT,
	
		DOWN_CH1_OUT,
		DOWN_MUX_A_OUT,	DOWN_MUX_B_OUT,
		DOWN_CH2_OUT,
		DOWN_SW_OUT,
		
		NUM_OUTPUTS
	};

	enum LightIds {
		UP_SW_A_LED, UP_SW_B_LED,
		UP_CH1_MUTE_LED,
		UP_MUX_A_LED, UP_MUX_B_LED,
		UP_CH2_MUTE_LED,

		UP_GATE_LED, UP_FLIP_LED,
		DOWN_GATE_LED, DOWN_FLIP_LED,
		
		DOWN_CH1_MUTE_LED,
		DOWN_MUX_A_LED,	DOWN_MUX_B_LED,
		DOWN_CH2_MUTE_LED,
		DOWN_SW_A_LED, DOWN_SW_B_LED,

		NUM_LIGHTS
	};

	BangDaButton() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;

	bool flipUp = true;
	bool flipDn = false;

	SchmittTrigger upFlipTrg;
	SchmittTrigger dnFlipTrg;

	PulseGenerator upPulse;
	PulseGenerator dnPulse;
};

void BangDaButton::step() {
	bool pressed = params[DA_BUTTON].value;


	if (dnFlipTrg.process(pressed)) {	// HOLD
		flipDn = !flipDn;
		dnPulse.trigger(0.02);
	}
	if (upFlipTrg.process(!pressed)){ // RELEASE
		flipUp = !flipUp;
		upPulse.trigger(0.02);
	}

	
	outputs[UP_CH1_OUT].value = pressed ? inputs[UP_CH1_IN].normalize(0.0) : 0.0f;
	outputs[UP_CH2_OUT].value = pressed ? 0.0f : inputs[UP_CH2_IN].normalize(0.0);
	outputs[UP_SW_OUT].value = pressed ? inputs[UP_SW_A_IN].normalize(0.0) : inputs[UP_SW_B_IN].normalize(0.0);
	outputs[UP_MUX_A_OUT].value = pressed ? inputs[UP_MUX_IN].normalize(0.0) : 0.0;
	outputs[UP_MUX_B_OUT].value = pressed ? 0.0 : inputs[UP_MUX_IN].normalize(0.0);
	
	outputs[DOWN_CH1_OUT].value = pressed ? inputs[DOWN_CH1_IN].normalize(0.0) : 0.0f;
	outputs[DOWN_CH2_OUT].value = pressed ? 0.0f : inputs[DOWN_CH2_IN].normalize(0.0);
	outputs[DOWN_SW_OUT].value = pressed ? inputs[DOWN_SW_B_IN].normalize(0.0) : inputs[DOWN_SW_A_IN].normalize(0.0);
	outputs[DOWN_MUX_A_OUT].value = pressed ? 0.0 : inputs[DOWN_MUX_IN].normalize(0.0);
	outputs[DOWN_MUX_B_OUT].value = pressed ? inputs[DOWN_MUX_IN].normalize(0.0) : 0.0;

	outputs[UP_GATE_OUT].value = pressed ? 0.0 : 10.0;
	outputs[DOWN_GATE_OUT].value = pressed ? 10.0 : 0.0;

	outputs[UP_FLIP_OUT].value = (flipUp) ? 10.0:0.0;
	outputs[DOWN_FLIP_OUT].value = (flipDn) ? 10.0:0.0;
	
	outputs[UP_TRIG_OUT].value = upPulse.process(1.0/engineGetSampleRate()) ? 10.0 : 0.0;
	outputs[DOWN_TRIG_OUT].value = dnPulse.process(1.0/engineGetSampleRate()) ? 10.0 : 0.0;

	
	lights[UP_CH1_MUTE_LED].value = !pressed;
	lights[UP_CH2_MUTE_LED].value = pressed;
	lights[UP_SW_A_LED].value = pressed;
	lights[UP_SW_B_LED].value = !pressed;
	lights[UP_MUX_A_LED].value = pressed;
	lights[UP_MUX_B_LED].value = !pressed;

	lights[DOWN_CH1_MUTE_LED].value = !pressed;
	lights[DOWN_CH2_MUTE_LED].value = pressed;
	lights[DOWN_SW_A_LED].value = !pressed;
	lights[DOWN_SW_B_LED].value = pressed;
	lights[DOWN_MUX_A_LED].value = !pressed;
	lights[DOWN_MUX_B_LED].value = pressed;
}

template <typename BASE>
struct PvCSmallLight : BASE {
	PvCSmallLight() {
		this->box.size = Vec(6, 6);
	}
};

struct DaButton : SVGSwitch, MomentarySwitch {
	DaButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/DaButton_up.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/DaButton_dn.svg")));
		box.size = Vec(82,82);
	}
};

BangDaButtonWidget::BangDaButtonWidget() {
	BangDaButton *module = new BangDaButton();
	setModule(module);
	box.size = Vec(15*6, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/BangDaButton.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead3>(Vec(15, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 30, 0)));
	addChild(createScrew<ScrewHead4>(Vec(15, 365)));
	addChild(createScrew<ScrewHead1>(Vec(box.size.x - 30, 365)));

	addInput(createInput<InPortAud>(Vec(4,20),module,BangDaButton::UP_SW_A_IN));
	addChild(createLight<PvCSmallLight<BlueLight>>(Vec(27,28),module,BangDaButton::UP_SW_A_LED));
	addOutput(createOutput<OutPortVal>(Vec(34,20),module,BangDaButton::UP_SW_OUT));
	addChild(createLight<PvCSmallLight<BlueLight>>(Vec(57,28),module,BangDaButton::UP_SW_B_LED));
	addInput(createInput<InPortAud>(Vec(64,20),module,BangDaButton::UP_SW_B_IN));

	addInput(createInput<InPortAud>(Vec(4,44),module,BangDaButton::UP_CH1_IN));
	addChild(createLight<PvCSmallLight<OrangeLight>>(Vec(27,52),module,BangDaButton::UP_CH1_MUTE_LED));
	addOutput(createOutput<OutPortVal>(Vec(34,44),module,BangDaButton::UP_CH1_OUT));
	
	addOutput(createOutput<OutPortVal>(Vec(4,68),module,BangDaButton::UP_MUX_A_OUT));
	addChild(createLight<PvCSmallLight<BlueLight>>(Vec(27,76),module,BangDaButton::UP_MUX_A_LED));
	addInput(createInput<InPortAud>(Vec(34,68),module,BangDaButton::UP_MUX_IN));
	addChild(createLight<PvCSmallLight<BlueLight>>(Vec(57,76),module,BangDaButton::UP_MUX_B_LED));
	addOutput(createOutput<OutPortVal>(Vec(64,68),module,BangDaButton::UP_MUX_B_OUT));
	
	addInput(createInput<InPortAud>(Vec(34,92),module,BangDaButton::UP_CH2_IN));
	addChild(createLight<PvCSmallLight<OrangeLight>>(Vec(57,100),module,BangDaButton::UP_CH2_MUTE_LED));
	addOutput(createOutput<OutPortVal>(Vec(64,92),module,BangDaButton::UP_CH2_OUT));
	
	addOutput(createOutput<OutPortBin>(Vec(6,124),module,BangDaButton::UP_GATE_OUT));
	addOutput(createOutput<OutPortBin>(Vec(30,124),module,BangDaButton::UP_TRIG_OUT));
	addOutput(createOutput<OutPortBin>(Vec(62,124),module,BangDaButton::UP_FLIP_OUT));

	addParam(createParam<DaButton>(Vec(4,149),module,BangDaButton::DA_BUTTON, 0, 1, 0));
	
	addOutput(createOutput<OutPortBin>(Vec(6,234),module,BangDaButton::DOWN_FLIP_OUT));
	addOutput(createOutput<OutPortBin>(Vec(38,234),module,BangDaButton::DOWN_TRIG_OUT));
	addOutput(createOutput<OutPortBin>(Vec(62,234),module,BangDaButton::DOWN_GATE_OUT));
	
	addInput(createInput<InPortAud>(Vec(34,266),module,BangDaButton::DOWN_CH1_IN));
	addChild(createLight<PvCSmallLight<OrangeLight>>(Vec(57,274),module,BangDaButton::DOWN_CH1_MUTE_LED));
	addOutput(createOutput<OutPortVal>(Vec(64,266),module,BangDaButton::DOWN_CH1_OUT));
	
	addOutput(createOutput<OutPortVal>(Vec(4,290),module,BangDaButton::DOWN_MUX_A_OUT));
	addChild(createLight<PvCSmallLight<BlueLight>>(Vec(27,298),module,BangDaButton::DOWN_MUX_A_LED));
	addInput(createInput<InPortAud>(Vec(34,290),module,BangDaButton::DOWN_MUX_IN));
	addChild(createLight<PvCSmallLight<BlueLight>>(Vec(57,298),module,BangDaButton::DOWN_MUX_B_LED));
	addOutput(createOutput<OutPortVal>(Vec(64,290),module,BangDaButton::DOWN_MUX_B_OUT));
	
	addInput(createInput<InPortAud>(Vec(4,314),module,BangDaButton::DOWN_CH2_IN));
	addChild(createLight<PvCSmallLight<OrangeLight>>(Vec(27,322),module,BangDaButton::DOWN_CH2_MUTE_LED));
	addOutput(createOutput<OutPortVal>(Vec(34,314),module,BangDaButton::DOWN_CH2_OUT));
	
	addInput(createInput<InPortAud>(Vec(4,338),module,BangDaButton::DOWN_SW_A_IN));
	addChild(createLight<PvCSmallLight<BlueLight>>(Vec(27,346),module,BangDaButton::DOWN_SW_A_LED));
	addOutput(createOutput<OutPortVal>(Vec(34,338),module,BangDaButton::DOWN_SW_OUT));
	addChild(createLight<PvCSmallLight<BlueLight>>(Vec(57,346),module,BangDaButton::DOWN_SW_B_LED));
	addInput(createInput<InPortAud>(Vec(64,338),module,BangDaButton::DOWN_SW_B_IN));

}