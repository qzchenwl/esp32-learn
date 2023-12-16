#include <Arduino.h>
#include <esp_heap_caps.h>

#include "my_screen.h"
#include "my_lvgl.h"
#include "my_camera.h"
#include "my_wifi.h"
#include "my_expander.h"
#include "my_i2s.h"

void setup() {
  Serial.begin(115200);
  Serial.println("setup ...");

  heap_caps_print_heap_info(MALLOC_CAP_8BIT);

//
//  Serial.println("setup my expander ...");
//  setup_my_expander();

  heap_caps_print_heap_info(MALLOC_CAP_8BIT);

//  Serial.println("setup my screen ...");
//  setup_my_screen();

  heap_caps_print_heap_info(MALLOC_CAP_8BIT);

  Serial.println("setup my camera ...");
  setup_my_camera();

  heap_caps_print_heap_info(MALLOC_CAP_8BIT);

//  Serial.println("setup my lvgl ...");
//  setup_my_lvgl();
//  my_lvgl_demo();

  heap_caps_print_heap_info(MALLOC_CAP_8BIT);

//  Serial.println("setup my wifi ...");
//  setup_my_wifi();

  heap_caps_print_heap_info(MALLOC_CAP_8BIT);

  Serial.println("setup my i2s ...");
  setup_my_i2s();

  heap_caps_print_heap_info(MALLOC_CAP_8BIT);

  Serial.println("setup done.");
}

int count = 0;
int last_time = 0;
int fps = 0;
void loop() {
//  my_lvgl_loop();
//  my_screen_loop();
//  my_wifi_loop();
//  my_i2s_loop();

//  delay(5);

  // 统计FPS
  ++count;
  int time_elapsed = millis() - last_time;
  if (time_elapsed >= 1000) {
    fps = count * 1000 / time_elapsed;
    last_time = millis();
    count = 0;
  }
  tft.setCursor(0, 118);
  tft.printf("fps: %d", fps);
}