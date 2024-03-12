#ifndef HARDWARE_OUTPUTS_H
#define HARDWARE_OUTPUTS_H

#include <Adafruit_NeoPixel.h>

extern const uint8_t NUM_PIXELS;

extern Adafruit_NeoPixel strip;

extern void initStrip();

extern void stripSetPixelColor(uint16_t n, uint32_t c);
extern void stripFill(uint32_t c, int first, uint16_t count);
extern uint32_t setColorBrightness(uint32_t c, uint16_t br);

#endif