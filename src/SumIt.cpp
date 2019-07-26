/*

SumIt

...still needs some work

*/////////////////////////////////////////////////////////////////////////////

#include "pvc.hpp"

struct SumIt : Module {
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

	int mode[4] {};
	
	dsp::SchmittTrigger addTrigger[4];
	dsp::SchmittTrigger subTrigger[4];
	dsp::SchmittTrigger resetTrigger;

	SumIt() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		for (int i = 0; i < 4; i++) {
			configParam(ATTEN + i, 0, 12, 12);
			configParam(ACT_UI + i, 0, 1, 0);
			configParam(INV_UI + i, 0, 1, 0);
		}
	}

	void process(const ProcessArgs &args) override;
	
// TODO reset, rand, json

	void onReset() override {
		for (int i = 0; i < 4; i++)	{
			mode[i] = 0;
		}
	}

	// void randomize() override;
	
	// json_t *dataToJson() override {
		// json_t *rootJ = json_object();
		// return rootJ;
	// }

	// void dataFromJson(json_t *rootJ) override {
	// }
};


void SumIt::process(const ProcessArgs &args) {
	float sum = 0.0f;
	for (int i = 0; i < 4; i++)
	{
// TODO: change switch logic...
//	make the invertTrigger also trigger active.
//  when active: trigger at the active side -> off, trigger at the other side -> switch

		if (addTrigger[i].process(inputs[ACT_TRIG + i].getVoltage() + params[ACT_UI + i].getValue())) {
			if (mode[i] == 1) mode[i] = 0;
			else mode[i] = 1;
		}
		if (subTrigger[i].process(inputs[INV_TRIG + i].getVoltage() + params[INV_UI + i].getValue())) {
			if (mode[i] == 2) mode[i] = 0;
			else mode[i] = 2;
			
		}

		float control = rescale(params[ATTEN + i].getValue(), 0, 12, 0.0f, 1.0f);
		float chanIn = inputs[i].getNormalVoltage(1.0f) * control;
		sum += (mode[i] == 2) ? -chanIn : (mode[i] == 1) ? chanIn : 0.0f;

		lights[ACT_LED + i].value = (mode[i] == 1) ? 1:0;
		lights[INV_LED + i].value = (mode[i] == 2) ? 1:0;
	}
	if (resetTrigger.process(inputs[RESET_TRIG].getVoltage())) {
			for (int i = 0; i < 4; i++)	{
				mode[i] = 0;
			}
		//reset();
	}
	clamp(sum, -10.0f, 10.0f);
	outputs[SUM_OUT].setVoltage(sum);
	outputs[INVSUM_OUT].setVoltage(-sum);
}

 struct EmptyButton : SVGSwitch {
	EmptyButton() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/empty.svg")));
		box.size = Vec(58,33);
	}
};

struct SumItWidget : ModuleWidget {
	SumItWidget(SumIt *module);
};

SumItWidget::SumItWidget(SumIt *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/SumIt.svg")));
	
	// screws
	addChild(createWidget<ScrewHead1>(Vec(0, 0)));
	addChild(createWidget<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createWidget<ScrewHead3>(Vec(0, 365)));
	addChild(createWidget<ScrewHead4>(Vec(box.size.x - 15, 365)));
	
	for (int i = 0; i < 4; i++)
	{
		float top = 76.0f;
		addInput(createInput<InPortAud>(Vec(4, 22 + top*i), module, SumIt::INPUT + i));
//TODO:	stepped and unstepped knobs
		addParam(createParam<PvCSnapKnob>(Vec(4, 46 + top*i), module, SumIt::ATTEN + i));
		
		addParam(createParam<EmptyButton>(Vec(31, 18 + top*i), module, SumIt::ACT_UI + i));
		addChild(createLight<EightPixLight<BlueLED>>(Vec(62, 30.5 + top*i), module, SumIt::ACT_LED + i));
		addInput(createInput<InPortBin>(Vec(36, 23.5 + top*i), module, SumIt::ACT_TRIG + i));
		
		addParam(createParam<EmptyButton>(Vec(31, 52 + top*i), module, SumIt::INV_UI + i));
		addChild(createLight<EightPixLight<OrangeLED>>(Vec(62, 64.5 + top*i), module, SumIt::INV_LED + i));
		addInput(createInput<InPortBin>(Vec(36, 57.5 + top*i), module, SumIt::INV_TRIG + i));
	}
		
	addOutput(createOutput<OutPortVal>(Vec(4,336), module, SumIt::SUM_OUT));
	addOutput(createOutput<OutPortVal>(Vec(34,336), module, SumIt::INVSUM_OUT));
	addInput(createInput<InPortBin>(Vec(64, 326), module, SumIt::RESET_TRIG));
}

Model *modelSumIt = createModel<SumIt, SumItWidget>("SumIt");