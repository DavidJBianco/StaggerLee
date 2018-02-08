// The following are the pins for the distance sensor
#define PIN_FORWARD_SENSOR 13 // Echo Pin
#define PIN_LED  16 // Onboard LED

// Pins for the motors
#define PIN_SPEED_LEFT 5
#define PIN_DIR_LEFT   0
#define PIN_SPEED_RIGHT  4
#define PIN_DIR_RIGHT    2

// sensor settings
#define OBSTACLE HIGH

// Speed settings for the motors
#define SPEED_STOP 0
#define SPEED_LOW 256
#define SPEED_MEDIUM 640
#define SPEED_HIGH 1023

// The calibration settings are meant to compensate for minor
// differences in speed between individual motors.
#define RIGHT_CALIBRATION 0
#define LEFT_CALIBRATION 0

// Set to 0 to disable serial debugging
#define SERIAL_DEBUGGING 1

// How often to run the main loop, in milliseconds
#define MAIN_LOOP_DELAY 150
