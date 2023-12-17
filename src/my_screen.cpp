#include <TJpg_Decoder.h>
#include "my_screen.h"

TFT_eSPI tft = TFT_eSPI();

static bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
  Serial.printf("tft_output x=%d, y=%d, w=%d, h=%d\n", x, y, w, h);
  if ((y >= tft.height())) return false;
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}

void setup_my_screen() {
  tft.init(); // 初始化显示器
  //tft.initDMA();
  tft.setSwapBytes(true);
  tft.setRotation(1); // 设置显示方向
  tft.fillScreen(TFT_BLACK); // 清屏
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  TJpgDec.setJpgScale(3);
  TJpgDec.setCallback(tft_output);
}