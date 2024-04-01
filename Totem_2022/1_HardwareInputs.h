#ifndef HARDWARE_INPUTS_H
#define HARDWARE_INPUTS_H

#include <Adafruit_LIS3DH.h>

extern const uint16_t DIAL_MIN;
extern const uint16_t DIAL_MAX;
extern const uint8_t MAX_BRIGHTNESS;

class MyMotionSensor : public Adafruit_LIS3DH {
  public:
  MyMotionSensor();

  void init();
  bool checkTapped();
};

extern MyMotionSensor motionSensor;

extern void setUpPins();
extern bool rageButtonPushed();
extern uint8_t getModeDialPosition();
extern uint8_t getColorDialPosition();
extern uint8_t getSelectorPosition(uint8_t pin);
extern void getBrightnessDial();
extern void getSpeedDial();

#endif