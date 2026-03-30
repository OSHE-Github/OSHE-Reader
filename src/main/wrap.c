#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vars.h>
#include <lvgl.h>
#include <ui.h>

const char* past_pargraph_array[1024];

void wrap_text(){

}

void display_next_page(){

  int32_t current = get_var_page_num();
  set_var_page_num(current + 1);
  
  char book1[100] = get_var_book_1();
  char book2[100]= get_var_book_2();
  if (book1 != 0 && active_book() == 1 ){ 
    lv_label_set_text(objects.book_label, get_var_page_text(//buffer for book 1));
    past_pargraph_array = //buffer for book 1;
  }else if (book2 != 0 && active_book() == 2 ){
    lv_label_set_text(objects.book_label, get_var_page_text(//buffer for book 2));
    past_pargraph_array = //buffer for book 2;
  }else{
    page_change_library(SCREEN_ID_BOOK);
  }
  //lv_label_set_text(objects.book_label, get_var_page_text(//buffer for book )); past_pargraph_array = //buffer for book;
}

void display_prev_page(){

 int32_t current = get_var_page_num();
 if (current > 0) {
   set_var_page_num(current - 1);
   lv_label_set_text(objects.book_label, get_var_page_text(past_pargraph_array));   
  }else {
    page_change_library(SCREEN_ID_BOOK);
  }

}
