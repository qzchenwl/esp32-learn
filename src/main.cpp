#include <Arduino.h>
#include <esp_camera.h>
#include <lvgl.h>
#include <TJpg_Decoder.h>

#include "my_screen.h"
#include "my_lvgl.h"
#include "my_camera.h"
#include "my_wifi.h"

// LVGL 显示刷新函数
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)color_p, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup ...");

  Serial.println("setup my screen ...");
  setup_my_screen();

  Serial.println("setup my camera ...");
  setup_my_camera();

  Serial.println("setup my lvgl ...");
  setup_my_lvgl();
  my_lvgl_demo();

  Serial.println("setup my wifi ...");
  setup_my_wifi();
}

void loop() {
  lv_timer_handler();

  int t1 = millis();
  camera_fb_t* fb = esp_camera_fb_get();
  t1 = millis() - t1;

  int t2 = millis();
  TJpgDec.drawJpg(0, 80, fb->buf, fb->len);
  t2 = millis() - t2;

  esp_camera_fb_return(fb);
  tft.setCursor(0, 0);
  tft.printf("t1=%dms t2=%dms", t1, t2);

  my_wifi_loop();

  delay(5);
}