#include <Arduino.h>
#include <esp_camera.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>
#include <base64.h>

#include "my_camera.h"
#include "my_screen.h"

void setup() {
  Serial.begin(115200);
  Serial.println("setup ...");

  WiFi.begin("Xiaomi", "87883222");
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

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

  String encodedImage = base64::encode(fb->buf, fb->len);

  esp_camera_fb_return(fb);
  tft.setCursor(0, 0);
  tft.printf("t1=%dms t2=%dms len=%d", t1, t2, encodedImage.length());
}