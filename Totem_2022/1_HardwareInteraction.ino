// imports
#include <Adafruit_NeoPixel.h> // 1.8.5
#include <Adafruit_LIS3DH.h> // 1.2.2


// pins
const uint8_t  STRIP_PIN = 5;
const uint8_t  RAGE_BUTTON_PIN = 2;
const uint8_t  SPEED_DIAL_PIN = A2;
const uint8_t  BRIGHTNESS_DIAL_PIN = A3;
const uint8_t  MODE_DIAL_PIN = A1;
const uint8_t  COLOR_DIAL_PIN = A0;
const uint8_t  MIC_PIN = A7;
const uint8_t  MO_SENS_INT_1_PIN = 3;

// other consts
const uint8_t  NUM_PIXELS = 156;
const uint16_t MAX_BRIGHTNESS = 255;      // not actual max brightness-- don't change this.
const uint16_t DIAL_MIN = 0;
const uint16_t DIAL_MAX = 1023;




// LED strip init
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

// Motion sensor inits
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
// Adjust this number for the sensitivity of the 'click' force
// this strongly depend on the range! for 16G, try 5-10
// for 8G, try 10-20. for 4G try 20-40. for 2G try 40-80
// higher numbers are less sensitive
#define CLICKTHRESHHOLD 80

void setUpPins() {
  pinMode(RAGE_BUTTON_PIN, INPUT_PULLUP); // rage button normally CLOSED to GND
//  pinMode(MO_SENS_INT_1_PIN, INPUT); 
  //  attachInterrupt(digitalPinToInterrupt(RAGE_BUTTON_PIN), rageFlash, HIGH ); cant figure out why this wont work
  // attachInterrupt(digitalPinToInterrupt(MO_SENS_INT_1_PIN), tapFlash, RISING  ); 
}

void initMoSens() {
  lis.begin(0x18);
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G
  lis.setClick(2, CLICKTHRESHHOLD);
  delay(100);
}

bool checkTapped() {
  if (!tapEnabled) {
    return false;
  } else {
    uint8_t tap = lis.getClick();
    if (tap == 0 || !(tap & 0x30)) {
      return false;
    } else {
      return true;
    }
  }
}

void initStrip() {
  strip.begin(); // initialize
  strip.setBrightness(150); // max brightness of strip (actual max is 255)
  strip.show();

  /*  72 pixels (2021 totem):
   *  Brightness val |    R   |    G   |    B   |    W    |  Rainbow  <- Amps
                100  |  0.45  |  0.45  |  0.45  |  1.04   |    0.55
                150  |  0.62  |  0.60  |  0.60  |  1.54   |    0.75
                200  |  0.78  |  0.77  |  0.77  |  1.88   |    0.95
                220  |  0.85  |  0.84  |  0.84  |  1.99   |    1.03   <- 220 is old totem val
  */
  /*  156 pixels (2022 totem):
   *  Brightness val |    R   |    G   |    B   |    W    |  Rainbow  <- Amps
                50   |  0.51  |  0.51  |  0.51  |  1.22   |    X
                75   |  0.69  |  0.69  |  0.69  |  X      |    X
                100  |  0.88  |  0.88  |  0.88  |  X      |    1.07
                120  |  1.02  |  X     |  X     |  X      |    1.22   <- 220 is old totem val
                150  |  X     |  X     |  X     |  X      |    1.54   <- 220 is old totem val
  */
}



// INPUTS
bool rageButtonPushed() {
  return digitalRead(RAGE_BUTTON_PIN);
}

uint8_t getModeDialPosition() {
  return getSelectorPosition(MODE_DIAL_PIN);
}

uint8_t getColorDialPosition() {
  return getSelectorPosition(COLOR_DIAL_PIN);
}

uint8_t getSelectorPosition(uint8_t pin) {
  uint16_t readVal = analogRead(pin);
  if (readVal < ( DIAL_MAX * 1.0 / 10.0)) {
    return 6;
  } else if (readVal < (DIAL_MAX * 3.0 / 10.0)) {
    return 5;
  } else if (readVal < (DIAL_MAX * 5.0 / 10.0) ) {
    return 4;
  } else if (readVal < (DIAL_MAX * 7.0 / 10.0) ) {
    return 3;
  } else if (readVal < (DIAL_MAX * 9.0 / 10.0) ) {
    return 2;
  } else {
    return 1;
  }
}

void getBrightnessDial() {
  uint16_t readVal =  analogRead(BRIGHTNESS_DIAL_PIN);
  brightness =  map(readVal, DIAL_MIN, DIAL_MAX, 0, MAX_BRIGHTNESS);
  // TODO: scale this dial logarithmically
}

void getSpeedDial() {
  effectSpeed = analogRead(SPEED_DIAL_PIN);
}

// OUTPUTS

// Set pixel color (taking brightness into account) from 'packed' 32-bit RGB color:
void stripSetPixelColor(uint16_t n, uint32_t c) {
  uint32_t newC = getColorWithBrightness(c);
  strip.setPixelColor(n, newC);
}

void stripFill(uint32_t c = 0, int first = 0, uint16_t count = 0) {
  uint32_t newC = getColorWithBrightness(c);
  if (first < 0) {
    strip.fill(newC, 0, count + first);
  }  else {
    strip.fill(newC, (uint16_t)first, count);
  }
}

// https://forums.adafruit.com/viewtopic.php?t=41143
uint32_t getColorWithBrightness(uint32_t c) {
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >>  8);
  uint8_t b = (uint8_t)(c);


  uint8_t newR = (r * brightness / MAX_BRIGHTNESS);
  uint8_t newG = (g * brightness / MAX_BRIGHTNESS);
  uint8_t newB = (b * brightness / MAX_BRIGHTNESS);

  return strip.Color(newR, newG, newB);
}

uint32_t setColorBrightness(uint32_t c, uint16_t br) {
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >>  8);
  uint8_t b = (uint8_t)(c);


  uint8_t newR = (r * br / MAX_BRIGHTNESS);
  uint8_t newG = (g * br / MAX_BRIGHTNESS);
  uint8_t newB = (b * br / MAX_BRIGHTNESS);

  return strip.Color(newR, newG, newB);
}








/* DEBOUNCE EXAMPLE

  #define BUTTON_DEBOUNCE_INTERVAL 750
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

*/
