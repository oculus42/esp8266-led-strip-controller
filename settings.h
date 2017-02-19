/**The MIT License (MIT)
Based on weather_station_color/settings.h by Daniel Eichhorn
See more at http://blog.squix.ch
*/

#ifndef _LED_SETTINGS
#define _LED_SETTINGS

// Setup
const int UPDATE_INTERVAL_SECS = 10 * 60;  // Update every 10 minutes
const boolean USE_TOUCHSCREEN_WAKE = true;       // use the touchscreen to wake up, ~90mA current draw
const boolean DEEP_SLEEP = false;                 // use the touchscreen for deep sleep, ~10mA current draw but doesnt work
const int AWAKE_TIME = 10;                   // how many seconds to stay 'awake' before going back to zzz

// Pins for the ILI9341
#define TFT_DC 15
#define TFT_CS 0

// pins for the touchscreen
#define STMPE_CS 16
#define STMPE_IRQ 4

// Size of the color selection boxes
#define BOXSIZE 40

#endif
