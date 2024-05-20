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

  bool tap_enabled;
};

extern MyMotionSensor motionSensor;

extern uint16_t speed;  // 0 to 1024

extern void setUpPins();
extern bool rageButtonPushed();
extern uint8_t getTopDialPosition();
extern uint8_t getBottomDialPosition();
extern bool checkModeDialChanged();
extern void getBrightnessDial();
extern void getSpeedDial();

#endif