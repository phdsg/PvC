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
    p->version = "0.5.8";
#endif
	p->website = "https://phdsg.github.io/PvC/";
	p->manual = "https://phdsg.github.io/PvC/";
	// For each module, specify the ModuleWidget
	p->addModel(createModel<BangDaButtonWidget>("PvC", "BangDaButton", "BangDaButton", CONTROLLER_TAG, SWITCH_TAG, UTILITY_TAG));
	p->addModel(createModel<CompairWidget>("PvC", "Compair", "Compair", LOGIC_TAG, SEQUENCER_TAG, RING_MODULATOR_TAG, DIGITAL_TAG, EFFECT_TAG, DUAL_TAG));
	p->addModel(createModel<MultyWidget>("PvC", "Multy", "Multy", MULTIPLE_TAG, SWITCH_TAG, UTILITY_TAG));
	p->addModel(createModel<OomphWidget>("PvC", "Oomph", "Oomph", WAVESHAPER_TAG, DISTORTION_TAG, EFFECT_TAG, DYNAMICS_TAG));
	p->addModel(createModel<ShutItWidget>("PvC", "ShutIt", "ShutIt", SWITCH_TAG, MULTIPLE_TAG, UTILITY_TAG));
	p->addModel(createModel<ShutItSlimWidget>("PvC", "ShutItSlim", "ShutItSlim", SWITCH_TAG, MULTIPLE_TAG, UTILITY_TAG));
	p->addModel(createModel<ShutItXLWidget>("PvC", "ShutItXL", "ShutItXL", SWITCH_TAG, MULTIPLE_TAG, UTILITY_TAG));
	p->addModel(createModel<SlimSeqWidget>("PvC", "SlimSeq", "SlimSeq", SWITCH_TAG, SEQUENCER_TAG));
	p->addModel(createModel<SumItWidget>("PvC", "SumIt", "SumIt", MIXER_TAG, UTILITY_TAG));
	p->addModel(createModel<VampsWidget>("PvC", "Vamps", "Vamps", AMPLIFIER_TAG, DUAL_TAG));
	p->addModel(createModel<VubarWidget>("PvC", "Vubar", "VuBar", VISUAL_TAG, UTILITY_TAG));


	
	
	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}