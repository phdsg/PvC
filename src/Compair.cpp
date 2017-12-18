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

TODO:
 -more sanity and functionality tests!
 -code clean-up, optimization, simplification

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"
#include "dsp/digital.hpp"

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

	bool flip = false;
	SchmittTrigger flipTrigger;
	bool outA = false;
	bool outB = false;

	enum OutputMode {
		ORIGINAL,
		BIPOLAR
	//	INV_BIPOLAR,
	//	INV_ORIGINAL
	};
	OutputMode outputMode = ORIGINAL;

	Compair() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		// reset();
	}

	void step() override;
	// void reset() override;
	// void randomize() override;

	json_t *toJson() override {
		json_t *rootJ = json_object();

		// outputMode
		json_t *outputModeJ = json_integer((int) outputMode);
		json_object_set_new(rootJ, "outputMode", outputModeJ);

		return rootJ;
	}

	void fromJson(json_t *rootJ) override {
		// outputMode
		json_t *outputModeJ = json_object_get(rootJ, "outputMode");
		if (outputModeJ)
			outputMode = (OutputMode)json_integer_value(outputModeJ);
	}

};

void Compair::step(){
	// get inputs and normalize B to A
	float inputA = inputs[AUDIO_A_IN].value;
	float inputB = inputs[AUDIO_B_IN].normalize(inputA);
	
	// get knob values
	float posA = params[POS_A_PARAM].value;
	float widthA = params[WIDTH_A_PARAM].value;
	float posB = params[POS_B_PARAM].value;
	float widthB = params[WIDTH_B_PARAM].value;;
	
	// teating bi-polar outputs
		float highVal = 5.0f;
		//float lowVal = 0.0f;
		float lowVal = outputMode == BIPOLAR ? -5.0f : 0.0f;

	// channel A CV inputs to knob values
	if (inputs[POS_A_IN].active)
		posA = (params[POS_A_PARAM].value + inputs[POS_A_IN].value);
	if (inputs[WIDTH_A_IN].active)
		widthA = (params[WIDTH_A_PARAM].value + inputs[WIDTH_A_IN].value);
	
	// compute window A
	float upperThreshA = posA + widthA*0.5f;
	float lowerThreshA = posA - widthA*0.5f;
	
	// check if input A is in window A
	outA = (inputA < upperThreshA && inputA > lowerThreshA) ? true : false;
		
	// channel B CV inputs to knob values and normalization
	if (inputs[POS_B_IN].active || inputs[POS_A_IN].active)
		posB = (params[POS_B_PARAM].value + inputs[POS_B_IN].normalize(inputs[POS_A_IN].value));
	
	if (inputs[WIDTH_B_IN].active || inputs[WIDTH_B_IN].active)
		widthB = (params[WIDTH_B_PARAM].value + inputs[WIDTH_B_IN].normalize(inputs[WIDTH_A_IN].value));

	// compute window B
	float upperThreshB = posB + widthB*0.5f;
	float lowerThreshB = posB - widthB*0.5f;

	// check if input B is in window B
	
	outB = (inputB < upperThreshB && inputB > lowerThreshB) ? true : false;
	
	// Gate/Not outputs and lights
	outputs[GATE_A_OUT].value = outA ? highVal : lowVal;
	outputs[NOT_A_OUT].value = /*lights[NOT_A_LED].value = */ !outA ? highVal : lowVal;
	lights[GATE_A_LED].setBrightnessSmooth( outA ? 0.79f : 0.0f );
	lights[OVER_A_LED].setBrightnessSmooth( (inputA > upperThreshA) ? (inputA - upperThreshA)*0.1f : 0.0f );
	lights[BELOW_A_LED].setBrightnessSmooth( (inputA < lowerThreshA) ? (lowerThreshA - inputA)*0.1f : 0.0f );

	outputs[GATE_B_OUT].value = outB ? highVal : lowVal;
	outputs[NOT_B_OUT].value = /* lights[NOT_B_LED].value = */ !outB ? highVal : lowVal;
	lights[GATE_B_LED].setBrightnessSmooth( outB ? 0.79f : 0.0f );
	lights[OVER_B_LED].setBrightnessSmooth( (inputB > upperThreshB) ? (inputB - upperThreshB)*0.1f : 0.0f );
	lights[BELOW_B_LED].setBrightnessSmooth( (inputB < lowerThreshB) ? (lowerThreshB - inputB)*0.1f : 0.0f );
	
	// logic input inverts
	if (params[INVERT_A_PARAM].value)
		outA = !outA;
	if (params[INVERT_B_PARAM].value)
		outB = !outB;

	// logic outputs and lights
	outputs[AND_OUT].value = lights[AND_LED].value = (outA && outB) ? highVal : lowVal;
	outputs[OR_OUT].value = lights[OR_LED].value = (outA || outB) ? highVal : lowVal;
	outputs[XOR_OUT].value = lights[XOR_LED].value = (outA != outB) ? highVal : lowVal;
	if (flipTrigger.process(outputs[XOR_OUT].value)) // trigger the FlipFlop
		flip = !flip;
	outputs[FLIP_OUT].value = lights[FLIP_LED].value = flip ? highVal : lowVal;
}
//
struct PvCBigKnob : RoundKnob {
	PvCBigKnob() {
		setSVG(SVG::load(assetPlugin(plugin, "res/components/PvCBigKnob.svg")));
		box.size = Vec(44,44);
	}
};
struct CompairToggle : SVGSwitch, ToggleSwitch {
	CompairToggle() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/PvCToggleA.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/PvCToggleB.svg")));
	}
};
// backdrop for the compare LEDs
struct LEDback : SVGScrew {
	LEDback() {
		sw->svg = SVG::load(assetPlugin(plugin, "res/components/LEDback.svg"));
		sw->wrap();
		box.size = Vec(20,20);
	}
};
// LEDs
template <typename BASE>
 struct CompairLight : BASE {
 	CompairLight() {
		this->box.size = Vec(20.0f, 20.0f);
 		this->bgColor = nvgRGBA(0x00, 0x00, 0x00, 0x00);
 	}
 };

