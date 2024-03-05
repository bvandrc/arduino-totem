/*
   color modes
*/

// set every other ring to each color
void setRings(uint32_t color1, uint32_t color2) {
   for (uint16_t ringNumber = 0; ringNumber < 10; ringNumber += 2) {
      setRing(ringNumber, color1);
      setRing(ringNumber + 1, color2);
   }
}

// set ring number to its color
void setRing(uint8_t ringNumber, uint32_t color) {
   static const uint8_t RING_WIDTH = 8;
   for (uint16_t i = 0 + (RING_WIDTH * ringNumber); i < RING_WIDTH + (RING_WIDTH * ringNumber); i++) {
      strip.setPixelColor(i, color);
   }
   strip.show();
}

// fill in strip from bottom to top
void fillIn() {
   uint32_t waitTime;
   for (uint16_t j = 0; j < NUM_COLORS; j++) {        // spiral up
      for (int i = strip.numPixels() - 1; i >= 0; i--) {
         strip.setPixelColor(i, colors[j]);
         waitTime = setDelay(4, 50);                // spiral speed
         if (wait(waitTime))
            return;                                   // stop color mode if either button pressed
         strip.show();
      }
      waitTime = setDelay(1000, 5000);                // hold time when full
      if (wait(waitTime))
         return;                                      // stop color mode if either button pressed
   }
}


void rainbowCycle() {
   for (int j = ((256 * 5) - 1); j >= 0; j--) {     // 5 cycles of all colors on wheel
      for (int i = 0; i < strip.numPixels(); i++) {
         strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) - j) & 255));
      }
      strip.show();
      uint16_t waitTime = setDelay(1, 20);
      if (wait(waitTime))
         return;                                      // stop color mode if either button pressed
   }
}



// do theater chase of 2 randomly selected colors
void chaseSetting() {
   uint32_t color1;
   uint32_t color2;

   // pick 2 random colors, but they can't be white
   do {
      color1 = colors[random(0, NUM_COLORS)];
      color2 = colors[random(0, NUM_COLORS)];
   } while  ((color2 == color1) || (color2 == white) || (color1 == white));

   if (theaterChase(color1, color2, 8000, 3)) {
      return;
   }
}

bool theaterChase(uint32_t color1, uint32_t color2, uint32_t changeTime, uint8_t width) {
   unsigned long initialTime = millis();
   while ((millis() - initialTime) < changeTime) {
      uint16_t waitTime = setDelay(40, 200);
      for (uint8_t q = 0; q < (width * 2); q++) {
         for (uint16_t i = 0; i < strip.numPixels(); i = i + (width * 2)) {
            for (uint8_t k = 0; k < width; k++) {
               strip.setPixelColor(i - q + k, color1);      //turn every third pixel on //NOTE: invert sign of q for direction switch
            }
         }
         strip.show();

         if (wait(waitTime))
            return true;                                    // stop color mode if either button pressed

         for (uint16_t i = 0; i < strip.numPixels(); i = i + (width * 2)) {
            for (uint8_t k = 0; k < width; k++) {
               strip.setPixelColor(i - q + k, color2);      //turn every third pixel on
            }
         }
      }
   }
   return false;
}
