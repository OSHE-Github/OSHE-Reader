#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: Light_Screen
lv_style_t *get_style_light_screen_MAIN_DEFAULT();
void add_style_light_screen(lv_obj_t *obj);
void remove_style_light_screen(lv_obj_t *obj);

// Style: Menu_Button
lv_style_t *get_style_menu_button_MAIN_DEFAULT();
lv_style_t *get_style_menu_button_MAIN_FOCUS_KEY();
lv_style_t *get_style_menu_button_MAIN_FOCUSED();
void add_style_menu_button(lv_obj_t *obj);
void remove_style_menu_button(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/