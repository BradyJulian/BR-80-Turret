//BR-80 Turret

//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2048;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Nothing to do (Stepper Library sets pins as outputs)
}

void loop() {
  // Rotate CW 180 degrees (pushing dart out of barrel)
  myStepper.setSpeed(15);
  myStepper.step(stepsPerRevolution*0.55);
  delay(100);

  // Rotate CCW 180 degrees (returning dart pusher)
  myStepper.setSpeed(15);
  myStepper.step(-stepsPerRevolution*0.55);
  delay(100);
}