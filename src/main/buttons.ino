#include <stdlib.h>
#include <stdio.h>
#include "buttons.h"
#include <Button2.h>
#include "lvgl.h"
#include "actions.h"
#include "ui.h"
#include "vars.h"
#include "wrap.h"

extern OneButton button1, button2, button3, button4, button5;

void button_init(void)
{
  button1.attachClick(button1_press);
  button2.attachClick(button2_press);
  button3.attachClick(button3_press);
  button3.attachLongPressStart(button3_long_press);
  button4.attachClick(button4_press);
  button5.attachClick(button5_press);
}

void button1_press()
{
  if (lv_screen_active() == objects.main)
  {
    page_change_settings(SCREEN_ID_MAIN);
  }
  else if (lv_screen_active() == objects.settings)
  {
    light_mode_toggle(1);
  }
  else if (lv_screen_active() == objects.library)
  {
    page_change_book(SCREEN_ID_LIBRARY);
    set_var_page_text(lorem);
    set_var_page_num("1");
    tick_screen_book();
  }
  else if (lv_screen_active() == objects.book)
  {
    display_prev_page();
  }
}

void button2_press()
{
  if (lv_screen_active() == objects.main)
  {
    page_change_library(SCREEN_ID_MAIN);
  }
  else if (lv_screen_active() == objects.settings)
  {
    light_mode_toggle(0);
  }
  else if (lv_screen_active() == objects.library)
  {
    page_change_book(SCREEN_ID_LIBRARY);
    set_var_page_text(lorem);
    set_var_page_num("1");
    tick_screen_book();
  }
  else if (lv_screen_active() == objects.book)
  {
    display_prev_page();
  }
}

void button3_press()
{
  if (lv_screen_active() == objects.settings)
  {
    page_change_main(SCREEN_ID_SETTINGS);
  }
  else if (lv_screen_active() == objects.library)
  {
    page_change_main(SCREEN_ID_LIBRARY);
  }
  else if (lv_screen_active() == objects.book)
  {
    page_change_library(SCREEN_ID_BOOK);
  }
}

void button4_press()
{
  if (lv_screen_active() == objects.library)
  {
    /*TODO: change titles of books to previous 2 files */
    //set_active_book(1);
  }
  else if (lv_screen_active() == objects.book)
  {
    display_next_page();
  }
}

void button5_press()
{
  if (lv_screen_active() == objects.library)
  {
    /*TODO: change titles of books to next 2 files */
    //set_active_book(2);
  }
  else if (lv_screen_active() == objects.book)
  {
    display_next_page();
  }
}

void button3_long_press()
{
  /*sleepy time :)*/
}

void page_change_main(enum ScreensEnum old_screen)
{
  create_screen_main();
  loadScreen(SCREEN_ID_MAIN);
  if (lv_screen_active() == objects.main)
  {
    delete_screen_by_id(old_screen);
  }
}

void page_change_settings(enum ScreensEnum old_screen)
{
  create_screen_settings();
  loadScreen(SCREEN_ID_SETTINGS);
  if (lv_screen_active() == objects.settings)
  {
    delete_screen_by_id(old_screen);
  }
}

void page_change_library(enum ScreensEnum old_screen)
{
  create_screen_library();
  loadScreen(SCREEN_ID_LIBRARY);
  if (lv_screen_active() == objects.library)
  {
    delete_screen_by_id(old_screen);
  }
}

void page_change_book(enum ScreensEnum old_screen)
{
  create_screen_book();
  loadScreen(SCREEN_ID_BOOK);
  if (lv_screen_active() == objects.book)
  {
    delete_screen_by_id(old_screen);
  }
}

void light_mode_toggle(int value)
{
  Serial.println("toggle");
  bool prev_value = get_var_light_mode();
  set_var_light_mode(value);
  if (prev_value != value)
  {
    lv_obj_invalidate(lv_screen_active());
  }
}
