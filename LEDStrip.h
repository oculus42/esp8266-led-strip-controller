
// Swap out the LED RGB Strip driver with a Wire protocol

#include <Wire.h>

#include "colors.h"
#include "settings.h"

#ifndef _LED_Pass
#define _LED_Pass

void setStripColor(void);
void setColor(color_s nextColor);
void fadeStripColor(color_s nextColor);
void increaseBrightness(void);
void decreaseBrightness(void);
void turnOffStrip(void);

#endif
