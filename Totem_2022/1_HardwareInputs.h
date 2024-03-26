#ifndef HARDWARE_INPUTS_H
#define HARDWARE_INPUTS_H

extern const uint16_t DIAL_MIN;
extern const uint16_t DIAL_MAX;
extern const uint8_t MAX_BRIGHTNESS;

extern void setUpPins();
extern void initMotionSensor();
extern bool checkTapped();

extern bool rageButtonPushed();
extern uint8_t getModeDialPosition();
extern uint8_t getColorDialPosition();
extern uint8_t getSelectorPosition(uint8_t pin);
extern void getBrightnessDial();
extern void getSpeedDial();

#endif