#include <Wire.h>

#ifndef _IR_COLORS
#define _IR_COLORS

// Structs and data for passing around colors
struct color_s {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct irColor_s {
  uint32_t irCode;
  uint16_t tft;
  color_s color;
};

const extern irColor_s remoteColors [20];

#endif
