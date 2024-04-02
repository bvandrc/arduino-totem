#include <QuickStats.h>           // for doing certainmath operations
QuickStats stats;                 // stats object

#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
#define RAGE_BUTTON_PIN 5
#define MODE_BUTTON_PIN 7
#define BRIGHTNESS_DIAL_PIN A0
#define SPEED_DIAL_PIN A2
#define BUTTON_DEBOUNCE_INTERVAL 750
const uint16_t DIAL_MAX = 1010;
const uint16_t DIAL_MIN = 10;

const uint16_t QUARTER_BRIGHTNESS = 255 / 4;
const uint16_t FULL_BRIGHTNESS = 220;

const uint32_t orange = strip.Color(255, 65, 6);
const uint32_t seafoam = strip.Color(0, 255, 130);
const uint32_t red = strip.Color(255, 0, 0);
const uint32_t green = strip.Color(0, 255, 0);
const uint32_t pink = strip.Color(255, 0, 255);
const uint32_t yellow = strip.Color(220, 200, 0);
const uint32_t purple = strip.Color(130, 0, 230);
const uint32_t white = strip.Color(255, 255, 255);
const uint32_t blue = strip.Color(0, 0, 255);
const uint32_t off = strip.Color(0, 0, 0);
const uint32_t colors[] = {orange, seafoam, red, green, pink, yellow, purple, white, blue};
#define NUM_COLORS 9
#define NUM_MODES 5
const uint8_t NUM_SAMPLES = 10;

// initializations
boolean brightnessLock = false;
boolean speedLock = false;

uint16_t currMode = 0;
uint16_t currSpeed = 0;
uint16_t currBrightness = QUARTER_BRIGHTNESS;     // just an initialization
uint16_t maxBrightness = QUARTER_BRIGHTNESS;      // just an initialization (for laptop amperage)

void setup() {
   Serial.begin(9600);
   pinMode(RAGE_BUTTON_PIN, INPUT_PULLUP);
   pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
   strip.begin();
   strip.setBrightness(QUARTER_BRIGHTNESS);
   strip.show(); // Initialize all pixels to 'off'
}

void loop() {
   checkModeChange();
   setbrightness();
   checkRageFlash();
   modeSelect();
}

void modeSelect() {
   switch (currMode) {
      case 0:
         rainbowCycle();
         break;
      case 1:
         rainbowAll();
         break;
      case 2:
         fillIn();
         break;
      case 3:
         chaseSetting();
         break;
      default:
         theaterChase(pink, purple, 10000, 7);
         break;
   }
}

bool wait(uint32_t waitTime) {
   unsigned long initialTime = millis();
   while ((millis() - initialTime) < waitTime) {
      setbrightness();
      getSpeedDial();
      if (checkRageFlash() or checkModeChange())
         return true;      // button has been pressed
   }
   return false;
}

bool checkModeChange() {
   if (modeButtonPressed()) {
      Serial.print("Pressed?");
      int numDebugPresses = 0;
      // while mode button is held, pressing rage button will cycle debug modes
      while (!digitalRead(MODE_BUTTON_PIN))
         if (!digitalRead(RAGE_BUTTON_PIN)) {
            // wait for rage button to be released before changing the setting
            while (!digitalRead(RAGE_BUTTON_PIN)) {
               delay(500);
            }
            numDebugPresses++;
            // set color according to selected setting to indicate that setting
            switch (numDebugPresses) {
               case 5:
                  strip.setBrightness(QUARTER_BRIGHTNESS);
                  setSolid(blue);
                  break;
               case 10:
                  strip.setBrightness(QUARTER_BRIGHTNESS);
                  setSolid(orange);
                  break;
               default:    // exit debug mode
                  setSolid(off);
                  break;
            }
            delay(500);    // allow pressing of ragebutton again
         }
      delay(500);          // delay before enforcing the mode or setting change

      switch (numDebugPresses) {
         case 0:     // simple mode change
            currMode++;                               // change mode
            if (currMode >= NUM_MODES) {
               currMode = 0;
               maxBrightness = FULL_BRIGHTNESS;
            }
            break;
         case 5:     // enable/disable brightness lock
            brightnessLock = !brightnessLock;
            break;
         case 10:     // enable/disable speed lock
            speedLock = !speedLock;
            break;
         default:    // no change
            strip.setBrightness(currBrightness);      // brightness shouldn't change
            return false;     // don't change mode
      }
      strip.setBrightness(currBrightness);      // brightness shouldn't change
      return true;            // change mode
   } else {
      return false;           // mode button not pressed, don't change mode
   }
}

