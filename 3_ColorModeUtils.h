#ifndef COLOR_MODE_UTILS_H
#define COLOR_MODE_UTILS_H

#include <FastLED.h>

enum WaitReturnCode {
  NO_CHANGE,
  MODE_CHANGED,
};

extern WaitReturnCode wait(uint32_t low_millis, uint32_t high_millis);

extern uint32_t gamma32(uint32_t color);
extern CRGB gamma32(CRGB color);
extern CRGB ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255);
extern void getNewColor(CRGB &color);
extern void getNewColors(CRGB *colors, uint8_t len_colors);
extern void getNewPixel(uint8_t &pixel);
extern CRGB getRandomColor();

#endif