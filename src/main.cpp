#include <Arduino.h>
#include <esp_camera.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>
#include <base64.h>

#include "my_camera.h"
#include "my_screen.h"
#include "my_i2s.h"
#include "my_i2s_2.h"


void setup() {
  Serial.begin(115200);
  Serial.println("setup ...");
  delay(2000);
  Serial.print("Total heap: ");
  Serial.println(ESP.getHeapSize());
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Min free heap: ");
  Serial.println(ESP.getMinFreeHeap());
  if (psramFound()) {
    Serial.println("PSRAM is found and initialized.");
    Serial.print("PSRAM largest free block: ");
    Serial.println(ESP.getMaxAllocPsram());
  } else {
    Serial.println("PSRAM is not found.");
  }
  setup_my_i2s();
  WiFi.begin("Xiaomi", "87883222");
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(WiFi.status());
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("setup my screen ...");
  setup_my_screen();

  Serial.println("setup my camera ...");
  setup_my_camera();


  Serial.println("setup my i2s_2 ...");
  setup_my_i2s_2();

  Serial.println("setup done");
//
//  delay(2000);
}

void loop() {
  my_i2s_2_loop();
  camera_fb_t* fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  tft.setCursor(0, 0);
  my_i2s_loop();
}