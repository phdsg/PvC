/*

SumIt

adds 8 inputs together and divides sum by number of active inputs

- volume knob controls output gain [0..2]V
- output clamped [-10..10]

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"
#include "dsp/digital.hpp"
#define CHANCOUNT 8


struct SumIt : Module {

	enum ParamIds {
		VOLUME,
		CH_ATTEN,
		CH_ACTIVE_UI = CH_ATTEN + CHANCOUNT,
		NUM_PARAMS = CH_ACTIVE_UI + CHANCOUNT
	};

	enum InputIds {
		CH_INPUT,
		CH_ACTIVE_TRIG = CH_INPUT + CHANCOUNT,
		NUM_INPUTS = CH_ACTIVE_TRIG + CHANCOUNT
	};

	enum OutputIds {
		AVG_OUT,
		SUM_OUT,
		NUM_OUTPUTS
	};

	enum LightIds {
		CH_ACTIVE_LED,
		NUM_LIGHTS = CH_ACTIVE_LED + CHANCOUNT
	};

	float mix = 0.0f;
	float sum = 0.0f;
	bool chanActive[CHANCOUNT] {};
	int count = 0;
	SchmittTrigger switchTrigger[CHANCOUNT];

	SumIt() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;

	void reset() override {
		mix = 0.0f;
		sum = 0.0f;
		count = 0;
		for (int i = 0; i < CHANCOUNT; i++) {
			chanActive[i] = false;
		}
		// TODO switches
	}
	//TODO json
};


void SumIt::step() {
	mix = 0.0f;
	sum = 0.0f;
	count = 0;
	
	// sum and count inputs
	for (int i = 0; i < CHANCOUNT; i++) {
		if (switchTrigger[i].process(inputs[CH_ACTIVE_TRIG + i].value + params[CH_ACTIVE_UI + i].value)) {
			chanActive[i] = !chanActive[i];
		}
		//TODO check switch
		sum += inputs[CH_INPUT + i].value * params[CH_ATTEN + i].value * chanActive[i];
		if (inputs[CH_INPUT + i].active && chanActive[i])
			count++;
		lights[CH_ACTIVE_LED + i].value = chanActive[i];
	}
	// divide sum by number of active inputs	
	mix = (count > 1 ) ? sum / count : sum;
	outputs[SUM_OUT].value = clamp(sum, -10.0f, 10.0f);
	// out gain and clamp
	outputs[AVG_OUT].value = clamp(mix * params[VOLUME].value, -10.0f, 10.0f);
}

struct EmptyButton : SVGSwitch, MomentarySwitch {
	EmptyButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(86,36);
	}
};

struct SumItWidget : ModuleWidget {
	SumItWidget(SumIt *module);
};

SumItWidget::SumItWidget(SumIt *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/panels/SumIt.svg")));
	// screws
	// addChild(Widget::create<ScrewHead1>(Vec(0, 0)));
	addChild(Widget::create<ScrewHead2>(Vec(box.size.x - 15, 0)));
	// addChild(Widget::create<ScrewHead3>(Vec(0, 365)));
	addChild(Widget::create<ScrewHead4>(Vec(box.size.x - 15, 365)));
	// inputs
	for (int i = 0; i < CHANCOUNT; i++) {
		addChild(ModuleLightWidget::create<FourPixLight<CyanLED>>(Vec(79,27 + 36*i), module, SumIt::CH_ACTIVE_LED + i));
		addParam(ParamWidget::create<EmptyButton>(Vec(2,19 + 36*i), module, SumIt::CH_ACTIVE_UI + i, 0, 1, 0));
		addParam(ParamWidget::create<PvCKnob>(Vec(4,26 + 36*i), module, SumIt::CH_ATTEN + i, -2.0f,2.0f,1.0f));
		addInput(Port::create<InPortAud>(Vec(28,26 + 36*i), Port::INPUT, module, SumIt::CH_INPUT + i));
		addInput(Port::create<InPortBin>(Vec(52,26 + 36*i), Port::INPUT, module, SumIt::CH_ACTIVE_TRIG + i));
	}
	// gain and out
	addParam(ParamWidget::create<PvCKnob>(Vec(4,336), module, SumIt::VOLUME, 0.0f,2.0f,1.0f));
	addOutput(Port::create<OutPortVal>(Vec(28,336), Port::OUTPUT, module, SumIt::AVG_OUT));
	addOutput(Port::create<OutPortVal>(Vec(64,336), Port::OUTPUT, module, SumIt::SUM_OUT));
}

Model *modelSumIt = Model::create<SumIt, SumItWidget>(
	"PvC", "SumIt", "SumIt", ATTENUATOR_TAG, AMPLIFIER_TAG, MIXER_TAG, SWITCH_TAG);