#include <Arduino.h>
#include <WiFi.h>
#include <M5Unified.h>

#include "AudioOutputM5Speaker.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"

#define WIFI_SSID "Xiaomi"
#define WIFI_PASS "87883222"

AudioGeneratorMP3 *mp3;
AudioFileSourceICYStream *file;
AudioFileSourceBuffer *buff;
AudioOutputM5Speaker *out;

void setup(void) {
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

  M5.Speaker.begin();
  M5.Speaker.setVolume(100);

  file = new AudioFileSourceICYStream("http://192.168.107.229:8081/static/a.mp3");
  buff = new AudioFileSourceBuffer(file, 2048);
  out = new AudioOutputM5Speaker(&M5.Speaker, 0);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(buff, out);
}

void loop() {
  static int lastms = 0;

  if (mp3->isRunning()) {
    if (millis()-lastms > 1000) {
      lastms = millis();
      USBSerial.printf("Running for %d ms...\n", lastms);
      USBSerial.flush();
    }
    if (!mp3->loop()) mp3->stop();
  } else {
    USBSerial.printf("MP3 done\n");
    delay(1000);
  }
}

