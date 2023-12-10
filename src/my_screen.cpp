#include <TJpg_Decoder.h>
#include "my_screen.h"

TFT_eSPI tft = TFT_eSPI();

static void write_dc(uint8_t val) {
  digitalWrite(3, val);
}

static void write_cs(uint8_t val) {
  digitalWrite(15, val);
}

static bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
  if ((y >= tft.height())) return false;
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}

void setup_my_screen() {
  pinMode(3, OUTPUT);
  pinMode(15, OUTPUT);

  tft.init(write_dc, write_cs); // 初始化显示器
  tft.setSwapBytes(true);
  tft.setRotation(1); // 设置显示方向
  tft.fillScreen(TFT_BLACK); // 清屏
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tft_output);
}