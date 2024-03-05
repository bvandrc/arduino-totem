void debugIndicate() {
  stripSetSolid(BLUE);
  strip.show();
  delay(5000);
}

void debugIndicateSettingChange(bool setting) {
  stripSetSolid(setting ? GREEN : RED);
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
