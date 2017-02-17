/***************************************************
  Based on the Adafruit TFT FeatherWing example
  ----> http://www.adafruit.com/products/3315
  
  With content from Daniel Eichhorn
  See more at http://blog.squix.ch

  Check out the links above for our tutorials and wiring diagrams

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <Adafruit_STMPE610.h>

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#endif
#ifdef __AVR_ATmega32U4__
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif
#ifdef ARDUINO_SAMD_FEATHER_M0
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif
#ifdef TEENSYDUINO
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#endif

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

#include "ArialRoundedMTBold_36.h"

// Additional UI functions
#include "GfxUi.h"
GfxUi ui = GfxUi(&tft);

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
int oldcolor, currentcolor;

TS_Point oldpoint, p;

boolean active = true;

void setup(void) {
  Serial.begin(115200);

  delay(10);
  Serial.println("FeatherWing TFT");
  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }
  Serial.println("Touchscreen started");
  
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  
  // make the color selection boxes
  // First Row
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_ORANGE);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_GREENYELLOW);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);

  // Second Row
  tft.fillRect(0, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.fillRect(BOXSIZE, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_PURPLE);
  tft.fillRect(BOXSIZE*2, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
  tft.fillRect(BOXSIZE*3, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  tft.fillRect(BOXSIZE*4, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_DARKGREY);
  tft.fillRect(BOXSIZE*5, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_BLACK);

 
  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  currentcolor = ILI9341_RED;
}

void paint() {
  if (p.y < BOXSIZE*2) {
     oldcolor = currentcolor;

     if (p.y < BOXSIZE) {
       if (p.x < BOXSIZE) { 
         currentcolor = ILI9341_RED; 
         tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = ILI9341_ORANGE;
         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = ILI9341_YELLOW;
         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*4) {
         currentcolor = ILI9341_GREENYELLOW;
         tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*5) {
         currentcolor = ILI9341_GREEN;
         tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*6) {
         currentcolor = ILI9341_CYAN;
         tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       }
     } else {
       if (p.x < BOXSIZE) { 
         currentcolor = ILI9341_BLUE; 
         tft.drawRect(0, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = ILI9341_PURPLE;
         tft.drawRect(BOXSIZE, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = ILI9341_MAGENTA;
         tft.drawRect(BOXSIZE*2, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*4) {
         currentcolor = ILI9341_WHITE;
         tft.drawRect(BOXSIZE*3, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*5) {
         currentcolor = ILI9341_DARKGREY;
         tft.drawRect(BOXSIZE*4, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       } else if (p.x < BOXSIZE*6) {
         currentcolor = ILI9341_BLACK;
         tft.drawRect(BOXSIZE*5, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
       }
     }

      if (oldcolor != currentcolor) {
        if (oldcolor == ILI9341_RED) 
          tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
        if (oldcolor == ILI9341_ORANGE) 
          tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_ORANGE);
        if (oldcolor == ILI9341_YELLOW) 
          tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
        if (oldcolor == ILI9341_GREENYELLOW) 
          tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_GREENYELLOW);
        if (oldcolor == ILI9341_GREEN) 
          tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
        if (oldcolor == ILI9341_CYAN) 
          tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
        if (oldcolor == ILI9341_BLUE) 
          tft.fillRect(0, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_BLUE);
        if (oldcolor == ILI9341_PURPLE) 
          tft.fillRect(BOXSIZE, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_PURPLE);
        if (oldcolor == ILI9341_MAGENTA) 
          tft.fillRect(BOXSIZE*2, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
        if (oldcolor == ILI9341_WHITE) 
          tft.fillRect(BOXSIZE*3, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        if (oldcolor == ILI9341_DARKGREY) 
          tft.fillRect(BOXSIZE*4, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_DARKGREY);
        if (oldcolor == ILI9341_BLACK) 
          tft.fillRect(BOXSIZE*5, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_BLACK);
     }
  }

  // Paint the Current Color
  tft.fillRect(BOXSIZE*3, BOXSIZE*2, BOXSIZE*3, BOXSIZE*2, currentcolor);

  // Paint the ON/OFF switch
  tft.setFont(&ArialRoundedMTBold_36);
  tft.fillRect(0, BOXSIZE*2, BOXSIZE*3, BOXSIZE*2, ILI9341_YELLOW);
  ui.setTextColor(ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(BOXSIZE*1.5, BOXSIZE*3.25, "ON");
}

boolean samePoint(TS_Point p1, TS_Point p2) {
  return p1.x == p2.x && p1.y == p2.y;
}

boolean closePoint(TS_Point p1, TS_Point p2) {
  return abs(p1.x - p2.x) < 10 && abs(p1.y - p2.y) < 10;
}

void loop() {
  // Retrieve a point  
  p = ts.getPoint(); 
 
  // Scale from ~0->4000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  if (!closePoint(p, oldpoint)) {
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z); 
    paint();
  }

  // Save last point
  oldpoint = p;
}
