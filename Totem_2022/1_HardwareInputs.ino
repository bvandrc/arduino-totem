#include <Adafruit_LIS3DH.h>  // 1.2.2

#include "1_HardwareInputs.h"
#include "Totem_2022.h"

// Pins
const uint8_t RAGE_BUTTON_PIN = 2;
const uint8_t SPEED_DIAL_PIN = A2;
const uint8_t BRIGHTNESS_DIAL_PIN = A3;
const uint8_t MODE_DIAL_PIN = A1;
const uint8_t COLOR_DIAL_PIN = A0;
const uint8_t MIC_PIN = A7;
const uint8_t MO_SENS_INT_1_PIN = 3;

// Other consts
const uint16_t DIAL_MIN = 0;
const uint16_t DIAL_MAX = 1023;
const uint8_t MAX_BRIGHTNESS = 255;  // not actual max brightness-- don't change this.

// Motion sensor

MyMotionSensor::MyMotionSensor() : Adafruit_LIS3DH() {
}

void MyMotionSensor::init() {
  (*this).begin(0x18);
  (*this).setRange(LIS3DH_RANGE_4_G);  // 2, 4, 8 or 16 G
  // Click threshold. Adjust this number for the sensitivity of the 'click' force
  // this strongly depend on the range! for 16G, try 5-10
  // for 8G, try 10-20. for 4G try 20-40. for 2G try 40-80
  // higher numbers are less sensitive
  (*this).setClick(2, 80);
  delay(100);
}

bool MyMotionSensor::checkTapped() {
  if (!tapEnabled) {
    return false;
  }

  uint8_t tap = (*this).getClick();
  if (tap == 0 || !(tap & 0x30)) {
    return false;
  } else {
    return true;
  }
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
  uint16_t readVal = analogRead(pin);
  if (readVal < (DIAL_MAX * 1.0 / 10.0)) {
    return 6;
  } else if (readVal < (DIAL_MAX * 3.0 / 10.0)) {
    return 5;
  } else if (readVal < (DIAL_MAX * 5.0 / 10.0)) {
    return 4;
  } else if (readVal < (DIAL_MAX * 7.0 / 10.0)) {
    return 3;
  } else if (readVal < (DIAL_MAX * 9.0 / 10.0)) {
    return 2;
  } else {
    return 1;
  }
}

uint8_t getModeDialPosition() {
  return getSelectorPosition(MODE_DIAL_PIN);
}

uint8_t getColorDialPosition() {
  return getSelectorPosition(COLOR_DIAL_PIN);
}

void getBrightnessDial() {
  uint16_t readVal = analogRead(BRIGHTNESS_DIAL_PIN);
  brightnessVal = map(readVal, DIAL_MIN, DIAL_MAX, 0, MAX_BRIGHTNESS);
  // TODO: scale this dial logarithmically
}

void getSpeedDial() {
  effectSpeed = analogRead(SPEED_DIAL_PIN);
}