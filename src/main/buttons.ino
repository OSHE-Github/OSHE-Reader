#include <buttons.h>
#include <lvgl.h>
#include <actions.h>
#include <stdlib.h>
#include <stdio.h>
#include <ui.h>
#include <vars.h>
#include <wrap.h>

void button_init(void)
{
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  pinMode(BUTTON5, INPUT);
}

void button_read(void)
{
  delay(30);
  if (digitalRead(BUTTON1) == HIGH)       // Top left rocker button pressed
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
      page_change_book();
    }
    else if (lv_screen_active() == objects.book)
    {
      display_prev_page();
    }
  }
  else if (digitalRead(BUTTON2) == HIGH)  // Top right rocker button pressed
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
      page_change_book();
    }
    else if (lv_screen_active() == objects.book)
    {
      display_prev_page();
    }
  }
  else if (digitalRead(BUTTON3) == HIGH)    // Center (home) button pressed
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
      page_change_main(SCREEN_ID_BOOK);
    }
  }
  else if (digitalRead(BUTTON4) == HIGH)    // Bottom left rocker button pressed
  {
    if (lv_screen_active() == objects.library)
    {
      /*TODO: change titles of books to previous 2 files */
      active_book(1);
    }
    else if (lv_screen_active() == objects.book)
    {
      display_next_page();
    }
  }
  else if (digitalRead(BUTTON5) == HIGH)    // Bottom right rocker button pressed
  {
    if (lv_screen_active() == objects.library)
    {
      /*TODO: change titles of books to next 2 files */
      active_book(2);
    }
    else if (lv_screen_active() == objects.book)
    {
      display_next_page();
    }
  }
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
