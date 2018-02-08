//
// StaggerLee - An autonomous robot car based on NodeMCU.
// Author: David J. Bianco <davidjbianco@gmail.com>
//

// load all the configurations, calibrations and settings for the bot
#include "config.h"
#include <Time.h>

void init_motors() {
  pinMode(PIN_SPEED_RIGHT, OUTPUT);
  pinMode(PIN_DIR_RIGHT, OUTPUT);
  pinMode(PIN_SPEED_LEFT, OUTPUT);
  pinMode(PIN_DIR_LEFT, OUTPUT);
}

void init_forward_sensor() {
  pinMode(PIN_FORWARD_SENSOR, INPUT);
}

void init_led() {
  pinMode(PIN_LED, OUTPUT);
  led_off();
}

// Initialize the random number generator. The standard Arduino way is to read a
// value from an unconnected analog pin, thereby getting an unpredictable value
// with which to seed the PRNG.  This isn't that unpredictable on the NodeMCU
// dev boards, but it's probably good enough for our purposes.
void init_random() {
  randomSeed(analogRead(A0));
}

void led_on() {
  digitalWrite(PIN_LED, HIGH);
}

void led_off() {
  digitalWrite(PIN_LED, LOW);
}

bool isCollision(int sensor) {
  return (digitalRead(sensor) == HIGH);
}

void stop() {
  // Stop both motors.  Make sure we don't calibrate speeds, though.
  // Zero means 0 when stopping.
  analogWrite(PIN_SPEED_RIGHT, SPEED_STOP);
  analogWrite(PIN_SPEED_LEFT, SPEED_STOP);
}

void forward(int speed) {
  digitalWrite(PIN_DIR_RIGHT, LOW);
  digitalWrite(PIN_DIR_LEFT, LOW);
  analogWrite(PIN_SPEED_RIGHT, speed);
  analogWrite(PIN_SPEED_LEFT, speed);
}

void backward(int speed) {
  digitalWrite(PIN_DIR_RIGHT, HIGH);
  digitalWrite(PIN_DIR_LEFT, HIGH);
  analogWrite(PIN_SPEED_RIGHT, speed);
  analogWrite(PIN_SPEED_LEFT, speed);
}

void right(int speed) {
  digitalWrite(PIN_DIR_RIGHT, HIGH);
  digitalWrite(PIN_DIR_LEFT, LOW);
  analogWrite(PIN_SPEED_RIGHT, speed);
  analogWrite(PIN_SPEED_LEFT, speed);
}

void left(int speed) {
  digitalWrite(PIN_DIR_RIGHT, LOW);
  digitalWrite(PIN_DIR_LEFT, HIGH);
  analogWrite(PIN_SPEED_RIGHT, speed);
  analogWrite(PIN_SPEED_LEFT, speed);
}

// Randomly turn either right or left
void random_turn(int speed) {
  int direction;

  direction = random(1,3);

  if(direction == 1) {
    left(speed);
  } else {
    right(speed);
  }
}

void collide() {
  if (SERIAL_DEBUGGING) {
    Serial.println("COLLISION!");
  }

  stop();
  random_turn(SPEED_MEDIUM);
  delay(250);
  stop();
}

void setup() {
  if (SERIAL_DEBUGGING) {
    Serial.begin(9600);
  }

  init_random();
  init_motors();
  init_forward_sensor();
  init_led();
  led_off();
}

void loop() {


  while(1) {
    if(isCollision(PIN_FORWARD_SENSOR)) {
      led_off();
      forward(SPEED_HIGH);
    } else {
      led_on();
      collide();
    }

    delay(MAIN_LOOP_DELAY);
  }
}
