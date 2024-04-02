#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "Totem_2022.h"

bool arrayIsUnique(uint32_t colors[], uint8_t len_colors) {
  for (uint8_t i = 0; i < len_colors; i++) {
    uint32_t origVal = colors[i];
    for (uint8_t j = 0; j < len_colors; j++) {
      if (i != j) {
        if (origVal == colors[j]) {
          return false;
        }
      }
    }
  }
  return true;
}

// apply custom delay range to speed dial, return ms for wait command
uint32_t getDelayMillis(uint32_t low_millis, uint32_t high_millis) {
  if (speed_global < 10) {
    return 0xFFFFFFFF;  // frozen
  } else {
    return map(speed_global, DIAL_MAX, 10, low_millis, high_millis);  // higher speed = lower millis
  }
}

WaitReturnCode wait(uint32_t low_millis, uint32_t high_millis) {
  uint32_t wait_time_millis = getDelayMillis(low_millis, high_millis);
  unsigned long initial_time = millis();
  while ((millis() - initial_time) < wait_time_millis) {
    bool brightness_changed = getBrightnessDial();
    if (brightness_changed) {
      return WaitReturnCode::BRIGHTNESS_CHANGED;
    }
    getSpeedDial();
    if (checkRageFlash() || checkTapFlash()) {
      return WaitReturnCode::NO_CHANGE;  // want to return so can re-instate normal mode colors after flash, but dont
                                         // want to change state (restart mode)
    }
    wait_time_millis = getDelayMillis(low_millis, high_millis);
    if (checkModeChange()) {
      return WaitReturnCode::MODE_CHANGED;
    }
  }
  return WaitReturnCode::NO_CHANGE;
}

void getNewColor(uint32_t &color) {
  uint32_t new_color;
  do {
    new_color = strip.COLORS[random(strip.LEN_COLORS)];
  } while (new_color == color);
  color = new_color;
}

void getTwoNewColors(uint32_t &color1, uint32_t &color2) {
  uint32_t new_color1;
  uint32_t new_color2;
  do {
    new_color1 = strip.COLORS[random(strip.LEN_COLORS)];
    new_color2 = strip.COLORS[random(strip.LEN_COLORS)];
  } while (new_color1 == color1 || new_color2 == color2 || new_color1 == new_color2);
  color1 = new_color1;
  color2 = new_color2;
}

void getNewPixel(uint8_t &pixel) {
  uint32_t new_pixel;
  do {
    new_pixel = random(strip.numPixels());
  } while (new_pixel == pixel || abs(new_pixel - pixel) < 5);
  pixel = new_pixel;
}

uint32_t getRandomColor() {
  return strip.ColorHSV(random(65535));
}
