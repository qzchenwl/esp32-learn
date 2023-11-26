/*
  ESP32 I2S Microphone Sample
  esp32-i2s-mic-sample.ino
  Sample sound from I2S microphone, display on Serial Plotter
  Requires INMP441 I2S microphone

  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/
#include <Arduino.h>
// Include I2S driver
#include <driver/i2s.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SD.h>
#include <SD_MMC.h>
#include <SPIFFS.h>
#include <FFat.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>


// Connections to INMP441 I2S microphone
#define I2S_WS 26
#define I2S_SD 25
#define I2S_SCK 27

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

// Define input buffer length
#define bufferLen 64
int16_t sBuffer[bufferLen];

void i2s_install() {
    // Set up I2S Processor configuration
    const i2s_config_t i2s_config = {
            .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
            .sample_rate = 44100,
            .bits_per_sample = i2s_bits_per_sample_t(16),
            .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
            .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
            .intr_alloc_flags = 0,
            .dma_buf_count = 8,
            .dma_buf_len = bufferLen,
            .use_apll = false
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
    // Set I2S pin configuration
    const i2s_pin_config_t pin_config = {
            .bck_io_num = I2S_SCK,
            .ws_io_num = I2S_WS,
            .data_out_num = -1,
            .data_in_num = I2S_SD
    };

    i2s_set_pin(I2S_PORT, &pin_config);
}


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Define SDA and SCL pins
#define OLED_SDA    32
#define OLED_SCL    33
#define OLED_ADDR   0x3C // OLED I2C address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

    // Set up Serial Monitor
    Serial.begin(9600);
    Serial.println(" ");

    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 allocation failed");
        for(;;); // Infinite loop
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    // Initialize the built-in clock
    setTime(14, 0, 0, 1, 1, 2021); // Set initial time to 14:00:00, January 1, 2021


    delay(1000);

    // Set up I2S
    i2s_install();
    i2s_setpin();
    i2s_start(I2S_PORT);


    delay(500);
}

const int DRAW_INTERVAL = 100;  // 绘制波形的时间间隔（毫秒）
const int TIME_RANGE = 5000; // 展示的时间范围（毫秒）
const int PIXELS_NEEDED = SCREEN_WIDTH * DRAW_INTERVAL / TIME_RANGE;


unsigned long lastDrawTime = 0; // 上次绘制波形的时间
int16_t displayBuffer[SCREEN_WIDTH] = {0};
void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastDrawTime < DRAW_INTERVAL) {
        return;
    }

    // False print statements to "lock range" on serial plotter display
    // Change rangelimit value to adjust "sensitivity"
    int rangelimit = 3000;
    Serial.print(rangelimit * -1);
    Serial.print(" ");
    Serial.print(rangelimit);
    Serial.print(" ");

    // Get I2S data and place in data buffer
    size_t bytesIn = 0;
    esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);

    display.clearDisplay();

    if (result == ESP_OK) {
        int16_t samples_read = bytesIn / sizeof(int16_t);
        if (samples_read > 0) {
            display.setCursor(0,0);
            // 绘制波形
            int16_t newPixels[PIXELS_NEEDED];
            for (int i = 0; i < samples_read - 1; ++i) {
                // 将音频样本值映射到OLED屏幕的高度
                int y = map(sBuffer[i], -32768, 32767, 0, SCREEN_HEIGHT);
                // 计算x坐标，根据需要调整
                int x = map(i, 0, samples_read - 1, 0, PIXELS_NEEDED);
                newPixels[x] = y;
                // 在OLED屏幕上绘制线条
//                display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
            }
            memmove(displayBuffer, displayBuffer + PIXELS_NEEDED, (SCREEN_WIDTH - PIXELS_NEEDED) * sizeof(int16_t));
            memcpy(displayBuffer + SCREEN_WIDTH - PIXELS_NEEDED, newPixels, PIXELS_NEEDED * sizeof(int16_t));
            for (int i = 0; i < SCREEN_WIDTH - 1; ++i) {
                display.drawLine(i, displayBuffer[i], i + 1, displayBuffer[i + 1], SSD1306_WHITE);
            }
        }
    }

    display.setCursor(0,0);

    // Get the current time
    int hours = hour();
    int minutes = minute();
    int seconds = second();

    // Format and display the time
    display.print("Time: ");
    if (hours < 10) display.print('0');
    display.print(hours);
    display.print(':');
    if (minutes < 10) display.print('0');
    display.print(minutes);
    display.print(':');
    if (seconds < 10) display.print('0');
    display.print(seconds);

    display.display();
}