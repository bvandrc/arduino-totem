#ifndef HARDWARE_OUTPUTS_H
#define HARDWARE_OUTPUTS_H

#include <Adafruit_NeoPixel.h>

extern const uint8_t NUM_IN_QUADRANT;

extern Adafruit_NeoPixel strip;

extern void initStrip();

extern uint32_t getColorBrightnessAdjusted(uint32_t color, uint8_t this_brightness);

extern void stripSetPixelColor(uint16_t index, uint32_t color);
extern void stripSetPixelColorBackside(uint16_t index, uint32_t color);
extern void stripSetPixelColorBothSides(uint16_t index, uint32_t color);
extern void stripSetPixelColorBothSidesAsym(uint16_t index, uint32_t color);
extern void stripSetPixelColorQuadrant(uint8_t quadrant, uint16_t index, uint32_t color);
extern void stripSetPixelColorAllQuadrants(uint16_t index, uint32_t color);
extern void stripFillSegment(uint32_t color, int first, uint16_t count);

#endif