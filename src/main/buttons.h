#include <lvgl.h>

#define BUTTON1 4 // top left rocker
#define BUTTON2 16 // top right rocker
#define BUTTON3 17 // center (home) button
#define BUTTON4 5 // bottom left rocker
#define BUTTON5 18 // bottom right rocker

void button_init(void);
void button1_press();
void button2_press();
void button3_press();
void button4_press();
void button5_press();
void button3_long_press();

void page_change_main(enum ScreensEnum old_screen);
void page_change_settings(enum ScreensEnum old_screen);
void page_change_library(enum ScreensEnum old_screen);
void page_change_book(enum ScreensEnum old_screen);
void light_mode_toggle(int value);