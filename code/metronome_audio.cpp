#include "metronome_audio.h"

#include "../third_party/gui_io.h"

#define DR_WAV_IMPLEMENTATION
#include "../third_party/dr_wav.h"
#define MINI_AL_IMPLEMENTATION
#include "../third_party/mini_al.h"

// For now? Maybe? Who knows?
static mal_device device;

static mal_uint32 on_send_frames_to_device (mal_device* device, mal_uint32 frame_count, void* samples) {
	sound* s = (sound*)device -> pUserData;
	if (!s)
		return 0;

	mal_decoder* decoder = (mal_decoder*)s -> data;
	if (!s -> is_playing || !decoder)
		return 0;


	mal_uint32 result = (mal_uint32)mal_decoder_read (decoder, frame_count, samples);
	if (result == 0) {
		device -> pUserData = NULL;
		s -> is_playing = false;
	}

	return result;
}

void audio_init () {
	mal_device_config config = mal_device_config_init (mal_format_s16, 1, 44100, NULL, on_send_frames_to_device);

	if (mal_device_init (NULL, mal_device_type_playback, NULL, &config, NULL, &device) != MAL_SUCCESS) {
		io_log_error ("Failed to open playback device");
		return;
	}

	if (mal_device_start (&device) != MAL_SUCCESS) {
		io_log_error ("Failed to start playback device");
		mal_device_uninit (&device);
		return;
	}
}

sound audio_load (const char* path) {
	sound result = { };
	result.is_playing = false;

	mal_decoder* decoder = (mal_decoder*)malloc (sizeof (mal_decoder));
	if (mal_decoder_init_file (path, NULL, decoder) != MAL_SUCCESS)
		io_log_error ("Could not load the file %s", path);

	result.data = decoder;

	return result;
}

void audio_play (sound* s) {
	mal_decoder_seek_to_frame ((mal_decoder*)s -> data, 0);
	s -> is_playing = true;
	device.pUserData = s;
}

void audio_unload (sound s) {
	mal_decoder_uninit ((mal_decoder*)s.data);
	free (s.data);
}

void audio_deinit () {
	mal_device_uninit (&device);
}