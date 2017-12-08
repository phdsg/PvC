#include "rack.hpp"


using namespace rack;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

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

struct SquareButton : SVGSwitch, MomentarySwitch {
	SquareButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/SquareButton.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/SquareButtonD.svg")));
	}
};

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


// Label widget by Jeremy Wentworth
/*
struct CenteredLabel : Widget {
	std::string text;
	int fontSize;
	
	CenteredLabel(int _fontSize = 10) {
		fontSize = _fontSize;
		box.size.y = BND_WIDGET_HEIGHT;
	}
	void draw(NVGcontext *vg) override {
		nvgTextAlign(vg, NVG_ALIGN_CENTER);
		nvgFillColor(vg, nvgRGB(178, 34, 34));
		nvgFontSize(vg, fontSize);
		nvgText(vg, box.pos.x, box.pos.y, text.c_str(), NULL);
	}
};
*/

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