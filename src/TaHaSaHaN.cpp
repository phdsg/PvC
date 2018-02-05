/*

TaHaSaHaN

TrackAndHoldAndSampleAndHoldandNoise

Input signal (random noise if not connected) is fed into a TnH and a SnH.
While the SnH takes one sample on every clocktrigger and holds it until
it receives the next trigger, the TnH follows the signal as long as the gate
is high and as gate goes low holds the last sample until the gate opens again.

the mix knob blends the two signals into one output.
left side: SaH
right side: TaH

*/////////////////////////////////////////////////////////////////////////////


#include "pvc.hpp"
#include "dsp/digital.hpp" // SchmittTrigger // PulseGenerator

struct TaHaSaHaN : Module {
	enum ParamIds {
		BLEND,
		
		NUM_PARAMS
	};

	enum InputIds {
		SAMPLE,
		TRIGGER,
		BLEND_CV,
		
		NUM_INPUTS
	};

	enum OutputIds {
		MIX,
		SNH,
		TNH,
		NOISE,
	
		NUM_OUTPUTS
	};

	enum LightIds {
	
		NUM_LIGHTS
	};

	TaHaSaHaN() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;

	SchmittTrigger sampleTrigger;

	float input = 0.0f;
	float snhOut = 0.0f;
	float tnhOut = 0.0f;

};

void TaHaSaHaN::step() {
	float noise = randomf()*10.0f - 5.0f;

	if (inputs[TRIGGER].active){
		input = inputs[SAMPLE].normalize(noise);
		if (sampleTrigger.process(inputs[TRIGGER].value)){
			snhOut = input;
		}
		if (inputs[TRIGGER].value > 0.01f){
			tnhOut = input;
		}
	}
	else snhOut = tnhOut = 0.0f;

	// TODO: better cv/knob interaction.
	float blend = params[BLEND].value;
	if (inputs[BLEND_CV].active)
		blend *= clampf(inputs[BLEND_CV].value * 0.1f, 0.0f, 1.0f);


	outputs[MIX].value = crossf(snhOut,tnhOut,blend);
	outputs[SNH].value = snhOut;
	outputs[TNH].value = tnhOut;
	outputs[NOISE].value = noise;

}


TaHaSaHaNWidget::TaHaSaHaNWidget() {
	TaHaSaHaN *module = new TaHaSaHaN();
	setModule(module);
	box.size = Vec(15*2, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/TaHaSaHaN.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead4>(Vec(0, 0)));
	// addChild(createScrew<ScrewHead1>(Vec(box.size.x - 15, 0)));
	// addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22),module, TaHaSaHaN::SAMPLE));
	addInput(createInput<InPortBin>(Vec(4,64),module, TaHaSaHaN::TRIGGER));
	
	addParam(createParam<PvCKnob>(Vec(4,124),module, TaHaSaHaN::BLEND, 0.0f, 1.0f, 0.5f));
	addInput(createInput<InPortCtrl>(Vec(4,150),module, TaHaSaHaN::BLEND_CV));
	addOutput(createOutput<OutPortVal>(Vec(4,196),module, TaHaSaHaN::MIX));

	addOutput(createOutput<OutPortVal>(Vec(4,252),module, TaHaSaHaN::TNH));
	addOutput(createOutput<OutPortVal>(Vec(4,294),module, TaHaSaHaN::SNH));
	addOutput(createOutput<OutPortVal>(Vec(4,336),module, TaHaSaHaN::NOISE));

}