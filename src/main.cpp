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

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPIFFS.h>
#include <JPEGDecoder.h>


#define TFT_CS     5
#define TFT_RST    4 // 你也可以将RST连接到ESP32的RST或者3.3V
#define TFT_DC     16

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
    Serial.begin(115200);
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS initialisation failed!");
        while (1) yield(); // 如果SPIFFS没有初始化成功，停在这里
    }
    Serial.println("\r\nInitialisation done.");

    tft.initR(INITR_BLACKTAB);
}

void renderJPEG(int x, int y) {
    // 设置在屏幕上的位置
    int drawX = 0;
    int drawY = 0;

    // 开始解码
    while (JpegDec.read()) { // 解码扫描线
        uint16_t *pImg = JpegDec.pImage;
        for (int i = 0; i < JpegDec.width; i++) {
            // 获取像素颜色并显示
            Serial.printf("x = %d, y = %d, color = %d\n", drawX + i, drawY, *pImg);
            tft.drawPixel(drawX + i, drawY, *pImg++);
        }
        drawY++;
    }
}

void loop() {
    Serial.println("loop");

    // 打开图片文件
    File jpgFile = SPIFFS.open("/1.jpeg", "r"); // 请确保路径和文件名与你的文件相匹配

    if (!jpgFile) {
        Serial.println("JPEG file not found");
        return;
    }

    // JPEG图片解码
    bool decoded = JpegDec.decodeFsFile(jpgFile);

    // 关闭文件
    jpgFile.close();

    if (decoded) {
        // 渲染图片
        renderJPEG(JpegDec.width, JpegDec.height);
    } else {
        Serial.println("JPEG decode failed!");
    }

    delay(5000); // 等待5秒
}
