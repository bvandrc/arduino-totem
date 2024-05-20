#include <Adafruit_LIS3DH.h>  // 1.2.2

#include "1_HardwareInputs.h"
#include "2_HardwareOutputs.h"
#include "Totem.h"

// Pins
const uint8_t RAGE_BUTTON_PIN = 2;
const uint8_t SPEED_DIAL_PIN = A2;
const uint8_t BRIGHTNESS_DIAL_PIN = A3;
const uint8_t TOP_DIAL_PIN = A1;
const uint8_t BOTTOM_DIAL_PIN = A0;
const uint8_t MIC_PIN = A7;
const uint8_t MO_SENS_INT_1_PIN = 3;

// Other consts
const uint16_t DIAL_MIN = 0;
const uint16_t DIAL_MAX = 1023;
const uint8_t MAX_BRIGHTNESS = 20;

uint16_t speed = 10;

// Motion sensor

MyMotionSensor::MyMotionSensor() : Adafruit_LIS3DH() {
  tap_enabled = true;
}

void MyMotionSensor::init() {
  begin(0x18);
  setRange(LIS3DH_RANGE_4_G);  // 2, 4, 8 or 16 G
  // Click threshold. Adjust this number for the sensitivity of the 'click' force
  // this strongly depend on the range! for 16G, try 5-10
  // for 8G, try 10-20. for 4G try 20-40. for 2G try 40-80
  // higher numbers are less sensitive
  // TODO: investigate settings
  // TODO: double click!
  setClick(2, 80);
  delay(100);
}

bool MyMotionSensor::checkTapped() {
  const uint8_t tap = getClick();
  return (tap == 0 || !(tap & 0x30)) ? false : true;
}

MyMotionSensor motionSensor = MyMotionSensor();

void setUpPins() {
  pinMode(RAGE_BUTTON_PIN, INPUT_PULLUP);  // rage button normally CLOSED to GND

  // pinMode(MO_SENS_INT_1_PIN, INPUT);
  // attachInterrupt(digitalPinToInterrupt(RAGE_BUTTON_PIN), rageFlash, HIGH ); cant figure out why this wont work
  // attachInterrupt(digitalPinToInterrupt(MO_SENS_INT_1_PIN), tapFlash, RISING  );
}

bool rageButtonPushed() {
  return digitalRead(RAGE_BUTTON_PIN);
}

uint8_t getSelectorPosition(uint8_t pin) {
  const uint16_t read_val = analogRead(pin);
  if (read_val < (DIAL_MAX * 0.1)) {
    return 6;
  } else if (read_val < (DIAL_MAX * 0.3)) {
    return 5;
  } else if (read_val < (DIAL_MAX * 0.5)) {
    return 4;
  } else if (read_val < (DIAL_MAX * 0.7)) {
    return 3;
  } else if (read_val < (DIAL_MAX * 0.9)) {
    return 2;
  } else {
    return 1;
  }
}

uint8_t getTopDialPosition() {
  return getSelectorPosition(TOP_DIAL_PIN);
}

uint8_t getBottomDialPosition() {
  return getSelectorPosition(BOTTOM_DIAL_PIN);
}

bool checkModeDialChanged() {
  static uint8_t top_dial_position = 1;
  const uint8_t new_top_tial_position = getTopDialPosition();
  if (new_top_tial_position != top_dial_position) {
    top_dial_position = new_top_tial_position;
    return true;
  } else {
    return false;
  }
}

void getBrightnessDial() {
  // TODO: scale this dial logarithmically?
  const uint16_t read_val = analogRead(BRIGHTNESS_DIAL_PIN);
  const uint8_t new_val = map(read_val, DIAL_MIN, DIAL_MAX, 0, MAX_BRIGHTNESS);
  FastLED.setBrightness(new_val);
}

void getSpeedDial() {
  speed = analogRead(SPEED_DIAL_PIN);
}