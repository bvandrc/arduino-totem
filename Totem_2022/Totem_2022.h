#ifndef MAIN_H
#define MAIN_H

extern uint16_t effectMode;   // Chase, Fade
extern uint16_t colorScheme;  // Rainbow, Pairs
extern uint16_t brightness;   // 0 to 255
extern uint16_t effectSpeed;  // 0 to 1024

extern bool tapEnabled;

extern bool checkModeChange();
extern bool checkRageFlash();
extern bool checkTapFlash();

#endif