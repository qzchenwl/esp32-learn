#include <Arduino.h>

#include "my_screen.h"
#include "my_lvgl.h"
#include "my_camera.h"
#include "my_wifi.h"
#include "my_expander.h"

void setup() {
  Serial.begin(115200);
  Serial.println("setup ...");

  Serial.println("setup my expander ...");
  setup_my_expander();

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

int count = 0;
int last_time = 0;
int fps = 0;
void loop() {
  my_lvgl_loop();
  my_screen_loop();
  my_wifi_loop();

  delay(5);

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