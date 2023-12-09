#include <Arduino.h>
#include <esp_camera.h>
#include <TJpg_Decoder.h>

#include "my_camera.h"
#include "my_screen.h"

void setup() {
  Serial.begin(115200);
  Serial.println("setup ...");

  Serial.println("setup my screen ...");
  setup_my_screen();

  Serial.println("setup my camera ...");
  setup_my_camera();

  delay(2000);
}

int count = 0;
void loop() {
  int t1 = millis();
  camera_fb_t* fb = esp_camera_fb_get();
  t1 = millis() - t1;

  int t2 = millis();
  TJpgDec.drawJpg(0, 10, fb->buf, fb->len);
  t2 = millis() - t2;

  esp_camera_fb_return(fb);
  tft.setCursor(0, 0);
  tft.printf("t1=%dms t2=%dms", t1, t2);
}