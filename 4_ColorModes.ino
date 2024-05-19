#include <FastLED.h>

#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "4_ColorModes.h"
#include "Totem.h"

const uint32_t MAX_HUE = 65536;

void fillRainbowEdges(uint16_t first_pixel_hue, uint8_t num_cycles = 1) {
  const uint16_t hue_offset_multiplier = MAX_HUE * num_cycles / NUM_AROUND_EDGE;
  for (uint8_t i = 0; i < NUM_AROUND_EDGE; i++) {
    const uint16_t hue_offset = (i * hue_offset_multiplier);
    // NOTE: +/- determines chase direction

    const uint16_t side_1_pixel_hue = first_pixel_hue - hue_offset;
    setPixelColorEdge(1, i, ColorHSV(side_1_pixel_hue));

    const uint16_t side_2_pixel_hue = first_pixel_hue + hue_offset;
    setPixelColorEdge(2, i, ColorHSV(side_2_pixel_hue));
  }
}

void fillRainbowQuadrants(uint16_t first_pixel_hue, uint8_t num_cycles = 1,
                          ChaseDirection direction = ChaseDirection::FORWARD) {
  const uint16_t hue_offset_multiplier = MAX_HUE * num_cycles / NUM_IN_QUADRANT;
  const int8_t direction_multiplier = direction == ChaseDirection::FORWARD ? -1 : 1;
  for (uint8_t i = 0; i < NUM_IN_QUADRANT; i++) {
    const uint16_t this_pixel_hue = first_pixel_hue + (direction_multiplier * i * hue_offset_multiplier);
    setPixelColorAllQuadrants(i, ColorHSV(this_pixel_hue));
  }
}

void fillRainbowMiddles(uint16_t first_pixel_hue, uint8_t num_cycles = 2) {
  const uint16_t side_1_hue_offset_multiplier = MAX_HUE * num_cycles / NUM_USED_IN_MIDDLE_SIDE_1;
  for (uint8_t i = 0; i < NUM_USED_IN_MIDDLE_SIDE_1; i++) {
    const uint16_t this_pixel_hue = first_pixel_hue - (i * side_1_hue_offset_multiplier);
    setPixelColorMiddleCropped(1, i, ColorHSV(this_pixel_hue));
  }

  const uint16_t side_2_hue_offset_multiplier = MAX_HUE * num_cycles / NUM_USED_IN_MIDDLE_SIDE_2;
  for (uint8_t i = 0; i < NUM_USED_IN_MIDDLE_SIDE_2; i++) {
    const uint16_t this_pixel_hue = first_pixel_hue + (i * side_2_hue_offset_multiplier);
    setPixelColorMiddleCropped(2, i, ColorHSV(this_pixel_hue));
  }
}

void rainbowChaseEdges(uint8_t num_cycles) {
  static const uint16_t HUE_STEP = MAX_HUE / NUM_AROUND_EDGE;  // can't just set to 1 or else is super slow
  static uint16_t first_pixel_hue = 0;
  while (true) {
    fillRainbowEdges(first_pixel_hue, num_cycles);
    fillRainbowMiddles(first_pixel_hue);
    showStrip();
    first_pixel_hue += HUE_STEP;  // wraps around to 0 and beyond, sweet!

    const WaitReturnCode return_code = wait(50, 5000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

void rainbowChaseQuadrants(uint8_t num_cycles) {
  static const uint16_t HUE_STEP = MAX_HUE / NUM_IN_QUADRANT;  // can't just set to 1 or else is super slow
  static uint16_t first_pixel_hue = 0;
  while (true) {
    fillRainbowQuadrants(first_pixel_hue, num_cycles,
                         num_times_rage_pushed % 2 == 0 ? ChaseDirection::FORWARD : ChaseDirection::BACKWARD);
    fillRainbowMiddles(first_pixel_hue);
    showStrip();
    first_pixel_hue += HUE_STEP;

    const WaitReturnCode return_code = wait(50, 5000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

void rainbowFade() {
  static const uint16_t HUE_STEP = 50;  // can't just set to 1 or else is super slow
  static uint16_t hue = 0;
  while (true) {
    fillStrip(ColorHSV((hue)));
    showStrip();
    hue += HUE_STEP;

    const WaitReturnCode return_code = wait(2, 100);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    }
  }
}

WaitReturnCode theaterChase(CRGB color1, CRGB color2, uint8_t width, bool clockwise, uint32_t time_length_millis) {
  const unsigned long initial_time = millis();
  while ((millis() - initial_time) < time_length_millis) {
    for (uint8_t stagger = 0; stagger < width * 2; stagger++) {
      for (uint8_t i = 0; i < FastLED.size() + ((2 * stagger) + 1); i += (width * 2)) {
        const int16_t start_of_stripe = i + ((clockwise ? 1 : -1) * stagger) - (width * 2);
        fillStrip(start_of_stripe, width, color1);
        fillStrip(start_of_stripe + width, width, color2);
      }
      showStrip();

      const WaitReturnCode return_code = wait(40, 1000);
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
    const WaitReturnCode return_code = theaterChase(colors[0], colors[1], 3, random(2), 10000);
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

  for (int16_t bullet_start = 0 - len_bullet; bullet_start < NUM_IN_QUADRANT; bullet_start++) {
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

uint8_t rageOrBumpBrightness(uint8_t prev_brightness) {
  if (prev_brightness < 5) {
    return prev_brightness + 1;
  }

  uint8_t new_brightness = prev_brightness * 1.5;
  if (new_brightness > MAX_BRIGHTNESS) {
    new_brightness = MAX_BRIGHTNESS;
  }
  return new_brightness;
}

void rageFlash() {
  const uint8_t prev_brightness = FastLED.getBrightness();
  const uint8_t new_brightness = rageOrBumpBrightness(prev_brightness);
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

  const uint8_t prev_brightness = FastLED.getBrightness();
  const uint8_t new_brightness = rageOrBumpBrightness(prev_brightness);
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
