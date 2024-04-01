#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "3_ColorModeUtils.h"
#include "Totem_2022.h"

// colors - visually tested
const uint32_t RED = strip.Color(255, 0, 0);
const uint32_t ORANGE = strip.Color(255, 30, 0);
const uint32_t YELLOW = strip.Color(255, 255, 0);
const uint32_t GREEN = strip.Color(0, 255, 0);
const uint32_t SEAFOAM = strip.Color(0, 255, 130);
const uint32_t BLUE = strip.Color(0, 0, 255);
const uint32_t PURPLE = strip.Color(200, 0, 255);
const uint32_t WHITE = strip.Color(255, 255, 255);

const uint32_t COLORS[] = {RED, ORANGE, YELLOW, GREEN, SEAFOAM, BLUE, PURPLE};  // order of rage flash as well
const uint8_t LEN_COLORS = 7;                                                   // better to hardcode array lengths

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
uint32_t getDelay(uint32_t lowTime, uint32_t highTime) {
  if (effectSpeed < 10) {
    return 4000000000;  // frozen
  } else {
    return map(effectSpeed, DIAL_MAX, 10, lowTime, highTime);  // higher speed = lower millis
  }
}

bool wait(uint32_t lowMillis, uint32_t highMillis) {
  uint32_t waitTimeMillis = getDelay(lowMillis, highMillis);
  unsigned long initialTime = millis();
  while ((millis() - initialTime) < waitTimeMillis) {
    getBrightnessDial();
    getSpeedDial();
    if (checkRageFlash() || checkTapFlash()) {
      return false;  // want to return so can re-instate normal mode colors after flash, but dont want to change state
                     // (restart mode)
    }
    waitTimeMillis = getDelay(lowMillis, highMillis);
    if (checkModeChange()) {
      return true;
    }
  }
  return false;
}

void getNewColor(uint32_t &color) {
  uint32_t newColor;
  do {
    newColor = COLORS[random(LEN_COLORS)];
  } while (newColor == color);
  color = newColor;
}

void getTwoNewColors(uint32_t &color1, uint32_t &color2) {
  uint32_t newColor1;
  uint32_t newColor2;
  do {
    newColor1 = COLORS[random(LEN_COLORS)];
    newColor2 = COLORS[random(LEN_COLORS)];
  } while (newColor1 == color1 || newColor2 == color2 || newColor1 == newColor2);
  color1 = newColor1;
  color2 = newColor2;
}

void getNewPixel(uint8_t &pixel) {
  uint32_t newPixel;
  do {
    newPixel = random(strip.numPixels());
  } while (newPixel == pixel || abs(newPixel - pixel) < 5);
  pixel = newPixel;
}

uint32_t getRandomColor() {
  return strip.gamma32(strip.ColorHSV(random(65535)));
}
