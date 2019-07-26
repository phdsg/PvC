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

#define STEPCOUNT 16

struct SlimSeq : Module {

	enum ParamIds {
		STEP1_KNOB,
		SCL_KNOB = STEP1_KNOB + STEPCOUNT,
		STEP1_SEL,
		CLOCK_UI = STEP1_SEL + STEPCOUNT,
		REVERSE_UI,
		RNDMODE_UI,
		HOLD_UI,
		RESET_UI,
		OFF_KNOB,

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

	SlimSeq() {
	    config( NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS );
		for (int i = 0; i < 8; i++)	{
			configParam(STEP1_SEL + STEPCOUNT-1 - i , 0, 1, 0);
			configParam(STEP1_KNOB + STEPCOUNT-1 - i , -1.0f, 1.0f, 0.0f);
			configParam(STEP1_SEL + i , 0, 1, 0);
			configParam(STEP1_KNOB + i, -1.0f, 1.0f, 0.0f);
		}
		configParam(SCL_KNOB, -1.0f, 1.0f, 1.0f);
		configParam(OFF_KNOB, -5.0f, 5.0f, 0.0f);
		
		onReset();

	};

	void process(const ProcessArgs &args) override;

	int currentPos = 0;
	int counterPos = 0;
	int resetPos = 0;

	bool isRunning = false;
	bool isHold = false;
	bool isRandom  = false;
	bool isReverse = false;
	bool isHopper = false;

	float stepInput = 0.0f;

	dsp::SchmittTrigger clockTrigger, reverseTrigger, randomTrigger, holdTrigger, resetTrigger;
	dsp::SchmittTrigger posButtons[STEPCOUNT];

