#include "rack.hpp"

using namespace rack;

extern Plugin *plugin;

// module widgets
struct BangDaButtonWidget : ModuleWidget {
	BangDaButtonWidget();
};
struct CompairWidget : ModuleWidget {
	CompairWidget();
	Menu *createContextMenu() override;
};
struct GeighthsWidget : ModuleWidget {
	GeighthsWidget();
};
struct MultyWidget : ModuleWidget {
	MultyWidget();
};
struct OomphWidget : ModuleWidget {
	OomphWidget();
};
struct ShutItWidget : ModuleWidget {
	ShutItWidget();
};
struct ShutItSlimWidget : ModuleWidget {
	ShutItSlimWidget();
};
struct ShutItXLWidget : ModuleWidget {
	ShutItXLWidget();
};
struct SlimSeqWidget : ModuleWidget {
	SlimSeqWidget();
};
struct SumItWidget : ModuleWidget {
	SumItWidget();
};
struct TaHaSaHWidget : ModuleWidget {
	TaHaSaHWidget();
};
struct VampsWidget : ModuleWidget {
	VampsWidget();
};
struct VubarWidget : ModuleWidget {
	VubarWidget();
};



// shared custom components

/// ports

struct OutPortBin : SVGPort {
	OutPortBin() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/OutPortBin.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};
struct OutPortVal : SVGPort {
	OutPortVal() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/OutPortVal.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};
struct InPortBin : SVGPort {
	InPortBin() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/InPortBin.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};
struct InPortAud : SVGPort {
	InPortAud() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/InPortAud.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};
struct InPortCtrl : SVGPort {
	InPortCtrl() {
		background->svg = SVG::load(assetPlugin(plugin, "res/components/InPortCtrl.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};

/// knobs & buttons

struct PvCKnob : RoundKnob {
	PvCKnob() {
		setSVG(SVG::load(assetPlugin(plugin, "res/components/PvCKnob.svg")));
		box.size = Vec(22,22);
	}
};
struct PvCSnapKnob : PvCKnob {
	PvCSnapKnob() {
		snap = true;
	}
};

/// colorful lights

struct WhiteLight : ModuleLightWidget {
 	WhiteLight() { addBaseColor(COLOR_WHITE); }
};
struct OrangeLight : ModuleLightWidget {
	OrangeLight() {	addBaseColor(COLOR_ORANGE);	}
};
struct PurpleLight : ModuleLightWidget {
	PurpleLight() {	addBaseColor(COLOR_PURPLE);	}
};
struct CyanLight : ModuleLightWidget {
	CyanLight() { addBaseColor(COLOR_CYAN);	}
};

/// custom screwheads

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
