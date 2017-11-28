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
	p->addModel(createModel<MultyWidget>("PvC", "Multy", "1x6 Multiple", MULTIPLE_TAG, UTILITY_TAG));
	p->addModel(createModel<VubarWidget>("PvC", "Vubar", "LED VU Meter", VISUAL_TAG, UTILITY_TAG));

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
