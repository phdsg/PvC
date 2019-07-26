/*
 Vamps

 a 2 RackUnit stereo mod of Andrew Belt's Fundamental VCA

 MAYBE TODO:
  - third channel

*/////////////////////////////////////////////////////////////////////////////



#include "pvc.hpp"


struct Vamps : Module {
	enum ParamIds {
		LEVEL,
		
		NUM_PARAMS
	};
	enum InputIds {
		EXP_CV,
		LIN_CV,
		IN_L,
		IN_R,
		
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_L,
		OUT_R,
		
		NUM_OUTPUTS
	};

	Vamps() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
		configParam(LEVEL, 0.0f, 1.0f, 0.5f);
	}
	void process(const ProcessArgs &args) override;
};


void Vamps::process(const ProcessArgs &args) {

	float left = inputs[IN_L].getVoltage() * params[LEVEL].getValue();
	float right = inputs[IN_R].getNormalVoltage(inputs[IN_L].getVoltage()) * params[LEVEL].getValue();
	
	const float expBase = 50.0f;

	if (inputs[LIN_CV].isConnected()) {
		float linCV = clamp(inputs[LIN_CV].getVoltage() * 0.1f, 0.0f, 1.0f);
		left *= linCV;
		right *= linCV;
	}
	if (inputs[EXP_CV].isConnected()) {
		float expCV = rescale(powf(expBase, clamp(inputs[EXP_CV].getVoltage() / 10.0f, 0.0f, 1.0f)), 1.0, expBase, 0.0f, 1.0f);
		left *= expCV;
		right *= expCV;
	}
	outputs[OUT_L].setVoltage(left);
	outputs[OUT_R].setVoltage(right);
}

struct VampsWidget : ModuleWidget {	VampsWidget(Vamps *module); };

VampsWidget::VampsWidget(Vamps *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/Vamps.svg")));
	// screws
	addChild(createWidget<ScrewHead1>(Vec(0, 0)));
	//addChild(createWidget<ScrewHead2>(Vec(box.size.x - 15, 0)));
	//addChild(createWidget<ScrewHead3>(Vec(0, 365)));
	addChild(createWidget<ScrewHead4>(Vec(box.size.x - 15, 365)));
	
	addInput(createInput<InPortAud>(Vec(4, 22), module, Vamps::IN_L));
	addInput(createInput<InPortAud>(Vec(4, 64), module, Vamps::IN_R));

	addParam(createParam<PvCKnob>(Vec(4, 120), module, Vamps::LEVEL));
	
	addInput(createInput<InPortCtrl>(Vec(4, 164), module, Vamps::EXP_CV));
	addInput(createInput<InPortCtrl>(Vec(4, 208), module, Vamps::LIN_CV));

	addOutput(createOutput<OutPortVal>(Vec(4, 296), module, Vamps::OUT_L));
	addOutput(createOutput<OutPortVal>(Vec(4, 336), module, Vamps::OUT_R));
}
Model *modelVamps = createModel<Vamps, VampsWidget>("Vamps");