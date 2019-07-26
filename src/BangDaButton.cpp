/*

BangDaButton

a cv sending button to trigger/switch things.

button provides gate/trig outs for press and release events as well
as two flipflops.
it also opens/closes 4 A/B switches 2 x 1to2 and 2 x 2to1
a "hidden" gate port can be used to control the button with external sources.

*/////////////////////////////////////////////////////////////////////////////



#include "pvc.hpp"

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

		DA_BUTTON_TRIG,

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
		UP_LED,
		DOWN_LED,
		
		NUM_LIGHTS
	};

	bool pressed = false;
	
	bool flipUp = true;
	bool flipDn = false;

	dsp::SchmittTrigger upFlipTrg;
	dsp::SchmittTrigger dnFlipTrg;

	dsp::PulseGenerator upPulse;
	dsp::PulseGenerator dnPulse;

	BangDaButton() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(DA_BUTTON, 0, 1, 0);
	}

	void process(const ProcessArgs &args) override;

	void onReset() override {
		pressed = false;
		flipUp = true;
		flipDn = false;		
	}
};

void BangDaButton::process(const ProcessArgs &args) {
	pressed = params[DA_BUTTON].getValue();
	if (inputs[DA_BUTTON_TRIG].isConnected()) {
		pressed = inputs[DA_BUTTON_TRIG].getVoltage() > 0 ? !pressed : pressed;
	}

	if (dnFlipTrg.process(pressed)) {	// HOLD
		flipDn = !flipDn;
		dnPulse.trigger(0.01);
	}
	if (upFlipTrg.process(!pressed)){ // RELEASE
		flipUp = !flipUp;
		upPulse.trigger(0.01);
	}

	outputs[UP_SW_OUT].setVoltage(pressed ? inputs[UP_SW_A_IN].getNormalVoltage(0.0f) : inputs[UP_SW_B_IN].getNormalVoltage(0.0f));
	outputs[UP_MUX_A_OUT].setVoltage(pressed * inputs[UP_MUX_IN].getNormalVoltage(0.0f));
	outputs[UP_MUX_B_OUT].setVoltage(!pressed * inputs[UP_MUX_IN].getNormalVoltage(0.0f));
	
	outputs[DOWN_SW_OUT].setVoltage(pressed ? inputs[DOWN_SW_B_IN].getNormalVoltage(0.0f) : inputs[DOWN_SW_A_IN].getNormalVoltage(0.0f));
	outputs[DOWN_MUX_A_OUT].setVoltage(!pressed * inputs[DOWN_MUX_IN].getNormalVoltage(0.0f));
	outputs[DOWN_MUX_B_OUT].setVoltage(pressed * inputs[DOWN_MUX_IN].getNormalVoltage(0.0f));

	outputs[UP_GATE_OUT].setVoltage(!pressed * 10.0f);
	outputs[DOWN_GATE_OUT].setVoltage(pressed * 10.0f);

	outputs[UP_FLIP_OUT].setVoltage(flipUp * 10.0f);
	outputs[DOWN_FLIP_OUT].setVoltage(flipDn * 10.0f);
	
	outputs[UP_TRIG_OUT].setVoltage(upPulse.process(1.0/args.sampleRate) * 10.0f);
	outputs[DOWN_TRIG_OUT].setVoltage(dnPulse.process(1.0/args.sampleRate) * 10.0f);
	
	lights[DOWN_LED].value = pressed;
	lights[UP_LED].value = !pressed;
}


struct DaButton : SVGSwitch {
	DaButton() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/DaButton_up.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/DaButton_dn.svg")));
		box.size = Vec(82,82);
	}
};
struct InPortT : SVGPort {
	InPortT() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/empty.svg")));
		sw->wrap();
		box.size = Vec(22,22);
	}
};

struct BangDaButtonWidget : ModuleWidget {
	BangDaButtonWidget(BangDaButton *module);
};

BangDaButtonWidget::BangDaButtonWidget(BangDaButton *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BangDaButton.svg")));
	
	// screws
	addChild(createWidget<ScrewHead3>(Vec(15, 0)));
	addChild(createWidget<ScrewHead2>(Vec(box.size.x - 30, 0)));
	addChild(createWidget<ScrewHead4>(Vec(15, 365)));
	addChild(createWidget<ScrewHead1>(Vec(box.size.x - 30, 365)));

	addInput(createInput<InPortAud>(Vec(4,22), module,BangDaButton::UP_SW_A_IN));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(28,30.5),module,BangDaButton::DOWN_LED));
	addOutput(createOutput<OutPortVal>(Vec(34,22), module,BangDaButton::UP_SW_OUT));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(57,30.5),module,BangDaButton::UP_LED));
	addInput(createInput<InPortAud>(Vec(64,22), module,BangDaButton::UP_SW_B_IN));
	
	addOutput(createOutput<OutPortVal>(Vec(4,68), module,BangDaButton::UP_MUX_A_OUT));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(27,76.5),module,BangDaButton::DOWN_LED));
	addInput(createInput<InPortAud>(Vec(34,68), module,BangDaButton::UP_MUX_IN));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(57,76.5),module,BangDaButton::UP_LED));
	addOutput(createOutput<OutPortVal>(Vec(64,68), module,BangDaButton::UP_MUX_B_OUT));
	
	addOutput(createOutput<OutPortBin>(Vec(6,124), module,BangDaButton::UP_GATE_OUT));
	addOutput(createOutput<OutPortBin>(Vec(34,124), module,BangDaButton::UP_TRIG_OUT));
	addOutput(createOutput<OutPortBin>(Vec(62,124), module,BangDaButton::UP_FLIP_OUT));

	addParam(createParam<DaButton>(Vec(4,149),module,BangDaButton::DA_BUTTON));
	
	addInput(createInput<InPortT>(Vec(4,179), module,BangDaButton::DA_BUTTON_TRIG));
	
	addOutput(createOutput<OutPortBin>(Vec(6,234), module,BangDaButton::DOWN_FLIP_OUT));
	addOutput(createOutput<OutPortBin>(Vec(34,234), module,BangDaButton::DOWN_TRIG_OUT));
	addOutput(createOutput<OutPortBin>(Vec(62,234), module,BangDaButton::DOWN_GATE_OUT));
	
	addOutput(createOutput<OutPortVal>(Vec(4,290), module,BangDaButton::DOWN_MUX_A_OUT));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(27,298.5),module,BangDaButton::UP_LED));
	addInput(createInput<InPortAud>(Vec(34,290), module,BangDaButton::DOWN_MUX_IN));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(57,298.5),module,BangDaButton::DOWN_LED));
	addOutput(createOutput<OutPortVal>(Vec(64,290), module,BangDaButton::DOWN_MUX_B_OUT));
	
	addInput(createInput<InPortAud>(Vec(4,336), module,BangDaButton::DOWN_SW_A_IN));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(27,344.5),module,BangDaButton::UP_LED));
	addOutput(createOutput<OutPortVal>(Vec(34,336), module,BangDaButton::DOWN_SW_OUT));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(57,344.5),module,BangDaButton::DOWN_LED));
	addInput(createInput<InPortAud>(Vec(64,336), module,BangDaButton::DOWN_SW_B_IN));
}

Model *modelBangDaButton = createModel<BangDaButton, BangDaButtonWidget>("BangDaButton");