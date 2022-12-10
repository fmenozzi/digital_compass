#include <FastLED.h>
#include <QMC5883LCompass.h>

const int NUM_LEDS = 24;
const int DATA_PIN = 6;
const int DELAY_MS = 50;

CRGB ring[NUM_LEDS];

QMC5883LCompass compass;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(ring, NUM_LEDS);

  compass.init();
  compass.setCalibration(-1927, 402, -715, 1688, -2215, 525);
}

void show_led(int idx, CRGB color) {
  ring[idx] = color;
  ring[idx].fadeLightBy(196);
}

void clear_led(int idx) {
  ring[idx] = CRGB::Black;
}

void draw_compass(int north_led_idx) {
  int idx_n = north_led_idx;
  int idx_e = (idx_n + NUM_LEDS*1/4) % NUM_LEDS;
  int idx_s = (idx_n + NUM_LEDS*2/4) % NUM_LEDS;
  int idx_w = (idx_n + NUM_LEDS*3/4) % NUM_LEDS;

  show_led(idx_n, CRGB::Red);
  show_led(idx_e, CRGB::White);
  show_led(idx_s, CRGB::White);
  show_led(idx_w, CRGB::White);

  FastLED.show();

  clear_led(idx_n);
  clear_led(idx_e);
  clear_led(idx_s);
  clear_led(idx_w);
}

// Determine the index of the Neopixel ring that corresponds to north, given
// the magnetometer heading in degrees. The top LED (that is, the LED pointing
// in the X-direction of the magnetometer) is index 0.
//
// Note that we want each arc centered on the LED, and not the LED's edge (e.g.
// for a Neopixel Ring 24, the arc angle will be 15 degrees, but since it's
// centered on the LED, the top LED will shine when the heading is between 352.5
// and 7.5 degrees).
//
// TODO: See if there's a closed form representation for this that doesn't
//       require this loop.
int convert_heading_to_north_led_index(float heading_deg) {
  const float ARC_ANGLE = 360.0 / NUM_LEDS;
  const float HALF_ARC_ANGLE = ARC_ANGLE / 2.0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (heading_deg < i*ARC_ANGLE + HALF_ARC_ANGLE) {
      return (NUM_LEDS - i) % NUM_LEDS;
    }
  }
  return NUM_LEDS - 1; // For the last half-arc
}

void loop() {
  compass.read();
  float heading = compass.getAzimuth();

  int north_led_idx = convert_heading_to_north_led_index(heading);
  draw_compass(north_led_idx);

  delay(DELAY_MS);
}
