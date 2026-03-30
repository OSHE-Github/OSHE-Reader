#include <string.h>
#include <vars.h>

/* LIGHT/DARK MODE TOGGLE SETTING */
bool light_mode;
bool get_var_light_mode() {
    return light_mode;
}

void set_var_light_mode(bool value) {
    light_mode = value;
}

/* PAGE NUMBERING */
int32_t page_num;
int32_t get_var_page_num() {
    return page_num;
}

void set_var_page_num(int32_t value) {
    page_num = value;
}

/* DISPLAYING PAGE */
char page_text[100] = { 0 };
const char *get_var_page_text() {
    return page_text;
}

void set_var_page_text(const char *value) {
    strncpy(page_text, value, sizeof(page_text) / sizeof(char));
    page_text[sizeof(page_text) / sizeof(char) - 1] = 0;
}

/* GETTING BOOK DATA FOR LIBRARY */
char book_1[1024] = { 0 };
const char *get_var_book_1() {
    return book_1;
}

void set_var_book_1(const char *value) {
    strncpy(book_1, value, sizeof(book_1) / sizeof(char));
    book_1[sizeof(book_1) / sizeof(char) - 1] = 0;
}

char book_2[1024] = { 0 };
const char *get_var_book_2() {
    return book_2;
}

void set_var_book_2(const char *value) {
    strncpy(book_2, value, sizeof(book_2) / sizeof(char));
    book_2[sizeof(book_2) / sizeof(char) - 1] = 0;
}