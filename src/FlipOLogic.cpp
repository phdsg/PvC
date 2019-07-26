/*

FlipOLogic

clock divider and logic gate module that provides a variety of ways to create
interesting rhythmic patterns by combining/comparing different clocksignals.

"main" input (center, below the logic outis feeding a series of 3 flipflops,
creating /2 (FLIP), /4 (FLOP) , /8 (FLAP) divisions of the original clock signal.
it's also the default input for logic input B.
FLOP is fed into the left (AND) column input and into logic input A.
FLAP is fed into the right (XOR) column input and into logic input C.

so, with one clock signal the logic section compares the main input vs
the FLOP and FLAP divisions and then the logic outs get
AND compared on the left side vs the left input and 
XOR compared on the right side vs the right input.

this internal routing can be broken with other external sources or by
repatching the module itself for even more interesting combinations.

*/////////////////////////////////////////////////////////////////////////////


#include "pvc.hpp"

struct FlipOLogic : Module {
	enum ParamIds {
		// IDEA: logic mode selectors for the left and right column
		// IDEA: left and right column outputs source selectors (previous or input)
		NUM_PARAMS
	};
	enum InputIds {
		FLIP_IN,
		LEFT_IN,
		RIGHT_IN,
		LGC_A_IN,
		LGC_B_IN,
		LGC_C_IN,

		NUM_INPUTS
	};
	enum OutputIds {
		FLIP_OUT,
		FLOP_OUT,
		FLAP_OUT,

		LGC_AND_OUT,
		LGC_NAND_OUT,
		LGC_OR_OUT,
		LGC_NOR_OUT,
		LGC_XOR_OUT,
		LGC_XNOR_OUT,

		LEFT_VS_AND_OUT,
		LEFT_VS_NAND_OUT,
		LEFT_VS_OR_OUT,
		LEFT_VS_NOR_OUT,
		LEFT_VS_XOR_OUT,
		LEFT_VS_XNOR_OUT,

		RIGHT_VS_AND_OUT,
		RIGHT_VS_NAND_OUT,
		RIGHT_VS_OR_OUT,
		RIGHT_VS_NOR_OUT,
		RIGHT_VS_XOR_OUT,
		RIGHT_VS_XNOR_OUT,

		NUM_OUTPUTS
	};
	enum LightIds {
		FLIP_LED,
		FLOP_LED,
		FLAP_LED,

		LGC_AND_LED,
		LGC_NAND_LED,
		LGC_OR_LED,
		LGC_NOR_LED,
		LGC_XOR_LED,
		LGC_XNOR_LED,

		LEFT_VS_AND_LED,
		LEFT_VS_NAND_LED,
		LEFT_VS_OR_LED,
		LEFT_VS_NOR_LED,
		LEFT_VS_XOR_LED,
		LEFT_VS_XNOR_LED,

		RIGHT_VS_AND_LED,
		RIGHT_VS_NAND_LED,
		RIGHT_VS_OR_LED,
		RIGHT_VS_NOR_LED,
		RIGHT_VS_XOR_LED,
		RIGHT_VS_XNOR_LED,

		NUM_LIGHTS
	};

	bool flpIn = false;

	bool flip = false;
	bool flop = false;
	bool flap = false;

	bool leftIn = false;
	bool rightIn = false;

	bool lgcInA = false;
	bool lgcInB = false;
	bool lgcInC = false;

	bool lgcAnd = false;
	bool lgcNand = false;
	bool lgcOr = false;
	bool lgcNor = false;
	bool lgcXor = false;
	bool lgcXnor = false;

	dsp::SchmittTrigger flipTrigger;
	dsp::SchmittTrigger flopTrigger;
	dsp::SchmittTrigger flapTrigger;

	FlipOLogic() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);}

	void process(const ProcessArgs &args) override;

	void onReset() override {
		flpIn = false;
		flip = flop = flap = false;
		leftIn = rightIn = false;
		lgcInA = lgcInB = lgcInC = false;
		lgcAnd = lgcNand = lgcOr = lgcNor = lgcXor = lgcXnor = false;
	}
};


