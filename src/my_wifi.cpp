#include <WiFi.h>
#include <esp_camera.h>
#include <WebServer.h>

#include "my_wifi.h"

static const char *ssid = "hello-esp32"; // 您的热点的 SSID
static WebServer server(80);

static void handle_root() {
  camera_fb_t *fb = esp_camera_fb_get(); // 获取摄像头图像
  if (!fb) {
    Serial.println("Camera capture failed");
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }

  // 将摄像头数据作为响应发送
  server.sendHeader("Content-Type", "image/jpeg");
  server.sendContent((char *)fb->buf, fb->len);

  // 释放帧缓冲区
  esp_camera_fb_return(fb);
}

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

  // 定义路由
  server.on("/", HTTP_GET, handle_root);

  // 启动服务器
  server.begin();
}

void my_wifi_loop() {
  server.handleClient();
}
