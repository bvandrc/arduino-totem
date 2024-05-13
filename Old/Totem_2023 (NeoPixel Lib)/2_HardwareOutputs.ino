#include <Adafruit_NeoPixel.h>  // 1.8.5

#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "Totem_2023.h"

// Pins
const uint8_t STRIP_PIN = 5;

// Other consts
const uint8_t NUM_PIXELS = 202;
const uint8_t NUM_HORIZONTAL_EDGE = 28;
const uint8_t NUM_VERTICAL_EDGE = 10;
const uint8_t NUM_IN_MIDDLE = 25;
const uint8_t NUM_AROUND_EDGE = (NUM_HORIZONTAL_EDGE * 2) + (NUM_VERTICAL_EDGE * 2);
const uint8_t NUM_IN_QUADRANT = NUM_AROUND_EDGE / 2;

const uint8_t MIDDLE_CROP_SIDE_1_START = 3;
const uint8_t MIDDLE_CROP_SIDE_1_END = 22;
const uint8_t MIDDLE_CROP_SIDE_2_START = 6;
const uint8_t MIDDLE_CROP_SIDE_2_END = 19;
const uint8_t NUM_USED_IN_MIDDLE_SIDE_1 = MIDDLE_CROP_SIDE_1_END - MIDDLE_CROP_SIDE_1_START;
const uint8_t NUM_USED_IN_MIDDLE_SIDE_2 = MIDDLE_CROP_SIDE_2_END - MIDDLE_CROP_SIDE_2_START;

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

  brightness = 1;
  speed = 10;
  begin();            // initialize
  setBrightness(20);  // max brightness of strip (actual max is 255)
  fill(0);
  show();
}

uint32_t MyNeoPixel::ColorHSV(uint16_t hue) {
  return gamma32(Adafruit_NeoPixel::ColorHSV(hue));
}

void MyNeoPixel::setPixelColor(uint16_t index, uint32_t color) {
  uint32_t new_color = getColorBrightnessAdjusted(color, brightness);
  Adafruit_NeoPixel::setPixelColor(index, new_color);
}

void MyNeoPixel::fill(uint32_t color = 0, int first = 0, uint16_t count = 0) {
  uint32_t new_color = getColorBrightnessAdjusted(color, brightness);
  if (first > NUM_PIXELS) {
    return;
  } else if (first < 0) {
    Adafruit_NeoPixel::fill(new_color, 0, count + first);
  } else {
    Adafruit_NeoPixel::fill(new_color, (uint16_t)first, count);
  }
}

