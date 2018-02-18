#include "rack.hpp"

using namespace rack;

extern Plugin *plugin;

// module widgets
extern Model *modelHeads;
extern Model *modelTails;
extern Model *modelBangDaButton;
extern Model *modelCompair;
extern Model *modelCoSuOf;
extern Model *modelFlipOLogic;
extern Model *modelGeighths;
extern Model *modelShutIt;
extern Model *modelSlimSeq;
extern Model *modelSumIt;
extern Model *modelTaHaSaHaN;
extern Model *modelVamps;
extern Model *modelVubar;
extern Model *modelPvCBlank;

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

struct LabelButtonL : SVGSwitch, MomentarySwitch {
	LabelButtonL() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/LabelButtonL_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/LabelButtonL_1.svg")));
		box.size = Vec(36,12);
	}
};
struct LabelButtonS : SVGSwitch, MomentarySwitch {
	LabelButtonS() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/LabelButtonS_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/components/LabelButtonS_1.svg")));

		box.size = Vec(24,12);
	}
};
struct ModeToggle : SVGSwitch, ToggleSwitch {
	ModeToggle() {
		addFrame(SVG::load(assetPlugin(plugin, "res/components/empty.svg")));
		box.size = Vec(12,6);
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

template <typename BASE>
 struct FourPixLight : BASE {
 	FourPixLight() {
 		this->box.size = Vec(4, 4);
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
