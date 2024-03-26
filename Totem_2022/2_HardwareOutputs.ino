#include <Adafruit_NeoPixel.h>  // 1.8.5

#include "1_HardwareInputs.h"
#include "Totem_2022.h"

// Pins
const uint8_t STRIP_PIN = 5;

// Other consts
const uint8_t NUM_PIXELS = 206;
const uint8_t NUM_IN_QUADRANT = 38;

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

void stripSetPixelColorBackside(uint16_t index, uint32_t color) {
  uint16_t otherSide;
  if (2 <= index && index <= 26) {
    // middle
    otherSide = map(index, 2, 26, 129, 105);
  } else if (27 <= index && index <= 36) {
    // left
    otherSide = map(index, 27, 36, 177, 168);
  } else if (37 <= index && index <= 64) {
    // top
    otherSide = map(index, 37, 64, 167, 140);
  } else if (65 <= index && index <= 74) {
    // right
    otherSide = map(index, 65, 74, 139, 130);
  } else if (75 <= index && index <= 102) {
    // bottom
    otherSide = map(index, 75, 102, 205, 178);
  }
  // TODO: inside lights (1,103, 104)
  stripSetPixelColor(otherSide, color);
}

void stripSetPixelColorBothSides(uint16_t index, uint32_t color) {
  stripSetPixelColor(index, color);
  stripSetPixelColorBackside(index, color);
}

void stripSetPixelColorBothSidesAsym(uint16_t index, uint32_t color) {
  stripSetPixelColor(index, color);
  stripSetPixelColor(index + (NUM_PIXELS / 2), color);
}

void stripSetPixelColorQuadrant(uint8_t quadrant, uint16_t index, uint32_t color) {
  uint16_t actualIndex;
  if (quadrant == 1 || quadrant == 4) {
    if (1 <= index && index <= 14) {
      // bottom
      actualIndex = map(index, 1, 14, 89, 102);
    } else if (15 <= index && index <= 24) {
      // left
      actualIndex = map(index, 15, 24, 27, 36);
    } else if (25 <= index && index <= NUM_IN_QUADRANT) {
      // top
      actualIndex = map(index, 25, NUM_IN_QUADRANT, 37, 50);
    }
  } else if (quadrant == 2 || quadrant == 3) {
    if (1 <= index && index <= 14) {
      // bottom
      actualIndex = map(index, 1, 14, 88, 75);
    } else if (15 <= index && index <= 24) {
      // right
      actualIndex = map(index, 15, 24, 74, 65);
    } else if (25 <= index && index <= NUM_IN_QUADRANT) {
      // top
      actualIndex = map(index, 25, NUM_IN_QUADRANT, 64, 51);
    }
  }

  if (quadrant == 1 || quadrant == 2) {
    stripSetPixelColor(actualIndex, color);
  } else {
    stripSetPixelColorBackside(actualIndex, color);
  }
}

void stripSetPixelColorAllQuadrants(uint16_t index, uint32_t color) {
  for (uint8_t i = 1; i <= 4; i++) {
    stripSetPixelColorQuadrant(i, index, color);
  }
}

void stripFillSegment(uint32_t color, int first, uint16_t count) {
  uint32_t newColor = getColorBrightnessAdjusted(color, brightness);
  if (first < 0) {
    strip.fill(newColor, 0, count + first);
  } else {
    strip.fill(newColor, (uint16_t)first, count);
  }
}
