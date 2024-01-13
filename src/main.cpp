#include "M5CoreS3.h"

void draw_function(LovyanGFX* gfx) {
  int x      = rand() % gfx->width();
  int y      = rand() % gfx->height();
  int r      = (gfx->width() >> 4) + 2;
  uint16_t c = rand();
  gfx->fillRect(x - r, y - r, r * 2, r * 2, c);
}

void setup() {
  String x = "";
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  int textsize = CoreS3.Display.height() / 60;
  if (textsize == 0) {
    textsize = 1;
  }
  CoreS3.Display.setTextSize(textsize);
}

void loop() {
  int x      = rand() % CoreS3.Display.width();
  int y      = rand() % CoreS3.Display.height();
  int r      = (CoreS3.Display.width() >> 4) + 2;
  uint16_t c = rand();
  CoreS3.Display.fillCircle(x, y, r, c);
  draw_function(&CoreS3.Display);
}
