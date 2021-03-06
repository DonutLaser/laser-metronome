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
#define BUTTON_HOVER_COLOR		59, 61, 67, 255
#define DIGIT_HOVER_COLOR		204, 204, 204, 255

#define BPM_TEXT_POSITION 		183, 53
#define BPM_DIGIT_3_POSITION	150, 30
#define BPM_DIGIT_2_POSITION	119, 30
#define BPM_DIGIT_1_POSITION	87, 30
#define COUNT_DIGIT_2_POSITION	43, 13
#define COUNT_DIGIT_1_POSITION	19, 13 
#define LENGTH_DIGIT_2_POSITION 43, 60
#define LENGTH_DIGIT_1_POSITION	19, 60 

#define METER_DIVIDER 			30, 53, 37, 2

#define ACTIVE_BPM_RECT 		87, 30, 89, 47
#define ACTIVE_METER_COUNT_RECT	34, 13, 29, 35
#define ACTIVE_METER_LEN_RECT	34, 60, 29, 35

#define DIGIT_SEGMENT_COUNT		7
#define BPM_DIGIT_COUNT			3
#define METER_DIGIT_COUNT 		2

#define SEGMENT_A 				4, 0
#define SEGMENT_B				19, 4
#define SEGMENT_C 				19, 24
#define SEGMENT_D				4, 40
#define SEGMENT_E 				0, 24
#define SEGMENT_F 				0, 4
#define SEGMENT_G 				4, 20

#define SEGMENT_MINI_A			3, 0
#define SEGMENT_MINI_B			15, 3 
#define SEGMENT_MINI_C			15, 18
#define SEGMENT_MINI_D			3, 30
#define SEGMENT_MINI_E			0, 18 
#define SEGMENT_MINI_F			0, 3 
#define SEGMENT_MINI_G			3, 15 

#define IMAGE_COUNT				7	
static const char* images[IMAGE_COUNT] = { 
	"..\\data\\images\\button_start.png",
	"..\\data\\images\\button_stop.png",
	"..\\data\\images\\bpm.png",
	"..\\data\\images\\segment_horizontal.png",
	"..\\data\\images\\segment_vertical.png",
	"..\\data\\images\\segment_horizontal_mini.png",
	"..\\data\\images\\segment_vertical_mini.png"
};

#define MIN_TEMPO				30
#define MAX_TEMPO				300

#define SOUND_COUNT				2
static const char* sounds[SOUND_COUNT] = {
	"..\\data\\audio\\main.wav",
	"..\\data\\audio\\secondary.wav"
};

static const char* icon_path = "..\\data\\images\\icon.ico";

#define TEMPO_CHANGE_STEP		5	
#define METER_CHANGE_STEP		10	

#define METER_COUNT_VALUE_COUNT	14
static const unsigned count_values[METER_COUNT_VALUE_COUNT] = {
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

#define METER_LEN_VALUE_COUNT	3
static const unsigned length_values[METER_LEN_VALUE_COUNT] = {
	4, 8, 16
};

#define START_COUNT				4
#define START_LENGTH			4

#endif