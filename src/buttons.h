#include <lvgl.h>

#define BUTTON1 4 // top left rocker
#define BUTTON2 16 // bottom left rocker
#define BUTTON3 17 // top right rocker
#define BUTTON4 5 // bottom right rocker
#define BUTTON5 18 // center (home) button

void button_init(void);
void button_read(lv_indev_t * indev, lv_indev_data_t * data);

void page_change_main(enum ScreensEnum old_screen);
void page_change_settings(enum ScreensEnum old_screen);
void page_change_library(enum ScreensEnum old_screen);
void page_change_book(enum ScreensEnum old_screen);
void light_mode_toggle(int value);