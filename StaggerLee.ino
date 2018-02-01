//
// StaggerLee - An autonomous robot car based on NodeMCU.
// Author: David J. Bianco <davidjbianco@gmail.com>
//

// load all the configurations, calibrations and settings for the bot
#include "config.h"

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

void led_on() {
  digitalWrite(PIN_LED, HIGH);
}

void led_off() {
  digitalWrite(PIN_LED, LOW);
}

int get_distance() {

  int duration = 0;
  int distance = 0;

  // Just in case, make sure we start measuring with sensor turned OFF
  digitalWrite(PIN_RANGE_TRIGGER, LOW);
  delayMicroseconds(5);

  // Send the sound pulse
  digitalWrite(PIN_RANGE_TRIGGER, HIGH);
  delayMicroseconds(15);

  // Turn off the sound
  digitalWrite(PIN_RANGE_TRIGGER, LOW);

  // take the reading
  duration = pulseIn(PIN_RANGE_ECHO, HIGH);

  // Calculate the distance (in cm).  Technically, we also need to know the
  // temperature and humidity to get an accurate reading, as well as calibrate
  // the sensor.  But for this robot, this will be good enough.
  distance = duration / 58.2;

  if ((distance > MAX_RANGE) || (distance <= MIN_RANGE)) {
    return(INFINITE_DISTANCE);
  } else {
    return(distance + DISTANCE_CALIBRATION);
  }
}

// The data from the HC-SR04 ultrasonic distance sensor can be kind of "noisy",
// frequently returning spurious high or low values.  Therefore, we take three
// readings and average them.  This helps smooth out the problems and gives a
// more reliable reading.
int get_distance_avg() {
  int d1, d2, d3;

  d1 = get_distance();
  d2 = get_distance();
  d3 = get_distance();

  return( (d1 + d2 + d3) / 3 );
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

void collide() {
  if (SERIAL_DEBUGGING) {
    Serial.println("COLLISION!");
  }

  stop();
  right(SPEED_MEDIUM);
  delay(250);
  stop();
}

void setup() {
  if (SERIAL_DEBUGGING) {
    Serial.begin(9600);
  }

  init_motors();
  init_distance_sensor();
  init_led();
  led_off();
}

void loop() {
  int distance;

  distance = get_distance_avg();

  if (distance < TURN_DISTANCE) {
    led_on();
    collide();
  } else {
    led_off();
    forward(SPEED_HIGH);
  }

  delay(MAIN_LOOP_DELAY);
}
