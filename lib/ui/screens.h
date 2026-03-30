#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SETTINGS = 2,
    SCREEN_ID_LIBRARY = 3,
    SCREEN_ID_BOOK = 4,
    _SCREEN_ID_LAST = 4
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *settings;
    lv_obj_t *library;
    lv_obj_t *book;
    lv_obj_t *light;
    lv_obj_t *dark;
    lv_obj_t *back_2;
    lv_obj_t *back_1;
    lv_obj_t *book_1;
    lv_obj_t *book_2;
    lv_obj_t *book_1_title;
    lv_obj_t *book_2_title;
    lv_obj_t *text;
    lv_obj_t *back_3;
    lv_obj_t *page_num;
} objects_t;

extern objects_t objects;

void create_screen_main();
void delete_screen_main();
void tick_screen_main();

void create_screen_settings();
void delete_screen_settings();
void tick_screen_settings();

void create_screen_library();
void delete_screen_library();
void tick_screen_library();

void create_screen_book();
void delete_screen_book();
void tick_screen_book();

void create_screen_by_id(enum ScreensEnum screenId);
void delete_screen_by_id(enum ScreensEnum screenId);
void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/