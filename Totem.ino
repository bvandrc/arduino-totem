#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "4_ColorModes.h"
#include "5_Debug.h"
#include "Totem.h"

// TODO: any locks (ie brightness lock?)

void setup() {
  // Serial.begin(9600);
  setUpPins();
  motionSensor.init();
  initStrip();
}

void loop() {
  getBrightnessDial();
  getSpeedDial();
  checkSelectorDialsChanged();
  doTheLightingMode();
}

bool checkRageFlash() {
  static uint8_t last_pressed_mode = 0;
  static uint8_t count = 0;

  if (rageButtonPushed()) {
    rageFlash();

    if ((last_pressed_mode == 6 && top_dial_position == 1) || (last_pressed_mode == 1 && top_dial_position == 6)) {
      count++;
    } else {
      count = 0;
    }

    if (count == 5) {
      debugToggleCurrentSetting();
      count = 0;
    }

    last_pressed_mode = top_dial_position;

    return true;
  } else {
    return false;
  }
}

bool checkTapFlash() {
  if (motionSensor.tap_enabled && motionSensor.checkTapped()) {
    tapFlash();
    return true;
  } else {
    return false;
  }
}

void doTheLightingMode() {
  switch (top_dial_position) {
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
      if (bottom_dial_position < 3) {
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
