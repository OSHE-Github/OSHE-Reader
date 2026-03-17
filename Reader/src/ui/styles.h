#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: button_light
lv_style_t *get_style_button_light_MAIN_DEFAULT();
lv_style_t *get_style_button_light_MAIN_FOCUS_KEY();
void add_style_button_light(lv_obj_t *obj);
void remove_style_button_light(lv_obj_t *obj);

// Style: screen_light
lv_style_t *get_style_screen_light_MAIN_DEFAULT();
void add_style_screen_light(lv_obj_t *obj);
void remove_style_screen_light(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/