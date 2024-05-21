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
    const uint16_t this_pixel_hue = first_pixel_hue - (i * side_2_hue_offset_multiplier);
    setPixelColorMiddleCropped(2, i, ColorHSV(this_pixel_hue));
  }
}

void rainbowEdgeChaseAccelerate(uint16_t& first_pixel_hue, uint8_t num_cycles) {
  int32_t hue_step = 100;
  while (rageButtonPushed()) {
    fillRainbowEdges(first_pixel_hue, num_cycles);
    fillRainbowMiddles(first_pixel_hue);
    showStrip();
    delay(1);
    first_pixel_hue += hue_step;
    if (hue_step < 4000) {
      hue_step += 5;
    }
  }

  const uint8_t prev_brightness = FastLED.getBrightness();
  unsigned long last_changed_millis = millis();
  bool on_off = true;

  while (hue_step > 20) {
    const unsigned long curr_millis = millis();
    if (curr_millis - last_changed_millis > 100) {
      FastLED.setBrightness((on_off || (hue_step < 1000)) ? prev_brightness : (prev_brightness * 0.6));
      last_changed_millis = curr_millis;
      on_off = !on_off;
    }
    fillRainbowEdges(first_pixel_hue, num_cycles);
    fillRainbowMiddles(first_pixel_hue);
    showStrip();
    delay(1);
    first_pixel_hue += hue_step;
    hue_step -= 10;
  }
  FastLED.setBrightness(prev_brightness);
}

void rainbowChaseEdges(uint8_t num_cycles, uint16_t rage_hue_step) {
  static const uint16_t HUE_STEP = MAX_HUE / NUM_AROUND_EDGE;  // can't just set to 1 or else is super slow
  static uint16_t first_pixel_hue = 0;
  while (true) {
    fillRainbowEdges(first_pixel_hue, num_cycles);
    fillRainbowMiddles(first_pixel_hue);
    showStrip();

    const WaitReturnCode return_code = wait(50, 5000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    } else if (return_code == WaitReturnCode::RAGE_PRESSED) {
      switch (getBottomDialPosition()) {
        case 1:
          rainbowEdgeChaseAccelerate(first_pixel_hue, num_cycles);
          break;
        case 2:
        case 3:
        case 4:
          while (rageButtonPushed()) {
            fillRainbowEdges(first_pixel_hue, num_cycles);
            fillRainbowMiddles(first_pixel_hue);
            showStrip();
            first_pixel_hue += rage_hue_step;
          }
          break;
        case 5:
          rageFlashOnCurrent();
          break;
        case 6:
          rageFlashRandomColor();
          break;
      }
    }

    first_pixel_hue += HUE_STEP;
  }
}

void rainbowQuadrantChaseAccelerate(uint16_t& first_pixel_hue, uint8_t num_cycles, ChaseDirection chase_direction) {
  int32_t hue_step = 100;
  while (rageButtonPushed()) {
    fillRainbowQuadrants(first_pixel_hue, num_cycles, chase_direction);
    fillRainbowMiddles(first_pixel_hue);
    showStrip();
    delay(1);
    first_pixel_hue += hue_step;
    if (hue_step < 5000) {
      hue_step += 5;
    }
  }

  const uint8_t prev_brightness = FastLED.getBrightness();
  unsigned long last_changed_millis = millis();
  bool on_off = true;

  while (hue_step > 20) {
    const unsigned long curr_millis = millis();
    if (curr_millis - last_changed_millis > 100) {
      FastLED.setBrightness((on_off || (hue_step < 500)) ? prev_brightness : (prev_brightness * 0.6));
      last_changed_millis = curr_millis;
      on_off = !on_off;
    }
    fillRainbowQuadrants(first_pixel_hue, num_cycles, chase_direction);
    fillRainbowMiddles(first_pixel_hue);
    showStrip();
    delay(1);
    first_pixel_hue += hue_step;
    hue_step -= 20;
  }
  FastLED.setBrightness(prev_brightness);
}

void rainbowChaseQuadrants(uint8_t num_cycles, uint16_t rage_hue_step) {
  static const uint16_t HUE_STEP = MAX_HUE / NUM_IN_QUADRANT;  // can't just set to 1 or else is super slow
  static uint16_t first_pixel_hue = 0;
  while (true) {
    ChaseDirection chase_direction =
        num_times_rage_pushed % 2 == 0 ? ChaseDirection::FORWARD : ChaseDirection::BACKWARD;
    fillRainbowQuadrants(first_pixel_hue, num_cycles, chase_direction);
    fillRainbowMiddles(first_pixel_hue);
    showStrip();

    const WaitReturnCode return_code = wait(50, 5000);
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    } else if (return_code == WaitReturnCode::RAGE_PRESSED) {
      switch (getBottomDialPosition()) {
        case 1:
          rainbowQuadrantChaseAccelerate(first_pixel_hue, num_cycles, chase_direction);
          break;
        case 2:
        case 3:
        case 4:
          while (rageButtonPushed()) {
            fillRainbowQuadrants(first_pixel_hue, num_cycles, chase_direction);
            fillRainbowMiddles(first_pixel_hue);
            showStrip();
            first_pixel_hue += rage_hue_step;
          }
          break;
        case 5:
          rageFlashOnCurrent();
          break;
        case 6:
          rageFlashRandomColor();
          break;
      }
    }

    first_pixel_hue += HUE_STEP;
  }
}

