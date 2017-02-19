#include "tft.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

GfxUi ui = GfxUi(&tft);

void disableBacklight() {
  ts.writeRegister8(STMPE_GPIO_CLR_PIN, _BV(2)); // backlight off  
  ts.writeRegister8(STMPE_INT_STA, 0xFF);
}

void enableBacklight() {
    ts.writeRegister8(STMPE_GPIO_SET_PIN, _BV(2));
}

void paintColors() {

  uint8_t x, y, inc = 0;

  for (y = 0; y < 4; y++) {
    for (x = 0; x < 5; x++) {
      tft.fillRect(BOXSIZE*x, BOXSIZE*y, BOXSIZE, BOXSIZE, remoteColors[inc].tft);
      inc++;
    }
  }
}

void paintSwitch(bool active) {
  // Paint the ON/OFF switch
  tft.setFont(&ArialRoundedMTBold_36);
  ui.setTextAlignment(CENTER);

  tft.fillRect(0, 240, 120, 80, active ? ILI9341_WHITE : ILI9341_BLACK);
  
  ui.setTextColor(active ? ILI9341_BLACK : ILI9341_WHITE);
  ui.drawString(56, 290, active ? "ON" : "OFF");
  
}
