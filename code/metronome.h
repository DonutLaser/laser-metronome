#if !defined (METRONOME_H)
#define METRONOME_H

#include "constants.h"
#include "metronome_audio.h"

#include "../third_party/gui_math.h"
#include "../third_party/gui_resources.h"
#include "../third_party/gui_time.h"

enum change_type { CT_BPM, CT_COUNT, CT_LENGTH, CT_NONE };

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
	digit meter_count_digit;
	digit meter_length_digit;
	gui_image images[IMAGE_COUNT];
	sound sounds[SOUND_COUNT];
	sound main_sound;

	v2 drag_origin;

	unsigned tempo;

	int count_index;
	unsigned count; 
	int length_index;
	unsigned length;
	change_type changing_value_type;

	bool playing;

	unsigned click_count;
	timer play_timer;
};

void metronome_init (void* memory, gui_window window);
void metronome_update (void* memory, metronome_input input, gui_window window);
void metronome_deinit ();

#endif