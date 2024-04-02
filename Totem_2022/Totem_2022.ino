#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "4_ColorModes.h"
#include "5_Debug.h"
#include "Totem_2022.h"

// Global variable inits
uint8_t top_dial_global = 1;     // Chase, Fade
uint8_t bottom_dial_global = 1;  // Rainbow, Pairs
uint8_t brightness_global = 1;   // 0 to 255
uint16_t speed_global = 10;      // 0 to 1024
bool tap_enabled_global = true;

// TODO: any locks (ie brightness lock?)

void setup() {
  // Serial.begin(9600);
  setUpPins();
  motionSensor.init();
  strip.init();
}

void loop() {
  getBrightnessDial();
  getSpeedDial();
  checkModeChange();
  doTheLightingMode();
}

bool checkModeChange() {
  uint8_t new_mode = getModeDialPosition();
  uint8_t new_bottom_dial_global = getColorDialPosition();
  if (new_mode != top_dial_global || new_bottom_dial_global != bottom_dial_global) {
    top_dial_global = new_mode;
    bottom_dial_global = new_bottom_dial_global;
    return true;
  } else {
    return false;
  }
}

bool checkRageFlash() {
  static uint8_t last_pressed_mode = 0;
  static uint8_t count = 0;

  if (rageButtonPushed()) {
    rageFlash();

    if ((last_pressed_mode == 6 && top_dial_global == 1) || (last_pressed_mode == 1 && top_dial_global == 6)) {
      count++;
    } else {
      count = 0;
    }

    if (count == 5) {
      debugToggleCurrentSetting();
      count = 0;
    }

    last_pressed_mode = top_dial_global;

    return true;
  } else {
    return false;
  }
}

bool checkTapFlash() {
  if (motionSensor.checkTapped()) {
    tapFlash();
    return true;
  } else {
    return false;
  }
}

void doTheLightingMode() {
  switch (top_dial_global) {
    case 1:
    case 2:
      rainbowChase();
      break;
      break;
    case 3:
      theaterChaseCycle();
      break;
    case 4:
      rainbowTwinkle();
      break;
    case 5:
      if (bottom_dial_global < 3) {
        fillUpQuadrantsCycle();
        break;
      } else {
        fillUpCycle();
        break;
      }
    case 6:
      rainbowFade();
      break;
    default:
      rainbowChase();
      break;
  }
}
