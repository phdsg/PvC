/*

CoSuOf

Co(mparator)
Su(bstractor)
Of(fsetter)

"a la D-A167"

INS:
 POS * POS_ATTN[0..1]
 NEG * NEG_ATTN[0..1]

OUTS:
 SUM = POS - NEG + OFFSET[-10..10]
 GATE = high[10] if SUM is > 0 / low[0] if SUM is <= 0
 NATE = !GATE

WARNING: due to how the module operates you can produce excessivly high (or
low) voltages on the SUM output (+/- 30V for the extreme cases of inputs).
i didn't want to clamp or scale the output.
so, use the attenuators and a meter to adjust for desired V-range!

*/////////////////////////////////////////////////////////////////////////////


#include "pvc.hpp"


struct CoSuOf : Module {
	enum ParamIds {
		POS_LVL,
		NEG_LVL,
		OFFSET,
		GAP,

		NUM_PARAMS
	};
	enum InputIds {
		POS_IN,
		NEG_IN,

		NUM_INPUTS
	};
	enum OutputIds {
		SUM_OUT,
		GATE_OUT,
		NATE_OUT,
		MUS_OUT,
		G_SUM,
		N_SUM,
		G_POS,
		N_POS,
		G_NEG,
		N_NEG,

		NUM_OUTPUTS
	};
	enum LightIds {
		GATE_LED,
		NATE_LED,

		NUM_LIGHTS
	};

	bool gate = false;
	
	CoSuOf() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(POS_LVL, 0.0f, 1.0f, 1.0f);
		configParam(NEG_LVL, 0.0f, 1.0f, 1.0f);
		configParam(OFFSET, -10.0f, 10.0f, 0.0f);
		configParam(GAP, 0.0f, 10.0f, 0.0f);
	}

	void process(const ProcessArgs &args) override;
	void onReset() override {
		gate = false;
	}
};

void CoSuOf::process(const ProcessArgs &args) {
	float posIn = inputs[POS_IN].getVoltage() * params[POS_LVL].getValue();
	float negIn = inputs[NEG_IN].getVoltage() * params[NEG_LVL].getValue();
	float sumOut = clamp(posIn - negIn + params[OFFSET].getValue(), -10.0f, 10.0f);
	float gap = params[GAP].getValue();

	if (sumOut > gap) gate = true;
	if (sumOut <= -gap) gate = false;

	outputs[SUM_OUT].setVoltage(sumOut); // TODO:
	outputs[MUS_OUT].setVoltage(-sumOut);
	outputs[GATE_OUT].setVoltage(gate * 10.0f);
	outputs[NATE_OUT].setVoltage(!gate * 10.0f);

	outputs[G_SUM].setVoltage(gate * sumOut);
	outputs[N_SUM].setVoltage(!gate * sumOut);
	outputs[G_POS].setVoltage(gate * posIn);
	outputs[N_POS].setVoltage(!gate * posIn);
	outputs[G_NEG].setVoltage(gate * negIn);
	outputs[N_NEG].setVoltage(!gate * negIn);

	lights[GATE_LED].value = gate;
	lights[NATE_LED].value = !gate;
}

struct CoSuOfWidget : ModuleWidget {
	CoSuOfWidget(CoSuOf *module);
};

CoSuOfWidget::CoSuOfWidget(CoSuOf *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/CoSuOf.svg")));
	// screws
	addChild(createWidget<ScrewHead1>(Vec(0, 0)));
	addChild(createWidget<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createWidget<ScrewHead3>(Vec(0, 365)));
	addChild(createWidget<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22), module,CoSuOf::POS_IN));
	addInput(createInput<InPortAud>(Vec(34,22), module,CoSuOf::NEG_IN));

	addParam(createParam<PvCKnob>(Vec(4,64),module,CoSuOf::POS_LVL));
	
	addParam(createParam<PvCKnob>(Vec(34,64),module,CoSuOf::NEG_LVL));
	

	addParam(createParam<PvCKnob>(Vec(19,104),module,CoSuOf::OFFSET));
	
	addOutput(createOutput<OutPortVal>(Vec(4,158), module,CoSuOf::SUM_OUT));
	addOutput(createOutput<OutPortVal>(Vec(34,158), module,CoSuOf::MUS_OUT));

	addParam(createParam<PvCKnob>(Vec(19,192),module,CoSuOf::GAP));
	

	addChild(createLight<FourPixLight<OrangeLED>>(Vec(13, 244),module, CoSuOf::GATE_LED));
	addChild(createLight<FourPixLight<BlueLED>>(Vec(43, 244),module, CoSuOf::NATE_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,250), module,CoSuOf::GATE_OUT));
	addOutput(createOutput<OutPortBin>(Vec(34,250), module,CoSuOf::NATE_OUT));

	addOutput(createOutput<OutPortVal>(Vec(4,288), module,CoSuOf::G_SUM));
	addOutput(createOutput<OutPortVal>(Vec(34,288), module,CoSuOf::N_SUM));
	addOutput(createOutput<OutPortVal>(Vec(4,312), module,CoSuOf::G_POS));
	addOutput(createOutput<OutPortVal>(Vec(34,312), module,CoSuOf::N_POS));
	addOutput(createOutput<OutPortVal>(Vec(4,336), module,CoSuOf::G_NEG));
	addOutput(createOutput<OutPortVal>(Vec(34,336), module,CoSuOf::N_NEG));

}

Model *modelCoSuOf = createModel<CoSuOf, CoSuOfWidget>("CoSuOf");