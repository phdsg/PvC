/*

COMPAIR

Dual Window Comparator (inspired by Joranalogue Compare2)

checks if an input voltage is between two values.
compare window is set by two parameters: position and width.
top knob on each channel (position) sets a center voltage of +/-5V.
the other one changes the width around the center from near zero to 10V.
so with both knobs at center the window is set at [-2.5..2.5].
both parameters are also controllable via cv input.
cv and knob values are added together when computing the window.
whenever the input signal is within that window the GATE output on that
channel will go from 0V to +5V and the NOT output will do the opposite.
logic outputs at the bottom compare the output values of both channels.
AND output goes high if A is high and B is high
OR output goes high if A or B is high
XOR output goes high if either A or B ar high and the other one is low.
FLIP output changes whenever the XOR out goes high.
channel B inputs are normalized to channel A inputs.
channel output to the logic section can be inverted.
all outputs are 0V-5V.

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"

struct Compair : Module {
	enum ParamIds {
		POS_A_PARAM,
		WIDTH_A_PARAM,
		POS_B_PARAM,
		WIDTH_B_PARAM,
		INVERT_A_PARAM,
		INVERT_B_PARAM,
	 	NUM_PARAMS
	};

	enum InputIds {
		AUDIO_A_IN,
		POS_A_IN,
		WIDTH_A_IN,
		AUDIO_B_IN,
		POS_B_IN,
		WIDTH_B_IN,
		NUM_INPUTS
	};

	enum OutputIds {
		GATE_A_OUT,
		NOT_A_OUT,
	 	GATE_B_OUT,
		NOT_B_OUT,
		AND_OUT,
		OR_OUT,
		XOR_OUT,
		FLIP_OUT,
	 	NUM_OUTPUTS
	};

	enum LightIds {
		GATE_A_LED,
		GATE_B_LED,
		AND_LED,
		OR_LED,
		XOR_LED,
		FLIP_LED,
		OVER_A_LED,
		BELOW_A_LED,
		OVER_B_LED,
		BELOW_B_LED,
		NUM_LIGHTS
	};

	bool outA = false;
	bool outB = false;
	bool flip = false;
	dsp::SchmittTrigger flipTrigger;

	// enum OutputMode {
		// ORIGINAL,
		// BIPOLAR
	//	INV_BIPOLAR,
	//	INV_ORIGINAL
	// };
	// OutputMode outputMode = ORIGINAL;

	Compair() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(POS_A_PARAM, -5.0f , 5.0f, 0.0f);
		configParam(POS_B_PARAM, -5.0f, 5.0f, 0.0f);
		configParam(WIDTH_B_PARAM, 0.01f , 10.001f, 5.0f);
		configParam(WIDTH_A_PARAM, 0.01f , 10.001f, 5.0f);
		configParam(INVERT_A_PARAM, 0, 1, 0);
		configParam(INVERT_B_PARAM, 0, 1, 0);
		onReset();
	}

	void process(const ProcessArgs &args) override;
	void onReset() override {
		outA = false;
		outB = false;
		flip = false;

		// outputMode = ORIGINAL;
	}
	// void onRandomize() override;

	// json_t *dataToJson() override {
	// 	json_t *rootJ = json_object();

	// 	// outputMode
	// 	json_t *outputModeJ = json_integer((int) outputMode);
	// 	json_object_set_new(rootJ, "outputMode", outputModeJ);

	// 	return rootJ;
	// }

	// void dataFromJson(json_t *rootJ) override {
	// 	// outputMode
	// 	json_t *outputModeJ = json_object_get(rootJ, "outputMode");
	// 	if (outputModeJ)
	// 		outputMode = (OutputMode)json_integer_value(outputModeJ);
	// }

};

void Compair::process(const ProcessArgs &args){
	// get inputs and normalize B to A
	float inputA = inputs[AUDIO_A_IN].getVoltage();
	float inputB = inputs[AUDIO_B_IN].getNormalVoltage(inputA);

	// get knob values
	float posA = params[POS_A_PARAM].getValue();
	float widthA = params[WIDTH_A_PARAM].getValue();
	float posB = params[POS_B_PARAM].getValue();
	float widthB = params[WIDTH_B_PARAM].getValue();;

	// testing bi-polar outputs
		float highVal = 5.0f;
		float lowVal = 0.0f;
		//float lowVal = outputMode == BIPOLAR ? -5.0f : 0.0f;

	// channel A CV inputs to knob values
	if (inputs[POS_A_IN].isConnected())
		posA = (params[POS_A_PARAM].getValue() + inputs[POS_A_IN].getVoltage());
	if (inputs[WIDTH_A_IN].isConnected())
		widthA = (params[WIDTH_A_PARAM].getValue() + inputs[WIDTH_A_IN].getVoltage());

	// compute window A
	float upperThreshA = posA + widthA*0.5f;
	float lowerThreshA = posA - widthA*0.5f;

	// check if input A is in window A
	outA = (inputA <= upperThreshA && inputA >= lowerThreshA) ? true : false;

	// channel B CV inputs to knob values and normalization
	if (inputs[POS_B_IN].isConnected() || inputs[POS_A_IN].isConnected())
		posB = (params[POS_B_PARAM].getValue() + inputs[POS_B_IN].getNormalVoltage(inputs[POS_A_IN].getVoltage()));

	if (inputs[WIDTH_B_IN].isConnected() || inputs[WIDTH_B_IN].isConnected())
		widthB = (params[WIDTH_B_PARAM].getValue() + inputs[WIDTH_B_IN].getNormalVoltage(inputs[WIDTH_A_IN].getVoltage()));

	// compute window B
	float upperThreshB = posB + widthB*0.5f;
	float lowerThreshB = posB - widthB*0.5f;

	// check if input B is in window B

	outB = (inputB <= upperThreshB && inputB >= lowerThreshB) ? true : false;

	// Gate/Not outputs and lights
	outputs[GATE_A_OUT].setVoltage(outA ? highVal : lowVal);
	outputs[NOT_A_OUT].setVoltage(!outA ? highVal : lowVal);
	lights[GATE_A_LED].setBrightness( outA ? 0.9f : (0.25f - clamp( fabsf(inputA-posA) * 0.025f, 0.0f, 0.4f) ) );
	lights[OVER_A_LED].setBrightness( (inputA > upperThreshA) ? (inputA - upperThreshA)*0.1f + 0.4f : 0.0f );
	lights[BELOW_A_LED].setBrightness( (inputA < lowerThreshA) ? (lowerThreshA - inputA)*0.1f + 0.4f : 0.0f );

	outputs[GATE_B_OUT].setVoltage(outB ? highVal : lowVal);
	outputs[NOT_B_OUT].setVoltage(!outB ? highVal : lowVal);
	lights[GATE_B_LED].setBrightness( outB ? 0.9f : (0.25f - clamp( fabsf(inputB-posB) * 0.025f, 0.0f, 0.4f) ) );
	lights[OVER_B_LED].setBrightness( (inputB > upperThreshB) ? (inputB - upperThreshB)*0.1f + 0.4f : 0.0f );
	lights[BELOW_B_LED].setBrightness( (inputB < lowerThreshB) ? (lowerThreshB - inputB)*0.1f + 0.4f : 0.0f );

	// logic input inverts
	if (params[INVERT_A_PARAM].getValue())
		outA = !outA;
	if (params[INVERT_B_PARAM].getValue())
		outB = !outB;

	// logic outputs and lights
	outputs[AND_OUT].setVoltage((outA && outB) ? highVal : lowVal);
	lights[AND_LED].setBrightness( (outA && outB) );
	outputs[OR_OUT].setVoltage((outA || outB) ? highVal : lowVal);
	lights[OR_LED].setBrightness( (outA || outB) );
	outputs[XOR_OUT].setVoltage((outA != outB) ? highVal : lowVal);
	lights[XOR_LED].setBrightness( (outA != outB) );
	if (flipTrigger.process(outputs[XOR_OUT].value)) // trigger the FlipFlop
		flip = !flip;
	outputs[FLIP_OUT].setVoltage(flip ? highVal : lowVal);
	lights[FLIP_LED].setBrightness( flip );
}
//

struct CompairToggle : SVGSwitch {
	CompairToggle() {
		//momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/CompairToggleUp.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/CompairToggleDn.svg")));
	}
};
// backdrop for the compare LEDs
struct CompairLightBg : SVGScrew {
	CompairLightBg() {
		sw->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/CompairLightBg.svg"));
		sw->wrap();
		box.size = Vec(22,22);
	}
};
// LEDs
template <typename BASE>
 struct CompairLight : BASE {
 	CompairLight() {
		this->box.size = Vec(22, 22);
 		this->bgColor = nvgRGBA(0x00, 0x00, 0x00, 0x00);
 	}
 };


struct CompairWidget : ModuleWidget {
	CompairWidget(Compair *module);
	// Menu *createContextMenu() override;
};

CompairWidget::CompairWidget(Compair *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/Compair.svg")));
	
	// SCREWS
	addChild(createWidget<ScrewHead1>(Vec(15, 0)));
	//addChild(createWidget<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createWidget<ScrewHead3>(Vec(15, 365)));
	//addChild(createWidget<ScrewHead4>(Vec(box.size.x - 30, 365)));

	// A Side
	addInput(createInput<InPortAud>(Vec(4,22), module,Compair::AUDIO_A_IN));
	addInput(createInput<InPortAud>(Vec(34,22), module,Compair::AUDIO_B_IN));

	addParam(createParam<PvCKnob>(Vec(4,64), module, Compair::POS_A_PARAM));

	addInput(createInput<InPortCtrl>(Vec(4,88), module,Compair::POS_A_IN));
	addParam(createParam<PvCKnob>(Vec(34,64), module, Compair::POS_B_PARAM));

	addInput(createInput<InPortCtrl>(Vec(34,88), module,Compair::POS_B_IN));

	addParam(createParam<PvCKnob>(Vec(34,128), module, Compair::WIDTH_B_PARAM));

	addInput(createInput<InPortCtrl>(Vec(34,152), module,Compair::WIDTH_B_IN));
	addParam(createParam<PvCKnob>(Vec(4,128), module, Compair::WIDTH_A_PARAM));

	addInput(createInput<InPortCtrl>(Vec(4,152), module,Compair::WIDTH_A_IN));

	addChild(createWidget<CompairLightBg>(Vec(4, 190)));
	addChild(createLight<CompairLight<BlueLED>>(Vec(4,190),module,Compair::BELOW_A_LED));
	addChild(createLight<CompairLight<WhiteLED>>(Vec(4,190),module,Compair::GATE_A_LED));
	addChild(createLight<CompairLight<RedLED>>(Vec(4,190),module,Compair::OVER_A_LED));
	addParam(createParam<CompairToggle>(Vec(4,190),module,Compair::INVERT_A_PARAM));

	addChild(createWidget<CompairLightBg>(Vec(34, 190)));
	addChild(createLight<CompairLight<BlueLED>>(Vec(34,190),module,Compair::BELOW_B_LED));
	addChild(createLight<CompairLight<WhiteLED>>(Vec(34,190),module,Compair::GATE_B_LED));
	addChild(createLight<CompairLight<RedLED>>(Vec(34,190),module,Compair::OVER_B_LED));
	addParam(createParam<CompairToggle>(Vec(34,190),module,Compair::INVERT_B_PARAM));
	

	addOutput(createOutput<OutPortBin>(Vec(4,230), module,Compair::GATE_A_OUT));
	addOutput(createOutput<OutPortBin>(Vec(4,254), module,Compair::NOT_A_OUT));
	addOutput(createOutput<OutPortBin>(Vec(34,230), module,Compair::GATE_B_OUT));
	addOutput(createOutput<OutPortBin>(Vec(34,254), module,Compair::NOT_B_OUT));

	addChild(createLight<FourPixLight<CyanLED>>(Vec(13,288),module,Compair::AND_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,294), module,Compair::AND_OUT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(43,288),module,Compair::OR_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,294), module,Compair::OR_OUT));

	addChild(createLight<FourPixLight<YellowLED>>(Vec(13,330),module,Compair::XOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,336), module,Compair::XOR_OUT));
	addChild(createLight<FourPixLight<GreenLED>>(Vec(43,330),module,Compair::FLIP_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,336), module,Compair::FLIP_OUT));
}


/*struct CompairOutputModeItem : MenuItem {
	Compair *compair;
	Compair::OutputMode outputMode;
	void onAction(EventAction &e) override {
		compair->outputMode = outputMode;
	}
	void process(const ProcessArgs &args) override {
		rightText = (compair->outputMode == outputMode) ? "✔" : "";
	}
};

Menu *CompaircreateWidgetContextMenu() {
	Menu *menu = ModulecreateWidgetContextMenu();

	MenuLabel *spacerLabel = new MenuLabel();
	menu->addChild(spacerLabel);

	Compair *compair = dynamic_cast<Compair*>(module);
	assert(compair);

	MenuLabel *modeLabel = new MenuLabel();
	modeLabel->text = "Output Mode";
	menu->addChild(modeLabel);

	CompairOutputModeItem *originalItem = new CompairOutputModeItem();
	originalItem->text = "Original [0V..+5V]";
	originalItem->compair = compair;
	originalItem->outputMode = Compair::ORIGINAL;
	menu->addChild(originalItem);

	CompairOutputModeItem *bipolarItem = new CompairOutputModeItem();
	bipolarItem->text = "Bi-Polar [-5V..+5V]";
	bipolarItem->compair = compair;
	bipolarItem->outputMode = Compair::BIPOLAR;
	menu->addChild(bipolarItem);

	return menu;
}*/

Model *modelCompair = createModel<Compair, CompairWidget>("Compair");