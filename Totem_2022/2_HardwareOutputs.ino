#include <Adafruit_NeoPixel.h>  // 1.8.5

#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "Totem_2022.h"

// Pins
const uint8_t STRIP_PIN = 5;

// Other consts
const uint8_t NUM_PIXELS = 206;
const uint8_t NUM_AROUND_EDGE = 76;
const uint8_t NUM_IN_QUADRANT = 38;  // NUM_AROUND_EDGE / 2
const uint8_t NUM_IN_MIDDLE = 24;
const uint8_t NUM_USED_IN_MIDDLE_SIDE_1 = 19;
const uint8_t NUM_USED_IN_MIDDLE_SIDE_2 = 13;

/*
Side 1 ("Front" - side with stems)
Middle | 2 - 26
Left   | 27 - 36
Top    | 37 - 64
Right  | 65 - 74
Bottom | 75 - 102

"Left" inside lights  - 1, 103
"Right" inside lights - 0, 104

Side 2 ("Back" - side with yellow flowers)
Middle | 105 - 129
Left   | 130 - 139
Top    | 140 - 167
Right  | 168 - 177
Bottom | 178 - 205
*/

MyNeoPixel::MyNeoPixel() : Adafruit_NeoPixel(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800) {
}

void MyNeoPixel::init() {
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

  begin();            // initialize
  setBrightness(20);  // max brightness of strip (actual max is 255)
  fill(0);
  show();
}

uint32_t MyNeoPixel::ColorHSV(uint16_t hue) {
  return gamma32(Adafruit_NeoPixel::ColorHSV(hue));
}

void MyNeoPixel::setPixelColor(uint16_t index, uint32_t color) {
  uint32_t new_color = getColorBrightnessAdjusted(color, brightness_global);
  Adafruit_NeoPixel::setPixelColor(index, new_color);
}

void MyNeoPixel::show() {
  // middle (outside of desired sign range)
  fill(0, 2, 4);
  fill(0, 25, 2);
  fill(0, 105, 5);
  fill(0, 123, 7);

  // always turn off inside lights
  setPixelColor(0, 0);
  setPixelColor(1, 0);
  setPixelColor(103, 0);
  setPixelColor(104, 0);

  Adafruit_NeoPixel::show();
}

void MyNeoPixel::fill(uint32_t color = 0, int first = 0, uint16_t count = 0) {
  uint32_t new_color = getColorBrightnessAdjusted(color, brightness_global);
  if (first < 0) {
    Adafruit_NeoPixel::fill(new_color, 0, count + first);
  } else {
    Adafruit_NeoPixel::fill(new_color, (uint16_t)first, count);
  }
}

uint32_t MyNeoPixel::getColorBrightnessAdjusted(uint32_t color, uint8_t this_brightness) {
  // https://forums.adafruit.com/viewtopic.php?t=41143
  uint8_t r = (uint8_t)(color >> 16);
  uint8_t g = (uint8_t)(color >> 8);
  uint8_t b = (uint8_t)(color);

  uint8_t new_r = (r * this_brightness) >> 8;
  uint8_t new_g = (g * this_brightness) >> 8;
  uint8_t new_b = (b * this_brightness) >> 8;

  return Color(new_r, new_g, new_b);
}

void MyNeoPixel::setPixelColorSide(uint8_t side, uint16_t index, uint32_t color) {
  uint16_t actual_index;
  if (side == 1) {
    actual_index = index;
  } else {
    if (2 <= index && index <= 26) {
      // middle
      actual_index = map(index, 2, 26, 129, 105);
    } else if (27 <= index && index <= 36) {
      // left
      actual_index = map(index, 27, 36, 177, 168);
    } else if (37 <= index && index <= 64) {
      // top
      actual_index = map(index, 37, 64, 167, 140);
    } else if (65 <= index && index <= 74) {
      // right
      actual_index = map(index, 65, 74, 139, 130);
    } else if (75 <= index && index <= 102) {
      // bottom
      actual_index = map(index, 75, 102, 205, 178);
    }
  }
  // NOTE: does not account for inside lights (0, 1, 103, 104)
  setPixelColor(actual_index, color);
}

