#include <Arduino.h>
#include <driver/dac.h>
#include "SoundData.h"

void setup() {
    // 启用DAC通道
    dac_output_enable(DAC_CHANNEL_1);
}

void loop() {
    // 遍历音频样本数组
    for (unsigned char i : sample) {
        // 输出当前样本值到DAC
        dac_output_voltage(DAC_CHANNEL_1, i);
        // 根据音频采样率延时（例如，16000Hz采样率对应62.5微秒）
        delayMicroseconds(62);
    }
    // 在音频播放结束后，可以加入一段延时
    delay(1000);
}
