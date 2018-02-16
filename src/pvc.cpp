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

	p->addModel(createModel<CoSuOfWidget>		("PvC", "CoSuOf", "CoSuOf", LOGIC_TAG, EFFECT_TAG));
	p->addModel(createModel<SumItWidget>		("PvC", "SumIt", "SumIt", MIXER_TAG, ATTENUATOR_TAG, AMPLIFIER_TAG));
	p->addModel(createModel<TaHaSaHaNWidget>	("PvC", "TaHaSaHaN", "TaHaSaHaN", SAMPLE_AND_HOLD_TAG, NOISE_TAG, RANDOM_TAG));
	p->addModel(createModel<VampsWidget>		("PvC", "Vamps", "Vamps", AMPLIFIER_TAG, ATTENUATOR_TAG, DUAL_TAG));
	p->addModel(createModel<VubarWidget>		("PvC", "Vubar", "VuBar", VISUAL_TAG));

	p->addModel(createModel<AorBtoOutWidget>	("PvC", "AorBtoOut", "AorBtoOut", LOGIC_TAG, SWITCH_TAG));
	p->addModel(createModel<InToAorBWidget>		("PvC", "InToAorB", "InToAorB", LOGIC_TAG, SWITCH_TAG));		
	p->addModel(createModel<CompairWidget>		("PvC", "Compair", "Compair", LOGIC_TAG, DIGITAL_TAG, EFFECT_TAG, DUAL_TAG));
	p->addModel(createModel<GeighthsWidget>		("PvC", "Geighths", "Geighths", LOGIC_TAG, SWITCH_TAG));
	
	p->addModel(createModel<BangDaButtonWidget>	("PvC", "BangDaButton", "BangDaButton", CONTROLLER_TAG, SWITCH_TAG));
	p->addModel(createModel<FlipOLogicWidget>	("PvC", "FlipOLogic", "FlipOLogic", LOGIC_TAG, SWITCH_TAG));
	p->addModel(createModel<ShutItWidget>		("PvC", "ShutIt", "ShutIt", SWITCH_TAG, MULTIPLE_TAG));
	
	p->addModel(createModel<SlimSeqWidget>		("PvC", "SlimSeq", "SlimSeq", SWITCH_TAG, SEQUENCER_TAG));
		
	p->addModel(createModel<PvCBlankWidget>		("PvC", "PvC64", "PvC64", BLANK_TAG));
	//p->addModel(createModel<BitSHWidget>		("PvC", "BitSH", "BitSH", BLANK_TAG));
	//p->addModel(createModel<CoinTreeWidget>		("PvC", "CoinTree", "CoinTree", LOGIC_TAG, RANDOM_TAG, SWITCH_TAG));

	
	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
/*
enum ModelTag {
	AMPLIFIER_TAG,
	ATTENUATOR_TAG,
	BLANK_TAG,
	CLOCK_TAG,
	CONTROLLER_TAG,
	DELAY_TAG,
	DIGITAL_TAG,
	DISTORTION_TAG,
	DRUM_TAG,
	DUAL_TAG,
	DYNAMICS_TAG,
	EFFECT_TAG,
	ENVELOPE_FOLLOWER_TAG,
	ENVELOPE_GENERATOR_TAG,
	EQUALIZER_TAG,
	EXTERNAL_TAG,
	FILTER_TAG,
	FUNCTION_GENERATOR_TAG,
	GRANULAR_TAG,
	LFO_TAG,
	LOGIC_TAG,
	LOW_PASS_GATE_TAG,
	MIDI_TAG,
	MIXER_TAG,
	MULTIPLE_TAG,
	NOISE_TAG,
	OSCILLATOR_TAG,
	PANNING_TAG,
	QUAD_TAG,
	QUANTIZER_TAG,
	RANDOM_TAG,
	REVERB_TAG,
	RING_MODULATOR_TAG,
	SAMPLE_AND_HOLD_TAG,
	SAMPLER_TAG,
	SEQUENCER_TAG,
	SLEW_LIMITER_TAG,
	SWITCH_TAG,
	SYNTH_VOICE_TAG,
	TUNER_TAG,
	UTILITY_TAG,
	VISUAL_TAG,
	WAVESHAPER_TAG,
	NUM_TAGS
};
*/