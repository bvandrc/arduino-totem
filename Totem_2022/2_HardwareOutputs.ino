#include <Adafruit_NeoPixel.h>  // 1.8.5

#include "1_HardwareInputs.h"
#include "Totem_2022.h"

// Pins
const uint8_t STRIP_PIN = 5;

// Other consts
const uint8_t NUM_PIXELS = 156;

// LED strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

void initStrip() {
  strip.begin();             // initialize
  strip.setBrightness(150);  // max brightness of strip (actual max is 255)
  strip.show();

  /*  72 pixels (2021 totem):
   *  Brightness val |    R   |    G   |    B   |    W    |  Rainbow  <- Amps
                100  |  0.45  |  0.45  |  0.45  |  1.04   |    0.55
                150  |  0.62  |  0.60  |  0.60  |  1.54   |    0.75
                200  |  0.78  |  0.77  |  0.77  |  1.88   |    0.95
                220  |  0.85  |  0.84  |  0.84  |  1.99   |    1.03   <- 220 is old totem val
  */
  /*  156 pixels (2022 totem):
   *  Brightness val |    R   |    G   |    B   |    W    |  Rainbow  <- Amps
                50   |  0.51  |  0.51  |  0.51  |  1.22   |    X
                75   |  0.69  |  0.69  |  0.69  |  X      |    X
                100  |  0.88  |  0.88  |  0.88  |  X      |    1.07
                120  |  1.02  |  X     |  X     |  X      |    1.22   <- 220 is old totem val
                150  |  X     |  X     |  X     |  X      |    1.54   <- 220 is old totem val
  */
}

// Set pixel color (taking brightness into account) from 'packed' 32-bit RGB color:
void stripSetPixelColor(uint16_t n, uint32_t c) {
  uint32_t newC = getColorWithBrightness(c);
  strip.setPixelColor(n, newC);
}

void stripFill(uint32_t c = 0, int first = 0, uint16_t count = 0) {
  uint32_t newC = getColorWithBrightness(c);
  if (first < 0) {
    strip.fill(newC, 0, count + first);
  } else {
    strip.fill(newC, (uint16_t)first, count);
  }
}

// https://forums.adafruit.com/viewtopic.php?t=41143
uint32_t getColorWithBrightness(uint32_t c) {
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >> 8);
  uint8_t b = (uint8_t)(c);

  uint8_t newR = (r * brightness / MAX_BRIGHTNESS);
  uint8_t newG = (g * brightness / MAX_BRIGHTNESS);
  uint8_t newB = (b * brightness / MAX_BRIGHTNESS);

  return strip.Color(newR, newG, newB);
}

uint32_t setColorBrightness(uint32_t c, uint16_t br) {
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >> 8);
  uint8_t b = (uint8_t)(c);

  uint8_t newR = (r * br / MAX_BRIGHTNESS);
  uint8_t newG = (g * br / MAX_BRIGHTNESS);
  uint8_t newB = (b * br / MAX_BRIGHTNESS);

  return strip.Color(newR, newG, newB);
}
