#ifndef MAIN_H
#define MAIN_H

enum WaitReturnCode : uint8_t {
  NO_CHANGE,
  MODE_CHANGED,
  FLASHED_OR_TAPPED,
};

extern WaitReturnCode readAllInputs();
extern uint8_t num_times_rage_pushed;

#endif