void FlipOLogic::process(const ProcessArgs &args) {
	flpIn = inputs[FLIP_IN].getVoltage() > 0 ? true : false;

	flip = flipTrigger.process(flpIn) ? !flip : flip;
	flop = flopTrigger.process(flip) ? !flop : flop;
	flap = flapTrigger.process(flop) ? !flap : flap;

	leftIn = inputs[LEFT_IN].getNormalVoltage(flop) > 0 ? true : false;
	rightIn = inputs[RIGHT_IN].getNormalVoltage(flap) > 0 ? true : false;

	lgcInA = inputs[LGC_A_IN].getNormalVoltage(leftIn) > 0 ? true : false;
	lgcInB = inputs[LGC_B_IN].getNormalVoltage(flip) > 0 ? true : false;
	lgcInC = inputs[LGC_C_IN].getNormalVoltage(rightIn) > 0 ? true : false;

	lgcAnd = lgcInA && lgcInB && lgcInC;
	lgcNand = !lgcAnd;
	lgcOr = lgcInA || lgcInB || lgcInC;
	lgcNor = !lgcInA && !lgcInB && !lgcInC;
	lgcXor = (lgcInA && !lgcInB && !lgcInC) || (!lgcInA && lgcInB && !lgcInC) || (!lgcInA && !lgcInB && lgcInC);
	lgcXnor = !lgcXor;

	outputs[FLIP_OUT].setVoltage(flip * 10.0f);
	lights[FLIP_LED].value = flip;
	outputs[FLOP_OUT].setVoltage(flop * 10.0f);
	lights[FLOP_LED].value = flop;
	outputs[FLAP_OUT].setVoltage(flap * 10.0f);
	lights[FLAP_LED].value = flap;

	outputs[LGC_AND_OUT].setVoltage(lgcAnd * 10.0f);
	lights[LGC_AND_LED].value = lgcAnd;
	outputs[LGC_NAND_OUT].setVoltage(lgcNand * 10.0f);
	lights[LGC_NAND_LED].value = lgcNand;
	outputs[LGC_OR_OUT].setVoltage(lgcOr * 10.0f);
	lights[LGC_OR_LED].value = lgcOr;
	outputs[LGC_NOR_OUT].setVoltage(lgcNor * 10.0f);
	lights[LGC_NOR_LED].value = lgcNor;
	outputs[LGC_XOR_OUT].setVoltage(lgcXor * 10.0f);
	lights[LGC_XOR_LED].value = lgcXor;
	outputs[LGC_XNOR_OUT].setVoltage(lgcXnor * 10.0f);
	lights[LGC_XNOR_LED].value = lgcXnor;

	outputs[LEFT_VS_AND_OUT].setVoltage((leftIn && lgcAnd) * 10.0f);
	outputs[LEFT_VS_NAND_OUT].setVoltage((leftIn && lgcNand) * 10.0f);
	outputs[LEFT_VS_OR_OUT].setVoltage((leftIn && lgcOr) * 10.0f);
	outputs[LEFT_VS_NOR_OUT].setVoltage((leftIn && lgcNor) * 10.0f);
	outputs[LEFT_VS_XOR_OUT].setVoltage((leftIn && lgcXor) * 10.0f);
	outputs[LEFT_VS_XNOR_OUT].setVoltage((leftIn && lgcXnor) * 10.0f);
	lights[LEFT_VS_AND_LED].value = (leftIn && lgcAnd);
	lights[LEFT_VS_NAND_LED].value = (leftIn && lgcNand);
	lights[LEFT_VS_OR_LED].value = (leftIn && lgcOr);
	lights[LEFT_VS_NOR_LED].value = (leftIn && lgcNor);
	lights[LEFT_VS_XOR_LED].value = (leftIn && lgcXor);
	lights[LEFT_VS_XNOR_LED].value = (leftIn && lgcXnor);

	outputs[RIGHT_VS_AND_OUT].setVoltage((rightIn != lgcAnd) * 10.0f);
	outputs[RIGHT_VS_NAND_OUT].setVoltage((rightIn != lgcNand) * 10.0f);
	outputs[RIGHT_VS_OR_OUT].setVoltage((rightIn != lgcOr) * 10.0f);
	outputs[RIGHT_VS_NOR_OUT].setVoltage((rightIn != lgcNor) * 10.0f);
	outputs[RIGHT_VS_XOR_OUT].setVoltage((rightIn != lgcXor) * 10.0f);
	outputs[RIGHT_VS_XNOR_OUT].setVoltage((rightIn != lgcXnor) * 10.0f);
	lights[RIGHT_VS_AND_LED].value = (rightIn != lgcAnd);
	lights[RIGHT_VS_NAND_LED].value = (rightIn != lgcNand);
	lights[RIGHT_VS_OR_LED].value = (rightIn != lgcOr);
	lights[RIGHT_VS_NOR_LED].value = (rightIn != lgcNor);
	lights[RIGHT_VS_XOR_LED].value = (rightIn != lgcXor);
	lights[RIGHT_VS_XNOR_LED].value = (rightIn != lgcXnor);
}


struct FlipOLogicWidget : ModuleWidget {
	FlipOLogicWidget(FlipOLogic *module);
};

