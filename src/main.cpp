/**
 * @file streams-i2s-serial.ino
 * @author Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-stream/streams-i2s-serial/README.md
 * 
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

#include <Arduino.h>
#include <driver/dac.h>


// Arduino Setup
void setup(void) {
    dac_output_enable(DAC_CHANNEL_1);
}

void loop() {
    // 产生一个简单的500Hz方波，持续时间1秒
    for (int i = 0; i < 500; i++) {
        // 将DAC输出设为高
        dac_output_voltage(DAC_CHANNEL_1, 255);
        delayMicroseconds(1000); // 延时1ms
        // 将DAC输出设为低
        dac_output_voltage(DAC_CHANNEL_1, 0);
        delayMicroseconds(1000); // 延时1ms
    }
    delay(1000); // 延时1秒
}