void MyNeoPixel::show() {
  // always turn off middle outside of desired range
  for (uint8_t i = 0; i < NUM_IN_MIDDLE; i++) {
    if (i < MIDDLE_CROP_SIDE_1_START || i > MIDDLE_CROP_SIDE_1_END) {
      setPixelColorMiddle(1, i, 0);
    }
    if (i < MIDDLE_CROP_SIDE_2_START || i > MIDDLE_CROP_SIDE_2_END) {
      setPixelColorMiddle(2, i, 0);
    }
  }
  Adafruit_NeoPixel::show();
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

const uint8_t SIDE1_MIDDLE_START = 0;
const uint8_t SIDE1_MIDDLE_END = SIDE1_MIDDLE_START + NUM_IN_MIDDLE - 1;
const uint8_t SIDE1_LEFT_START = SIDE1_MIDDLE_END + 1;
const uint8_t SIDE1_LEFT_END = SIDE1_LEFT_START + NUM_VERTICAL_EDGE - 1;
const uint8_t SIDE1_TOP_START = SIDE1_LEFT_END + 1;
const uint8_t SIDE1_TOP_END = SIDE1_TOP_START + NUM_HORIZONTAL_EDGE - 1;
const uint8_t SIDE1_RIGHT_START = SIDE1_TOP_END + 1;
const uint8_t SIDE1_RIGHT_END = SIDE1_RIGHT_START + NUM_VERTICAL_EDGE - 1;
const uint8_t SIDE1_BOTTOM_START = SIDE1_RIGHT_END + 1;
const uint8_t SIDE1_BOTTOM_END = SIDE1_BOTTOM_START + NUM_HORIZONTAL_EDGE - 1;
const uint8_t SIDE2_MIDDLE_START = SIDE1_BOTTOM_END + 1;
const uint8_t SIDE2_MIDDLE_END = SIDE2_MIDDLE_START + NUM_IN_MIDDLE - 1;
const uint8_t SIDE2_LEFT_START = SIDE2_MIDDLE_END + 1;
const uint8_t SIDE2_LEFT_END = SIDE2_LEFT_START + NUM_VERTICAL_EDGE - 1;
const uint8_t SIDE2_TOP_START = SIDE2_LEFT_END + 1;
const uint8_t SIDE2_TOP_END = SIDE2_TOP_START + NUM_HORIZONTAL_EDGE - 1;
const uint8_t SIDE2_RIGHT_START = SIDE2_TOP_END + 1;
const uint8_t SIDE2_RIGHT_END = SIDE2_RIGHT_START + NUM_VERTICAL_EDGE - 1;
const uint8_t SIDE2_BOTTOM_START = SIDE2_RIGHT_END + 1;
const uint8_t SIDE2_BOTTOM_END = SIDE2_BOTTOM_START + NUM_HORIZONTAL_EDGE - 1;

void MyNeoPixel::setPixelColorSide(uint8_t side, uint8_t index, uint32_t color) {
  uint16_t actual_index;
  if (side == 1) {
    actual_index = index;
  } else {
    if (SIDE1_MIDDLE_START <= index && index <= SIDE1_MIDDLE_END) {
      actual_index = map(index,
                         SIDE1_MIDDLE_START,  //
                         SIDE1_MIDDLE_END,    //
                         SIDE2_MIDDLE_END,    //
                         SIDE2_MIDDLE_START);
    } else if (SIDE1_LEFT_START <= index && index <= SIDE1_LEFT_END) {
      actual_index = map(index,
                         SIDE1_LEFT_START,  //
                         SIDE1_LEFT_END,    //
                         SIDE2_RIGHT_END,   //
                         SIDE2_RIGHT_START);
    } else if (SIDE1_TOP_START <= index && index <= SIDE1_TOP_END) {
      actual_index = map(index,
                         SIDE1_TOP_START,  //
                         SIDE1_TOP_END,    //
                         SIDE2_TOP_END,    //
                         SIDE2_TOP_START);
    } else if (SIDE1_RIGHT_START <= index && index <= SIDE1_RIGHT_END) {
      actual_index = map(index,
                         SIDE1_RIGHT_START,  //
                         SIDE1_RIGHT_END,    //
                         SIDE2_LEFT_END,     //
                         SIDE2_LEFT_START);
    } else if (SIDE1_BOTTOM_START <= index && index <= SIDE1_BOTTOM_END) {
      actual_index = map(index,
                         SIDE1_BOTTOM_START,  //
                         SIDE1_BOTTOM_END,    //
                         SIDE2_BOTTOM_END,    //
                         SIDE2_BOTTOM_START);
    }
  }
  setPixelColor(actual_index, color);
}

void MyNeoPixel::setPixelColorEdge(uint8_t side, uint8_t index, uint32_t color) {
  if (index >= NUM_AROUND_EDGE) {
    return;
  }
  uint8_t actual_index = index + SIDE1_LEFT_START;
  setPixelColorSide(side, actual_index, color);
}

void MyNeoPixel::setPixelColorMiddle(uint8_t side, uint8_t index, uint32_t color) {
  if (index >= NUM_IN_MIDDLE) {
    return;
  }

  uint8_t actual_index;
  if (side == 1) {
    actual_index = index;
  } else {
    // setPixelColorSide sets mirror of side, but for the middle we want to go left to right on either side
    actual_index = map(index, 0, NUM_IN_MIDDLE - 1, NUM_IN_MIDDLE - 1, 0);
  }

  setPixelColorSide(side, actual_index, color);
}

void MyNeoPixel::setPixelColorMiddleCropped(uint8_t side, uint8_t index, uint32_t color) {
  if (index >= (side == 1 ? NUM_USED_IN_MIDDLE_SIDE_1  //
                          : NUM_USED_IN_MIDDLE_SIDE_2)) {
    return;
  }

  uint8_t actual_index = index + (side == 1 ? MIDDLE_CROP_SIDE_1_START  //
                                            : MIDDLE_CROP_SIDE_2_START);
  setPixelColorMiddle(side, actual_index, color);
}

void MyNeoPixel::setPixelColorQuadrant(uint8_t quadrant, uint8_t index, uint32_t color) {
  if (index >= NUM_IN_QUADRANT) {
    return;
  }

  static const uint8_t Q_SIDE_LENGTH = (NUM_HORIZONTAL_EDGE) / 2;

  static const uint8_t Q_INDEX_BOTTOM_START = 0;
  static const uint8_t Q_INDEX_BOTTOM_END = Q_SIDE_LENGTH - 1;
  static const uint8_t Q_INDEX_SIDE_START = Q_INDEX_BOTTOM_END + 1;
  static const uint8_t Q_INDEX_SIDE_END = Q_INDEX_SIDE_START + NUM_VERTICAL_EDGE - 1;
  static const uint8_t Q_INDEX_TOP_START = Q_INDEX_SIDE_END + 1;
  static const uint8_t Q_INDEX_TOP_END = Q_INDEX_TOP_START + Q_SIDE_LENGTH - 1;

  static const uint8_t Q1_ACTUAL_BOTTOM_START = NUM_AROUND_EDGE - Q_SIDE_LENGTH;
  static const uint8_t Q1_ACTUAL_BOTTOM_END = Q1_ACTUAL_BOTTOM_START + Q_SIDE_LENGTH - 1;
  static const uint8_t Q1_ACTUAL_SIDE_START = 0;
  static const uint8_t Q1_ACTUAL_SIDE_END = Q1_ACTUAL_SIDE_START + NUM_VERTICAL_EDGE - 1;
  static const uint8_t Q1_ACTUAL_TOP_START = Q1_ACTUAL_SIDE_END + 1;
  static const uint8_t Q1_ACTUAL_TOP_END = Q1_ACTUAL_TOP_START + Q_SIDE_LENGTH - 1;

  static const uint8_t Q2_ACTUAL_BOTTOM_START = Q1_ACTUAL_BOTTOM_START - 1;
  static const uint8_t Q2_ACTUAL_BOTTOM_END = Q2_ACTUAL_BOTTOM_START - Q_SIDE_LENGTH + 1;
  static const uint8_t Q2_ACTUAL_SIDE_START = Q2_ACTUAL_BOTTOM_END - 1;
  static const uint8_t Q2_ACTUAL_SIDE_END = Q2_ACTUAL_SIDE_START - NUM_VERTICAL_EDGE + 1;
  static const uint8_t Q2_ACTUAL_TOP_START = Q2_ACTUAL_SIDE_END - 1;
  static const uint8_t Q2_ACTUAL_TOP_END = Q2_ACTUAL_TOP_START - Q_SIDE_LENGTH + 1;

  const boolean left_q = quadrant == 1 || quadrant == 4;
  const uint8_t Q_ACTUAL_BOTTOM_START = left_q ? Q1_ACTUAL_BOTTOM_START : Q2_ACTUAL_BOTTOM_START;
  const uint8_t Q_ACTUAL_BOTTOM_END = left_q ? Q1_ACTUAL_BOTTOM_END : Q2_ACTUAL_BOTTOM_END;
  const uint8_t Q_ACTUAL_SIDE_START = left_q ? Q1_ACTUAL_SIDE_START : Q2_ACTUAL_SIDE_START;
  const uint8_t Q_ACTUAL_SIDE_END = left_q ? Q1_ACTUAL_SIDE_END : Q2_ACTUAL_SIDE_END;
  const uint8_t Q_ACTUAL_TOP_START = left_q ? Q1_ACTUAL_TOP_START : Q2_ACTUAL_TOP_START;
  const uint8_t Q_ACTUAL_TOP_END = left_q ? Q1_ACTUAL_TOP_END : Q2_ACTUAL_TOP_END;

  uint16_t edge_index;
  if (Q_INDEX_BOTTOM_START <= index && index <= Q_INDEX_BOTTOM_END) {
    edge_index = map(index,                  //
                     Q_INDEX_BOTTOM_START,   //
                     Q_INDEX_BOTTOM_END,     //
                     Q_ACTUAL_BOTTOM_START,  //
                     Q_ACTUAL_BOTTOM_END);
  } else if (Q_INDEX_SIDE_START <= index && index <= Q_INDEX_SIDE_END) {
    edge_index = map(index,                //
                     Q_INDEX_SIDE_START,   //
                     Q_INDEX_SIDE_END,     //
                     Q_ACTUAL_SIDE_START,  //
                     Q_ACTUAL_SIDE_END);
  } else if (Q_INDEX_TOP_START <= index && index <= Q_INDEX_TOP_END) {
    edge_index = map(index,               //
                     Q_INDEX_TOP_START,   //
                     Q_INDEX_TOP_END,     //
                     Q_ACTUAL_TOP_START,  //
                     Q_ACTUAL_TOP_END);
  }

  setPixelColorEdge(quadrant == 1 || quadrant == 2 ? 1 : 2, edge_index, color);
}

void MyNeoPixel::setPixelColorAllQuadrants(uint8_t index, uint32_t color) {
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
