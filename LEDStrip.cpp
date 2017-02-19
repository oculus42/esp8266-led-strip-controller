// RGB Driver
#include "LEDStrip.h"

RGBdriver Driver(RGB_CLK, RGB_DATA);

float offset;
int16_t r1, g1, b1;
uint8_t brightness = 127, len, i;

color_s currentColor, oldColor;
color_s offColor = color_s {0, 0, 0};

// LED Strip can start in unexpected state so we must turn it off.
void setupLEDStrip() {
  // Turn off strip
  Driver.begin();
  Driver.SetColor(0, 0, 0);
  Driver.end();
}

void setStripColor() {
  offset = (float)brightness / 255.0;
  Driver.begin(); // begin
  Driver.SetColor(
    (uint8_t)(currentColor.red * offset),
    (uint8_t)(currentColor.green * offset),
    (uint8_t)(currentColor.blue * offset)
  );
  Driver.end();
}

void setColor(color_s nextColor) {
  currentColor = nextColor;
  setStripColor();
}


void fadeStripColor(color_s nextColor) {
  oldColor = currentColor;

  r1 = (nextColor.red - currentColor.red);
  g1 = (nextColor.green - currentColor.green);
  b1 = (nextColor.blue - currentColor.blue);

  len = max(abs(r1), max(abs(g1), abs(b1)));

  for (i = 1; i < len; ++i) {
    currentColor.red = oldColor.red + (1.0 * r1 * i) / len;
    currentColor.green = oldColor.green + (1.0 * g1 * i) / len;
    currentColor.blue = oldColor.blue + (1.0 * b1 * i) / len;

    setStripColor();
    delay(5);
    // yield();
  }

  currentColor = nextColor;
  setStripColor();
}


void turnOffStrip() {
  setColor(offColor);
  // fadeStripColor(offColor);
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
