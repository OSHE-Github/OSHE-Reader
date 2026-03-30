// SCREEN
#include <EPD_5IN83_V2.h>
// UI
#include <actions.h>
#include <fonts.h>
#include <images.h>
#include <screens.h>
#include <structs.h>
#include <styles.h>
#include <ui.h>
#include <vars.h>
// LVGL
#include <lvgl.h>
// OTHER
#include <stdlib.h>
#include <SPI.h>
#include <buttons.h>

#define SCREEN_HOR 648
#define SCREEN_VER 480
#define BUFFER_SIZE (SCREEN_HOR * SCREEN_VER / 8)

void setup()
{
  Serial.begin(115200);
  
  /* Initialize screen */
  pinMode(EPD_CS_PIN, OUTPUT);
  pinMode(EPD_DC_PIN, OUTPUT);
  pinMode(EPD_RST_PIN, OUTPUT);
  pinMode(EPD_BUSY_PIN, INPUT);

  digitalWrite(EPD_CS_PIN, 1);
  SPI.begin(EPD_SCK_PIN, 12, EPD_MOSI_PIN, EPD_CS_PIN);
  EPD_5IN83_V2_Init();
  EPD_5IN83_V2_Clear();

  /* Initialize buffer & display */
  lv_init();
  lv_tick_set_cb(get_millis);
  Serial.println("LV init");
  uint8_t *buf = (uint8_t*) malloc(BUFFER_SIZE);
  lv_display_t *display = lv_display_create(SCREEN_HOR, SCREEN_VER);
  lv_display_set_flush_cb(display, screen_flush_cb);    // Setting display flush callback
  lv_display_set_buffers(display, buf, NULL, BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

  /* Initialize UI */
  ui_init();
  Serial.println("UI init");
  set_var_light_mode(1);
  button_init();
}

void loop()
{
  lv_timer_handler();   // LVGL timer
  ui_tick();            // EEZ studio timer
  sleep(10);
  button_read();
}

uint32_t get_millis(void)
{
  return millis();
}

void sleep(int time)
{
  delay(time);
}

void screen_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
  Serial.println("Flushing");
  if(area->x1 == 0 && area->y1 == 0)
  {
    EPD_5IN83_V2_SendCommand(0x13); // Tell display we're going to send pixel data
  }
  // Calculate size of buffer
  uint32_t len = (area->x2 - area->x1 + EPD_5IN83_V2_WIDTH * (area->y2 - area->y1)) / 8;
  px_map += 8;  // Adjusts pixel buffer (first 8 bytes are metadata or smth)

  if (get_var_light_mode() == 1)
  {
    EPD_5IN83_V2_SendData_Invert(px_map, len); // transfer pixel data
  }
  else
  {
    EPD_5IN83_V2_SendData(px_map, len); // transfer pixel data
  }

  if(lv_display_flush_is_last(display))
  {
    EPD_5IN83_V2_TurnOnDisplay();
  }
  lv_display_flush_ready(display);  // Tell LVGL the buffer can be flushed and reused
}
