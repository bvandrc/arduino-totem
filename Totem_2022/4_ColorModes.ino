#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "4_ColorModes.h"
#include "Totem_2022.h"

// void singleColorTwinkle(uint16_t hue) {
//   //   hue = 0;  // red
//   //   hue = 21845;  // green
//   //   hue = 43690;  // blue
//   //   hue = 3458;  // orange
//   //   hue = 52246;  // purple
//   //   hue = 27306;  // seafoam

//   for (uint8_t i = 0; i < strip.numPixels(); i++) {
//     if (speed_global < 10) {
//       strip.fill(strip.gamma32(strip.ColorHSV(hue)));
//       break;
//     }

//     uint8_t saturation = random(150, 256);
//     uint8_t value = random(100, 256);

//     strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, saturation, value)));
//   }

//   strip.show();

//   if (wait(10, 500)) {
//     return;
//   }
// }

void rainbowChase() {
  const uint16_t hue_step = 65536 / NUM_AROUND_EDGE;
  for (uint16_t first_pixel_hue = 0; first_pixel_hue < 65536; first_pixel_hue += hue_step) {
    for (uint8_t i = 0; i < NUM_AROUND_EDGE; i++) {
      uint16_t this_pixel_hue = first_pixel_hue + (i * hue_step);
      for (uint8_t side = 1; side <= 2; side++) {
        strip.setPixelColorEdge(side, i, strip.gamma32(strip.ColorHSV(this_pixel_hue)));
      }
    }

    strip.show();

    if (wait(50, 5000)) {
      return;
    }
  }
}

void rainbowFade() {
  for (long hue = 0; hue < 5 * 65536; hue += 256) {
    strip.fill(strip.gamma32(strip.ColorHSV(hue)));
    strip.show();

    if (wait(10, 500)) {
      return;
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

    if (wait(2, 200)) {
      return;
    }
  }
}

bool colorTwinkle(uint32_t colors[], uint8_t len_colors, uint32_t timeLengthMillis) {
  unsigned long initial_time = millis();

  for (uint8_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, colors[random(len_colors)]);
    strip.show();
  }

  uint8_t pixel_to_change = 0;
  while ((millis() - initial_time) < timeLengthMillis) {
    getNewPixel(pixel_to_change);
    strip.setPixelColor(pixel_to_change, colors[random(len_colors)]);
    strip.show();

    if (wait(2, 200)) {
      return true;
    }
  }
  return false;
}

void colorTwinkleCycle() {
  static uint32_t color1 = strip.ORANGE;
  static uint32_t color2 = strip.ORANGE;

  while (true) {
    getTwoNewColors(color1, color2);
    uint32_t colors[] = {color1, color2};
    if (colorTwinkle(colors, 2, 5000)) {
      return;
    };
  }
}

bool theaterChase(uint32_t color1, uint32_t color2, uint8_t width, bool clockwise, uint32_t time_length_millis) {
  unsigned long initial_time = millis();
  while ((millis() - initial_time) < time_length_millis) {
    for (uint8_t stagger = 0; stagger < width * 2; stagger++) {
      for (uint8_t i = 0; i < strip.numPixels(); i += (width * 2)) {
        int signed_stagger = clockwise ? stagger : -stagger;
        strip.fill(color1, i + signed_stagger - (width * 2), width);
        strip.fill(color2, i + signed_stagger - (width * 2) + width, width);
      }
      strip.show();

      if (wait(40, 1000)) {
        return true;
      }
    }
  }
  return false;
}

void theaterChaseCycle() {
  static uint32_t color1 = strip.ORANGE;
  static uint32_t color2 = strip.ORANGE;

  while (true) {
    getTwoNewColors(color1, color2);
    if (theaterChase(color1, color2, 3, random(2), 10000)) {
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

bool fillUp(uint32_t color) {
  for (uint8_t i = 0; i < NUM_IN_QUADRANT; i++) {
    strip.setPixelColorAllQuadrants(i, color);
    strip.show();
    if (wait(10, 1000)) {
      return true;
    }
  }
  return false;
}

void fillUpCycle() {
  static uint32_t color1 = strip.ORANGE;

  strip.clear();
  strip.show();

  while (true) {
    getNewColor(color1);
    if (fillUp(color1)) {
      return;
    }
    if (wait(20, 1000)) {
      return;
    }
  }
}

bool fillUpQuadrants(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4) {
  for (uint8_t i = 0; i < NUM_IN_QUADRANT; i++) {
    strip.setPixelColorQuadrant(1, i, color1);
    strip.setPixelColorQuadrant(2, i, color2);
    strip.setPixelColorQuadrant(3, i, color3);
    strip.setPixelColorQuadrant(4, i, color4);
    strip.show();
    if (wait(20, 1000)) {
      return true;
    }
  }
  return false;
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

    if (fillUpQuadrants(color1, color2, color3, color4)) {
      return;
    }
    if (wait(20, 1000)) {
      return;
    }
  }
}

void rageFlash() {
  static uint32_t prev_flash_color = strip.ORANGE;

  uint8_t prev_brightness = brightness_global;
  brightness_global = brightness_global < 200 ? 200 : MAX_BRIGHTNESS;

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
  brightness_global = brightness_global < 200 ? 200 : MAX_BRIGHTNESS;

  bullet(strip.getColorBrightnessAdjusted(color1, 50), color2, 10, 35);
  brightness_global = prev_brightness;
}
