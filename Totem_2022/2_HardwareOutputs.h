#ifndef HARDWARE_OUTPUTS_H
#define HARDWARE_OUTPUTS_H

#include <Adafruit_NeoPixel.h>

extern const uint8_t NUM_AROUND_EDGE;
extern const uint8_t NUM_IN_QUADRANT;

class MyNeoPixel : public Adafruit_NeoPixel {
  public:
  MyNeoPixel();

  // overrides
  void setPixelColor(uint16_t index, uint32_t color);
  void show();
  void fill(uint32_t color = 0, int first = 0, uint16_t count = 0);

  // new functions
  void init();
  uint32_t getColorBrightnessAdjusted(uint32_t color, uint8_t this_brightness);
  void setPixelColorBackside(uint16_t index, uint32_t color);
  void setPixelColorBothSides(uint16_t index, uint32_t color);
  void setPixelColorEdge(uint16_t index, uint32_t color);
  void setPixelColorQuadrant(uint8_t quadrant, uint16_t index, uint32_t color);
  void setPixelColorAllQuadrants(uint16_t index, uint32_t color);

  // constants
  static const uint32_t RED;
  static const uint32_t ORANGE;
  static const uint32_t YELLOW;
  static const uint32_t GREEN;
  static const uint32_t SEAFOAM;
  static const uint32_t BLUE;
  static const uint32_t PURPLE;

  static const uint32_t COLORS[];
  static const uint8_t LEN_COLORS;
};

extern MyNeoPixel strip;

#endif