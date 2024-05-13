#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "5_Debug.h"
#include "Totem.h"

void debugIndicate() {
  fillStrip(CRGB::Blue);
  showStrip();
  delay(5000);
}

void debugIndicateSettingChange(bool setting) {
  fillStrip(setting ? CRGB::Green : CRGB::Red);
  showStrip();
  delay(5000);
}

void debugToggleCurrentSetting() {
  debugIndicate();

  switch (bottom_dial_position) {
    case 1:
      motionSensor.tap_enabled = !motionSensor.tap_enabled;
      debugIndicateSettingChange(motionSensor.tap_enabled);
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    default:
      break;
  }
}