template <typename BASE>
 struct LogicLight : BASE {
 	LogicLight() {
		this->box.size = Vec(4.0f, 4.0f);
 //		this->bgColor = nvgRGBA(0x00, 0x00, 0x00, 0x00);
 	}
 };

CompairWidget::CompairWidget(){
	Compair *module = new Compair();
	setModule(module);
	box.size = Vec(8 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/Compair.svg")));
		addChild(panel);
	}
	// SCREWS
	addChild(createScrew<ScrewHead1>(Vec(15, 0)));
	//addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(15, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 30, 365)));

	// A
	addInput(createInput<InPort>(Vec(35,234),module,Compair::AUDIO_A_IN));
	addParam(createParam<PvCBigKnob>(Vec(10,60), module, Compair::POS_A_PARAM, -5.f , 5.f, 0));
	addInput(createInput<ModInPort>(Vec(7,190),module,Compair::POS_A_IN));
	addParam(createParam<PvCBigKnob>(Vec(10,128), module, Compair::WIDTH_A_PARAM, 0.01f , 10.f, 5.f));
	addInput(createInput<ModInPort>(Vec(35,190),module,Compair::WIDTH_A_IN));
	addOutput(createOutput<OutPort>(Vec(7,278),module,Compair::GATE_A_OUT));
	addChild(createScrew<LEDback>(Vec(7, 234)));
	addChild(createLight<CompairLight<BlueLight>>(Vec(8,235),module,Compair::BELOW_A_LED));
	addChild(createLight<CompairLight<WhiteLight>>(Vec(8,235),module,Compair::GATE_A_LED));
	addChild(createLight<CompairLight<RedLight>>(Vec(8,235),module,Compair::OVER_A_LED));
	addOutput(createOutput<OutPort>(Vec(35,278),module,Compair::NOT_A_OUT));
	
	// B
	addInput(createInput<InPort>(Vec(63,234),module,Compair::AUDIO_B_IN));
	addParam(createParam<PvCBigKnob>(Vec(66,60), module, Compair::POS_B_PARAM, -5.f , 5.f, 0));
	addInput(createInput<ModInPort>(Vec(90,190),module,Compair::POS_B_IN));
	addParam(createParam<PvCBigKnob>(Vec(66,128), module, Compair::WIDTH_B_PARAM, 0.01f , 10.f, 5.f));
	addInput(createInput<ModInPort>(Vec(63,190),module,Compair::WIDTH_B_IN));
	addOutput(createOutput<OutPort>(Vec(90,278),module,Compair::GATE_B_OUT));
	addChild(createScrew<LEDback>(Vec(90, 234)));
	addChild(createLight<CompairLight<BlueLight>>(Vec(91,235),module,Compair::BELOW_B_LED));
	addChild(createLight<CompairLight<WhiteLight>>(Vec(91,235),module,Compair::GATE_B_LED));
	addChild(createLight<CompairLight<RedLight>>(Vec(91,235),module,Compair::OVER_B_LED));
	addOutput(createOutput<OutPort>(Vec(63,278),module,Compair::NOT_B_OUT));
	// Invert toggles
	addParam(createParam<CompairToggle>(Vec(11,238),module,Compair::INVERT_A_PARAM, 0, 1, 0));
	addParam(createParam<CompairToggle>(Vec(94,238),module,Compair::INVERT_B_PARAM, 0, 1, 0));
	// LOGIC
	addOutput(createOutput<OutPort>(Vec(7,324),module,Compair::AND_OUT));
	addChild(createLight<LogicLight<WhiteLight>>(Vec(16,318),module,Compair::AND_LED));
	addOutput(createOutput<OutPort>(Vec(35,324),module,Compair::OR_OUT));
	addChild(createLight<LogicLight<OrangeLight>>(Vec(44,318),module,Compair::OR_LED));
	addOutput(createOutput<OutPort>(Vec(63,324),module,Compair::XOR_OUT));
	addChild(createLight<LogicLight<YellowLight>>(Vec(72,318),module,Compair::XOR_LED));
	addOutput(createOutput<OutPort>(Vec(90,324),module,Compair::FLIP_OUT));
	addChild(createLight<LogicLight<GreenLight>>(Vec(99,318),module,Compair::FLIP_LED));
}

struct CompairOutputModeItem : MenuItem {
	Compair *compair;
	Compair::OutputMode outputMode;
	void onAction(EventAction &e) override {
		compair->outputMode = outputMode;
	}
	void step() override {
		rightText = (compair->outputMode == outputMode) ? "✔" : "";
	}
};

Menu *CompairWidget::createContextMenu() {
	Menu *menu = ModuleWidget::createContextMenu();

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
}
