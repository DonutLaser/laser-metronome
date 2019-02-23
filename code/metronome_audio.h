#if !defined (METRONOME_AUDIO_H)
#define METRONOME_AUDIO_H

struct sound {
	void* data;
	bool is_playing;
};

void audio_init ();

sound audio_load (const char* path);
void audio_play (sound* s);

void audio_unload (sound s);
void audio_deinit ();

#endif