#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "4_ColorModes.h"
#include "Totem_2022.h"

void rainbowChase() {
  static const uint16_t HUE_STEP = 65536 / NUM_AROUND_EDGE;
  for (uint16_t first_pixel_hue = 0; first_pixel_hue < 65536; first_pixel_hue += HUE_STEP) {
    for (uint8_t i = 0; i < NUM_AROUND_EDGE; i++) {
      // +/- determines chase direction
      uint16_t this_pixel_hue = first_pixel_hue - (i * HUE_STEP);
      strip.setPixelColorEdge(1, i, strip.ColorHSV(this_pixel_hue));
    }

    for (uint8_t i = 0; i < NUM_AROUND_EDGE; i++) {
      uint16_t this_pixel_hue = first_pixel_hue + (i * HUE_STEP);
      strip.setPixelColorEdge(2, i, strip.ColorHSV(this_pixel_hue));
    }

    for (uint8_t i = 0; i < NUM_USED_IN_MIDDLE_SIDE_1; i++) {
      uint16_t this_pixel_hue = first_pixel_hue - (i * 65536 / NUM_USED_IN_MIDDLE_SIDE_1);
      strip.setPixelColorMiddleSide1(i, strip.ColorHSV(this_pixel_hue));
    }

    for (uint8_t i = 0; i < NUM_USED_IN_MIDDLE_SIDE_2; i++) {
      uint16_t this_pixel_hue = first_pixel_hue + (i * 65536 / NUM_USED_IN_MIDDLE_SIDE_2);
      strip.setPixelColorMiddleSide2(i, strip.ColorHSV(this_pixel_hue));
    }

    strip.show();

    WaitReturnCode return_code = wait(50, 5000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    } else if (return_code == WaitReturnCode::BRIGHTNESS_CHANGED) {
      first_pixel_hue -= HUE_STEP;
    }
  }
}

void rainbowFade() {
  const uint8_t HUE_STEP = 50;
  for (uint16_t hue = 0; hue < 65536; hue += HUE_STEP) {
    strip.fill(strip.ColorHSV((hue)));
    strip.show();

    WaitReturnCode return_code = wait(1, 200);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    } else if (return_code == WaitReturnCode::BRIGHTNESS_CHANGED) {
      hue -= HUE_STEP;
    }
  }
}