FlipOLogicWidget::FlipOLogicWidget(FlipOLogic *module) {
	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/FlipOLogic.svg")));

	// screws
	//addChild(createWidget<ScrewHead1>(Vec(0, 0)));
	addChild(createWidget<ScrewHead2>(Vec(box.size.x - 15, 0)));
	addChild(createWidget<ScrewHead3>(Vec(0, 365)));
	//addChild(createWidget<ScrewHead4>(Vec(box.size.x - 15, 365)));

	addInput(createInput<InPortBin>(Vec(4,22), module,FlipOLogic::LGC_A_IN));
	addInput(createInput<InPortBin>(Vec(34,22), module,FlipOLogic::LGC_B_IN));
	addInput(createInput<InPortBin>(Vec(64,22), module,FlipOLogic::LGC_C_IN));

	addChild(createLight<FourPixLight<BlueLED>>(Vec(13,55),module,FlipOLogic::LEFT_VS_AND_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,60), module,FlipOLogic::LEFT_VS_AND_OUT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(43,65),module,FlipOLogic::LGC_AND_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,70), module,FlipOLogic::LGC_AND_OUT));
	addChild(createLight<FourPixLight<PurpleLED>>(Vec(73,55),module,FlipOLogic::RIGHT_VS_AND_LED));
	addOutput(createOutput<OutPortBin>(Vec(64,60), module,FlipOLogic::RIGHT_VS_AND_OUT));

	addChild(createLight<FourPixLight<BlueLED>>(Vec(13,95),module,FlipOLogic::LEFT_VS_NAND_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,100), module,FlipOLogic::LEFT_VS_NAND_OUT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(43,105),module,FlipOLogic::LGC_NAND_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,110), module,FlipOLogic::LGC_NAND_OUT));
	addChild(createLight<FourPixLight<PurpleLED>>(Vec(73,95),module,FlipOLogic::RIGHT_VS_NAND_LED));
	addOutput(createOutput<OutPortBin>(Vec(64,100), module,FlipOLogic::RIGHT_VS_NAND_OUT));

	addChild(createLight<FourPixLight<BlueLED>>(Vec(13,135),module,FlipOLogic::LEFT_VS_OR_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,140), module,FlipOLogic::LEFT_VS_OR_OUT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(43,145),module,FlipOLogic::LGC_OR_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,150), module,FlipOLogic::LGC_OR_OUT));
	addChild(createLight<FourPixLight<PurpleLED>>(Vec(73,135),module,FlipOLogic::RIGHT_VS_OR_LED));
	addOutput(createOutput<OutPortBin>(Vec(64,140), module,FlipOLogic::RIGHT_VS_OR_OUT));

	addChild(createLight<FourPixLight<BlueLED>>(Vec(13,175),module,FlipOLogic::LEFT_VS_NOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,180), module,FlipOLogic::LEFT_VS_NOR_OUT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(43,185),module,FlipOLogic::LGC_NOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,190), module,FlipOLogic::LGC_NOR_OUT));
	addChild(createLight<FourPixLight<PurpleLED>>(Vec(73,175),module,FlipOLogic::RIGHT_VS_NOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(64,180), module,FlipOLogic::RIGHT_VS_NOR_OUT));

	addChild(createLight<FourPixLight<BlueLED>>(Vec(13,215),module,FlipOLogic::LEFT_VS_XOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,220), module,FlipOLogic::LEFT_VS_XOR_OUT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(43,225),module,FlipOLogic::LGC_XOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,230), module,FlipOLogic::LGC_XOR_OUT));
	addChild(createLight<FourPixLight<PurpleLED>>(Vec(73,215),module,FlipOLogic::RIGHT_VS_XOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(64,220), module,FlipOLogic::RIGHT_VS_XOR_OUT));

	addChild(createLight<FourPixLight<BlueLED>>(Vec(13,255),module,FlipOLogic::LEFT_VS_XNOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,260), module,FlipOLogic::LEFT_VS_XNOR_OUT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(43,265),module,FlipOLogic::LGC_XNOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,270), module,FlipOLogic::LGC_XNOR_OUT));
	addChild(createLight<FourPixLight<PurpleLED>>(Vec(73,255),module,FlipOLogic::RIGHT_VS_XNOR_LED));
	addOutput(createOutput<OutPortBin>(Vec(64,260), module,FlipOLogic::RIGHT_VS_XNOR_OUT));

	addInput(createInput<InPortBin>(Vec(4,294), module,FlipOLogic::LEFT_IN));
	addInput(createInput<InPortBin>(Vec(34,298), module,FlipOLogic::FLIP_IN));
	addInput(createInput<InPortBin>(Vec(64,294), module,FlipOLogic::RIGHT_IN));

	addChild(createLight<FourPixLight<BlueLED>>(Vec(13,331),module,FlipOLogic::FLOP_LED));
	addOutput(createOutput<OutPortBin>(Vec(4,336), module,FlipOLogic::FLOP_OUT));
	addChild(createLight<FourPixLight<OrangeLED>>(Vec(43,331),module,FlipOLogic::FLIP_LED));
	addOutput(createOutput<OutPortBin>(Vec(34,336), module,FlipOLogic::FLIP_OUT));
	addChild(createLight<FourPixLight<PurpleLED>>(Vec(73,331),module,FlipOLogic::FLAP_LED));
	addOutput(createOutput<OutPortBin>(Vec(64,336), module,FlipOLogic::FLAP_OUT));
}

Model *modelFlipOLogic = createModel<FlipOLogic, FlipOLogicWidget>("FlipOLogic");