#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "4_ColorModes.h"
#include "5_Debug.h"

// Global variable inits
uint8_t effectMode = 1;     // Chase, Fade
uint8_t colorScheme = 1;    // Rainbow, Pairs
uint8_t brightness = 1;     // 0 to 255
uint16_t effectSpeed = 10;  // 0 to 1024
bool tapEnabled = true;

// TODO: any locks (ie brightness lock?)

void setup() {
  //  Serial.begin(9600);
  setUpPins();
  initMotionSensor();
  initStrip();
}

void loop() {
  getBrightnessDial();
  getSpeedDial();
  checkModeChange();
  doTheLightingMode();
}

bool checkModeChange() {
  uint8_t newMode = getModeDialPosition();
  uint8_t newColorScheme = getColorDialPosition();
  if (newMode != effectMode || newColorScheme != colorScheme) {
    effectMode = newMode;
    colorScheme = newColorScheme;
    return true;
  } else {
    return false;
  }
}

bool checkRageFlash() {
  static uint8_t lastPressedMode = 0;
  static uint8_t cnt = 0;

  if (rageButtonPushed()) {
    rageFlash();

    if ((lastPressedMode == 6 && effectMode == 1) || (lastPressedMode == 1 && effectMode == 6)) {
      cnt++;
    } else {
      cnt = 0;
    }

    if (cnt == 5) {
      debugToggleCurrentSetting();
      cnt = 0;
    }

    lastPressedMode = effectMode;

    return true;
  } else {
    return false;
  }
}

bool checkTapFlash() {
  if (checkTapped()) {
    tapFlash();
    return true;
  } else {
    return false;
  }
}

void doTheLightingMode() {
  switch (effectMode) {
    case 1:
      rainbowChase();
      break;
    case 2:
      rainbowFade();
      break;
    case 3:
      theaterChaseCycle();
      break;
    case 4:
      rainbowTwinkle();
      break;
    case 5:
      if (colorScheme < 3) {
        fillUpQuadrantsCycle();
        break;
      } else {
        fillUpCycle();
        break;
      }
    case 6:
      setOneColorMode(colorScheme);
      break;
    default:
      rainbowChase();
      break;
  }
}
