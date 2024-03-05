#ifndef HARDWARE_INTERACTION_H
#define HARDWARE_INTERACTION_H

#include <Adafruit_NeoPixel.h>

extern const uint8_t NUM_PIXELS;
extern const uint16_t MAX_BRIGHTNESS;
extern const uint16_t DIAL_MIN;
extern const uint16_t DIAL_MAX;

extern Adafruit_NeoPixel strip;

extern void setUpPins();
extern void initMotionSensor();
extern bool checkTapped();
extern void initStrip();

// INPUTS
extern bool rageButtonPushed();
extern uint8_t getModeDialPosition();
extern uint8_t getColorDialPosition();
extern uint8_t getSelectorPosition(uint8_t pin);
extern void getBrightnessDial();
extern void getSpeedDial();

// OUTPUTS
extern void stripSetPixelColor(uint16_t n, uint32_t c);
extern void stripFill(uint32_t c, int first, uint16_t count);
extern uint32_t setColorBrightness(uint32_t c, uint16_t br);

#endif