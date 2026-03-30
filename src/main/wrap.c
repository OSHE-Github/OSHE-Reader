#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vars.h>
#include <lvgl.h>
#include <ui.h>

void wrap_text(){

}

void display_next_page(){

  int32_t current = get_var_page_num();
  set_var_page_num(current + 1);
  lv_label_set_text(objects.book_label, get_var_page_text());

}

void display_prev_page(){

 int32_t current = get_var_page_num();
 if (current > 0) {
   set_var_page_num(current - 1);
      
  }

}