void rainbowTwinkle() {
  for (uint8_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, getRandomColor());
    strip.show();
  }
  uint8_t pixel_to_change = 0;
  while (true) {
    getNewPixel(pixel_to_change);
    strip.setPixelColor(pixel_to_change, getRandomColor());
    strip.show();

    WaitReturnCode return_code = wait(2, 200);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

WaitReturnCode theaterChase(uint32_t color1, uint32_t color2, uint8_t width, bool clockwise,
                            uint32_t time_length_millis) {
  unsigned long initial_time = millis();
  while ((millis() - initial_time) < time_length_millis) {
    for (uint8_t stagger = 0; stagger < width * 2; stagger++) {
      for (uint8_t i = 0; i < strip.numPixels(); i += (width * 2)) {
        int signed_stagger = clockwise ? stagger : -stagger;
        strip.fill(color1, i + signed_stagger - (width * 2), width);
        strip.fill(color2, i + signed_stagger - (width * 2) + width, width);
      }
      strip.show();

      WaitReturnCode return_code = wait(40, 1000);
      if (return_code == WaitReturnCode::MODE_CHANGED) {
        return WaitReturnCode::MODE_CHANGED;
      } else if (return_code == WaitReturnCode::BRIGHTNESS_CHANGED) {
        stagger--;
      }
    }
  }
  return WaitReturnCode::NO_CHANGE;
}

void theaterChaseCycle() {
  static uint32_t color1 = strip.ORANGE;
  static uint32_t color2 = strip.ORANGE;

  while (true) {
    getTwoNewColors(color1, color2);
    if (theaterChase(color1, color2, 3, random(2), 10000) == WaitReturnCode::MODE_CHANGED) {
      return;
    };
  }
}

void bullet(uint32_t background_color, uint32_t bullet_color, uint8_t bullet_width, uint16_t delay_micros) {
  for (uint8_t i = 0; i < NUM_IN_QUADRANT + bullet_width; i++) {
    strip.fill(background_color);
    for (uint8_t j = 0; j < bullet_width; j++) {
      strip.setPixelColorAllQuadrants(i + j, bullet_color);
    }
    strip.show();
    delayMicroseconds(delay_micros);
  }
}

WaitReturnCode fillUp(uint32_t color) {
  for (uint8_t i = 0; i < NUM_IN_QUADRANT; i++) {
    strip.setPixelColorAllQuadrants(i, color);
    strip.show();
    WaitReturnCode return_code = wait(10, 1000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return WaitReturnCode::MODE_CHANGED;
    } else if (return_code == WaitReturnCode::BRIGHTNESS_CHANGED) {
      i--;
    }
  }
  return WaitReturnCode::NO_CHANGE;
}

void fillUpCycle() {
  static uint32_t color1 = strip.ORANGE;

  strip.clear();
  strip.show();

  while (true) {
    getNewColor(color1);
    if (fillUp(color1) == WaitReturnCode::MODE_CHANGED) {
      return;
    }
    if (wait(20, 1000) == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

WaitReturnCode fillUpQuadrants(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4) {
  for (uint8_t i = 0; i < NUM_IN_QUADRANT; i++) {
    strip.setPixelColorQuadrant(1, i, color1);
    strip.setPixelColorQuadrant(2, i, color2);
    strip.setPixelColorQuadrant(3, i, color3);
    strip.setPixelColorQuadrant(4, i, color4);
    strip.show();

    WaitReturnCode return_code = wait(20, 1000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return WaitReturnCode::MODE_CHANGED;
    } else if (return_code == WaitReturnCode::BRIGHTNESS_CHANGED) {
      i--;
    }
  }
  return WaitReturnCode::NO_CHANGE;
}

void fillUpQuadrantsCycle() {
  static uint32_t color1 = strip.ORANGE;
  static uint32_t color2 = strip.ORANGE;
  static uint32_t color3 = strip.ORANGE;
  static uint32_t color4 = strip.ORANGE;

  strip.clear();
  strip.show();

  while (true) {
    uint32_t og_color1 = color1;
    uint32_t og_color2 = color2;
    uint32_t og_color3 = color3;
    uint32_t og_color4 = color4;

    while (true) {
      getNewColor(color1);
      getNewColor(color2);
      getNewColor(color3);
      getNewColor(color4);
      uint32_t colors[] = {color1, color2, color3, color4};
      if (arrayIsUnique(colors, 4) && color1 != og_color1 && color2 != og_color2 && color3 != og_color3 &&
          color4 != og_color4) {
        break;
      }
    }

    if (fillUpQuadrants(color1, color2, color3, color4) == WaitReturnCode::MODE_CHANGED) {
      return;
    }
    if (wait(20, 1000) == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

void rageFlash() {
  static uint32_t prev_flash_color = strip.ORANGE;

  uint8_t prev_brightness = brightness_global;
  brightness_global = brightness_global < MAX_BRIGHTNESS * 0.75 ? MAX_BRIGHTNESS * 0.75 : MAX_BRIGHTNESS;

  static uint32_t rage_color = strip.ORANGE;
  getNewColor(rage_color);

  do {
    strip.fill(rage_color);
    strip.show();
    delay(50);
    strip.clear();
    strip.show();
    delay(50);
  } while (rageButtonPushed());

  prev_flash_color = rage_color;
  brightness_global = prev_brightness;
}

void tapFlash() {
  static uint32_t color1 = strip.ORANGE;
  static uint32_t color2 = strip.ORANGE;
  getTwoNewColors(color1, color2);

  uint8_t prev_brightness = brightness_global;
  brightness_global = brightness_global < MAX_BRIGHTNESS * 0.75 ? MAX_BRIGHTNESS * 0.75 : MAX_BRIGHTNESS;

  bullet(strip.getColorBrightnessAdjusted(color1, brightness_global * 0.5), color2, 10, 35);
  brightness_global = prev_brightness;
}
