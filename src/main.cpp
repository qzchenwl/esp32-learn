#include <Arduino.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SD.h>
#include <SD_MMC.h>
#include <FFat.h>
#include "Audio.h"

// I2S Connections
#define I2S_DOUT      27
#define I2S_BCLK      26
#define I2S_LRC       25

// Create Audio object
Audio audio;

String ssid = "cwllll";
String password = "18650807813";

void setup() {
    Serial.begin(115200);
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.println("connecting to WIFI cwllll");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("connected!");

    // Setup I2S
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

    // Set Volume
    audio.setVolume(21);
    // Open music file
    audio.connecttoFS(SPIFFS, "alloy.wav");
}

void loop() {
    if (!audio.isRunning()) {
        delay(1000);
        audio.connecttoFS(SPIFFS, "alloy.wav");
    }
    audio.loop();
}