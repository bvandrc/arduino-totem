#include <Adafruit_NeoPixel.h>  // 1.8.5

#include "1_HardwareInputs.h"
#include "Totem_2022.h"

// Pins
const uint8_t STRIP_PIN = 5;

// Other consts
const uint8_t NUM_PIXELS = 205;

// LED strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

/*
Side 1 ("Front" - side with stems)
Middle | 2 - 26
Left   | 27 - 36
Top    | 37 - 64
Right  | 65 - 74
Bottom | 75 - 102

"Left" inside lights  - 1, 103
"Right" inside lights - 104

Side 2 ("Back" - side with yellow flowers)
Middle | 105 - 129
Left   | 130 - 139
Top    | 140 - 167
Right  | 168 - 177
Bottom | 178 - 205
*/
void initStrip() {
  strip.begin();            // initialize
  strip.setBrightness(20);  // max brightness of strip (actual max is 255)
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

// https://forums.adafruit.com/viewtopic.php?t=41143
uint32_t getColorBrightnessAdjusted(uint32_t color, uint8_t this_brightness) {
  uint8_t r = (uint8_t)(color >> 16);
  uint8_t g = (uint8_t)(color >> 8);
  uint8_t b = (uint8_t)(color);

  uint8_t newR = (r * this_brightness / MAX_BRIGHTNESS);
  uint8_t newG = (g * this_brightness / MAX_BRIGHTNESS);
  uint8_t newB = (b * this_brightness / MAX_BRIGHTNESS);

  return strip.Color(newR, newG, newB);
}

// Set pixel color (taking brightness into account) from 'packed' 32-bit RGB color:
void stripSetPixelColor(uint16_t index, uint32_t color) {
  uint32_t newColor = getColorBrightnessAdjusted(color, brightness);
  strip.setPixelColor(index, newColor);
}

void stripFill(uint32_t color, int first, uint16_t count) {
  uint32_t newColor = getColorBrightnessAdjusted(color, brightness);
  if (first < 0) {
    strip.fill(newColor, 0, count + first);
  } else {
    strip.fill(newColor, (uint16_t)first, count);
  }
}
