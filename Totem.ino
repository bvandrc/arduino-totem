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
  while (rageButtonPushed()) {
    if (millis() - start_millis > 5000) {
      static const uint8_t top_dial_position = getTopDialPosition();
      static const uint8_t bottom_dial_position = getBottomDialPosition();

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
  readAllInputs();
  doTheLightingMode();
}

void doTheLightingMode() {
  switch (getTopDialPosition()) {
    case 1:
      rainbowChaseEdges(1);
      break;
    case 2:
      rainbowChaseEdges(2);
      break;
    case 3:
      rainbowChaseEdges(4);
      break;
    case 4:
      rainbowChaseQuadrants(1);
      break;
    case 5:
      theaterChaseCycle();
      break;
    case 6:
      rainbowFade();
      break;
    default:
      rainbowChaseEdges(1);
      break;
  }
}

uint8_t num_times_rage_pushed = 0;

bool checkRageFlash() {
  static uint8_t last_pressed_mode = 0;
  static uint8_t debug_mode_count = 0;

  if (rageButtonPushed()) {
    rageFlash();

    num_times_rage_pushed++;

    const uint8_t top_dial_position = getTopDialPosition();
    if ((last_pressed_mode == 6 && top_dial_position == 1) || (last_pressed_mode == 1 && top_dial_position == 6)) {
      debug_mode_count++;

      if (debug_mode_count == 5) {
        debugMode();
        debug_mode_count = 0;
      }
    } else {
      debug_mode_count = 0;
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

WaitReturnCode readAllInputs() {
  getBrightnessDial();
  getSpeedDial();
  if (checkRageFlash() || checkTapFlash()) {
    return WaitReturnCode::FLASHED_OR_TAPPED;  // return so can re-instate normal mode colors after flash
  }
  if (checkModeDialChanged()) {
    return WaitReturnCode::MODE_CHANGED;
  }
  return WaitReturnCode::NO_CHANGE;
}
