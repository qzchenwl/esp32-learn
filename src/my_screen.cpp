#include <TJpg_Decoder.h>
#include <esp_camera.h>
#include "my_screen.h"
#include "my_expander.h"

TFT_eSPI tft = TFT_eSPI();

static bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
  if ((y >= tft.height())) return false;
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}

void setup_my_screen() {
  tft.init(); // 初始化显示器
  tft.setSwapBytes(true);
  tft.setRotation(1); // 设置显示方向
  tft.fillScreen(TFT_BLACK); // 清屏
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tft_output);
}

void my_screen_loop() {
  int t1 = millis();
  camera_fb_t* fb = esp_camera_fb_get();
  t1 = millis() - t1;

  int t2 = millis();
  TJpgDec.drawJpg(0, 80, fb->buf, fb->len);
  t2 = millis() - t2;

  esp_camera_fb_return(fb);
  tft.setCursor(0, 0);
  tft.printf("t1=%dms t2=%dms", t1, t2);
}