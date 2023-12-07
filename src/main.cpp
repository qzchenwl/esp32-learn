/**
 * https://github.com/adafruit/Adafruit-ST7735-Library/blob/master/examples/graphicstest/graphicstest.ino
 *
 * | TFT | ESP32 |
 * |-----|-------|
 * | GND | GND   |
 * | VCC | 3.3V  |
 * | SCL | GPIO18 |
 * | SDA | GPIO23 |
 * | RST | GPIO4 |
 * | DC  | GPIO16 |
 * | CS  | GPIO5 |
 * | BLK | - |
 *
 */

#include <TFT_eSPI.h>
#include <SPIFFS.h>
#include <TJpg_Decoder.h>
#include "images/jpeg272x233.h"

TFT_eSPI tft = TFT_eSPI();  // 创建 TFT 对象

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
    if ((y >= tft.height())) return false;
    tft.pushImage(x, y, w, h, bitmap);
    return true;
}

void setup() {
    Serial.begin(115200);
    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(tft_output);

    tft.init(); // 初始化显示器
    tft.setSwapBytes(true);
    tft.setRotation(1); // 设置显示方向
    tft.fillScreen(TFT_BLACK); // 清屏
    tft.initDMA();

    // 初始化 SPIFFS
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS 初始错误!");
        return;
    }

    // 尝试打开图像文件
    int time;

    time = millis();
    TJpgDec.drawJpg(0, 0, test_jpeg_272x233, test_jpeg_272x233_len);
    time = millis() - time;
    Serial.printf("解码时间: %dms\n", time);

    time = millis();
    TJpgDec.drawJpg(0, 0, test_jpeg_272x233, test_jpeg_272x233_len);
    time = millis() - time;
    Serial.printf("解码时间: %dms\n", time);
}

void loop() {
    // 此示例中 loop 不执行任何操作
}
