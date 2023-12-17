#include <Arduino.h>
#include <esp_camera.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>
#include <base64.h>

#include "my_camera.h"
#include "my_screen.h"
#include "my_i2s.h"


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
  if(psramFound()){
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


  delay(2000);
}

int count = 0;
int fps = 0;
int fss = 0;
void loop() {
  int t1 = millis();
  // fps
  count++;
  if (millis() - fps > 1000) {
    fps = millis();
    fss = count;
    //Serial.printf("fps=%d\n", count);
    count = 0;
  }

  camera_fb_t* fb = esp_camera_fb_get();
  t1 = millis() - t1;

  int t2 = millis();
  TJpgDec.drawJpg(0, 10, fb->buf, fb->len);
  t2 = millis() - t2;
  //print image base64
  //String image_base64 = base64::encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);
  tft.setCursor(0, 0);
  tft.printf("t1=%dms t2=%dms l=%d ", t1, t2, fss);
  my_i2s_loop();
}