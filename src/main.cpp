#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

#include "M5Unified.h"
#include "SSEHTTPClient.h"

#define WIFI_SSID "cwllll iPhone"
#define WIFI_PASS "12345678"


void setup(void) {
  USBSerial.begin(15200);
  USBSerial.println("M5CoreS3 User Demo");

  M5.begin();
  M5.Display.setTextSize(2);
  M5.Display.println("M5CoreS3 User Demo");

  USBSerial.printf("Connecting to %s", WIFI_SSID);
  M5.Display.printf("Connecting to %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    USBSerial.print('.');
    M5.Display.print('.');
    delay(1000);
  }
  USBSerial.println("\nConnected!");
  M5.Display.println("\nConnected!");

  WiFiClient client;
  SSEHTTPClient http;
  if (!http.begin(client, "http://172.20.10.9:8080/test-sse")) {
    USBSerial.println("HTTPClient begin failed");
    M5.Display.println("HTTPClient begin failed");
    while(true) { delay(1000000); }
  }
  int httpStatusCode = http.GET();
  if (httpStatusCode != HTTP_CODE_OK) {
    USBSerial.printf("HTTPClient GET return %d\n", httpStatusCode);
    M5.Display.printf("HTTPClient GET return %d\n", httpStatusCode);
    while(true) { delay(1000000); }
  }

  while(true) {
    int time = millis();
    char eventData[1024];
    int ret = http.pollEventData(eventData, sizeof(eventData));
    if (ret == HTTPC_ERROR_SSE_INCOMPLETE) {
      USBSerial.println("HTTPC_ERROR_INCOMPLETE_SSE");
      delay(500);
      continue;
    } else if (ret == HTTPC_ERROR_SSE_EOF) {
      USBSerial.println("OK");
      return;
    } else if (ret < 0) {
      USBSerial.printf("pollEventData error: %d\n", ret);
      return;
    } else {
      time = millis() - time;
      USBSerial.printf("time: %d\n", time);
      USBSerial.printf("RECEIVE: %s\n", eventData);
    }
  }
}

void loop() {
}

