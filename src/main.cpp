#include "Arduino.h"
#include "SoundData.h"

const int speakerPin = 25; // GPIO25用于扬声器

void setup() {
    // 初始化GPIO25为PWM输出
    ledcSetup(0, 16000, 8); // 使用通道0, 16 kHz PWM频率, 8位分辨率
    ledcAttachPin(speakerPin, 0); // 将GPIO25连接到通道0
}

void loop() {
    // 遍历并播放音频样本数组
    for (unsigned char i : sample) {
        ledcWrite(0, i); // 直接将样本值写入PWM
        delayMicroseconds(62); // 根据16kHz采样率等待
    }

    // 可能需要在这里添加一些逻辑来重复播放或停止
}
