#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SCREEN_2 = 2,
    _SCREEN_ID_LAST = 2
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *screen_2;
    lv_obj_t *screen2_button;
    lv_obj_t *main_button;
} objects_t;

extern objects_t objects;

void create_screen_main();
void delete_screen_main();
void tick_screen_main();

void create_screen_screen_2();
void delete_screen_screen_2();
void tick_screen_screen_2();

void create_screen_by_id(enum ScreensEnum screenId);
void delete_screen_by_id(enum ScreensEnum screenId);
void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

// Groups

typedef struct _groups_t {
    lv_group_t *keypad;
} groups_t;

extern groups_t groups;

void ui_create_groups();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/