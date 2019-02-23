#include "metronome.h"

#include "constants.h"
#include "metronome_gl.h"

#include "../third_party/gui_math.h"
#include "../third_party/gui_window.h"
#include "../third_party/gui_io.h"

enum image { I_START, I_STOP, I_BPM, I_SEGMENT_HOR, I_SEGMENT_VER };

static int digit_to_code (unsigned digit) {
	switch (digit) {
		case 0: return 0b00111111;
		case 1: return 0b00000110;
		case 2: return 0b01011011;
		case 3: return 0b01001111;
		case 4: return 0b01100110;
		case 5: return 0b01101101;
		case 6: return 0b01111101;
		case 7: return 0b00000111;
		case 8: return 0b01111111;
		case 9: return 0b01101111;
		default:
			io_log_error ("%d is not a digit", digit);
			return 0;
	}
}

static void update_digit_segments (int code, digit* result) {
	for (unsigned i = 0; i < DIGIT_SEGMENT_COUNT; ++i) {
		result -> segments[i].on = code & 0b00000001;
		code >>= 1;
	}
}

static void draw_bpm (metronome_app* app, metronome_input input, gui_window window) {
	if (app -> changing_tempo) {
		int tempo_change = (int)(input.mouse_pos.x - app -> drag_origin.x) / TEMPO_CHANGE_STEP;
		app -> tempo += tempo_change;

		app -> tempo = CLAMP (app -> tempo, MIN_TEMPO, MAX_TEMPO);

		if (tempo_change != 0)
			app -> drag_origin = input.mouse_pos;
	}

	rect whole_rect = make_rect (ACTIVE_BPM_RECT);
	if (is_point_in_rect (whole_rect, input.mouse_pos)) {
		if (input.lmb_down) {
			app -> changing_tempo = true;
			app -> drag_origin = input.mouse_pos;
			wnd_capture_cursor (window);
		}
	}

	v2 positions[] = { 
		make_v2 (BPM_DIGIT_1_POSITION),
		make_v2 (BPM_DIGIT_2_POSITION),
		make_v2 (BPM_DIGIT_3_POSITION)
	};

	unsigned tempo = app -> tempo;
	for (int i = BPM_DIGIT_COUNT - 1; i >= 0; --i) {
		unsigned digit = tempo % 10;
		update_digit_segments (digit_to_code (digit), &app -> bpm_digits[i]);
		tempo /= 10;	
	}

	v4 white_color = make_color (255, 255, 255, 255);
	for (unsigned i = app -> tempo < 100 ? 1 : 0; i < BPM_DIGIT_COUNT; ++i) {
		for (unsigned j = 0; j < DIGIT_SEGMENT_COUNT; ++j) {
			digit_segment segment = app -> bpm_digits[i].segments[j];
			if (segment.on) {
				rect r = make_rect (positions[i] + segment.position, 0.0f, 0.0f);
				gl_draw_image (r, white_color, segment.image);
			}
		}
	}

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

		result = input.lmb_up && !app -> changing_tempo;
		if (input.lmb_pressed)
			image_rect.y += 2;
	}
	else
		result = input.space_down;

	gl_draw_rect (r, color);
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

	bool images[] = { true, false, false, true, false, false, true }; // True - horizontal, false - vertical
	v2 positions[] = { 
		make_v2 (SEGMENT_A),
		make_v2 (SEGMENT_B),
		make_v2 (SEGMENT_C),
		make_v2 (SEGMENT_D),
		make_v2 (SEGMENT_E),
		make_v2 (SEGMENT_F),
		make_v2 (SEGMENT_G)
	};

	for (unsigned i = 0; i < BPM_DIGIT_COUNT; ++i) {
		for (unsigned j = 0; j < DIGIT_SEGMENT_COUNT; ++j) {
			digit_segment* segment = &app -> bpm_digits[i].segments[j];
			segment -> image = images[j] ? app -> images[I_SEGMENT_HOR] : app -> images[I_SEGMENT_VER];
			segment -> position = positions[j];
			segment -> on = false;	
		}
	}

	app -> playing = false;
	app -> changing_tempo = false;
	app -> tempo = 120;

	gl_init (window);

	audio_init ();

	app -> main_sound = audio_load ("W:\\metronome\\data\\audio\\ping.wav");
}

void metronome_update (void* memory, metronome_input input, gui_window window) {
	metronome_app* app = (metronome_app*)memory;

	if (app -> changing_tempo && input.lmb_up) {
		app -> changing_tempo = false;
		wnd_uncapture_cursor ();
	}

	draw_meter (app);
	draw_bpm (app, input, window);

	if (draw_button (app, input)) {
		audio_play (&app -> main_sound);
		app -> playing = !app -> playing;
	}

	if (app -> playing)
		draw_app_border ();
}

void metronome_deinit () {
	audio_deinit ();
}