bool checkRageFlash() {
   if (!digitalRead(RAGE_BUTTON_PIN)) {
      static uint32_t prevFlashColor = orange;
      strip.setBrightness(maxBrightness);                // set bright for flash
      uint32_t color = colors[random(0, NUM_COLORS)];    // pick random color
      while (color == prevFlashColor) {
         color = colors[random(0, NUM_COLORS)];
      }
      do {                                   // flash until released
         setSolid(color);
         delay(50);
         setSolid(off);
         delay(50);
      } while (!digitalRead(RAGE_BUTTON_PIN));

      prevFlashColor = color;
      strip.setBrightness(currBrightness);    // revert to original brightness
      return true;      // rage button was pressed
   }
   return false;        // rage button wasn't pressed
}

void setbrightness() {
   if (!brightnessLock) {     // only allowed change if not locked
      uint16_t newBrightness = getBrightnessDial();
      if (newBrightness != currBrightness) {
         strip.setBrightness(newBrightness);
         strip.show();
         currBrightness = newBrightness;
      }
   }
}

uint16_t getBrightnessDial() {
   static float brightnessVals[NUM_SAMPLES];    // array of samples for smoothing
   static uint8_t currSample = 0;
   if (currSample >= NUM_SAMPLES) {
      currSample = 0;
   }
   uint16_t brightnessDial = analogRead(BRIGHTNESS_DIAL_PIN);
   brightnessDial = constrain(brightnessDial, 130, DIAL_MAX);
   brightnessDial = map(brightnessDial, 130, DIAL_MAX, 1, maxBrightness);
   brightnessVals[currSample] = brightnessDial;
   currSample++;
   float curAvg = stats.average(brightnessVals, NUM_SAMPLES);
   if ((abs(curAvg - currBrightness) > 5) && (stats.stdev(brightnessVals, NUM_SAMPLES) < 15)) {       // dial must be consistent (unfluctuating) and different than previous value to change
      return (uint16_t)curAvg;
   } else {
      return currBrightness;
   }
}

int setDelay(int low_millis, int high_millis) {
   uint16_t speedDialChanged = map(currSpeed, DIAL_MIN, DIAL_MAX, DIAL_MAX, DIAL_MIN);
   speedDialChanged = map(speedDialChanged, DIAL_MIN, DIAL_MAX, low_millis, high_millis);
   Serial.println(speedDialChanged);
   return speedDialChanged;
}

int getSpeedDial() {
   if (!speedLock) {
      uint16_t speedDial = analogRead(SPEED_DIAL_PIN);
      speedDial = constrain(speedDial, DIAL_MIN, DIAL_MAX);
      currSpeed = speedDial;
   }
}


boolean modeButtonPressed() {
   static unsigned long prevPress = 0;
   unsigned long currMillis = millis();
   if (prevPress > currMillis) {
      prevPress = currMillis;
   }
   if (currMillis - prevPress < BUTTON_DEBOUNCE_INTERVAL) {
      return false;
   } else if (!digitalRead(MODE_BUTTON_PIN)) {
      prevPress = currMillis;
      return true;
   } else {
      return false;
   }
}

/*
   void brightnessDisplay() {
   do {
      uint16_t newBrightness = getBrightnessDial();
      int brightnessDisplay = map(newBrightness, 1, maxBrightness, 1, strip.numPixels());
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
         if (i <= brightnessDisplay) {
            strip.setPixelColor(i, orange);
         } else {
            strip.setPixelColor(i, off);
         }
      }
      strip.show();
   } while (digitalRead(RAGE_BUTTON_PIN) && digitalRead(MODE_BUTTON_PIN));
   delay(200);
   }

   void speedDisplay() {
   do {
      uint16_t newSpeed = analogRead(SPEED_DIAL_PIN);
      newSpeed = constrain(newSpeed, DIAL_MIN, DIAL_MAX);
      uint16_t speedDisplay = map(newSpeed, DIAL_MIN, DIAL_MAX, 1, strip.numPixels());
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
         if (i <= speedDisplay) {
            strip.setPixelColor(i, purple);
         } else {
            strip.setPixelColor(i, off);
         }
      }
      strip.show();
   } while (digitalRead(RAGE_BUTTON_PIN) && digitalRead(MODE_BUTTON_PIN));
   delay(200);
   }
*/
