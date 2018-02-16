#include "rack.hpp"

using namespace rack;

extern Plugin *plugin;

// module widgets
struct AorBtoOutWidget : ModuleWidget { AorBtoOutWidget(); };
struct BangDaButtonWidget : ModuleWidget { BangDaButtonWidget(); };
// struct BitSHWidget : ModuleWidget { BitSHWidget(); };
// struct CoinTreeWidget : ModuleWidget { CoinTreeWidget(); };
struct CompairWidget : ModuleWidget { CompairWidget(); Menu *createContextMenu() override; };
struct CoSuOfWidget : ModuleWidget { CoSuOfWidget(); };
struct FlipOLogicWidget : ModuleWidget { FlipOLogicWidget(); };
struct GeighthsWidget : ModuleWidget { GeighthsWidget(); };
struct InToAorBWidget : ModuleWidget { InToAorBWidget(); };
struct ShutItWidget : ModuleWidget { ShutItWidget(); };
struct SlimSeqWidget : ModuleWidget { SlimSeqWidget(); };
struct SumItWidget : ModuleWidget { SumItWidget(); };
struct TaHaSaHaNWidget : ModuleWidget { TaHaSaHaNWidget(); };
struct VampsWidget : ModuleWidget { VampsWidget(); };
struct VubarWidget : ModuleWidget { VubarWidget(); };
struct PvCBlankWidget : ModuleWidget { PvCBlankWidget(); };

//////////////////////////////////////////////////////////////////////////////

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
struct PvCLEDKnob : PvCKnob {
	PvCLEDKnob() {
		setSVG(SVG::load(assetPlugin(plugin, "res/components/PvCKnobT.svg")));
	}
};
struct PvCSnapKnob : PvCKnob {
	PvCSnapKnob() {
		snap = true;
	}
};

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

template <typename BASE>
struct PvCBigLED : BASE {
	PvCBigLED() {
		this->box.size = Vec(22, 22);
	}
};


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
