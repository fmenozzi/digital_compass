#include <FastLED.h>
#include <QMC5883LCompass.h>

const int NUM_LEDS = 24;
const int DATA_PIN = 6;
const int DELAY_MS = 50;

CRGB ring[NUM_LEDS];

QMC5883LCompass compass;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(ring, NUM_LEDS);
  FastLED.setBrightness(32);
  FastLED.clear();

  calibrate();
}

void calibrate() {
  int x0 = 0, x1 = 0, y0 = 0, y1 = 0, z0 = 0, z1 = 0;
  int i0 = 0, i1 = 1, i2 = 2, i3 = 3;
  int x, y, z;
  bool changed = false;
  int t = 0;
  int c = 0;

  compass.init();

  while (1) {
    // Nice ring chase sequence while we calibrate.
    FastLED.clear();
    ring[i0] = CRGB::White;
    ring[i1] = CRGB::White;
    ring[i2] = CRGB::White;
    ring[i3] = CRGB::Red;
    FastLED.show();
    i0 = (i0 + 1) % NUM_LEDS;
    i1 = (i1 + 1) % NUM_LEDS;
    i2 = (i2 + 1) % NUM_LEDS;
    i3 = (i3 + 1) % NUM_LEDS;
    delay(40);

    compass.read();
    x = compass.getX();
    y = compass.getY();
    z = compass.getZ();

    changed = false;
    if (x < x0) {
      x0 = x;
      changed = true;
    }
    if (x > x1) {
      x1 = x;
      changed = true;
    }
    if (y < y0) {
      y0 = y;
      changed = true;
    }
    if (y > y1) {
      y1 = y;
      changed = true;
    }
    if (z < z0) {
      z0 = z;
      changed = true;
    }
    if (z > z1) {
      z1 = z;
      changed = true;
    }

    if (changed) {
      c = millis();
    }

    t = millis();

    if (t - c > 5000) {
      FastLED.clear();
      break;
    }
  }

  compass.setCalibration(x0, x1, y0, y1, z0, z1);
}

void draw_compass(int north_led_idx) {
  int idx_n = north_led_idx;
  int idx_e = (idx_n + NUM_LEDS*1/4) % NUM_LEDS;
  int idx_s = (idx_n + NUM_LEDS*2/4) % NUM_LEDS;
  int idx_w = (idx_n + NUM_LEDS*3/4) % NUM_LEDS;

  ring[idx_n] = CRGB::Red;
  ring[idx_e] = CRGB::White;
  ring[idx_s] = CRGB::White;
  ring[idx_w] = CRGB::White;

  FastLED.show();

  ring[idx_n] = CRGB::Black;
  ring[idx_e] = CRGB::Black;
  ring[idx_s] = CRGB::Black;
  ring[idx_w] = CRGB::Black;
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
