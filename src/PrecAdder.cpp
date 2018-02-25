/*

working title

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"
#include "dsp/digital.hpp"

struct PrecAdder : Module {
	enum ParamIds {
		ATTEN,
		ACT_UI = ATTEN + 4,
		INV_UI = ACT_UI + 4,
		NUM_PARAMS = INV_UI + 4
	};
	enum InputIds {
		INPUT,
		ACT_TRIG = INPUT + 4,
		INV_TRIG = ACT_TRIG + 4,
		RESET_TRIG = INV_TRIG + 4,
		NUM_INPUTS 
	};
	enum OutputIds {
		SUM_OUT,
		INVSUM_OUT,

		NUM_OUTPUTS
	};
	enum LightIds {
		ACT_LED,
		INV_LED = ACT_LED + 4,
		NUM_LIGHTS = INV_LED + 4
	};

	bool chanActive[4] {};
	bool chanInvert[4] {};

	SchmittTrigger activeTrigger[4];
	SchmittTrigger invertTrigger[4];
	SchmittTrigger resetTrigger;

	PrecAdder() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {	}

	void step() override;
	
// TODO reset, rand, json

	void reset() override {
		for (int i = 0; i < 4; i++)	{
			chanActive[i] = false;
			chanInvert[i] = false;
		}
	}

	// void randomize() override;
	
	// json_t *toJson() override {
		// json_t *rootJ = json_object();
		// return rootJ;
	// }

	// void fromJson(json_t *rootJ) override {
	// }
};


void PrecAdder::step() {
	float sum = 0.0f;
	for (int i = 0; i < 4; i++)
	{
		if (activeTrigger[i].process(inputs[ACT_TRIG + i].value + params[ACT_UI + i].value)) {
			chanActive[i] = !chanActive[i];
		}
		if (invertTrigger[i].process(inputs[INV_TRIG + i].value + params[INV_UI + i].value)) {
			chanInvert[i] = !chanInvert[i];
		}
		float control = rescale(params[ATTEN + i].value, 0, 12, 0.0f, 1.0f);
		float chanIn = inputs[i].normalize(1.0f) * control;
		sum += ((chanInvert[i]) ? -chanIn : chanIn) * chanActive[i];

		lights[ACT_LED + i].value = chanActive[i];
		lights[INV_LED + i].value = chanInvert[i];
	}
	if (resetTrigger.process(inputs[RESET_TRIG].value)) {
		reset();
	}
	clamp(sum, -10.0f, 10.0f);
	outputs[SUM_OUT].value = sum;
	outputs[INVSUM_OUT].value = -sum;
}

 struct EmptyButton : SVGSwitch, MomentarySwitch {
	EmptyButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(58,33);
	}
};

struct PrecAdderWidget : ModuleWidget {
	PrecAdderWidget(PrecAdder *module);
};

PrecAdderWidget::PrecAdderWidget(PrecAdder *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/panels/PrecAdder.svg")));
	
	// screws
	addChild(Widget::create<ScrewHead1>(Vec(0, 0)));
	addChild(Widget::create<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(Widget::create<ScrewHead3>(Vec(0, 365)));
	addChild(Widget::create<ScrewHead4>(Vec(box.size.x - 15, 365)));
	
	for (int i = 0; i < 4; i++)
	{
		float top = 76.0f;
		addInput(Port::create<InPortAud>(Vec(4, 22 + top*i), Port::INPUT, module, PrecAdder::INPUT + i));
		addParam(ParamWidget::create<PvCSnapKnob>(Vec(4, 46 + top*i), module, PrecAdder::ATTEN + i, 0, 12, 12));
		
		addParam(ParamWidget::create<EmptyButton>(Vec(31, 18 + top*i), module, PrecAdder::ACT_UI + i, 0, 1, 0));
		addChild(ModuleLightWidget::create<EightPixLight<BlueLED>>(Vec(62, 30.5 + top*i), module, PrecAdder::ACT_LED + i));
		addInput(Port::create<InPortBin>(Vec(36, 23.5 + top*i), Port::INPUT, module, PrecAdder::ACT_TRIG + i));
		
		addParam(ParamWidget::create<EmptyButton>(Vec(31, 52 + top*i), module, PrecAdder::INV_UI + i, 0, 1, 0));
		addChild(ModuleLightWidget::create<EightPixLight<OrangeLED>>(Vec(62, 64.5 + top*i), module, PrecAdder::INV_LED + i));
		addInput(Port::create<InPortBin>(Vec(36, 57.5 + top*i), Port::INPUT, module, PrecAdder::INV_TRIG + i));
	}
		
	addOutput(Port::create<OutPortVal>(Vec(4,336), Port::OUTPUT, module, PrecAdder::SUM_OUT));
	addOutput(Port::create<OutPortVal>(Vec(34,336), Port::OUTPUT, module, PrecAdder::INVSUM_OUT));
	addInput(Port::create<InPortBin>(Vec(64, 326), Port::INPUT, module, PrecAdder::RESET_TRIG));
}

Model *modelPrecAdder = Model::create<PrecAdder, PrecAdderWidget>(
	"PvC", "PrecAdder", "PrecAdder", BLANK_TAG);