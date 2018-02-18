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

		NUM_OUTPUTS
	};
	enum LightIds {
		GATE_LED,
		NATE_LED,

		NUM_LIGHTS
	};

	bool gate = false;
	
	CoSuOf() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
	void reset() override {
		gate = false;
	}
};

void CoSuOf::step() {

	float sumOut = (inputs[POS_IN].value * params[POS_LVL].value) - (inputs[NEG_IN].value * params[NEG_LVL].value) + params[OFFSET].value;
	float gap = params[GAP].value;

	if (sumOut > gap) gate = true;
	if (sumOut <= -gap) gate = false;

	outputs[SUM_OUT].value = sumOut; // TODO:
	outputs[GATE_OUT].value = gate * 10.0f;
	outputs[NATE_OUT].value = !gate * 10.0f;
	
	lights[GATE_LED].value = gate;
	lights[NATE_LED].value = !gate;
}

struct CoSuOfWidget : ModuleWidget {
	CoSuOfWidget(CoSuOf *module);
};

CoSuOfWidget::CoSuOfWidget(CoSuOf *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/panels/CoSuOf.svg")));
	// screws
	addChild(Widget::create<ScrewHead1>(Vec(0, 0)));
	addChild(Widget::create<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(Widget::create<ScrewHead3>(Vec(0, 365)));
	addChild(Widget::create<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(Port::create<InPortAud>(Vec(4,22), Port::INPUT, module,CoSuOf::POS_IN));
	addParam(ParamWidget::create<PvCKnob>(Vec(4,48),module,CoSuOf::POS_LVL, 0.0f, 1.0f, 1.0f));

	addInput(Port::create<InPortAud>(Vec(4,90), Port::INPUT, module,CoSuOf::NEG_IN));
	addParam(ParamWidget::create<PvCKnob>(Vec(4,116),module,CoSuOf::NEG_LVL, 0.0f, 1.0f, 1.0f));

	addParam(ParamWidget::create<PvCKnob>(Vec(4,168),module,CoSuOf::OFFSET, -10.0f, 10.0f, 0.0f));
	addOutput(Port::create<OutPortVal>(Vec(4,208), Port::OUTPUT, module,CoSuOf::SUM_OUT));

	addChild(ModuleLightWidget::create<PvCBigLED<GreenRedLight>>(Vec(4, 258),module, CoSuOf::GATE_LED));
	addParam(ParamWidget::create<PvCLEDKnob>(Vec(4,258),module,CoSuOf::GAP, 0.0f, 10.0f, 0.0f));
	addOutput(Port::create<OutPortBin>(Vec(4,300), Port::OUTPUT, module,CoSuOf::GATE_OUT));
	addOutput(Port::create<OutPortBin>(Vec(4,336), Port::OUTPUT, module,CoSuOf::NATE_OUT));
}

Model *modelCoSuOf = Model::create<CoSuOf, CoSuOfWidget>(
	"PvC", "CoSuOf", "CoSuOf", LOGIC_TAG, ATTENUATOR_TAG);