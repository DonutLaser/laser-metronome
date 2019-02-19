#include "metronome.h"

#include "constants.h"
#include "metronome_gl.h"

#include "../third_party/gui_math.h"
#include "../third_party/gui_window.h"

static bool draw_button (metronome_input input) {
	bool result = false;
	rect r = make_rect (0, WINDOW_HEIGHT - BUTTON_HEIGHT, WINDOW_WIDTH, BUTTON_HEIGHT);
	v4 color = make_color (BUTTON_DEFAULT_COLOR);

	if (is_point_in_rect (r, input.mouse_pos)) {
		color = make_color (BUTTON_HOVER_COLOR);

		result = input.lmb_up;
	}

	gl_draw_rect (r, color);
	return result;
}

void metronome_init (void* memory, gui_window window) {
	gl_init (window);
}

void metronome_update (void* memory, metronome_input input) {
	draw_button (input);
}