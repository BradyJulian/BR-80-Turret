//BR-80 Turret

//Libraries
#include <Servo.h>
#include <Stepper.h>



//Constants
  //Joystick
  int fireState;
  int xNeut = 517;
  int yNeut = 547;
  int max_joy_disp = 1023;
  float xRange = max_joy_disp - xNeut;
  float yRange = max_joy_disp - yNeut;
  float xSensitivity = 500; //NEMA 17 Sensitivity
  float xTraSensitivity; //NEMA 17 delay Sensitivity
  float ySensitivity = 250; //Y Servo Sensitivity

  //Servo
  Servo yServo;
  float yAngle = 0;

  //NEMA 17
  int noOfSteps, steps_per_rev, current_step;
  int microSecondsDelay;  // Delay in microseconds between each step

  //Firing
  const int stepsPerRevolutionTrig = 2048; // Defines the number of steps per rotation for trigger motor
  bool calAct = 1; //setting calibration requirent on at the start of the code
  int calState; //stores value of trigger motor



//Pin Definitions
  //Joystick control
  #define Joy_button A0  // Digital pin for the push button (SW)
  #define yPin A1  // Analog pin for Y-axis (VRY)
  #define xPin A2  // Analog pin for X-axis (VRX)

  //Servo 
  #define SERVO_PIN 3

  //NEMA 17
  #define EN_PIN 2    // LOW: Driver enabled, HIGH: Driver disabled
  #define STEP_PIN 6  // Step on the rising edge
  #define DIR_PIN 7  // Set stepping direction

  //Firing
  #define Trig_cal 12 //microswitch to calibrate trigger motor position
  #define MOSFET 4 //MOSFET triggers flywheel motors
  Stepper myStepper = Stepper(stepsPerRevolutionTrig, 8, 10, 9, 11); // Creates an instance of stepper class
  // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
  // IN1 = 8, IN2 = 9, IN3 = 10, IN4 = 11



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


  //Firing
  pinMode(Trig_cal, INPUT);
  pinMode(Joy_button, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT); //using built-in LED to troubleshoot
  pinMode(MOSFET, OUTPUT);
  digitalWrite(MOSFET, LOW);

    //setting stepper motor speed
    myStepper.setSpeed(15);
}



////////////////////////////////////////
void loop() {
  //Trigger motor calibration -- returns dart pusher to retracted position upon startup or after firing
  if(calAct == 1){
    //Stepper motor will move CCW by one step at a time until the microswitch is pressed
    do{
    calState = digitalRead(Trig_cal); //reading the microswitch
    myStepper.step(-1); //1 step CCW
    }while(calState != LOW); 

    calAct = 0; //removing calibration requirement
  } 


  //Joystick
    // Read the analog values from X and Y axes (returns a value from 0 to 1023)
    float xValue = analogRead(xPin) - xNeut;
    float yValue = (analogRead(yPin) - yNeut)/yRange;
    
    //reading the joystick's button to determine whether firing sequence should be entered
    fireState = digitalRead(Joy_button);


  if(fireState == HIGH) //Button released -- positioning
  { 
    //Sensitivity
      //X axis - NEMA 17
        xTraSensitivity = abs(xValue)*2;
        microSecondsDelay = 1500 - xTraSensitivity;
        noOfSteps = abs(floor(xValue/xSensitivity));
      //Y axis - Servo
        yValue = yValue/ySensitivity;


      float temp_angle = yAngle + yValue;
      //Servo
        if(temp_angle > 0 && temp_angle < 180){
          yAngle = temp_angle;
          yServo.write(yAngle);
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
  else if(fireState == LOW) //Button pressed -- firing sequence
  {
      //--~~ Firing Sequence ~~--
      digitalWrite(LED_BUILTIN, HIGH); // LED on if button is pressed
      digitalWrite(MOSFET, HIGH);
      delay(1500); //letting flywheels rev up

      // Rotate CW 180 degrees (pushing dart out of barrel)
      myStepper.step(stepsPerRevolutionTrig*0.40);
      delay(100);

      calAct = 1; //requires code to re calibrate at the start of the loop
      digitalWrite(MOSFET, LOW);
      digitalWrite(LED_BUILTIN, LOW);  // LED is off unless button is pressed
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
