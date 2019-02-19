#if !defined (METRONOME_H)
#define METRONOME_H

#include "../third_party/gui_math.h"

struct gui_window;

struct metronome_input {
	v2 mouse_pos;
	bool lmb_down;
	bool lmb_up;

	bool space_down;
};

void metronome_init (void* memory, gui_window window);
void metronome_update (void* memory, metronome_input input);

#endif