#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "5_Debug.h"
#include "Totem_2022.h"

void debugIndicate() {
  strip.fill(strip.BLUE);
  strip.show();
  delay(5000);
}

void debugIndicateSettingChange(bool setting) {
  strip.fill(setting ? strip.GREEN : strip.RED);
  strip.show();
  delay(5000);
}

void debugToggleCurrentSetting() {
  debugIndicate();

  switch (colorScheme) {
    case 1:
      tapEnabled = !tapEnabled;
      debugIndicateSettingChange(tapEnabled);
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
