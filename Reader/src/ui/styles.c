#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: button_light
//

void init_style_button_light_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &ui_font_noto_serif);
    lv_style_set_text_color(style, lv_color_hex(0xff000000));
    lv_style_set_bg_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_color(style, lv_color_hex(0xff000000));
    lv_style_set_border_width(style, 2);
};

lv_style_t *get_style_button_light_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_button_light_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_button_light_MAIN_FOCUS_KEY(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff000000));
    lv_style_set_border_color(style, lv_color_hex(0xff000000));
    lv_style_set_text_font(style, &ui_font_noto_serif);
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_width(style, 2);
};

lv_style_t *get_style_button_light_MAIN_FOCUS_KEY() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_button_light_MAIN_FOCUS_KEY(style);
    }
    return style;
};

void add_style_button_light(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_button_light_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_button_light_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
};

void remove_style_button_light(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_button_light_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_button_light_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
};

//
// Style: screen_light
//

void init_style_screen_light_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xffffffff));
};

lv_style_t *get_style_screen_light_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_screen_light_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_screen_light(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_screen_light_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_screen_light(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_screen_light_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_button_light,
        add_style_screen_light,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_button_light,
        remove_style_screen_light,
    };
    remove_style_funcs[styleIndex](obj);
}