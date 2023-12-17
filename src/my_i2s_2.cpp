
#include <Audio.h>

// I2S Connections
#define I2S_DOUT      1
#define I2S_BCLK      2
#define I2S_LRC       17

Audio audio;

void setup_my_i2s_2() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    delay(100000);
    return;
  }

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(21);
  audio.connecttoFS(SPIFFS, "/alloy.wav");
}

void my_i2s_2_loop() {
  if (!audio.isRunning()) {
    delay(1000);
    audio.connecttoFS(SPIFFS, "alloy.wav");
  }
  audio.loop();
}
