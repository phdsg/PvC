/*

TaHaSaH

TrackAndHoldAndSampleAndHold

Input signal (random noise if not connected) is fed into a TnH and a SnH.
While the SnH takes one sample on every clocktrigger and holds it until
it receives the next trigger, the TnH follows the signal as long as the gate
is high and as gate goes low holds the last sample until the gate opens again.

the mix knob blends the two signals into one output.
left side: SaH
right side: TaH

TODO:

- 2 channels (B normalized A)

*/////////////////////////////////////////////////////////////////////////////


#include "pvc.hpp"
#include "dsp/digital.hpp" // SchmittTrigger // PulseGenerator

struct TaHaSaH : Module {
	enum ParamIds {
		BLEND_A_KNOB,
		BLEND_B_KNOB,

		NUM_PARAMS
	};
	enum InputIds {
		SAMPLE_A_IN,
		TRIGGER_A_IN,
		BLEND_A_CV,
		
		SAMPLE_B_IN,
		TRIGGER_B_IN,
		BLEND_B_CV,

		NUM_INPUTS
	};
	enum OutputIds {
		MIX_A_OUT,
		MIX_B_OUT,

		NUM_OUTPUTS
	};
	enum LightIds {
		MIX_A_POS,
		MIX_A_NEG,

		MIX_B_POS,
		MIX_B_NEG,
		
		NUM_LIGHTS
	};

	TaHaSaH() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;

	SchmittTrigger triggerA;
	SchmittTrigger triggerB;
	float inputA = 0.0f;
	float inputB = 0.0f;
	float sampleAOut = 0.0f;
	float sampleBOut = 0.0f;
	float trackAOut = 0.0f;
	float trackBOut = 0.0f;
};



void TaHaSaH::step() {

	if (inputs[TRIGGER_A_IN].active){
		float noiseA = randomf()*10.0f - 5.0f;
		inputA = inputs[SAMPLE_A_IN].normalize(noiseA);
		if (triggerA.process(inputs[TRIGGER_A_IN].value)){
			sampleAOut = inputA;
		}
		if (inputs[TRIGGER_A_IN].value > 0.01f){
			trackAOut = inputA;
		}
	} else sampleAOut = trackAOut = 0.0f;

	if (inputs[TRIGGER_B_IN].active){
		float noiseB = randomf()*10.0f - 5.0f;
		inputB = inputs[SAMPLE_B_IN].normalize(noiseB);
		if (triggerB.process(inputs[TRIGGER_B_IN].value)){
			sampleBOut = inputB;
		}
		if (inputs[TRIGGER_B_IN].value > 0.01f){
			trackBOut = inputB;
		}
	} else sampleBOut = trackBOut = 0.0f;


	// TODO: better handling of cv/knob interaction.
	float blendA = params[BLEND_A_KNOB].value;
	if (inputs[BLEND_A_CV].active)
		blendA *= clampf(inputs[BLEND_A_CV].value * 0.1f, 0.0f, 1.0f);

	float blendB = params[BLEND_B_KNOB].value;
	if (inputs[BLEND_B_CV].active)
		blendB *= clampf(inputs[BLEND_B_CV].value * 0.1f, 0.0f, 1.0f);

	outputs[MIX_A_OUT].value = crossf(sampleAOut,trackAOut,blendA);
	lights[MIX_A_POS].setBrightnessSmooth(outputs[MIX_A_OUT].value >= 0 ? outputs[MIX_A_OUT].value*0.1f : 0.0f);
	lights[MIX_A_NEG].setBrightnessSmooth(outputs[MIX_A_OUT].value < 0 ? -outputs[MIX_A_OUT].value*0.1f : 0.0f);
	outputs[MIX_B_OUT].value = crossf(sampleBOut,trackBOut,blendB);
	lights[MIX_B_POS].setBrightnessSmooth(outputs[MIX_B_OUT].value >= 0 ? outputs[MIX_B_OUT].value*0.1f : 0.0f);
	lights[MIX_B_NEG].setBrightnessSmooth(outputs[MIX_B_OUT].value < 0 ? -outputs[MIX_B_OUT].value*0.1f : 0.0f);
}

template <typename BASE>
struct SampleLight : BASE {
	SampleLight() {
		this->box.size = Vec(22, 22);
	}
};

TaHaSaHWidget::TaHaSaHWidget() {
	TaHaSaH *module = new TaHaSaH();
	setModule(module);
	box.size = Vec(15*2, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/panel2HEdual.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead4>(Vec(0, 0)));
	addChild(createScrew<ScrewHead1>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22),module, TaHaSaH::SAMPLE_A_IN));
	addInput(createInput<InPortBin>(Vec(4,50),module, TaHaSaH::TRIGGER_A_IN));

	addChild(createLight<SampleLight<GreenRedLight>>(Vec(4,80),module,TaHaSaH::MIX_A_POS));

	addOutput(createOutput<OutPortVal>(Vec(4,108),module, TaHaSaH::MIX_A_OUT));
	addParam(createParam<PvCKnob>(Vec(4,134),module, TaHaSaH::BLEND_A_KNOB, 0.0f, 1.0f, 0.5f));
	addInput(createInput<InPortCtrl>(Vec(4,160),module, TaHaSaH::BLEND_A_CV));

	addInput(createInput<InPortAud>(Vec(4,198),module, TaHaSaH::SAMPLE_B_IN));
	addInput(createInput<InPortBin>(Vec(4,226),module, TaHaSaH::TRIGGER_B_IN));

	addChild(createLight<SampleLight<GreenRedLight>>(Vec(4,256),module,TaHaSaH::MIX_B_POS));

	addOutput(createOutput<OutPortVal>(Vec(4,284),module, TaHaSaH::MIX_B_OUT));
	addParam(createParam<PvCKnob>(Vec(4,310),module, TaHaSaH::BLEND_B_KNOB, 0.0f, 1.0f, 0.5f));
	addInput(createInput<InPortCtrl>(Vec(4,336),module, TaHaSaH::BLEND_B_CV));

}