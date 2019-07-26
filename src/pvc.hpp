#include "rack.hpp"

using namespace rack;

extern Plugin *pluginInstance;

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

// extern Model *modelPvCBlank;
// extern Model *modelPlainVoidCanvas;

//////////////////////////////////////////////////////////////////////////////

struct OutPortBin : SVGPort {
	OutPortBin() {
		// background->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/OutPortBin.svg"));
		// background->wrap();
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/OutPortBin.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};
struct OutPortVal : SVGPort {
	OutPortVal() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/OutPortVal.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};
struct InPortBin : SVGPort {
	InPortBin() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/InPortBin.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};
struct InPortAud : SVGPort {
	InPortAud() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/InPortAud.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};
struct InPortCtrl : SVGPort {
	InPortCtrl() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/InPortCtrl.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};

/// knobs & buttons

struct PvCKnob : RoundKnob {
	PvCKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/PvCKnob.svg")));
		box.size = Vec(22,22);
	}
};
struct PvCLEDKnob : PvCKnob {
	PvCLEDKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/PvCKnobT.svg")));
	}
};
struct PvCSnapKnob : PvCKnob {
	PvCSnapKnob() {
		snap = true;
	}
};

struct LabelButtonL : SVGSwitch {
	LabelButtonL() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LabelButtonL_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LabelButtonL_1.svg")));
		box.size = Vec(36,12);
	}
};

struct LabelButtonS : SVGSwitch {
	LabelButtonS() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LabelButtonS_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LabelButtonS_1.svg")));
		box.size = Vec(24,12);
	}
};

/// lights

struct PvCLight : GrayModuleLightWidget{
	PvCLight() { bgColor = nvgRGB(0x00, 0x00, 0x00); }
};
struct RedLED : PvCLight {
 	RedLED() {
 		addBaseColor(SCHEME_RED); }
};
struct GreenLED : PvCLight {
 	GreenLED() {
 		addBaseColor(SCHEME_GREEN); }
};
struct BlueLED : PvCLight {
 	BlueLED() {
 		addBaseColor(SCHEME_BLUE); }
};
struct WhiteLED : PvCLight {
 	WhiteLED() {
 		addBaseColor(SCHEME_WHITE); }
};
struct OrangeLED : PvCLight {
	OrangeLED() {
		addBaseColor(SCHEME_ORANGE);	}
};
struct YellowLED : PvCLight {
	YellowLED() {
		addBaseColor(SCHEME_YELLOW);	}
};
struct PurpleLED : PvCLight {
	PurpleLED() {
		addBaseColor(SCHEME_PURPLE);	}
};
struct CyanLED : PvCLight {
	CyanLED() {
		addBaseColor(SCHEME_CYAN);	}
};
struct GreenRedLED : PvCLight {
 	GreenRedLED() {
 		addBaseColor(SCHEME_GREEN);
 		addBaseColor(SCHEME_RED); }
};
struct WhiteRedLED : PvCLight {
	WhiteRedLED() {
		addBaseColor(SCHEME_WHITE);
		addBaseColor(SCHEME_RED);
	}
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

template <typename BASE>
 struct EightPixLight : BASE {
 	EightPixLight() {
 		this->box.size = Vec(8, 8);
 	}
 };

/// screws

struct ScrewHead1 : SVGScrew {
	ScrewHead1() {
		sw->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/ScrewHead1.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};
struct ScrewHead2 : SVGScrew {
	ScrewHead2() {
		sw->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/ScrewHead2.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};
struct ScrewHead3 : SVGScrew {
	ScrewHead3() {
		sw->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/ScrewHead3.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};
struct ScrewHead4 : SVGScrew {
	ScrewHead4() {
		sw->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/ScrewHead4.svg"));
		sw->wrap();
		box.size = sw->box.size;
	}
};
