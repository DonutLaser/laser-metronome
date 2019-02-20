#include "metronome.h"

#include "constants.h"
#include "metronome_gl.h"

#include "../third_party/gui_math.h"
#include "../third_party/gui_window.h"
#include "../third_party/gui_io.h"

enum image { I_START, I_STOP, I_BPM };

static void draw_bpm (metronome_app* app) {
	rect bpm_rect = make_rect (BPM_TEXT_POSITION, 0, 0);
	gl_draw_image (bpm_rect, make_color (255, 255, 255, 255), app -> images[I_BPM]);
}

static void draw_meter (metronome_app* app) {
	rect divider_rect = make_rect (METER_DIVIDER);
	v4 color = make_color (ACCENT_COLOR);
	gl_draw_rect (divider_rect, color);
}

static bool draw_button (metronome_app* app, metronome_input input) {
	bool result = false;
	rect r = make_rect (0, WINDOW_HEIGHT - BUTTON_HEIGHT, WINDOW_WIDTH, BUTTON_HEIGHT);
	v4 color = make_color (BUTTON_DEFAULT_COLOR);

	rect image_rect = r;
	if (is_point_in_rect (r, input.mouse_pos)) {
		color = make_color (BUTTON_HOVER_COLOR);

		result = input.lmb_up;
	}
	else
		result = input.space_down;

	gl_draw_rect (r, color);

	if (input.lmb_down)
		image_rect.y += 2;

	gl_draw_image (image_rect, make_color (255, 255, 255, 255), 
				   app -> playing ? app -> images[I_STOP] : app -> images[I_START]);

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

	for (unsigned i = 0; i < IMAGE_COUNT; ++i) {
		if (!resources_load_image (images[i], &app -> images[i]))
			io_log_error ("Image at %s could not be loaded", images[i]);

		gl_load_image (&app -> images[i]);
	}

	app -> playing = false;

	gl_init (window);
}

void metronome_update (void* memory, metronome_input input) {
	metronome_app* app = (metronome_app*)memory;

	draw_meter (app);
	draw_bpm (app);

	if (draw_button (app, input))
		app -> playing = !app -> playing;

	if (app -> playing)
		draw_app_border ();
}