void MyNeoPixel::setPixelColorEdge(uint8_t side, uint16_t index, uint32_t color) {
  if (index >= NUM_AROUND_EDGE) {
    return;
  }
  uint16_t actual_index;

  if (0 <= index && index <= 9) {
    // left
    actual_index = map(index, 0, 9, 27, 36);
  } else if (10 <= index && index <= 37) {
    // top
    actual_index = map(index, 10, 37, 37, 64);
  } else if (38 <= index && index <= 47) {
    // right
    actual_index = map(index, 38, 47, 65, 74);
  } else if (48 <= index && index <= 75) {
    // bottom
    actual_index = map(index, 48, 75, 75, 102);
  }

  setPixelColorSide(side, actual_index, color);
}

void MyNeoPixel::setPixelColorMiddle(uint8_t side, uint16_t index, uint32_t color) {
  if (index >= NUM_IN_MIDDLE) {
    return;
  }

  uint16_t actual_index = map(index, 0, NUM_IN_MIDDLE - 1, 2, 26);
  setPixelColorSide(side, actual_index, color);
}

void MyNeoPixel::setPixelColorMiddleSide1(uint16_t index, uint32_t color) {
  if (index >= NUM_USED_IN_MIDDLE_SIDE_1) {
    return;
  }

  uint16_t actual_index = index + ((NUM_IN_MIDDLE - NUM_USED_IN_MIDDLE_SIDE_1) / 2) + 2;
  setPixelColorMiddle(1, actual_index, color);
}

void MyNeoPixel::setPixelColorMiddleSide2(uint16_t index, uint32_t color) {
  if (index >= NUM_USED_IN_MIDDLE_SIDE_2) {
    return;
  }

  uint16_t actual_index = index + ((NUM_IN_MIDDLE - NUM_USED_IN_MIDDLE_SIDE_2) / 2) + 2;
  setPixelColorMiddle(2, actual_index, color);
}

void MyNeoPixel::setPixelColorQuadrant(uint8_t quadrant, uint16_t index, uint32_t color) {
  if (index >= NUM_IN_QUADRANT) {
    return;
  }

  uint16_t actual_index;
  if (quadrant == 1 || quadrant == 4) {
    if (0 <= index && index <= 13) {
      // bottom
      actual_index = map(index, 0, 13, 89, 102);
    } else if (14 <= index && index <= 23) {
      // left
      actual_index = map(index, 14, 23, 27, 36);
    } else if (24 <= index && index <= 37) {
      // top
      actual_index = map(index, 24, 37, 37, 50);
    }
  } else if (quadrant == 2 || quadrant == 3) {
    if (0 <= index && index <= 13) {
      // bottom
      actual_index = map(index, 0, 13, 88, 75);
    } else if (14 <= index && index <= 23) {
      // right
      actual_index = map(index, 14, 23, 74, 65);
    } else if (24 <= index && index <= 37) {
      // top
      actual_index = map(index, 24, 37, 64, 51);
    }
  }

  setPixelColorSide(quadrant == 1 || quadrant == 2 ? 1 : 2, actual_index, color);
}

void MyNeoPixel::setPixelColorAllQuadrants(uint16_t index, uint32_t color) {
  if (index >= NUM_IN_QUADRANT) {
    return;
  }

  for (uint8_t quadrant = 1; quadrant <= 4; quadrant++) {
    setPixelColorQuadrant(quadrant, index, color);
  }
}

// colors - visually tested
const uint32_t MyNeoPixel::RED = Color(255, 0, 0);
const uint32_t MyNeoPixel::ORANGE = Color(255, 30, 0);
const uint32_t MyNeoPixel::YELLOW = Color(255, 255, 0);
const uint32_t MyNeoPixel::GREEN = Color(0, 255, 0);
const uint32_t MyNeoPixel::SEAFOAM = Color(0, 255, 130);
const uint32_t MyNeoPixel::BLUE = Color(0, 0, 255);
const uint32_t MyNeoPixel::PURPLE = Color(200, 0, 255);

const uint32_t MyNeoPixel::COLORS[] = {RED, ORANGE, YELLOW, GREEN, SEAFOAM, BLUE, PURPLE};
const uint8_t MyNeoPixel::LEN_COLORS = 7;  // better to hardcode array lengths

// LED strip
MyNeoPixel strip = MyNeoPixel();
