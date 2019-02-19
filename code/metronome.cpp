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
	else
		result = input.space_down;

	gl_draw_rect (r, color);
	return result;
}

static void draw_app_border () {
	rect t = make_rect (0, 0, WINDOW_WIDTH, PLAY_INDICATOR_WIDTH);
	rect r = make_rect (WINDOW_WIDTH - PLAY_INDICATOR_WIDTH, 0, PLAY_INDICATOR_WIDTH, WINDOW_HEIGHT);
	rect b = make_rect (0, WINDOW_HEIGHT - PLAY_INDICATOR_WIDTH, WINDOW_WIDTH, PLAY_INDICATOR_WIDTH);
	rect l = make_rect (0, 0, PLAY_INDICATOR_WIDTH, WINDOW_HEIGHT);

	v4 color = make_color (PLAY_INDICATOR_COLOR);

	gl_draw_rect (t, color);
	gl_draw_rect (r, color);
	gl_draw_rect (b, color);
	gl_draw_rect (l, color);
}

void metronome_init (void* memory, gui_window window) {
	metronome_app* app = (metronome_app*)memory;

	app -> playing = false;

	gl_init (window);
}

void metronome_update (void* memory, metronome_input input) {
	metronome_app* app = (metronome_app*)memory;
	if (draw_button (input))
		app -> playing = !app -> playing;

	if (app -> playing)
		draw_app_border ();
}