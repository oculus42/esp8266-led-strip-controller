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

#include "tft.h"
#include "colors.h"

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
int oldcolor, currentcolor;

TS_Point oldpoint, p;

boolean active = true;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIXELCOUNT 9
#define PIXELPIN 4

// Temp pin setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800);

uint8_t currentPixel = 0;

void paintColors() {
  
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
}


void paintSwitch() {
  // Paint the ON/OFF switch
  tft.setFont(&ArialRoundedMTBold_36);
  ui.setTextAlignment(CENTER);

  if (active) {
    tft.fillRect(0, BOXSIZE*2, BOXSIZE*3, BOXSIZE*2, ILI9341_WHITE);
    ui.setTextColor(ILI9341_BLACK);
    ui.drawString(BOXSIZE*1.4, BOXSIZE*3.25, "ON");
  } else {
    tft.fillRect(0, BOXSIZE*2, BOXSIZE*3, BOXSIZE*2, ILI9341_BLACK);
    ui.setTextColor(ILI9341_WHITE);
    ui.drawString(BOXSIZE*1.4, BOXSIZE*3.25, "OFF");
  }
}

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
  
  paintColors();
  paintSwitch();

  currentcolor = ILI9341_RED;

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void updatePixel() {
  strip.setPixelColor(currentPixel, currentcolor);
  currentPixel = (currentPixel + 1) % PIXELCOUNT;
  strip.show();
}

void checkSettings() {
  // Color buttons first
  if (p.y < BOXSIZE*2) {
     oldcolor = currentcolor;

     if (p.y < BOXSIZE) {
       if (p.x < BOXSIZE) { 
         currentcolor = ILI9341_RED; 
       } else if (p.x < BOXSIZE*2) {
         currentcolor = ILI9341_ORANGE;
       } else if (p.x < BOXSIZE*3) {
         currentcolor = ILI9341_YELLOW;
       } else if (p.x < BOXSIZE*4) {
         currentcolor = ILI9341_GREENYELLOW;
       } else if (p.x < BOXSIZE*5) {
         currentcolor = ILI9341_GREEN;
       } else if (p.x < BOXSIZE*6) {
         currentcolor = ILI9341_CYAN;
       }
     } else {
       if (p.x < BOXSIZE) { 
         currentcolor = ILI9341_BLUE; 
       } else if (p.x < BOXSIZE*2) {
         currentcolor = ILI9341_PURPLE;
       } else if (p.x < BOXSIZE*3) {
         currentcolor = ILI9341_MAGENTA;
       } else if (p.x < BOXSIZE*4) {
         currentcolor = ILI9341_WHITE;
       } else if (p.x < BOXSIZE*5) {
         currentcolor = ILI9341_DARKGREY;
       } else if (p.x < BOXSIZE*6) {
         currentcolor = ILI9341_BLACK;
       }
     }
  } else if (p.y < BOXSIZE*5 && p.x < BOXSIZE*3 ) {
    active = !active;
    paintSwitch();
  }

  // Paint the Current Color inset slightly
  tft.fillRect(BOXSIZE*3 + 2, BOXSIZE*2 + 2, BOXSIZE*3 - 4, BOXSIZE*2 - 4, currentcolor);
  updatePixel();
}

boolean samePoint(TS_Point p1, TS_Point p2) {
  return p1.x == p2.x && p1.y == p2.y;
}

boolean closePoint(TS_Point p1, TS_Point p2) {
  return abs(p1.x - p2.x) < 5 && abs(p1.y - p2.y) < 5;
}

void loop() {
  // Retrieve a point  
  p = ts.getPoint(); 
 
  // Scale from ~0->4000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  // ts.touched() solves for buffered input
  if (ts.touched() && !closePoint(p, oldpoint)) {
    Serial.print("\nX = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z); 
    checkSettings();
  } else {
    Serial.print('.');
  }

  // Save last point
  oldpoint = p;
}
