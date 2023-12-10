#include <WiFi.h>

#include "my_wifi.h"

static const char *ssid = "hello-esp32"; // 您的热点的 SSID

void setup_my_wifi() {

  // 设置 ESP32 为接入点模式
  WiFi.mode(WIFI_AP);

  // 启动接入点模式，不设置密码
  WiFi.softAP(ssid);

  Serial.println();
  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("ip: ");
  Serial.println(WiFi.softAPIP());
}