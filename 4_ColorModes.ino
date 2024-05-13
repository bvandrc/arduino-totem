#include <FastLED.h>

#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "4_ColorModes.h"
#include "Totem.h"

void rainbowChase() {
  static const uint16_t HUE_STEP = 65536 / NUM_AROUND_EDGE;
  for (uint16_t first_pixel_hue = 0; first_pixel_hue < 65536; first_pixel_hue += HUE_STEP) {
    for (uint8_t i = 0; i < NUM_AROUND_EDGE; i++) {
      // +/- determines chase direction
      uint16_t this_pixel_hue = first_pixel_hue - (i * HUE_STEP);
      setPixelColorEdge(1, i, ColorHSV(this_pixel_hue));
    }

    for (uint8_t i = 0; i < NUM_AROUND_EDGE; i++) {
      uint16_t this_pixel_hue = first_pixel_hue + (i * HUE_STEP);
      setPixelColorEdge(2, i, ColorHSV(this_pixel_hue));
    }

    for (uint8_t i = 0; i < NUM_USED_IN_MIDDLE_SIDE_1; i++) {
      uint16_t this_pixel_hue = first_pixel_hue - (i * 65536 * 2 / NUM_USED_IN_MIDDLE_SIDE_1);
      setPixelColorMiddleCropped(1, i, ColorHSV(this_pixel_hue));
    }

    for (uint8_t i = 0; i < NUM_USED_IN_MIDDLE_SIDE_2; i++) {
      uint16_t this_pixel_hue = first_pixel_hue + (i * 65536 * 2 / NUM_USED_IN_MIDDLE_SIDE_2);
      setPixelColorMiddleCropped(2, i, ColorHSV(this_pixel_hue));
    }

    // static const uint8_t NUM_MIDDLE_REPEATS = 2;
    // static const uint8_t NUM_MIDDLE_SIDE1_SEGMENT = NUM_USED_IN_MIDDLE_SIDE_1 / NUM_MIDDLE_REPEATS;
    // static const uint8_t NUM_MIDDLE_SIDE2_SEGMENT = NUM_USED_IN_MIDDLE_SIDE_2 / NUM_MIDDLE_REPEATS;

    // fill_rainbow_circular(leds + SIDE1_LEFT_START, NUM_AROUND_EDGE, initial_hue, true);

    // fill_rainbow_circular(leds + SIDE2_LEFT_START, NUM_AROUND_EDGE, initial_hue, true);

    // fill_rainbow_circular(leds + SIDE1_MIDDLE_START + MIDDLE_CROP_SIDE_1_START,  //
    //                       NUM_MIDDLE_SIDE1_SEGMENT, initial_hue, true);

    // fill_rainbow_circular(leds + SIDE1_MIDDLE_START + MIDDLE_CROP_SIDE_1_START + NUM_MIDDLE_SIDE1_SEGMENT,  //
    //                       NUM_MIDDLE_SIDE1_SEGMENT, initial_hue, true);

    // fill_rainbow_circular(leds + SIDE2_MIDDLE_START + MIDDLE_CROP_SIDE_2_START,  //
    //                       NUM_MIDDLE_SIDE2_SEGMENT, initial_hue, true);

    // fill_rainbow_circular(leds + SIDE2_MIDDLE_START + MIDDLE_CROP_SIDE_2_START + NUM_MIDDLE_SIDE2_SEGMENT,  //
    //                       NUM_MIDDLE_SIDE2_SEGMENT, initial_hue, true);

    showStrip();

    WaitReturnCode return_code = wait(50, 5000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

void rainbowFade() {
  const uint8_t HUE_STEP = 50;
  for (uint16_t hue = 0; hue < 65536; hue += HUE_STEP) {
    fillStrip(ColorHSV((hue)));
    showStrip();

    WaitReturnCode return_code = wait(5, 100);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

void rainbowTwinkle() {
  for (uint8_t i = 0; i < FastLED.size(); i++) {
    setPixelColor(i, getRandomColor());
    showStrip();
  }
  uint8_t pixel_to_change = 0;
  while (true) {
    getNewPixel(pixel_to_change);
    setPixelColor(pixel_to_change, getRandomColor());
    showStrip();

    WaitReturnCode return_code = wait(2, 200);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

WaitReturnCode theaterChase(CRGB color1, CRGB color2, uint8_t width, bool clockwise, uint32_t time_length_millis) {
  unsigned long initial_time = millis();
  while ((millis() - initial_time) < time_length_millis) {
    for (uint8_t stagger = 0; stagger < width * 2; stagger++) {
      for (uint8_t i = 0; i < FastLED.size() + ((2 * stagger) + 1); i += (width * 2)) {
        int signed_stagger = clockwise ? stagger : -stagger;
        fillStrip(i + signed_stagger - (width * 2), width, color1);
        fillStrip(i + signed_stagger - (width * 2) + width, width, color2);
      }
      showStrip();

      WaitReturnCode return_code = wait(40, 1000);
      if (return_code == WaitReturnCode::MODE_CHANGED) {
        return WaitReturnCode::MODE_CHANGED;
      }
    }
  }
  return WaitReturnCode::NO_CHANGE;
}

void theaterChaseCycle() {
  static const uint8_t LEN_COLORS = 2;
  static CRGB colors[LEN_COLORS];

  while (true) {
    getNewColors(colors, LEN_COLORS);
    WaitReturnCode return_code = theaterChase(colors[0], colors[1], 3, random(2), 10000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    };
  }
}

void bullet(CRGB* bullet, uint8_t len_bullet, uint16_t delay_millis) {
  static const uint8_t NUM_QUADRANTS = 4;
  struct OldLedInfo {
    CRGB color;
    uint8_t actual_index;
  };
  OldLedInfo old_led_info[NUM_QUADRANTS][len_bullet];
  CRGB* leds = FastLED.leds();

  for (int bullet_start = 0 - len_bullet; bullet_start < NUM_IN_QUADRANT; bullet_start++) {
    for (uint8_t bullet_index = 0; bullet_index < len_bullet; bullet_index++) {
      uint8_t quadrant_index = bullet_start + bullet_index;
      if (quadrant_index >= 0 && quadrant_index < NUM_IN_QUADRANT) {
        for (uint8_t quadrant = 1; quadrant <= NUM_QUADRANTS; quadrant++) {
          uint8_t actual_index = quadrantIndexToActualIndex(quadrant, quadrant_index);
          OldLedInfo this_old_led_info;
          this_old_led_info.actual_index = actual_index;
          this_old_led_info.color = leds[actual_index];
          old_led_info[quadrant - 1][bullet_index] = this_old_led_info;

          leds[actual_index] = bullet[bullet_index];
        }
      }
    }

    FastLED.show();

    for (uint8_t bullet_index = 0; bullet_index < len_bullet; bullet_index++) {
      uint8_t quadrant_index = bullet_start + bullet_index;
      if (quadrant_index >= 0 && quadrant_index < NUM_IN_QUADRANT) {
        for (uint8_t quadrant = 1; quadrant <= NUM_QUADRANTS; quadrant++) {
          OldLedInfo this_old_led_info = old_led_info[quadrant - 1][bullet_index];
          leds[this_old_led_info.actual_index] = this_old_led_info.color;
        }
      }
    }

    delay(delay_millis);
  }
}

WaitReturnCode fillUp(CRGB color) {
  for (uint8_t i = 0; i < NUM_IN_QUADRANT; i++) {
    setPixelColorAllQuadrants(i, color);
    showStrip();
    WaitReturnCode return_code = wait(10, 1000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return WaitReturnCode::MODE_CHANGED;
    }
  }
  return WaitReturnCode::NO_CHANGE;
}

void fillUpCycle() {
  static CRGB color1;

  FastLED.showColor(0);

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

WaitReturnCode fillUpQuadrants(CRGB color1, CRGB color2, CRGB color3, CRGB color4) {
  for (uint8_t i = 0; i < NUM_IN_QUADRANT; i++) {
    setPixelColorQuadrant(1, i, color1);
    setPixelColorQuadrant(2, i, color2);
    setPixelColorQuadrant(3, i, color3);
    setPixelColorQuadrant(4, i, color4);
    showStrip();

    WaitReturnCode return_code = wait(20, 1000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return WaitReturnCode::MODE_CHANGED;
    }
  }
  return WaitReturnCode::NO_CHANGE;
}

void fillUpQuadrantsCycle() {
  static const uint8_t LEN_COLORS = 2;
  static CRGB colors[LEN_COLORS];

  FastLED.showColor(0);

  while (true) {
    getNewColors(colors, LEN_COLORS);

    WaitReturnCode return_code = fillUpQuadrants(colors[0], colors[1], colors[0], colors[1]);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
    return_code = wait(20, 1000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

uint8_t rageOrBumpBrightness(uint8_t prev_brightness) {
  uint8_t new_brightness = prev_brightness * 1.5;
  if (new_brightness > MAX_BRIGHTNESS) {
    new_brightness = MAX_BRIGHTNESS;
  } else if (new_brightness < 5) {
    new_brightness = 5;
  }
  return new_brightness;
}

void rageFlash() {
  uint8_t prev_brightness = FastLED.getBrightness();
  uint8_t new_brightness = rageOrBumpBrightness(prev_brightness);
  FastLED.setBrightness(new_brightness);

  static CRGB rage_color;
  getNewColor(rage_color);

  do {
    fillStrip(rage_color);
    showStrip();
    delay(50);
    FastLED.showColor(0);
    delay(50);
  } while (rageButtonPushed());

  FastLED.setBrightness(prev_brightness);
}

void tapFlash() {
  static const uint8_t LEN_COLORS = 2;
  static CRGB colors[LEN_COLORS];
  getNewColors(colors, LEN_COLORS);

  uint8_t prev_brightness = FastLED.getBrightness();
  uint8_t new_brightness = rageOrBumpBrightness(prev_brightness);
  FastLED.setBrightness(new_brightness);

  // set background
  fillStrip(colors[0]);

  // set bullet
  static const uint8_t LEN_BULLET = 12;
  CRGB bullet_colors[LEN_BULLET];
  fill_solid(bullet_colors, LEN_BULLET, colors[1]);
  bullet(bullet_colors, LEN_BULLET, 2);

  FastLED.setBrightness(prev_brightness);
}
