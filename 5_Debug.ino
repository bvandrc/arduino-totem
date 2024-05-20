#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "5_Debug.h"
#include "Totem.h"

void indicateSettingChange(bool on_or_off) {
  fillStrip(on_or_off ? CRGB::Green : CRGB::Red);
  showStrip();
  delay(5000);
}

void debugMode() {
  // inidicate is in debug mode
  fillStrip(CRGB::Blue);
  showStrip();
  delay(5000);

  switch (getBottomDialPosition()) {
    case 1:
      // no change
      break;
    case 2:
      motionSensor.tap_enabled = !motionSensor.tap_enabled;
      indicateSettingChange(motionSensor.tap_enabled);
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
