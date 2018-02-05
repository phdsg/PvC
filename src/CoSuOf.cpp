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

		NUM_OUTPUTS
	};
	enum LightIds {
		GATE_LED,
		NATE_LED,

		NUM_LIGHTS
	};

	CoSuOf() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
	bool gate = false;
};


void CoSuOf::step() {

	float sumOut = (inputs[POS_IN].value * params[POS_LVL].value) - (inputs[NEG_IN].value * params[NEG_LVL].value) + params[OFFSET].value;
	float gap = params[GAP].value;

	if (sumOut > gap) gate = true;
	if (sumOut <= -gap) gate = false;

	outputs[SUM_OUT].value = sumOut/3.0f;
	outputs[GATE_OUT].value = gate * 10.0f;
	outputs[NATE_OUT].value = !gate * 10.0f;
	
	lights[GATE_LED].value = gate;
	lights[NATE_LED].value = !gate;
}

CoSuOfWidget::CoSuOfWidget() {
	CoSuOf *module = new CoSuOf();
	setModule(module);
	box.size = Vec(15*2, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/CoSuOf.svg")));
		addChild(panel);
	}
	// screws
	addChild(createScrew<ScrewHead1>(Vec(0, 0)));
	addChild(createScrew<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createScrew<ScrewHead3>(Vec(0, 365)));
	addChild(createScrew<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortAud>(Vec(4,22),module,CoSuOf::POS_IN));
	addParam(createParam<PvCKnob>(Vec(4,48),module,CoSuOf::POS_LVL, 0.0f, 1.0f, 0.5f));

	addInput(createInput<InPortAud>(Vec(4,90),module,CoSuOf::NEG_IN));
	addParam(createParam<PvCKnob>(Vec(4,116),module,CoSuOf::NEG_LVL, 0.0f, 1.0f, 0.5f));

	addParam(createParam<PvCKnob>(Vec(4,168),module,CoSuOf::OFFSET, -10.0f, 10.0f, 0.0f));
	addOutput(createOutput<OutPortVal>(Vec(4,208),module,CoSuOf::SUM_OUT));

	addChild(createLight<PvCBigLED<GreenRedLight>>(Vec(4, 258),module, CoSuOf::GATE_LED));
	addParam(createParam<PvCLEDKnob>(Vec(4,258),module,CoSuOf::GAP, 0.0f, 10.0f, 0.0f));
	addOutput(createOutput<OutPortBin>(Vec(4,300),module,CoSuOf::GATE_OUT));
	addOutput(createOutput<OutPortBin>(Vec(4,336),module,CoSuOf::NATE_OUT));
}