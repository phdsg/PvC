#include "rack.hpp"


using namespace rack;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct MultyWidget : ModuleWidget {
	MultyWidget();
};


//// custom components

// ports
struct BluePort : SVGPort {
	BluePort() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/BluePort.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};
struct RedPort : SVGPort {
	RedPort() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/RedPort.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};
//button
struct MuteBezel : SVGSwitch, MomentarySwitch {
	MuteBezel() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/MuteBezel.svg")));
	}
};
//screws
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