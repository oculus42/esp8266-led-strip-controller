#ifndef _LED_STRIP
#define _LED_STRIP

void setupLEDStrip(void);
void setStripColor(void);
void setColor(color_s nextColor);
void fadeStripColor(color_s nextColor); 
void increaseBrightness(void);
void decreaseBrightness(void);

#endif
