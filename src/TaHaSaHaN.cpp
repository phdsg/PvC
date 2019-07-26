/*

TaHaSaHaN

TrackAndHoldAndSampleAndHoldandNoise

the mix knob blends SaH and TaH signals into one output.
left side: SaH
right side: TaH

TODO: cv handling

*/////////////////////////////////////////////////////////////////////////////


#include "pvc.hpp"

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

	dsp::SchmittTrigger sampleTrigger;

	float input = 0.0f;
	float snhOut = 0.0f;
	float tnhOut = 0.0f;
	float noise = 0.0f;

	TaHaSaHaN() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(BLEND, 0.0f, 1.0f, 0.5f);
	}

	void process(const ProcessArgs &args) override;

	void onReset() override {
		input = snhOut = tnhOut = noise = 0.0f;
	}

};

void TaHaSaHaN::process(const ProcessArgs &args) {
	noise = random::uniform()*10.0f - 5.0f;

	if (inputs[TRIGGER].isConnected()){
		input = inputs[SAMPLE].getNormalVoltage(noise);
		if (sampleTrigger.process(inputs[TRIGGER].getVoltage())){
			snhOut = input;
		}
		if (inputs[TRIGGER].getVoltage() > 0.01f){
			tnhOut = input;
		}
	}
	else snhOut = tnhOut = 0.0f;

	// TODO: better cv/knob interaction.
	float blend = params[BLEND].getValue();
	if (inputs[BLEND_CV].isConnected())
		blend *= clamp(inputs[BLEND_CV].getVoltage() * 0.1f, 0.0f, 1.0f);

	outputs[MIX].setVoltage(crossfade(snhOut,tnhOut,blend));
	outputs[SNH].setVoltage(snhOut);
	outputs[TNH].setVoltage(tnhOut);
	outputs[NOISE].setVoltage(noise);

}

struct TaHaSaHaNWidget : ModuleWidget {
	TaHaSaHaNWidget(TaHaSaHaN *module);
};

TaHaSaHaNWidget::TaHaSaHaNWidget(TaHaSaHaN *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/TaHaSaHaN.svg")));

	// screws
	addChild(createWidget<ScrewHead4>(Vec(0, 0)));
	// addChild(createWidget<ScrewHead1>(Vec(box.size.x - 15, 0)));
	// addChild(createWidget<ScrewHead3>(Vec(0, 365)));
	addChild(createWidget<ScrewHead2>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22), module, TaHaSaHaN::SAMPLE));
	addInput(createInput<InPortBin>(Vec(4,64), module, TaHaSaHaN::TRIGGER));
	

	addOutput(createOutput<OutPortVal>(Vec(4,132), module, TaHaSaHaN::TNH));
	addOutput(createOutput<OutPortVal>(Vec(4,176), module, TaHaSaHaN::SNH));
	addOutput(createOutput<OutPortVal>(Vec(4,220), module, TaHaSaHaN::NOISE));

	addParam(createParam<PvCKnob>(Vec(4,258),module, TaHaSaHaN::BLEND));

	addInput(createInput<InPortCtrl>(Vec(4,282), module, TaHaSaHaN::BLEND_CV));
	addOutput(createOutput<OutPortVal>(Vec(4,336), module, TaHaSaHaN::MIX));
	
}

Model *modelTaHaSaHaN = createModel<TaHaSaHaN, TaHaSaHaNWidget>("TaHaSaHaN");