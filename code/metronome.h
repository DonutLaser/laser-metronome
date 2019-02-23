#if !defined (METRONOME_H)
#define METRONOME_H

#include "constants.h"
#include "metronome_audio.h"

#include "../third_party/gui_math.h"
#include "../third_party/gui_resources.h"

struct gui_window;

struct digit_segment {
	gui_image image;
	v2 position;
	bool on;
};

struct digit {
	digit_segment segments[DIGIT_SEGMENT_COUNT];
};

struct metronome_input {
	v2 mouse_pos;
	bool lmb_down;
	bool lmb_pressed;
	bool lmb_up;

	bool space_down;
};

struct metronome_app {
	digit bpm_digits[BPM_DIGIT_COUNT];
	gui_image images[IMAGE_COUNT];
	sound main_sound;

	v2 drag_origin;
	unsigned tempo;
	bool playing;
	bool changing_tempo;
};

void metronome_init (void* memory, gui_window window);
void metronome_update (void* memory, metronome_input input, gui_window window);
void metronome_deinit ();

#endif