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

void init_distance_sensor() {
  pinMode(PIN_RANGE_TRIGGER, OUTPUT);
  pinMode(PIN_RANGE_ECHO, INPUT);
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

bool distance_outlier(int last_distance, int new_distance) {
  if(abs(last_distance - new_distance) > 20) {
    Serial.print("Outlier: ");
    Serial.println(abs(last_distance - new_distance));
    return true;
  } else {
    return false;
  }
}

int get_distance_smoothed() {
  int distance, return_distance;
  static int previous_distance = INFINITE_DISTANCE;

  distance = get_distance();

  if(previous_distance == INFINITE_DISTANCE) {
    previous_distance = distance;
    return distance;
  } else {
    if(! distance_outlier(previous_distance, distance)) {
      previous_distance = distance;
      return distance;
    } else {
      previous_distance = distance;
      return INFINITE_DISTANCE;
    }
  }
}

int get_distance() {
  int duration = 0;
  int distance = 0;

  // Just in case, make sure we start measuring with sensor turned OFF
  digitalWrite(PIN_RANGE_TRIGGER, LOW);
  delayMicroseconds(2);

  // Send the sound pulse
  digitalWrite(PIN_RANGE_TRIGGER, HIGH);
  delayMicroseconds(10);

  // Turn off the sound
  digitalWrite(PIN_RANGE_TRIGGER, LOW);

  // take the reading
  duration = pulseIn(PIN_RANGE_ECHO, HIGH);

  // Calculate the distance (in cm).  Technically, we also need to know the
  // temperature and humidity to get an accurate reading, as well as calibrate
  // the sensor.  But for this robot, this will be good enough.
  distance = duration / 58.2;
  distance = distance + DISTANCE_CALIBRATION;

  return distance;
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
  init_distance_sensor();
  init_led();
  led_off();
}

void loop() {

  int distance;

  /*
  while(1){
    distance = get_distance_smoothed();
    Serial.println(distance);
    delay(MAIN_LOOP_DELAY);
  }
  */

  distance = get_distance_smoothed();

  Serial.println(distance);

  if(distance <= BACK_DISTANCE) {
    led_on();
    backward(SPEED_HIGH);
    delay(500);
  } else if(distance <= TURN_DISTANCE) {
    led_on();
    collide();
  } else {
    led_off();
    forward(SPEED_HIGH);
  }

  delay(MAIN_LOOP_DELAY);
}
