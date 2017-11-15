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