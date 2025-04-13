#include "Arduino.h"
#include "motor.h"

// Pin definitions
#define MA_PIN_1     A0
#define MA_PIN_2     A1
#define MB_PIN_1     A4
#define MB_PIN_2     A3
#define M_SLEEP_PIN  D2
#define M_SERVO_PIN  D10

void set_motors_forward(int pwm_val) {
  analogWrite(MA_PIN_1, pwm_val);
  analogWrite(MA_PIN_2, 0);
  analogWrite(MB_PIN_1, pwm_val);
  analogWrite(MB_PIN_2, 0);
}

void set_motors_reverse(int pwm_val) {
  analogWrite(MA_PIN_1, 0);
  analogWrite(MA_PIN_2, pwm_val);
  analogWrite(MB_PIN_1, 0);
  analogWrite(MB_PIN_2, pwm_val);
}

void set_motors_clockwise(int pwm_val) {
  analogWrite(MA_PIN_1, 0);
  analogWrite(MA_PIN_2, pwm_val);
  analogWrite(MB_PIN_1, pwm_val);
  analogWrite(MB_PIN_2, 0);
}

void set_motors_counter_clockwise(int pwm_val) {
  analogWrite(MA_PIN_1, pwm_val);
  analogWrite(MA_PIN_2, 0);
  analogWrite(MB_PIN_1, 0);
  analogWrite(MB_PIN_2, pwm_val);
}

void stop_motors() {
  analogWrite(MA_PIN_1, 0);
  analogWrite(MA_PIN_2, 0);
  analogWrite(MB_PIN_1, 0);
  analogWrite(MB_PIN_2, 0);
}

void activate_servo_motor() {
  digitalWrite(M_SERVO_PIN, HIGH);
}

void deactivate_servo_motor() {
  digitalWrite(M_SERVO_PIN, LOW);
}

void setup_motors() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Initializing motors...");

  pinMode(MA_PIN_1, OUTPUT);
  pinMode(MA_PIN_2, OUTPUT);
  pinMode(MB_PIN_1, OUTPUT);
  pinMode(MB_PIN_2, OUTPUT);

  pinMode(M_SLEEP_PIN, OUTPUT);
  pinMode(M_SERVO_PIN, OUTPUT);

  digitalWrite(M_SLEEP_PIN, HIGH);

  Serial.println("Motors initialized successfully");
}