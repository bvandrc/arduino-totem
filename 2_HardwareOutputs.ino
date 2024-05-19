#include <FastLED.h>

#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "Totem.h"

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

CRGB leds[NUM_PIXELS];

void initStrip() {
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

  FastLED.addLeds<WS2812, STRIP_PIN, GRB>(leds, NUM_PIXELS);
  // FastLED.setCorrection(TypicalLEDStrip);
  FastLED.clear();
  FastLED.setDither(BINARY_DITHER);
  FastLED.setMaxRefreshRate(200);
}

void setPixelColor(uint16_t index, CRGB color) {
  leds[index] = color;
}

void fillStrip(int16_t first, uint16_t count, CRGB color) {
  if (first >= FastLED.size()) {
    return;
  } else if (first < 0) {
    fill_solid(leds, count + first, color);
  } else {
    fill_solid(leds + (uint16_t)first, count, color);
  }
}

void fillStrip(CRGB color) {
  fill_solid(leds, FastLED.size(), color);
}

void showStrip() {
  // always turn off middle outside of desired range
  for (uint8_t i = 0; i < NUM_IN_MIDDLE; i++) {
    if (i < MIDDLE_CROP_SIDE_1_START || i > MIDDLE_CROP_SIDE_1_END) {
      setPixelColorMiddle(1, i, 0);
    }
    if (i < MIDDLE_CROP_SIDE_2_START || i > MIDDLE_CROP_SIDE_2_END) {
      setPixelColorMiddle(2, i, 0);
    }
  }

  FastLED.show();
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

uint8_t sideIndexToActualIndex(uint8_t side, uint8_t index) {
  uint8_t actual_index;
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
  return actual_index;
}

void setPixelColorSide(uint8_t side, uint8_t index, CRGB color) {
  const uint8_t actual_index = sideIndexToActualIndex(side, index);
  setPixelColor(actual_index, color);
}

uint8_t edgeIndexToSideIndex(uint8_t index) {
  if (index >= NUM_AROUND_EDGE) {
    return;
  }

  const uint8_t side_index = index + SIDE1_LEFT_START;
  return side_index;
}

uint8_t edgeIndexToActualIndex(uint8_t side, uint8_t index) {
  const uint8_t side_index = edgeIndexToSideIndex(index);
  const uint8_t actual_index = sideIndexToActualIndex(side, side_index);
  return actual_index;
}

void setPixelColorEdge(uint8_t side, uint8_t index, CRGB color) {
  const uint8_t actual_index = edgeIndexToSideIndex(index);
  setPixelColorSide(side, actual_index, color);
}

void setPixelColorMiddle(uint8_t side, uint8_t index, CRGB color) {
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

void setPixelColorMiddleCropped(uint8_t side, uint8_t index, CRGB color) {
  if (index >= (side == 1 ? NUM_USED_IN_MIDDLE_SIDE_1  //
                          : NUM_USED_IN_MIDDLE_SIDE_2)) {
    return;
  }

  uint8_t actual_index = index + (side == 1 ? MIDDLE_CROP_SIDE_1_START  //
                                            : MIDDLE_CROP_SIDE_2_START);
  setPixelColorMiddle(side, actual_index, color);
}

uint8_t quadrantToSide(uint8_t quadrant) {
  const uint8_t side = quadrant <= 2 ? 1 : 2;
  return side;
}

boolean quadrantIsLeftSide(uint8_t quadrant) {
  return quadrant == 1 || quadrant == 4;
}

uint8_t quadrantIndexToEdgeIndex(uint8_t quadrant, uint8_t index) {
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

  const boolean left_q = quadrantIsLeftSide(quadrant);
  const uint8_t Q_ACTUAL_BOTTOM_START = left_q ? Q1_ACTUAL_BOTTOM_START : Q2_ACTUAL_BOTTOM_START;
  const uint8_t Q_ACTUAL_BOTTOM_END = left_q ? Q1_ACTUAL_BOTTOM_END : Q2_ACTUAL_BOTTOM_END;
  const uint8_t Q_ACTUAL_SIDE_START = left_q ? Q1_ACTUAL_SIDE_START : Q2_ACTUAL_SIDE_START;
  const uint8_t Q_ACTUAL_SIDE_END = left_q ? Q1_ACTUAL_SIDE_END : Q2_ACTUAL_SIDE_END;
  const uint8_t Q_ACTUAL_TOP_START = left_q ? Q1_ACTUAL_TOP_START : Q2_ACTUAL_TOP_START;
  const uint8_t Q_ACTUAL_TOP_END = left_q ? Q1_ACTUAL_TOP_END : Q2_ACTUAL_TOP_END;

  uint8_t edge_index;
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
  return edge_index;
}

uint8_t quadrantIndexToActualIndex(uint8_t quadrant, uint8_t index) {
  const uint8_t side = quadrantToSide(quadrant);
  const uint8_t edge_index = quadrantIndexToEdgeIndex(quadrant, index);
  const uint8_t actual_index = edgeIndexToActualIndex(side, edge_index);
  return actual_index;
}

void setPixelColorQuadrant(uint8_t quadrant, uint8_t index, CRGB color) {
  const uint8_t side = quadrantToSide(quadrant);
  const uint8_t edge_index = quadrantIndexToEdgeIndex(quadrant, index);
  setPixelColorEdge(side, edge_index, color);
}

void setPixelColorAllQuadrants(uint8_t index, CRGB color) {
  for (uint8_t quadrant = 1; quadrant <= 4; quadrant++) {
    setPixelColorQuadrant(quadrant, index, color);
  }
}
