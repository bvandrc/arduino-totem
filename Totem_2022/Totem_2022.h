#ifndef MAIN_H
#define MAIN_H

extern uint8_t top_dial_global;     // Chase, Fade
extern uint8_t bottom_dial_global;  // Rainbow, Pairs
extern uint8_t brightness_global;   // 0 to 255 NOTE: do NOT name "brightness", conflicts with NeoPixel lib
extern uint16_t speed_global;       // 0 to 1024
extern bool tap_enabled_global;

extern bool checkModeChange();
extern bool checkRageFlash();
extern bool checkTapFlash();

#endif