#ifndef HARDWARE_OUTPUTS_H
#define HARDWARE_OUTPUTS_H

#include <FastLED.h>
extern const uint8_t NUM_AROUND_EDGE;
extern const uint8_t NUM_IN_QUADRANT;
extern const uint8_t NUM_IN_MIDDLE;
extern const uint8_t NUM_USED_IN_MIDDLE_SIDE_1;
extern const uint8_t NUM_USED_IN_MIDDLE_SIDE_2;

extern void initStrip();
extern void showStrip();
extern void setPixelColor(uint16_t index, CRGB color);
extern void fillStrip(int first, uint16_t count, CRGB color);
extern void fillStrip(CRGB color);
extern void setPixelColorEdge(uint8_t side, uint8_t index, CRGB color);
extern void setPixelColorMiddleCropped(uint8_t side, uint8_t index, CRGB color);
extern void setPixelColorQuadrant(uint8_t quadrant, uint8_t index, CRGB color);
extern void setPixelColorAllQuadrants(uint8_t index, CRGB color);

#endif