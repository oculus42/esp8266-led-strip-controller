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
uint8_t colorIndex;

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

  uint8_t x, y, inc = 0;

  for (y = 0; y < 3; y++) {
    for (x = 0; x < 6; x++) {
      tft.fillRect(BOXSIZE*x, BOXSIZE*y, BOXSIZE, BOXSIZE, remoteColors[inc].tft);
      inc++;
    }
  }
}


void paintSwitch() {
  // Paint the ON/OFF switch
  tft.setFont(&ArialRoundedMTBold_36);
  ui.setTextAlignment(CENTER);

  if (active) {
    tft.fillRect(0, BOXSIZE*3, BOXSIZE*3, BOXSIZE*2, ILI9341_WHITE);
    ui.setTextColor(ILI9341_BLACK);
    ui.drawString(BOXSIZE*1.4, BOXSIZE*4.25, "ON");
  } else {
    tft.fillRect(0, BOXSIZE*3, BOXSIZE*3, BOXSIZE*2, ILI9341_BLACK);
    ui.setTextColor(ILI9341_WHITE);
    ui.drawString(BOXSIZE*1.4, BOXSIZE*4.25, "OFF");
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
  colorIndex = 0;

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void updatePixel() {
  color_s col = remoteColors[colorIndex].color;
  
  strip.setPixelColor(currentPixel, strip.Color(col.red, col.green, col.blue));
  currentPixel = (currentPixel + 1) % PIXELCOUNT;
  strip.show();
}

void checkSettings() {
  uint8_t offset_x, offset_y;
  
  // Color buttons first
  if (p.y < BOXSIZE*3) {
    oldcolor = currentcolor;

    offset_y = floor(p.y / BOXSIZE) * 6;
    offset_x = floor(p.x / BOXSIZE);

    colorIndex = offset_y + offset_x;
    currentcolor = remoteColors[colorIndex].tft;
    
  } else if (p.y < BOXSIZE*5 && p.x < BOXSIZE*3 ) {
    active = !active;
    paintSwitch();
  }

  if (oldcolor != currentcolor) {
    // Paint the Current Color inset slightly
    tft.fillRect(BOXSIZE*3 + 2, BOXSIZE*3 + 2, BOXSIZE*3 - 4, BOXSIZE*2 - 4, currentcolor);
    updatePixel();
  }
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
