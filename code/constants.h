#if !defined (CONSTANTS_H)
#define CONSTANTS_H

#define WINDOW_WIDTH			263
#define WINDOW_HEIGHT			151

#define LARGE_MARGIN			30
#define SMALL_MARGIN			13

#define PLAY_INDICATOR_WIDTH	2
#define BUTTON_HEIGHT			43

#define BG_COLOR 				48, 48, 48, 255
#define ACCENT_COLOR			227, 175, 85, 255
#define MAIN_COLOR				51, 94, 216, 255
#define PLAY_INDICATOR_COLOR	150, 49, 49, 255

#define BUTTON_DEFAULT_COLOR 	59, 59, 59, 255
#define BUTTON_HOVER_COLOR		48, 50, 57, 255

#define BPM_TEXT_POSITION 		183, 53
#define BPM_DIGIT_3_POSITION	150, 30
#define BPM_DIGIT_2_POSITION	119, 30
#define BPM_DIGIT_1_POSITION	87, 30
#define METER_COUNT_POSITION	30, 13
#define METER_LENGTH_POSITION 	30, 60

#define METER_DIVIDER 			30, 53, 27, 2

#define IMAGE_COUNT				3
static const char* images[IMAGE_COUNT] = { 
	"W:\\metronome\\data\\images\\button_start.png",
	"W:\\metronome\\data\\images\\button_stop.png",
	"W:\\metronome\\data\\images\\bpm.png"
};

#endif