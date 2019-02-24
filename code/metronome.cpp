#include "metronome.h"

#include "constants.h"
#include "metronome_gl.h"

#include "../third_party/gui_math.h"
#include "../third_party/gui_window.h"
#include "../third_party/gui_io.h"

enum image { I_START, I_STOP, I_BPM, I_SEGMENT_HOR, I_SEGMENT_VER, I_SEGMENT_HOR_MINI, I_SEGMENT_VER_MINI };
enum sound_type { S_SECONDARY, S_MAIN };

static void play (metronome_app* app) {
	if (timer_get_value (app -> play_timer) >= app -> play_timer.target_miliseconds) {
		audio_play (&app -> sounds[app -> click_count == 0 ? S_MAIN : S_SECONDARY]);
		timer_reset (&app -> play_timer);

		++app -> click_count;
		if (app -> click_count == 4)
			app -> click_count = 0;
	}
}

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

static void handle_input (metronome_app* app, metronome_input input, gui_window window) {
	if (app -> changing_value_type != CT_NONE) {
		if (input.lmb_up) {
			app -> changing_value_type = CT_NONE; 
			wnd_uncapture_cursor ();
		}

		return;
	}

	rect bpm_rect = make_rect (ACTIVE_BPM_RECT);
	rect count_rect = make_rect (ACTIVE_METER_COUNT_RECT);
	rect length_rect = make_rect (ACTIVE_METER_LEN_RECT);

	if (is_point_in_rect (bpm_rect, input.mouse_pos) && input.lmb_down)
		app -> changing_value_type = CT_BPM;
	else if (is_point_in_rect (count_rect, input.mouse_pos) && input.lmb_down)
		app -> changing_value_type = CT_COUNT;
	else if (is_point_in_rect (length_rect, input.mouse_pos) && input.lmb_down)
		app -> changing_value_type = CT_LENGTH;

	if (app -> changing_value_type != CT_NONE) {
		app -> drag_origin = input.mouse_pos;
		wnd_capture_cursor (window);
	}
}

static void update_digit_segments (int code, digit* result) {
	for (unsigned i = 0; i < DIGIT_SEGMENT_COUNT; ++i) {
		result -> segments[i].on = code & 1;
		code >>= 1;
	}
}

static void draw_digit (v2 origin, digit d) {
	for (unsigned j = 0; j < DIGIT_SEGMENT_COUNT; ++j) {
		digit_segment segment = d.segments[j];
		if (segment.on) {
			rect r = make_rect (origin + segment.position, 0.0f, 0.0f);
			gl_draw_image (r, make_color (255, 255, 255, 255), segment.image);
		}
	}
}

static void update_bpm (metronome_app* app, metronome_input input, gui_window window) {
	if (app -> changing_value_type != CT_BPM)
		return;

	int tempo_change = (int)(input.mouse_pos.x - app -> drag_origin.x) / TEMPO_CHANGE_STEP;

	if (tempo_change != 0) {
		app -> tempo += tempo_change;
		app -> tempo = CLAMP (app -> tempo, MIN_TEMPO, MAX_TEMPO);
		app -> play_timer.target_miliseconds = 60000 / app -> tempo;

		app -> drag_origin = input.mouse_pos;

		unsigned tempo = app -> tempo;
		for (int i = BPM_DIGIT_COUNT - 1; i >= 0; --i) {
			unsigned digit = tempo % 10;
			update_digit_segments (digit_to_code (digit), &app -> bpm_digits[i]);
			tempo /= 10;	
		}
	}
}

static void draw_bpm (metronome_app* app) {
	v2 positions[] = { 
		make_v2 (BPM_DIGIT_1_POSITION),
		make_v2 (BPM_DIGIT_2_POSITION),
		make_v2 (BPM_DIGIT_3_POSITION)
	};

	for (unsigned i = app -> tempo < 100 ? 1 : 0; i < BPM_DIGIT_COUNT; ++i)
		draw_digit (positions[i], app -> bpm_digits[i]);

	rect bpm_rect = make_rect (BPM_TEXT_POSITION, 0, 0);
	gl_draw_image (bpm_rect, make_color (255, 255, 255, 255), app -> images[I_BPM]);
}

static void update_meter_count (metronome_app* app, metronome_input input, gui_window window) {
	if (app -> changing_value_type != CT_COUNT)	
		return;

	int change = (int)(input.mouse_pos.x - app -> drag_origin.x) / METER_CHANGE_STEP;

	if (change != 0) {
		app -> count_index += change < 0 ? -1 : 1;
		app -> count_index = CLAMP (app -> count_index, 0, METER_COUNT_VALUE_COUNT - 1);
		app -> count = count_values[app -> count_index];

		app -> drag_origin = input.mouse_pos;

		update_digit_segments (digit_to_code (app -> count), &app -> meter_count_digit);
	}
}

static void draw_meter_count (metronome_app* app) {
	draw_digit (make_v2 (METER_COUNT_POSITION), app -> meter_count_digit);
}

