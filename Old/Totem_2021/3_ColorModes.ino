void solidModeTest(uint8_t num) {
  switch (num) {
    case 1:
      stripSetSolid(ORANGE);
      strip.show();
      break;
    case 2:
      stripSetSolid(RED);
      strip.show();
      break;
    case 3:
      stripSetSolid(BLUE);
      strip.show();
      break;
    case 4:
      stripSetSolid(WHITE);
      strip.show();
      break;
    case 5:
      stripSetSolid(PURPLE);
      strip.show();
      break;
    default:
      break;
  }
}

void rainbowChase() {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (uint8_t i = 0; i < strip.numPixels() / 2; i++) {
      uint16_t pixelHue = firstPixelHue + (i * 65536L / (strip.numPixels() / 2));
      stripSetPixelColorBothSides(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();

    if (wait(2, 50)) {
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
  uint8_t pixelToChange  = 0;
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

  uint8_t pixelToChange  = 0;
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
      for (uint8_t indx = 0; indx < strip.numPixels() ; indx += (width * 2)) {
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

void bullet(uint32_t background_color, uint32_t bullet_color, uint8_t bullet_width, uint16_t delaymillis) {
  for (uint8_t i = 0; i < (strip.numPixels() / 4); i++) {
    stripSetSolid(background_color);
    for (uint8_t j = 0; j < bullet_width; j++) {
      stripSetPixelColorQuadrants(i + j, bullet_color);
    }
    strip.show();
    delay(delaymillis);
  }

}

bool fillUp(uint32_t c) {
  for (uint8_t i = 0; i < (strip.numPixels() / 4); i++) {
    stripSetPixelColorQuadrants(i, c);
    strip.show();
    if (wait(20, 1000)) {
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
      return ;
    }
    if (wait(20, 1000)) {
      return ;
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
      if (arrayisUnique(colors, 4) && color1 != ogColor1 && color2 != ogColor2 && color3 != ogColor3 && color4 != ogColor4) {
        break;
      }
    }

    if (fillUpQuadrants(color1, color2, color3, color4)) {
      return ;
    }
    if (wait(20, 1000)) {
      return ;
    }
  }
}



void rageFlash() {
  static uint32_t color1 = ORANGE;
  static uint32_t color2 = ORANGE;
  getTwoNewColors(color1, color2);

  uint16_t prevBrightness = brightness;
  brightness = brightness < 200 ? 200 : MAX_BRIGHTNESS; 

  do {
    bullet(setColorBrightness(color1, 50), color2, 5, 7);
  } while (rageButtonPushed());
  brightness = prevBrightness;
}

void tapFlash(){
  static uint32_t color1 = ORANGE;
  static uint32_t color2 = ORANGE;
  getTwoNewColors(color1, color2);

  uint16_t prevBrightness = brightness;
  brightness = brightness < 200 ? 200 : MAX_BRIGHTNESS; 

  bullet(setColorBrightness(color1, 50), color2, 5, 7);
  brightness = prevBrightness;
}
