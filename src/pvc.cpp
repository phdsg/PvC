#include "pvc.hpp"


// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	// This is the unique identifier for your plugin
	p->slug = "PvC";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
	p->website = "https://phdsg.github.io/PvC/";
	// For each module, specify the ModuleWidget subclass, manufacturer slug (for saving in patches), manufacturer human-readable name, module slug, and module name
	p->addModel(createModel<CompairWidget>("PvC", "Compair", "Compair", LOGIC_TAG, SEQUENCER_TAG, RING_MODULATOR_TAG, DIGITAL_TAG, EFFECT_TAG, DUAL_TAG));
	p->addModel(createModel<MultyWidget>("PvC", "Multy", "Multy", MULTIPLE_TAG, UTILITY_TAG));
	p->addModel(createModel<VampsWidget>("PvC", "Vamps", "Vamps", AMPLIFIER_TAG, DUAL_TAG));
	p->addModel(createModel<VubarWidget>("PvC", "Vubar", "VuBar", VISUAL_TAG, UTILITY_TAG));
	//p->addModel(createModel<VoobarWidget>("PvC", "Voobar", "LED VU Meter 2", VISUAL_TAG, UTILITY_TAG));
	p->addModel(createModel<OomphWidget>("PvC", "Oomph", "Oomph", WAVESHAPER_TAG, DISTORTION_TAG, EFFECT_TAG, DYNAMICS_TAG));


	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
