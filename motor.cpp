#include "motor.h"

// Pin definitions
#define MA_PIN_1     A0
#define MA_PIN_2     A1
#define MB_PIN_1     A4
#define MB_PIN_2     A3
#define M_SLEEP_PIN  D2

void setMotorsForward(int pwmVal) {
  analogWrite(MA_PIN_1, pwmVal);
  analogWrite(MA_PIN_2, 0);
  analogWrite(MB_PIN_1, pwmVal);
  analogWrite(MB_PIN_2, 0);
}

void setMotorsReverse(int pwmVal) {
  analogWrite(MA_PIN_1, 0);
  analogWrite(MA_PIN_2, pwmVal);
  analogWrite(MB_PIN_1, 0);
  analogWrite(MB_PIN_2, pwmVal);
}

void setMotorsClockwise(int pwmVal) {
  analogWrite(MA_PIN_1, 0);
  analogWrite(MA_PIN_2, pwmVal);
  analogWrite(MB_PIN_1, pwmVal);
  analogWrite(MB_PIN_2, 0);
}

void setMotorsCounterClockwise(int pwmVal) {
  analogWrite(MA_PIN_1, pwmVal);
  analogWrite(MA_PIN_2, 0);
  analogWrite(MB_PIN_1, 0);
  analogWrite(MB_PIN_2, pwmVal);
}

void stopMotors() {
  analogWrite(MA_PIN_1, 0);
  analogWrite(MA_PIN_2, 0);
  analogWrite(MB_PIN_1, 0);
  analogWrite(MB_PIN_2, 0);
} 

void setupMotors() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Initializing motors...");

  pinMode(MA_PIN_1, OUTPUT);
  pinMode(MA_PIN_2, OUTPUT);
  pinMode(MB_PIN_1, OUTPUT);
  pinMode(MB_PIN_2, OUTPUT);
  pinMode(M_SLEEP_PIN, OUTPUT);
  digitalWrite(M_SLEEP_PIN, HIGH);

  Serial.println("Motors initialized successfully");
}