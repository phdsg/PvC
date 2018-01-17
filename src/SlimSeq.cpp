/*

--no-title-yet-- (SlimSeq)

16 step seq (based on m.lueders' sequential switch)

TODO:
	momentary control inputs (instead of flips)
	virtual position count when in random mode so we return to the "right"
	 point in the sequence when leaving random mode
	unipolar/bipolar options
	(maybe) gates
	loopmodes: ping-pong, one-shot
	different random play mode: coin-toss direction
	sequence length control

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"

#include "dsp/digital.hpp" // SchmittTrigger

#define STEPCOUNT 16

struct SlimSeq : Module {

	enum ParamIds {
		STEP1_KNOB,
		OUT_KNOB = STEP1_KNOB + STEPCOUNT,
		STEP1_SEL,
		CLOCK_UI = STEP1_SEL + STEPCOUNT,
		REVERSE_UI,
		RNDMODE_UI,
		HOLD_UI,
		RESET_UI,
		NUM_PARAMS
	};

	enum InputIds {
		CLOCK_IN,
		REVERSE_IN,
		RNDMODE_IN,
		HOLD_IN,
		RESET_IN,
		STEP1_IN,

		NUM_INPUTS = STEP1_IN + STEPCOUNT
	};

	enum OutputIds {
		OUT,
		NUM_OUTPUTS
	};

	enum LightIds {
		REVERSE_LIGHT,
		RNDMODE_LIGHT,
		FORWARD_LIGHT,
		STEP1_LIGHT,
		OUT_POS_LED = STEP1_LIGHT + STEPCOUNT,
		OUT_NEG_LED,
		NUM_LIGHTS
	};

	SlimSeq() : Module( NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS ) {
		reset();
	};

	void step() override;

	int currentPos = 0;
	int resetPos = 0;

	bool isHold = false;
	bool isRandom  = false;
	bool isReverse = false;

	SchmittTrigger clockTrigger, reverseTrigger, randomTrigger, holdTrigger, resetTrigger;

	SchmittTrigger clockButton, reverseButton, randomButton, holdButton, resetButton;

	SchmittTrigger posButtons[STEPCOUNT];

	void reset() override {
		currentPos = 0;
		resetPos = 0;
		isHold = false;
		isRandom = false;
		isReverse = false;
		
		for(int i=0; i<STEPCOUNT; i++){
			lights[STEP1_LIGHT + i].value = 0.0f;
		}
	};
};


void SlimSeq::step() {
	float out = 0.0f;

	// playmodes
	if(inputs[CLOCK_IN].active && !isHold) {
		if (clockTrigger.process(inputs[CLOCK_IN].value) ) {
			if (isRandom) {
				currentPos = roundf(randomf()*15.0f);
			}
			else {
				if (isReverse)
					currentPos--;
				else
					currentPos++;
			}
		}
	}

	// playmode triggers & controls
	if (reverseTrigger.process(inputs[REVERSE_IN].value)) isReverse  = !isReverse;
	if (reverseButton.process(params[REVERSE_UI].value)) isReverse  = !isReverse;
	
	if (randomTrigger.process(inputs[RNDMODE_IN].value)) isRandom  = !isRandom;
	if (randomButton.process(params[RNDMODE_UI].value)) isRandom  = !isRandom;

	if (holdTrigger.process(inputs[HOLD_IN].value)) isHold = !isHold;
	if (holdButton.process(params[HOLD_UI].value)) isHold = !isHold;

	if (resetTrigger.process(inputs[RESET_IN].value)) currentPos = resetPos;
	if (resetButton.process(params[RESET_UI].value)) currentPos = resetPos;

	// position selectors
	for(int i=0; i<STEPCOUNT; i++) {
		if(posButtons[i].process(params[STEP1_SEL+i].value)) {
			resetPos = i;
			currentPos = (!inputs[CLOCK_IN].active || isHold) ? i : currentPos;
		}
	}

	// do the steps
	while ( currentPos < 0 )
		currentPos += STEPCOUNT;
	while ( currentPos >= STEPCOUNT )
		currentPos -= STEPCOUNT;
	
	// calc out
	out = (inputs[STEP1_IN+currentPos].active) ? // ...
		inputs[STEP1_IN+currentPos].normalize(0.0f) * (params[STEP1_KNOB+currentPos].value * 0.2f)
		: params[STEP1_KNOB+currentPos].value;

	outputs[OUT].value = clampf(out * params[OUT_KNOB].value, -10.0f, 10.0f);

	// lights
		// direction
	lights[REVERSE_LIGHT].setBrightnessSmooth(isHold ? 1.0f : isRandom ? 0.0f : isReverse ? 1.0f : 0.0f);
	lights[RNDMODE_LIGHT].setBrightnessSmooth(isHold ? 1.0f : isRandom ? 1.0f : 0.0f);
	lights[FORWARD_LIGHT].setBrightnessSmooth(isHold ? 1.0f : isRandom ? 0.0f : isReverse ? 0.0f : 1.0f);
		// steps
	for(int i=0; i<STEPCOUNT; i++) {
		lights[STEP1_LIGHT+i].setBrightnessSmooth(i == currentPos ? 1.0f:0.0f);
		lights[STEP1_LIGHT+i].setBrightnessSmooth(i == resetPos ? 0.5f:0.0f);
	}
		// out
	lights[OUT_POS_LED].setBrightnessSmooth(outputs[OUT].value < 0 ? -outputs[OUT].value*0.2f : 0.0f);
	lights[OUT_NEG_LED].setBrightnessSmooth(outputs[OUT].value > 0 ?  outputs[OUT].value*0.2f : 0.0f);
};

template <typename BASE>
struct PvCBigLight : BASE {
	PvCBigLight() {
		this->box.size = Vec(22, 22);
	}
};
struct BlueOrangeLight : ModuleLightWidget {
	BlueOrangeLight() {
		addBaseColor(COLOR_BLUE);
		addBaseColor(COLOR_ORANGE);
	}
};
struct StepButton : SVGSwitch, MomentarySwitch {
	StepButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(11,11);
	}
};
struct ControlButton : StepButton {
	ControlButton() {
		box.size = Vec(24,14);
	}
};
struct PvCKnobT : PvCKnob {
	PvCKnobT() {
		setSVG(SVG::load(assetPlugin(plugin, "res/components/PvCKnobT.svg")));
	}
};

SlimSeqWidget::SlimSeqWidget() {

	SlimSeq *module = new SlimSeq();
	setModule(module);
	box.size = Vec(15*8, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/panels/SlimSeq.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead2>(Vec(15, 0)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x-30, 0)));
	addChild(createScrew<ScrewHead3>(Vec(15, 365)));
	addChild(createScrew<ScrewHead1>(Vec(box.size.x-30, 365)));
	// steps
	for (int i = 0; i < 8; i++)	{
		int top = 42;
		// left
		addChild(createLight<PvCBigLight<BlueLight>>(Vec(4,22 + top * i),module,SlimSeq::STEP1_LIGHT + STEPCOUNT-1 - i));
		addParam(createParam<PvCKnobT>(Vec(4,22 + top * i),module,SlimSeq::STEP1_KNOB + STEPCOUNT-1 - i , -5.0f, 5.0f, 0.0f));
		addInput(createInput<InPortAud>(Vec(18,42 + top * i),module,SlimSeq::STEP1_IN + + STEPCOUNT-1 - i));
		addParam(createParam<StepButton>(Vec(29,29 + top * i),module,SlimSeq::STEP1_SEL + STEPCOUNT-1 - i , 0, 1, 0));
		// right
		addParam(createParam<StepButton>(Vec(80,29 + top * i),module,SlimSeq::STEP1_SEL + i , 0, 1, 0));
		addChild(createLight<PvCBigLight<BlueLight>>(Vec(94,22 + top * i),module,SlimSeq::STEP1_LIGHT + i));
		addInput(createInput<InPortAud>(Vec(80,42 + top * i),module,SlimSeq::STEP1_IN + i));
		addParam(createParam<PvCKnobT>(Vec(94,22 + top * i),module,SlimSeq::STEP1_KNOB + i, -5.0f, 5.0f, 0.0f));
	}
	// direction lights
	addChild(createLight<SmallLight<OrangeLight>>(Vec(49,22),module,SlimSeq::REVERSE_LIGHT));
	addChild(createLight<SmallLight<OrangeLight>>(Vec(57,22),module,SlimSeq::RNDMODE_LIGHT));
	addChild(createLight<SmallLight<OrangeLight>>(Vec(65,22),module,SlimSeq::FORWARD_LIGHT));
	// control inputs
	addInput(createInput<InPortBin>(Vec(49,48),module,SlimSeq::CLOCK_IN));
	//addParam(createParam<ControlButton>(Vec(48,70),module,SlimSeq::CLOCK_UI , 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(49,90),module,SlimSeq::REVERSE_IN));
	addParam(createParam<ControlButton>(Vec(48,112),module,SlimSeq::REVERSE_UI , 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(49,132),module,SlimSeq::RNDMODE_IN));
	addParam(createParam<ControlButton>(Vec(48,154),module,SlimSeq::RNDMODE_UI , 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(49,174),module,SlimSeq::HOLD_IN));
	addParam(createParam<ControlButton>(Vec(48,192),module,SlimSeq::HOLD_UI , 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(49,216),module,SlimSeq::RESET_IN));
	addParam(createParam<ControlButton>(Vec(48,238),module,SlimSeq::RESET_UI , 0, 1, 0));
	// main out
	addChild(createLight<PvCBigLight<BlueOrangeLight>>(Vec(49,286),module,SlimSeq::OUT_POS_LED));
	addParam(createParam<PvCKnobT>(Vec(49,286),module,SlimSeq::OUT_KNOB, -1.0f, 1.0f, 1.0f));
	addOutput(createOutput<OutPortVal>(Vec(49,326),module,SlimSeq::OUT));
}