void rainbowFadeAccelerate(uint16_t& hue) {
  int32_t hue_step = 100;
  while (rageButtonPushed()) {
    fillStrip(ColorHSV((hue)));
    showStrip();
    hue += hue_step;
    if (hue_step < 3000) {
      hue_step += 3;
    }
  }

  const uint8_t prev_brightness = FastLED.getBrightness();
  unsigned long last_changed_millis = millis();
  bool on_off = true;

  while (hue_step > 20) {
    const unsigned long curr_millis = millis();
    if (curr_millis - last_changed_millis > 100) {
      FastLED.setBrightness((on_off || (hue_step < 500)) ? prev_brightness : (prev_brightness * 0.6));
      last_changed_millis = curr_millis;
      on_off = !on_off;
    }
    fillStrip(ColorHSV((hue)));
    showStrip();
    hue += hue_step;
    hue_step -= 5;
  }
  FastLED.setBrightness(prev_brightness);
}

void rainbowFade() {
  static const uint16_t MAX_HUE_STEP = 150;  // greater the step, faster the speed
  static const uint16_t HUE_DIV = pow(DIAL_MAX, 2) / MAX_HUE_STEP;

  static uint16_t hue = 0;
  uint16_t hue_step;

  while (true) {
    fillStrip(ColorHSV((hue)));
    showStrip();

    WaitReturnCode return_code = readAllInputs();
    hue_step = pow(speed, 2) / HUE_DIV;
    if (return_code == WaitReturnCode::MODE_CHANGED) {
      return;
    } else if (return_code == WaitReturnCode::RAGE_PRESSED) {
      switch (getBottomDialPosition()) {
        case 1:
          rainbowFadeAccelerate(hue);
          break;
        case 2:
        case 3:
        case 4:
          hue_step = 2000;
          break;
        case 5:
          rageFlashOnCurrent();
          break;
        case 6:
          rageFlashRandomColor();
          break;
      }
    }
    hue += hue_step;
    delay(1);
  }
}

void theaterChasePaint(CRGB color1, CRGB color2, uint8_t width, uint8_t stagger, bool clockwise) {
  for (uint8_t i = 0; i < FastLED.size() + ((2 * stagger) + 1); i += (width * 2)) {
    const int16_t start_of_stripe = i + ((clockwise ? 1 : -1) * stagger) - (width * 2);
    fillStrip(start_of_stripe, width, color1);
    fillStrip(start_of_stripe + width, width, color2);
  }
  showStrip();
}

void theaterChaseCycle() {
  static const uint8_t WIDTH = 3;
  static const uint32_t COLOR_CHANGE_TIME_MILLIS = 10000;
  static const uint8_t MAX_STAGGER = WIDTH * 2;
  static const uint8_t LEN_COLORS = 2;
  CRGB colors[LEN_COLORS];
  getNewColors(colors, LEN_COLORS);

  unsigned long last_changed_time = millis();

  while (true) {
    for (uint8_t stagger = 0; stagger < MAX_STAGGER; stagger++) {
      unsigned long curr_millis = millis();
      if (curr_millis - last_changed_time > COLOR_CHANGE_TIME_MILLIS) {
        getNewColors(colors, LEN_COLORS);
        last_changed_time = curr_millis;
      }

      theaterChasePaint(colors[0], colors[1], WIDTH, stagger, true);

      const WaitReturnCode return_code = wait(40, 1000);
      if (return_code == WaitReturnCode::MODE_CHANGED) {
        return;
      } else if (return_code == WaitReturnCode::RAGE_PRESSED) {
        switch (getBottomDialPosition()) {
          case 1:
          // TODO: acceleration mode
          case 2:
          case 3:
          case 4:
            while (rageButtonPushed()) {
              for (uint8_t stagger_rage = 0; stagger_rage < MAX_STAGGER; stagger_rage++) {
                theaterChasePaint(colors[0], colors[1], WIDTH, stagger_rage, true);
                delay(20);
              }
            }
            getNewColors(colors, LEN_COLORS);
            break;
          case 5:
            rageFlashOnCurrent();
            break;
          case 6:
            rageFlashRandomColor();
            break;
        }
      }
    }
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

/**
 * Returns a higher brightness than current
 */
uint8_t rageOrBumpBrightness(uint8_t prev_brightness) {
  const uint8_t min_new_brightness = prev_brightness + 1;
  const uint8_t new_brightness = prev_brightness * 1.3;
  return constrain(new_brightness, prev_brightness + 1, MAX_BRIGHTNESS);
}

void rageFlashRandomColor() {
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

void rageFlashOnCurrent() {
  const uint8_t prev_brightness = FastLED.getBrightness();

  do {
    FastLED.setBrightness(prev_brightness * 0.5);
    showStrip();
    delay(70);
    FastLED.setBrightness(prev_brightness);
    showStrip();
    delay(70);
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
