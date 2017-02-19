
#include "colors.h"
#include "RGBdriver.h"

#ifndef _LED_STRIP
#define _LED_STRIP

#define RGB_CLK 5
#define RGB_DATA 4

void setupLEDStrip(void);
void setStripColor(void);
void setColor(color_s nextColor);
void fadeStripColor(color_s nextColor); 
void increaseBrightness(void);
void decreaseBrightness(void);
void turnOffStrip(void);

#endif
