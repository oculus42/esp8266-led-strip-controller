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
#include "LEDStrip.h"

irColor_s currentCol, oldCol;
uint8_t colorIndex;

TS_Point oldPoint, p;

boolean active = true;
boolean sleep = false;

unsigned long lastActionMillis;



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

  // we'll use STMPE's GPIO 2 for backlight control
  ts.writeRegister8(STMPE_GPIO_DIR, _BV(2));
  ts.writeRegister8(STMPE_GPIO_ALT_FUNCT, _BV(2));
  // backlight on
  ts.writeRegister8(STMPE_GPIO_SET_PIN, _BV(2));

  // White by default
  colorIndex = 15;
  currentCol = remoteColors[colorIndex];
  
  paintColors();
  paintSwitch(active);
  updateColor();

  lastActionMillis = millis();
}

void updateColor() {
  fadeStripColor(remoteColors[colorIndex].color);
}

void checkSettings() {
  uint8_t offset_x, offset_y;
  
  // Color buttons first
  if (p.y < BOXSIZE*4) {

    // No color changes if off
    if (!active) {
      return;
    }
    
    oldCol = currentCol;

    offset_y = floor(p.y / BOXSIZE) * 5;
    offset_x = floor(p.x / BOXSIZE);

    colorIndex = offset_y + offset_x;
    currentCol = remoteColors[colorIndex];
    
  } else if (p.y >= 240 && p.x < 120 ) {
    active = !active;
  
    paintSwitch(active);

    if (active) {
      updateColor();
    } else {
      turnOffStrip();
    }

    // Prevent double-touch
    delay(100);
    lastActionMillis = millis();
    return;
  }
  
  // Only if there is a change do we do anything.
  if (oldCol.tft != currentCol.tft) {
    // Paint the Current Color inset slightly
    tft.fillRect(BOXSIZE*3 + 2, BOXSIZE*4 + 2, BOXSIZE*3 - 4, BOXSIZE*2 - 4, currentCol.tft);
    updateColor();
    
    // Set last action time (for sleep)
    lastActionMillis = millis();
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

  // If sleeping, onthing happens until touched
  if (sleep) {
    if (ts.touched()) {
      sleep = false;
      
      lastActionMillis = millis();
      enableBacklight();
      delay(500);
      yield();
    }
  } else {

    if (ts.touched()) {
      
      // Scale from ~0->4000 to tft.width using the calibration #'s
      p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
      p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    
      // ts.touched() solves for buffered input
      if (ts.touched() && !closePoint(p, oldPoint)) {
        Serial.print("\nX = "); Serial.print(p.x);
        Serial.print("\tY = "); Serial.print(p.y);
        Serial.print("\tPressure = "); Serial.println(p.z); 
        checkSettings();
      } else {
        Serial.print('.');
      }
  
      // Save last point
      oldPoint = p;
    } else {
      if (lastActionMillis + (AWAKE_TIME * 1000) < millis()) {
        
        disableBacklight();
        sleep = true;
      }
    }
  }
}
