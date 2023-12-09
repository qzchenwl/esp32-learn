#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <esp_camera.h>

#include "images/jpeg272x233.h"

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

const char* ssid = "Xiaomi";
const char* password = "87883222";

TFT_eSPI tft = TFT_eSPI();  // 创建 TFT 对象

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
    if ((y >= tft.height())) return false;
    tft.pushImage(x, y, w, h, bitmap);
    return true;
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

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

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 1;
  config.fb_count = 1;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // drop down frame size for higher initial frame rate
  if(config.pixel_format == PIXFORMAT_JPEG){
    s->set_framesize(s, FRAMESIZE_QVGA);
  }
}

int count = 0;
int begin_time = 0;
int fps = 0;
void loop() {
  camera_fb_t* fb = esp_camera_fb_get();
  TJpgDec.drawJpg(0, 20, fb->buf, fb->len);
  esp_camera_fb_return(fb);
  count ++;

  int end_time = millis();
  if (end_time - begin_time >= 1000) {
    fps = 1000 * count / (end_time - begin_time);

    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.printf("FPS: %d", fps);

    begin_time = millis();
    count = 0;
  }

}
