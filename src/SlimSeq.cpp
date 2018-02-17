/*

--no-title-yet-- (SlimSeq)

16 step seq (based on m.lueders' sequential switch)

TODO:

	-if-bool cleanup
	-save modestates and resetpos
	-adjustable step count
	-playmodes: pendulum/ping-pong, one-shot
	-output slew (portamento)
	-useful unclocked/held operation
	-(maybe) momentary control inputs (instead of flips) / optional
	-(maybe) unipolar/bipolar options
	-(maybe) gates

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"

#include "dsp/digital.hpp" // SchmittTrigger PulseGenerator
#include "dsp/filter.hpp" // SlewLimiter

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
	int counterPos = 0;
	int resetPos = 0;

	bool isRunning = false;
	bool isHold = false;
	bool isRandom  = false;
	bool isReverse = false;
	bool isHopper = false;

	float stepInput = 0.0f;

	SchmittTrigger clockTrigger, reverseTrigger, randomTrigger, holdTrigger, resetTrigger;
	SchmittTrigger clockButton, reverseButton, randomButton, holdButton, resetButton;
	SchmittTrigger posButtons[STEPCOUNT];

	void reset() override {
		currentPos = 0;
		counterPos = 0;
		resetPos = 0;

		isRunning = false;
		isHold = false;
		isRandom = false;
		isReverse = false;
		isHopper = false;

		stepInput = 0.0f;

		for(int i=0; i<STEPCOUNT; i++){
			lights[STEP1_LIGHT + i].value = 0.0f;
		}
	};
};


void SlimSeq::step() {
	stepInput = inputs[STEP1_IN+currentPos].normalize(5.0f) * params[STEP1_KNOB+currentPos].value;
	
	isRunning = (isHold) ? false : inputs[CLOCK_IN].active;
	// clocked
	if(isRunning) {
		if (clockTrigger.process(inputs[CLOCK_IN].value) ) {
			counterPos += (isReverse) ? -1 : 1;

			if (isRandom) {
				if (isHopper) {
					currentPos += round(randomUniform()*15.0f);
				}
				else {
					currentPos += (round(randomUniform())) ? 1 : -1;
				}
			}
			else {
				currentPos = counterPos;
			}
		}
	}

	// playmode triggers & controls
	if (isRandom) {	// in rnd mode use rev trigger to switch hopper/walker
		if (reverseTrigger.process(inputs[REVERSE_IN].value)) isHopper  = !isHopper;
		if (reverseButton.process(params[REVERSE_UI].value)) isHopper = !isHopper;
	}
	else {
		if (reverseTrigger.process(inputs[REVERSE_IN].value)) isReverse  = !isReverse;
		if (reverseButton.process(params[REVERSE_UI].value)) isReverse  = !isReverse;
	}

	if (randomTrigger.process(inputs[RNDMODE_IN].value)) isRandom  = !isRandom;
	if (randomButton.process(params[RNDMODE_UI].value)) isRandom  = !isRandom;

	if (holdTrigger.process(inputs[HOLD_IN].value)) isHold = !isHold;
	if (holdButton.process(params[HOLD_UI].value)) isHold = !isHold;

	if (resetTrigger.process(inputs[RESET_IN].value)) {
		 counterPos = resetPos;
		 currentPos = resetPos;
	}
	if (resetButton.process(params[RESET_UI].value)) {
		counterPos = resetPos;
		currentPos = resetPos;
	}

	// position selectors
	for(int i=0; i<STEPCOUNT; i++) {
		if(posButtons[i].process(params[STEP1_SEL+i].value)) {
			resetPos = i;
			currentPos = (!isRunning) ? i : currentPos;
		}
	}

	// loop bg counter (to have a good return point when leaving rnd)
	while ( counterPos < 0 )
		counterPos += STEPCOUNT;
	while ( counterPos > (STEPCOUNT-1) )
		counterPos -= STEPCOUNT;

	// loop the current
	while ( currentPos < 0 )
		currentPos += STEPCOUNT;
	while ( currentPos > (STEPCOUNT-1) )
		currentPos -= STEPCOUNT;


		// calc out
	outputs[OUT].value = clamp(stepInput * params[OUT_KNOB].value, -10.0f, 10.0f);

	// lights
		// direction
	lights[REVERSE_LIGHT].value = isHold ? 1.0f : isRandom&&isHopper ? 0.0f : isRandom&&!isHopper ? 1.0f : isReverse ? 1.0f : 0.0f;
	lights[RNDMODE_LIGHT].value = isHold ? 1.0f : isRandom&&isHopper ? 1.0f : isRandom&&!isHopper ? 0.0f : 0.0f;
	lights[FORWARD_LIGHT].value = isHold ? 1.0f : isRandom&&isHopper ? 0.0f : isRandom&&!isHopper ? 1.0f : isReverse ? 0.0f : 1.0f;
		// steplights
	for(int i=0; i<STEPCOUNT; i++) {
		lights[STEP1_LIGHT+i].value = (i == currentPos) ? 1.0f : (i == resetPos) ? 0.5f:0.0f;
	}
		// out
	lights[OUT_POS_LED].value = (outputs[OUT].value < 0) ? -outputs[OUT].value*0.2f : 0.0f;
	lights[OUT_NEG_LED].value = (outputs[OUT].value > 0) ?  outputs[OUT].value*0.2f : 0.0f;

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

struct SlimSeqWidget : ModuleWidget {
	SlimSeqWidget(SlimSeq *module);
};

SlimSeqWidget::SlimSeqWidget(SlimSeq *module) : ModuleWidget(module) {

	box.size = Vec(15*8, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/panels/SlimSeq.svg")));
		addChild(panel);
	}
	// screws
	addChild(Widget::create<ScrewHead2>(Vec(15, 0)));
	addChild(Widget::create<ScrewHead4>(Vec(box.size.x-30, 0)));
	addChild(Widget::create<ScrewHead3>(Vec(15, 365)));
	addChild(Widget::create<ScrewHead1>(Vec(box.size.x-30, 365)));
	// steps
	for (int i = 0; i < 8; i++)	{
		int top = 42;
		// left 16-9
		addChild(ModuleLightWidget::create<PvCBigLED<BlueLight>>(Vec(18,42 + top * i),module,SlimSeq::STEP1_LIGHT + STEPCOUNT-1 - i));
		addParam(ParamWidget::create<PvCLEDKnob>(Vec(18,42 + top * i),module,SlimSeq::STEP1_KNOB + STEPCOUNT-1 - i , -1.0f, 1.0f, 0.0f));
		addInput(Port::create<InPortAud>(Vec(4,22 + top * i), Port::INPUT, module,SlimSeq::STEP1_IN + + STEPCOUNT-1 - i));
		addParam(ParamWidget::create<StepButton>(Vec(29,29 + top * i),module,SlimSeq::STEP1_SEL + STEPCOUNT-1 - i , 0, 1, 0));
		// right 1-8
		addParam(ParamWidget::create<StepButton>(Vec(80,29 + top * i),module,SlimSeq::STEP1_SEL + i , 0, 1, 0));
		addChild(ModuleLightWidget::create<PvCBigLED<BlueLight>>(Vec(80,42 + top * i),module,SlimSeq::STEP1_LIGHT + i));
		addInput(Port::create<InPortAud>(Vec(94,22 + top * i), Port::INPUT, module,SlimSeq::STEP1_IN + i));
		addParam(ParamWidget::create<PvCLEDKnob>(Vec(80,42 + top * i),module,SlimSeq::STEP1_KNOB + i, -1.0f, 1.0f, 0.0f));
	}
	// direction lights
	addChild(ModuleLightWidget::create<SmallLight<OrangeLight>>(Vec(49,22),module,SlimSeq::REVERSE_LIGHT));
	addChild(ModuleLightWidget::create<SmallLight<OrangeLight>>(Vec(57,22),module,SlimSeq::RNDMODE_LIGHT));
	addChild(ModuleLightWidget::create<SmallLight<OrangeLight>>(Vec(65,22),module,SlimSeq::FORWARD_LIGHT));
	// control inputs
	addInput(Port::create<InPortBin>(Vec(49,48), Port::INPUT, module,SlimSeq::CLOCK_IN));
	//addParam(ParamWidget::create<ControlButton>(Vec(48,70),module,SlimSeq::CLOCK_UI , 0, 1, 0));
	addInput(Port::create<InPortBin>(Vec(49,90), Port::INPUT, module,SlimSeq::REVERSE_IN));
	addParam(ParamWidget::create<ControlButton>(Vec(48,112),module,SlimSeq::REVERSE_UI , 0, 1, 0));
	addInput(Port::create<InPortBin>(Vec(49,132), Port::INPUT, module,SlimSeq::RNDMODE_IN));
	addParam(ParamWidget::create<ControlButton>(Vec(48,154),module,SlimSeq::RNDMODE_UI , 0, 1, 0));
	addInput(Port::create<InPortBin>(Vec(49,174), Port::INPUT, module,SlimSeq::HOLD_IN));
	addParam(ParamWidget::create<ControlButton>(Vec(48,192),module,SlimSeq::HOLD_UI , 0, 1, 0));
	addInput(Port::create<InPortBin>(Vec(49,216), Port::INPUT, module,SlimSeq::RESET_IN));
	addParam(ParamWidget::create<ControlButton>(Vec(48,238),module,SlimSeq::RESET_UI , 0, 1, 0));
	// main out
	addChild(ModuleLightWidget::create<PvCBigLED<GreenRedLight>>(Vec(49,292),module,SlimSeq::OUT_POS_LED));
	addParam(ParamWidget::create<PvCLEDKnob>(Vec(49,292),module,SlimSeq::OUT_KNOB, -1.0f, 1.0f, 1.0f));
	addOutput(Port::create<OutPortVal>(Vec(49,326), Port::OUTPUT, module,SlimSeq::OUT));
}

Model *modelSlimSeq = Model::create<SlimSeq, SlimSeqWidget>(
	"PvC", "SlimSeq", "SlimSeq", SEQUENCER_TAG, SWITCH_TAG);