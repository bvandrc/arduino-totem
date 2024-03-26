#ifndef HARDWARE_OUTPUTS_H
#define HARDWARE_OUTPUTS_H

#include <Adafruit_NeoPixel.h>

extern const uint8_t NUM_PIXELS;

extern Adafruit_NeoPixel strip;

extern void initStrip();

extern uint32_t getColorBrightnessAdjusted(uint32_t color, uint8_t this_brightness);

extern void stripSetPixelColor(uint16_t index, uint32_t color);
extern void stripFill(uint32_t color, int first, uint16_t count);

#endif