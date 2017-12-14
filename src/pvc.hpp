#include "rack.hpp"


using namespace rack;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct CompairWidget : ModuleWidget {
	CompairWidget();
};

struct MultyWidget : ModuleWidget {
	MultyWidget();
};

struct ShapeWidget : ModuleWidget {
	ShapeWidget();
};

struct VampsWidget : ModuleWidget {
	VampsWidget();
};

struct VubarWidget : ModuleWidget {
	VubarWidget();
};

struct VoobarWidget : ModuleWidget {
	VoobarWidget();
};

// custom components

// ports

struct OutPort : SVGPort {
	OutPort() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/OutPort.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};

struct InPort : SVGPort {
	InPort() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/InPort.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};

struct ModInPort : SVGPort {
	ModInPort() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/ModInPort.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};

// knobs & buttons

struct PvCKnob : RoundKnob {
	PvCKnob() {
		setSVG(SVG::load(assetPlugin(plugin, "res/components/PvCKnob.svg")));
		box.size = Vec(22,22);
	}
};
struct PvCBigKnob : RoundKnob {
	PvCBigKnob() {
		setSVG(SVG::load(assetPlugin(plugin, "res/components/PvCBigKnob.svg")));
		box.size = Vec(44,44);
	}
};

struct SquareButton : SVGSwitch, MomentarySwitch {
	SquareButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/SquareButton.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/SquareButtonD.svg")));
	}
};

struct PvCToggle : SVGSwitch, ToggleSwitch {
	PvCToggle() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/PvCToggleA.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/PvCToggleB.svg")));
	}
};
/*
struct PvCFader : SVGFader {
	PvCFader() {
		maxHandlePos = Vec(1, 1);
		minHandlePos = Vec(1, 225);
		background->svg = SVG::load(assetPlugin(plugin, "res/components/PvCFader.svg"));
		background->wrap();
		box.size = background->box.size;
		handle->svg = SVG::load(assetPlugin(plugin, "res/components/PvCFaderCap.svg"));
		handle->wrap();
	}
};
*/
struct PvCFader : SVGSlider {
	PvCFader() {
		maxHandlePos = Vec(1, 1);
		minHandlePos = Vec(1, 225);
		background->svg = SVG::load(assetPlugin(plugin, "res/components/PvCFader.svg"));
		background->wrap();
		box.size = background->box.size;
		handle->svg = SVG::load(assetPlugin(plugin, "res/components/PvCFaderCap.svg"));
		handle->wrap();
	}
};

// lights

struct WhiteLight : ModuleLightWidget {
 	WhiteLight() {
 		addBaseColor(COLOR_WHITE);
 	}
};

struct OrangeLight : ModuleLightWidget {
	OrangeLight() {
		addBaseColor(COLOR_ORANGE);
	}
};

struct PurpleLight : ModuleLightWidget {
	PurpleLight() {
		addBaseColor(COLOR_PURPLE);
	}
};

struct CyanLight : ModuleLightWidget {
	CyanLight() {
		addBaseColor(COLOR_CYAN);
	}
};

// backdrop for the meter leds
struct LEDback : SVGScrew {
	LEDback() {
		sw->svg = SVG::load(assetPlugin(plugin, "res/components/LEDback.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};


// custom screwheads
struct ScrewHead1 : SVGScrew {
	ScrewHead1() {
		sw->svg = SVG::load(assetPlugin(plugin, "res/components/ScrewHead1.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};

struct ScrewHead2 : SVGScrew {
	ScrewHead2() {
		sw->svg = SVG::load(assetPlugin(plugin, "res/components/ScrewHead2.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};

struct ScrewHead3 : SVGScrew {
	ScrewHead3() {
		sw->svg = SVG::load(assetPlugin(plugin, "res/components/ScrewHead3.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};

struct ScrewHead4 : SVGScrew {
	ScrewHead4() {
		sw->svg = SVG::load(assetPlugin(plugin, "res/components/ScrewHead4.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};
