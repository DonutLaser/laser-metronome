#include "../third_party/gui_window.h"

GUI_MAIN {
	gui_window window = wnd_create ("Laser Metronome", 263, 151, false);
	wnd_set_style (window, S_FIXEDSIZE);

	while (wnd_update (&window)) {
		wnd_swap_buffers (window);
	}

	wnd_close ();
	return 0;
}