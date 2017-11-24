#include "rack.hpp"


using namespace rack;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct MultyWidget : ModuleWidget {
	MultyWidget();
};

struct ShiftieWidget : ModuleWidget {
	ShiftieWidget();
};

struct VubarWidget : ModuleWidget {
	VubarWidget();
};

//// custom components

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
// knobs & buttons
struct SquareButton : SVGSwitch, MomentarySwitch {
	SquareButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/SquareButton.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/SquareButtonD.svg")));
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