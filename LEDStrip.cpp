#include "LEDStrip.h"

float offset;
int16_t r1, g1, b1;
uint8_t brightness = 127, len, i;

color_s currentColor, oldColor;
color_s offColor = color_s {0, 0, 0};

void setStripColor() {

  Wire.beginTransmission(WIRE_ADDR_LED); // transmit to other controller
  Wire.write(currentColor.red);
  Wire.write(currentColor.green);
  Wire.write(currentColor.blue);
  Wire.endTransmission();    // stop transmitting
}

void setColor(color_s nextColor) {
  currentColor = nextColor;
  setStripColor();
}

void fadeStripColor(color_s nextColor) {
  // No actual fade, since that's handled by the other controller
  oldColor = currentColor;
  currentColor = nextColor;
  setStripColor();
}

void turnOffStrip() {
  setColor(offColor);
}

void increaseBrightness() {
   // Brighter
  if (brightness == 0) {
    brightness == 7;
  } else if (brightness <= 247) {
    brightness += 8;
  }

  setStripColor();
}

void decreaseBrightness() {
  // Darker
  if (brightness >= 7) {
    brightness -= 8;
  } else if (brightness != 0) {
    brightness = 0;
  }

  setStripColor();
}
