//Full Moving Base Test

//Libraries
#include <Servo.h>


//Pin Definitions
  //Joystick control
  const int xPin = A0;  // Analog pin for X-axis (VRX)
  const int yPin = A2;  // Analog pin for Y-axis (VRY)
  const int Joy_button = A1;  // Digital pin for the push button (SW)

  //Servo 
  #define SERVO_PIN 9

  //NEMA 17
  #define EN_PIN 2    // LOW: Driver enabled, HIGH: Driver disabled
  #define STEP_PIN 9  // Step on the rising edge
  #define DIR_PIN 10  // Set stepping direction


//Constants
  //Joystick
  int xNeut = 517;
  int yNeut = 547;
  int max_joy_disp = 1023;
  //float xRange = max_joy_disp - xNeut;
  float yRange = max_joy_disp - yNeut;
  float xSensitivity = 150; //NEMA 17 Sensitivity
  float ySensitivity = 150; //Y Servo Sensitivity

  //Servo
  Servo yServo;
  float yAngle = 0;

  //NEMA 17
  int noOfSteps;
  int microSecondsDelay = 500;  // Delay in microseconds between each step


///////////////////////////////////////////
void setup() {
  //Joystick
  digitalWrite(Joy_button, INPUT);

  //Servo
  yServo.attach(SERVO_PIN);

  //NEMA 17
    // Configure pin modes
    pinMode(EN_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    // Initialize pin states
    digitalWrite(EN_PIN, LOW);   // Enable the driver
    digitalWrite(DIR_PIN, LOW);  // Set initial direction
}



////////////////////////////////////////
void loop() {
  //Joystick
    // Read the analog values from X and Y axes (returns a value from 0 to 1023)
    float xValue = analogRead(xPin) - xNeut;
    float yValue = (analogRead(yPin) - yNeut)/yRange;
    
    // Read the digital value from the push button (LOW = Pressed, HIGH = Released)
    int JoyButVal = analogRead(Joy_button);
    //JoyButVal = digitalRead(Joy_button);

  //Sensitivity
  yValue = yValue/ySensitivity;
  noOfSteps = abs(floor(xValue/xSensitivity));


  //Servo
    if((yAngle + yValue) > 0 && (yAngle + yValue) < 180){
    yServo.write(yAngle + yValue);
    yAngle = yAngle + yValue;
    }

  //NEMA 17
  if(xValue > 0){
    // Move motor in one direction
    digitalWrite(DIR_PIN, LOW);  // Set direction to LOW
    moveSteps(noOfSteps);
  }
  else if(xValue < 0){
    // Move motor in the opposite direction
    digitalWrite(DIR_PIN, HIGH); // Set direction to HIGH
    moveSteps(noOfSteps);
    }
}


///////////////////////////////////
void moveSteps(int steps) {
 for (int i = 0; i < steps; i++) {
   digitalWrite(STEP_PIN, HIGH);
   delayMicroseconds(microSecondsDelay);
   digitalWrite(STEP_PIN, LOW);
   delayMicroseconds(microSecondsDelay);
  }
 
 }

 