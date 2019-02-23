#include "metronome.h"
#include "constants.h"

#include "../third_party/gui_window.h"

#if 0
#define GUI_DEBUG
#endif

GUI_MAIN {
#if defined (GUI_DEBUG)
	wnd_create_console ();
#endif

	gui_window window = wnd_create ("Laser Metronome", WINDOW_WIDTH, WINDOW_HEIGHT, false);
	wnd_set_style (window, S_FIXEDSIZE);
	window.bg_color = make_color (BG_COLOR);

	void* memory = malloc (1024 * 1024 * 3); // 3 MB

	metronome_init (memory, window);

	metronome_input input = { };
	while (wnd_update (&window)) {
		input.mouse_pos = input_get_mouse_position (window);
		input.lmb_down = input_is_mb_down (window, M_LEFT);
		input.lmb_pressed = input_is_mb_pressed (window, M_LEFT);
		input.lmb_up = input_is_mb_up (window, M_LEFT);
		input.space_down = input_is_key_down (window, K_SPACE);

		metronome_update (memory, input, window);
		wnd_swap_buffers (window);
	}

	metronome_deinit ();
	free (memory);

	wnd_close ();
	return 0;
}