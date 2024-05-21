#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "Totem.h"

// OLD colors - visually tested
// const uint32_t RED = Color(255, 0, 0);
// const uint32_t ORANGE = Color(255, 30, 0);
// const uint32_t YELLOW = Color(255, 255, 0);
// const uint32_t GREEN = Color(0, 255, 0);
// const uint32_t SEAFOAM = Color(0, 255, 130);
// const uint32_t BLUE = Color(0, 0, 255);
// const uint32_t PURPLE = Color(200, 0, 255);

const uint8_t LEN_COLORS = 7;  // better to hardcode array lengths
const CRGB COLORS[LEN_COLORS] = {
    CRGB::Red,          //
    CRGB::OrangeRed,    // Orange
    CRGB::Yellow,       //
    CRGB::Lime,         // Green
    CRGB::SpringGreen,  // Cyan
    CRGB::Blue,         //
    CRGB::Indigo,       //
                        // CRGB::Magenta, // Pink
};

WaitReturnCode wait(uint32_t low_millis, uint32_t high_millis) {
  static const uint8_t SPEED_DIAL_FROZEN_THRESHOLD = 10;
  bool frozen = false;
  uint32_t wait_time_millis;
  const unsigned long initial_time = millis();
  do {
    WaitReturnCode return_code = readAllInputs();
    if (return_code != WaitReturnCode::NO_CHANGE) {
      return return_code;  // return so can re-instate normal mode colors after flash
    }

    frozen = speed < SPEED_DIAL_FROZEN_THRESHOLD;
    // TODO: find better curve
    wait_time_millis =
        map(speed, SPEED_DIAL_FROZEN_THRESHOLD, DIAL_MAX, high_millis, low_millis);  // higher speed = lower millis

// from FASTLed.delay()
#ifndef FASTLED_ACCURATE_CLOCK
    // make sure to allow at least one ms to pass to ensure the clock moves forward
    ::delay(1);
#endif
    FastLED.show();
    yield();

  } while (frozen || ((millis() - initial_time) < wait_time_millis));
  return WaitReturnCode::NO_CHANGE;
}

boolean hueIsInRange(uint8_t hue_1, uint8_t hue_2, uint8_t min_difference) {
  const int16_t difference = abs((uint16_t)hue_1 - (uint16_t)hue_2);
  return difference < min_difference || difference > 255 - min_difference;
}

bool isAlreadyInArray(CRGB color, CRGB *colors, uint8_t len_colors) {
  const uint8_t prev_hue = rgb2hsv_approximate(color).hue;

  for (uint8_t i = 0; i < len_colors; i++) {
    if (hueIsInRange(rgb2hsv_approximate(colors[i]).hue, prev_hue, 50)) {
      return true;
    }
  }
  return false;
}

void getNewColor(CRGB &color) {
  const uint8_t prev_hue = rgb2hsv_approximate(color).hue;

  CRGB new_color;
  do {
    new_color = COLORS[random8(LEN_COLORS)];
  } while (hueIsInRange(rgb2hsv_approximate(new_color).hue, prev_hue, 50));
  color = new_color;
}

void getNewColors(CRGB *colors, uint8_t len_colors) {
  for (uint8_t i = 0; i < len_colors; i++) {
    CRGB new_color;
    do {
      new_color = COLORS[random8(LEN_COLORS)];
    } while (isAlreadyInArray(new_color, colors, len_colors));
    colors[i] = new_color;
  }
}

uint32_t CHSVtoColorCode(CRGB c) {
  return c.r << 16 | c.g << 8 | c.b;
}

// from Aadfruit Neopixel library
static const uint8_t PROGMEM _NeoPixelGammaTable[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
    3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,
    8,   8,   8,   9,   9,   9,   10,  10,  10,  11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,
    16,  17,  17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,  25,  26,  27,  27,  28,
    29,  29,  30,  31,  31,  32,  33,  34,  34,  35,  36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,
    46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  68,
    69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,  82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,
    97,  99,  100, 102, 103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125, 127, 129, 130,
    132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 170, 172,
    174, 176, 178, 180, 182, 184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215, 218, 220,
    223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252, 255};
uint8_t gamma8(uint8_t x) {
  return pgm_read_byte(&_NeoPixelGammaTable[x]);
}
uint32_t gamma32(uint32_t color) {
  uint8_t *y = (uint8_t *)&color;
  for (uint8_t i = 0; i < 4; i++)
    y[i] = gamma8(y[i]);
  return color;
}
CRGB gamma32(CRGB color) {
  return CRGB((gamma32(CHSVtoColorCode(color))));
}
CRGB ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255) {
  uint8_t r, g, b;

  hue = (hue * 1530L + 32768) / 65536;

  if (hue < 510) {
    b = 0;
    if (hue < 255) {
      r = 255;
      g = hue;
    } else {
      r = 510 - hue;
      g = 255;
    }
  } else if (hue < 1020) {
    r = 0;
    if (hue < 765) {
      g = 255;
      b = hue - 510;
    } else {
      g = 1020 - hue;
      b = 255;
    }
  } else if (hue < 1530) {
    g = 0;
    if (hue < 1275) {
      r = hue - 1020;
      b = 255;
    } else {  //
      r = 255;
      b = 1530 - hue;
    }
  } else {
    r = 255;
    g = b = 0;
  }

  uint32_t v1 = 1 + val;
  uint16_t s1 = 1 + sat;
  uint8_t s2 = 255 - sat;
  uint32_t c = ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) | (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
               (((((b * s1) >> 8) + s2) * v1) >> 8);

  return CRGB(gamma32(c));
}

CRGB getRandomColor() {
  return ColorHSV(random16());
}
