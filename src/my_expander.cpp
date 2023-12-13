#include <Arduino.h>
#include <PCF8574.h>

PCF8574 pcf8574(0x20, 3, 15);
unsigned long timeElapsed;

void setup_my_expander() {
  pcf8574.pinMode(P0, OUTPUT);
  pcf8574.pinMode(P1, OUTPUT);

  if (!pcf8574.begin()) {
    Serial.println("Error initializing PCF8574");
  }
}
