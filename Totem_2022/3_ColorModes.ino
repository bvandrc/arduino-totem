#include "1_HardwareInteraction.h"
#include "2_ColorModeUtils.h"
#include "Totem_2022.h"

//  void setSolidMode(uint8_t index) {
//   const uint32_t COLOR_OPTIONS[] = {RED, GREEN, BLUE, ORANGE, PURPLE, SEAFOAM};
//   stripSetSolid(COLOR_OPTIONS[index - 1]);
//   strip.show();
// }

void setOneColorMode(uint8_t index) {
  const uint32_t COLOR_OPTIONS[] = {RED, GREEN, BLUE, ORANGE, PURPLE, SEAFOAM};

  uint16_t pixelHue;
  switch (index) {
    case 1:
      pixelHue = 0;  // red
      break;
    case 2:
      pixelHue = 21845;  // green
      break;
    case 3:
      pixelHue = 43690;  // blue
      break;
    case 4:
      pixelHue = 3458;  // orange
      break;
    case 5:
      pixelHue = 52246;  // purple
      break;
    case 6:
      pixelHue = 27306;  // seafoam
      break;
  }

  for (uint8_t i = 0; i < strip.numPixels(); i++) {
    if (effectSpeed < 10) {
      stripSetSolid(COLOR_OPTIONS[index - 1]);
      break;
    }

    uint8_t pixelSaturation = random(150, 256);
    uint8_t pixelValue = random(100, 256);

    stripSetPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, pixelSaturation, pixelValue)));
  }

  strip.show();

  if (wait(10, 500)) {
    return;
  }
}

void rainbowChase() {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (uint8_t i = 0; i < strip.numPixels() / 2; i++) {
      uint16_t pixelHue = firstPixelHue + (i * 65536L / (strip.numPixels() / 2));
      stripSetPixelColorBothSides(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();

    if (wait(1, 50)) {
      return;
    }
  }
}

void rainbowFade() {
  for (long hue = 0; hue < 5 * 65536; hue += 256) {
    stripSetSolid(strip.gamma32(strip.ColorHSV(hue)));

    strip.show();

    if (wait(10, 500)) {
      return;
    }
  }
}

void rainbowTwinkle() {
  for (uint8_t i = 0; i < strip.numPixels(); i++) {
    stripSetPixelColor(i, getRandomColor());
    strip.show();
  }
  uint8_t pixelToChange = 0;
  while (true) {
    getNewPixel(pixelToChange);
    stripSetPixelColor(pixelToChange, getRandomColor());
    strip.show();

    if (wait(2, 200)) {
      return;
    }
  }
}

bool colorTwinkle(uint32_t colors[], uint8_t len_colors, uint32_t timeLengthMillis) {
  unsigned long initialTime = millis();

  for (uint8_t i = 0; i < strip.numPixels(); i++) {
    stripSetPixelColor(i, colors[random(len_colors)]);
    strip.show();
  }

  uint8_t pixelToChange = 0;
  while ((millis() - initialTime) < timeLengthMillis) {
    getNewPixel(pixelToChange);
    stripSetPixelColor(pixelToChange, colors[random(len_colors)]);
    strip.show();

    if (wait(2, 200)) {
      return true;
    }
  }
  return false;
}

void colorTwinkleCycle() {
  static uint32_t color1 = ORANGE;
  static uint32_t color2 = ORANGE;

  while (true) {
    getTwoNewColors(color1, color2);
    uint32_t colors[] = {color1, color2};
    if (colorTwinkle(colors, 2, 5000)) {
      return;
    };
  }
}

bool theaterChase(uint32_t color1, uint32_t color2, uint8_t width, bool clockwise, uint32_t timeLengthMillis) {
  unsigned long initialTime = millis();
  while ((millis() - initialTime) < timeLengthMillis) {
    for (uint8_t stagger = 0; stagger < width * 2; stagger++) {
      for (uint8_t indx = 0; indx < strip.numPixels(); indx += (width * 2)) {
        int signedStagger = clockwise ? stagger : -stagger;
        stripFill(color1, indx + signedStagger - (width * 2), width);
        stripFill(color2, indx + signedStagger - (width * 2) + width, width);
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
  static uint32_t color1 = ORANGE;
  static uint32_t color2 = ORANGE;

  while (true) {
    getTwoNewColors(color1, color2);
    if (theaterChase(color1, color2, 3, random(2), 10000)) {
      return;
    };
  }
}

void bullet(uint32_t background_color, uint32_t bullet_color, uint8_t bullet_width, uint16_t delayMicros) {
  for (uint8_t i = 0; i < (strip.numPixels() / 4); i++) {
    stripSetSolid(background_color);
    for (uint8_t j = 0; j < bullet_width; j++) {
      stripSetPixelColorQuadrants(i + j, bullet_color);
    }
    strip.show();
    delayMicroseconds(delayMicros);
  }
}

bool fillUp(uint32_t c) {
  for (uint8_t i = 0; i < (strip.numPixels() / 4); i++) {
    stripSetPixelColorQuadrants(i, c);
    strip.show();
    if (wait(10, 1000)) {
      return true;
    }
  }
  return false;
}

void fillUpCycle() {
  static uint32_t color1 = ORANGE;

  stripSetSolid(OFF);
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
  for (uint8_t i = 0; i < (strip.numPixels() / 4); i++) {
    stripSetPixelColorQuadrant(1, i, color1);
    stripSetPixelColorQuadrant(2, i, color2);
    stripSetPixelColorQuadrant(3, i, color3);
    stripSetPixelColorQuadrant(4, i, color4);
    strip.show();
    if (wait(20, 1000)) {
      return true;
    }
  }
  return false;
}

void fillUpQuadrantsCycle() {
  static uint32_t color1 = ORANGE;
  static uint32_t color2 = ORANGE;
  static uint32_t color3 = ORANGE;
  static uint32_t color4 = ORANGE;

  stripSetSolid(OFF);
  strip.show();

  while (true) {
    uint32_t ogColor1 = color1;
    uint32_t ogColor2 = color2;
    uint32_t ogColor3 = color3;
    uint32_t ogColor4 = color4;

    while (true) {
      getNewColor(color1);
      getNewColor(color2);
      getNewColor(color3);
      getNewColor(color4);
      uint32_t colors[] = {color1, color2, color3, color4};
      if (arrayIsUnique(colors, 4) && color1 != ogColor1 && color2 != ogColor2 && color3 != ogColor3 &&
          color4 != ogColor4) {
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
  static uint32_t prevFlashColor = ORANGE;

  uint16_t prevBrightness = brightness;
  brightness = brightness < 200 ? 200 : MAX_BRIGHTNESS;

  uint32_t rageColor = COLORS[random(0, LEN_COLORS)];  // pick random color
  while (rageColor == prevFlashColor) {
    rageColor = COLORS[random(0, LEN_COLORS)];
  }

  do {
    stripSetSolid(rageColor);
    strip.show();
    delay(50);
    stripSetSolid(OFF);
    strip.show();
    delay(50);
  } while (rageButtonPushed());

  prevFlashColor = rageColor;
  brightness = prevBrightness;
}

void tapFlash() {
  static uint32_t color1 = ORANGE;
  static uint32_t color2 = ORANGE;
  getTwoNewColors(color1, color2);

  uint16_t prevBrightness = brightness;
  brightness = brightness < 200 ? 200 : MAX_BRIGHTNESS;

  bullet(setColorBrightness(color1, 50), color2, 10, 35);
  brightness = prevBrightness;
}
