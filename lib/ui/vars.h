#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_LIGHT_MODE = 0,
    FLOW_GLOBAL_VARIABLE_PAGE_NUM = 1,
    FLOW_GLOBAL_VARIABLE_PAGE_TEXT = 2,
    FLOW_GLOBAL_VARIABLE_BOOK_1 = 3,
    FLOW_GLOBAL_VARIABLE_BOOK_2 = 4
};

// Native global variables

extern bool get_var_light_mode();
extern void set_var_light_mode(bool value);
extern int32_t get_var_page_num();
extern void set_var_page_num(int32_t value);
extern const char *get_var_page_text();
extern void set_var_page_text(const char *value);
extern const char *get_var_book_1();
extern void set_var_book_1(const char *value);
extern const char *get_var_book_2();
extern void set_var_book_2(const char *value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/