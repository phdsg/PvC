#include "pvc.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	// This is the unique identifier for your plugin
	p->slug = "PvC";

#ifdef VERSION
	p->version = TOSTRING(VERSION);
#else
    p->version = "dev";
#endif
	// p->website = "https://phdsg.github.io/PvC/";
	// p->manual = "https://phdsg.github.io/PvC/";
	// For each module, specify the ModuleWidget
	p->addModel(createModel<BangDaButtonWidget>("PvC", "BangDaButton", "BangDaButton", CONTROLLER_TAG, SWITCH_TAG));
	// p->addModel(createModel<BitSHWidget>("PvC", "BitSH", "BitSH", DIGITAL_TAG, SEQUENCER_TAG, LOGIC_TAG, SWITCH_TAG));
	p->addModel(createModel<CompairWidget>("PvC", "Compair", "Compair", LOGIC_TAG, SEQUENCER_TAG, DIGITAL_TAG, EFFECT_TAG, DUAL_TAG));
	p->addModel(createModel<CoSuOfWidget>("PvC", "CoSuOf", "CoSuOf", LOGIC_TAG, EFFECT_TAG));
	p->addModel(createModel<GeighthsWidget>("PvC", "Geighths", "Geighths", SEQUENCER_TAG, SWITCH_TAG));
	p->addModel(createModel<ShutItWidget>("PvC", "ShutIt", "ShutIt", SWITCH_TAG, MULTIPLE_TAG));
	p->addModel(createModel<ShutItXLWidget>("PvC", "ShutItXL", "ShutItXL", SWITCH_TAG, MULTIPLE_TAG));
	p->addModel(createModel<SlimSeqWidget>("PvC", "SlimSeq", "SlimSeq", SWITCH_TAG, SEQUENCER_TAG));
	p->addModel(createModel<SumItWidget>("PvC", "SumIt", "SumIt", MIXER_TAG));
	p->addModel(createModel<TaHaSaHaNWidget>("PvC", "TaHaSaHaN", "TaHaSaHaN", SAMPLE_AND_HOLD_TAG));
	p->addModel(createModel<VampsWidget>("PvC", "Vamps", "Vamps", AMPLIFIER_TAG, DUAL_TAG));
	p->addModel(createModel<VubarWidget>("PvC", "Vubar", "VuBar", VISUAL_TAG));
	p->addModel(createModel<PvCBlankWidget>("PvC", "PvC64", "PvC64", BLANK_TAG));

	
	
	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}