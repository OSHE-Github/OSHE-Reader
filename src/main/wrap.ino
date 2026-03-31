#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vars.h>
#include <lvgl.h>
#include <ui.h>
#include <buttons.h>
#include "wrap.h"

int bookPick = 1;
extern char *lorem, *ipsum;

void set_active_book(int book){
    bookPick = book;
}

int get_active_book(){
    return bookPick;
}

void display_next_page(){

  lv_label_set_text(objects.page_num, "2");
  lv_label_set_text(objects.text, ipsum);
  lv_obj_invalidate(lv_screen_active());
}

void display_prev_page(){

  lv_label_set_text(objects.page_num, "1");
  lv_label_set_text(objects.text, lorem);
  lv_obj_invalidate(lv_screen_active());
}
