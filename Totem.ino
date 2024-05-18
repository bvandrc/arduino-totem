#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "4_ColorModes.h"
#include "5_Debug.h"
#include "Totem.h"

// TODO: any locks (ie brightness lock?)

boolean no_do_lighting_mode = false;

void setup() {
  // Serial.begin(9600);
  setUpPins();
  initStrip();

  // initial debug
  const unsigned long start_millis = millis();
  while (rageButtonPushed() && (millis() - start_millis < 10000)) {
    if (millis() - start_millis > 5000) {
      checkSelectorDialsChanged();

      // top dial = outputs
      if (top_dial_position == 2 || top_dial_position == 3) {
        no_do_lighting_mode = true;
        FastLED.setDither(DISABLE_DITHER);
        FastLED.setBrightness(2);
        if (top_dial_position == 2) {
          fillRainbowEdges(0);
          fillRainbowMiddles(0);
        } else if (top_dial_position == 3) {
          fillStrip(CRGB::Lime);
        }
        showStrip();
      }

      // botttom dial = outputs
      if (bottom_dial_position == 2) {
        motionSensor.tap_enabled = false;
      }

      break;
    }
  }

  if (motionSensor.tap_enabled) {
    motionSensor.init();
  }
}

void loop() {
  if (no_do_lighting_mode)
    return;
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
      debugMode();
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
      rainbowChaseEdges();
      break;
    case 2:
      rainbowChaseQuadrants();
      break;
    case 3:
      theaterChaseCycle();
      break;
    case 4:
      rainbowTwinkle();
      break;
    case 5:
      fillUpQuadrantsCycle();
      break;
    case 6:
      rainbowFade();
      break;
    default:
      rainbowChaseEdges();
      break;
  }
}
