#include <Arduino.h>
#include <esp_camera.h>
#include "my_camera.h"
#include <WiFi.h>

#include <HTTPClient.h>
const char* serverName = "http://192.168.12.229:8000/upload";


const char* ssid = "Xiaomi";
const char* password = "87883222";
void setup() {
  Serial.begin(115200);
  Serial.println("setup ...");
  setup_my_camera();

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");


  Serial.println("setup my screen ...");

  Serial.println("setup my camera ...");

  delay(2000);
}

int count = 0;
void loop() {
  int t1 = millis();
  camera_fb_t* fb = esp_camera_fb_get();
  t1 = millis() - t1;

  int t2 = millis();
  t2 = millis() - t2;

  //Serial.println(String((char *)fb->buf, fb->len));


  // 关闭 HTTP 连接

  Serial.printf("t1=%dms t2=%dms l=%d\r\n", t1, t2, fb->len);
  esp_camera_fb_return(fb);
}