	void onReset() override {
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


void SlimSeq::process(const ProcessArgs &args) {
	stepInput = inputs[STEP1_IN+currentPos].getNormalVoltage(5.0f) * params[STEP1_KNOB+currentPos].getValue();
	
	isRunning = (isHold) ? false : inputs[CLOCK_IN].isConnected();
	// clocked
	if(isRunning) {
		if (clockTrigger.process(inputs[CLOCK_IN].getVoltage()) ) {
			counterPos += (isReverse) ? -1 : 1;

			if (isRandom) {
				if (isHopper) {
					currentPos += round(random::uniform()*15.0f);
				}
				else {
					currentPos += (round(random::uniform())) ? 1 : -1;
				}
			}
			else {
				currentPos = counterPos;
			}
		}
	}

	// playmode triggers & controls
	if (isRandom) {	// in rnd mode use rev trigger to switch hopper/walker
		if (reverseTrigger.process(inputs[REVERSE_IN].getVoltage() + params[REVERSE_UI].getValue()))
			isHopper  = !isHopper;
	}
	else {
		if (reverseTrigger.process(inputs[REVERSE_IN].getVoltage() + params[REVERSE_UI].getValue()))
			isReverse  = !isReverse;
	}

	if (randomTrigger.process(inputs[RNDMODE_IN].getVoltage() + params[RNDMODE_UI].getValue()))
		isRandom  = !isRandom;
	
	if (holdTrigger.process(inputs[HOLD_IN].getVoltage() + params[HOLD_UI].getValue()))
		isHold = !isHold;
	
	if (resetTrigger.process(inputs[RESET_IN].getVoltage() + params[RESET_UI].getValue())) {
		 counterPos = resetPos;
		 currentPos = resetPos;
	}
	

	// position selectors
	for(int i=0; i<STEPCOUNT; i++) {
		if(posButtons[i].process(params[STEP1_SEL+i].getValue())) {
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
	outputs[OUT].setVoltage(clamp(stepInput * params[SCL_KNOB].getValue() + params[OFF_KNOB].getValue(), -10.0f, 10.0f));

	// lights
		// direction
	lights[REVERSE_LIGHT].value = isHold ? 1.0f : isRandom&&isHopper ? 0.0f : isRandom&&!isHopper ? 1.0f : isReverse ? 1.0f : 0.0f;
	lights[RNDMODE_LIGHT].value = isHold ? 1.0f : isRandom&&isHopper ? 1.0f : isRandom&&!isHopper ? 0.0f : 0.0f;
	lights[FORWARD_LIGHT].value = isHold ? 1.0f : isRandom&&isHopper ? 0.0f : isRandom&&!isHopper ? 1.0f : isReverse ? 0.0f : 1.0f;
		// steplights
	for(int i=0; i<STEPCOUNT; i++) {
		lights[STEP1_LIGHT+i].value = (i == currentPos) ? 1.0f : (i == resetPos) ? 0.5f : 0.0f;
	}
		// out
	lights[OUT_POS_LED].value = (outputs[OUT].value < 0) ? -outputs[OUT].value*0.1f : 0.0f;
	lights[OUT_NEG_LED].value = (outputs[OUT].value > 0) ?  outputs[OUT].value*0.1f : 0.0f;

};

struct StepButton : SVGSwitch {
	StepButton() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/empty.svg")));
		box.size = Vec(73,30);
	}
};
// struct ControlButton : StepButton {
// 	ControlButton() {
// 		box.size = Vec(73,30);
// 	}
// };

struct SlimSeqWidget : ModuleWidget {
	SlimSeqWidget(SlimSeq *module);
};

SlimSeqWidget::SlimSeqWidget(SlimSeq *module) {
		setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/SlimSeq.svg")));

	// screws
	addChild(createWidget<ScrewHead2>(Vec(15, 0)));
	addChild(createWidget<ScrewHead4>(Vec(box.size.x-30, 0)));
	addChild(createWidget<ScrewHead3>(Vec(15, 365)));
	addChild(createWidget<ScrewHead1>(Vec(box.size.x-30, 365)));

	// control inputs
	addInput(createInput<InPortBin>(Vec(4,22), module,SlimSeq::CLOCK_IN));
	//addParam(createParam<ControlButton>(Vec(48,70),module,SlimSeq::CLOCK_UI , 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(34,22), module,SlimSeq::REVERSE_IN));
	// addParam(createParam<ControlButton>(Vec(48,270),module,SlimSeq::REVERSE_UI , 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(64,22), module,SlimSeq::HOLD_IN));
	// addParam(createParam<ControlButton>(Vec(48,270),module,SlimSeq::HOLD_UI , 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(94,22), module,SlimSeq::RNDMODE_IN));
	// addParam(createParam<ControlButton>(Vec(48,270),module,SlimSeq::RNDMODE_UI , 0, 1, 0));
	addInput(createInput<InPortBin>(Vec(124,22), module,SlimSeq::RESET_IN));
	// addParam(createParam<ControlButton>(Vec(48,270),module,SlimSeq::RESET_UI , 0, 1, 0));

	// direction lights
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(67,62),module,SlimSeq::REVERSE_LIGHT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(73,62),module,SlimSeq::RNDMODE_LIGHT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(79,62),module,SlimSeq::FORWARD_LIGHT));

	// steps
	for (int i = 0; i < 8; i++)	{
		int top = 31;
		// left 16-9
		addParam(createParam<StepButton>(Vec(1,69 + top * i),module,SlimSeq::STEP1_SEL + STEPCOUNT-1 - i));
		
		addParam(createParam<PvCKnob>(Vec(16,73 + top * i),module,SlimSeq::STEP1_KNOB + STEPCOUNT-1 - i));
		
		addInput(createInput<InPortCtrl>(Vec(40,73 + top * i), module,SlimSeq::STEP1_IN + + STEPCOUNT-1 - i));
		addChild(createLight<FourPixLight<BlueLED>>(Vec(65,82 + top * i),module,SlimSeq::STEP1_LIGHT + STEPCOUNT-1 - i));
		// right 1-8
		addParam(createParam<StepButton>(Vec(76,69 + top * i),module,SlimSeq::STEP1_SEL + i));
		
		addChild(createLight<FourPixLight<BlueLED>>(Vec(81,82 + top * i),module,SlimSeq::STEP1_LIGHT + i));
		addInput(createInput<InPortCtrl>(Vec(88,73 + top * i), module,SlimSeq::STEP1_IN + i));
		addParam(createParam<PvCKnob>(Vec(112,73 + top * i),module,SlimSeq::STEP1_KNOB + i));
		
	}
	
	// main out
	
	addParam(createParam<PvCKnob>(Vec(36,323),module,SlimSeq::SCL_KNOB));
	
	addChild(createLight<FourPixLight<GreenRedLED>>(Vec(73,330),module,SlimSeq::OUT_POS_LED));
	addOutput(createOutput<OutPortVal>(Vec(64,336), module,SlimSeq::OUT));
	addParam(createParam<PvCKnob>(Vec(92,323),module,SlimSeq::OFF_KNOB));
	
}

Model *modelSlimSeq = createModel<SlimSeq, SlimSeqWidget>("SlimSeq");