static void update_meter_length (metronome_app* app, metronome_input input, gui_window window) {
	if (app -> changing_value_type != CT_LENGTH)
		return;

	int change = (int)(input.mouse_pos.x - app -> drag_origin.x) / METER_CHANGE_STEP;

	if (change != 0) {
		app -> length_index += change < 0 ? -1 : 1;
		app -> length_index = CLAMP (app -> length_index, 0, METER_LEN_VALUE_COUNT -1);
		app -> length = length_values[app -> length_index];

		app -> drag_origin = input.mouse_pos;

		update_digit_segments (digit_to_code (app -> length), &app -> meter_length_digit);
	}
}

static void draw_meter_length (metronome_app* app) {
	draw_digit (make_v2 (METER_LENGTH_POSITION), app -> meter_length_digit);
}

// 	rect divider_rect = make_rect (METER_DIVIDER);
// 	v4 color = make_color (ACCENT_COLOR);
// 	gl_draw_rect (divider_rect, color);

static bool draw_button (metronome_app* app, metronome_input input) {
	bool result = false;
	rect r = make_rect (0, WINDOW_HEIGHT - BUTTON_HEIGHT, WINDOW_WIDTH, BUTTON_HEIGHT);
	v4 color = make_color (BUTTON_DEFAULT_COLOR);

	rect image_rect = r;
	if (is_point_in_rect (r, input.mouse_pos)) {
		color = make_color (BUTTON_HOVER_COLOR);

		result = input.lmb_up && app -> changing_value_type == CT_NONE;
		if (input.lmb_pressed)
			image_rect.y += 2;
	}

	gl_draw_rect (r, color);
	gl_draw_image (image_rect, make_color (255, 255, 255, 255), 
				   app -> playing ? app -> images[I_STOP] : app -> images[I_START]);

	return result || input.space_down;
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

static void init_digit (metronome_app* app, v2 positions[], bool images[], digit* d, image hor_type, image ver_type) {
	for (unsigned i = 0; i < DIGIT_SEGMENT_COUNT; ++i) {
		digit_segment* segment = &d -> segments[i];
		segment -> image = images[i] ? app -> images[hor_type] : app -> images[ver_type];
		segment -> position = positions[i];
		segment -> on = false;	
	}
}

void metronome_init (void* memory, gui_window window) {
	metronome_app* app = (metronome_app*)memory;

	for (unsigned i = 0; i < IMAGE_COUNT; ++i) {
		if (!resources_load_image (images[i], &app -> images[i]))
			io_log_error ("Image at %s could not be loaded", images[i]);

		gl_load_image (&app -> images[i]);
	}

	app -> playing = false;
	app -> changing_value_type = CT_NONE; 
	app -> tempo = 120;
	app -> count_index = 2;
	app -> count = START_COUNT;
	app -> length_index = 0;
	app -> length = START_LENGTH;
	app -> click_count = 0;
	app -> play_timer.target_miliseconds = 60000 / app -> tempo;

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

	for (unsigned i = 0; i < BPM_DIGIT_COUNT; ++i)
		init_digit (app, positions, images, &app -> bpm_digits[i], I_SEGMENT_HOR, I_SEGMENT_VER);

	v2 mini_positions[] = { 
		make_v2 (SEGMENT_MINI_A),
		make_v2 (SEGMENT_MINI_B),
		make_v2 (SEGMENT_MINI_C),
		make_v2 (SEGMENT_MINI_D),
		make_v2 (SEGMENT_MINI_E),
		make_v2 (SEGMENT_MINI_F),
		make_v2 (SEGMENT_MINI_G)
	};

	for (unsigned i = 0; i < DIGIT_SEGMENT_COUNT; ++i) {
		init_digit (app, mini_positions, images, &app -> meter_count_digit, I_SEGMENT_HOR_MINI, I_SEGMENT_VER_MINI);
		init_digit (app, mini_positions, images, &app -> meter_length_digit, I_SEGMENT_HOR_MINI, I_SEGMENT_VER_MINI);
	}

	unsigned tempo = app -> tempo;
	for (int i = BPM_DIGIT_COUNT - 1; i >= 0; --i) {
		unsigned digit = tempo % 10;
		update_digit_segments (digit_to_code (digit), &app -> bpm_digits[i]);
		tempo /= 10;	
	}

	update_digit_segments (digit_to_code (app -> count), &app -> meter_count_digit);
	update_digit_segments (digit_to_code (app -> length), &app -> meter_length_digit);

	gl_init (window);
	audio_init ();

	for (unsigned i = 0; i < SOUND_COUNT; ++i)
		app -> sounds[i] = audio_load (sounds[i]);
}

void metronome_update (void* memory, metronome_input input, gui_window window) {
	metronome_app* app = (metronome_app*)memory;

	handle_input (app, input, window);

	update_meter_count (app, input, window);
	draw_meter_count (app);

	// Here for now
	rect divider_rect = make_rect (METER_DIVIDER);
	v4 color = make_color (ACCENT_COLOR);
	gl_draw_rect (divider_rect, color);

	update_meter_length (app, input, window);
	draw_meter_length (app);

	update_bpm (app, input, window);
	draw_bpm (app);

	if (draw_button (app, input)) {
		app -> playing = !app -> playing;
		if (!app -> playing) {
			app -> click_count = 0; 
			timer_reset (&app -> play_timer);
		}
	}

	if (app -> playing) {
		draw_app_border ();
		play (app);
	}
}

void metronome_deinit () {
	audio_deinit ();
}
