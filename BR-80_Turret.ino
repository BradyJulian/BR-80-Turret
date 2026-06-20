//BR-80 Turret

//Libraries
#include <Stepper.h>

//Global Variables
const int stepsPerRevolution = 2048; // Defines the number of steps per rotation
bool calAct = 1; //setting calibration requirent on at the start of the code
int calState; //stores value of trigger motor
int fireState; //stores value of trigger button

//Pin definitions
#define Trig_cal A0 //microswitch to calibrate trigger motor position
#define Joy_button A1 //Joystick button press - triggers firing sequence
#define MOSFET 2 //MOSFET triggers flywheel motors
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11); // Creates an instance of stepper class
  // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence


void setup() {
  //Pin assignments
  pinMode(Trig_cal, INPUT);
  pinMode(Joy_button, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT); //using built-in LED to troubleshoot
  pinMode(MOSFET, OUTPUT);
  digitalWrite(MOSFET, LOW);

  //setting stepper motor speed
  myStepper.setSpeed(15);
}



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

  //reading the joystick's button to determine whether firing sequence should be entered
  fireState = digitalRead(Joy_button); 

  if(fireState == LOW) {
    //--~~ Firing Sequence ~~--
    digitalWrite(LED_BUILTIN, HIGH); // LED on if button is pressed
    digitalWrite(MOSFET, HIGH);
    delay(1500); //letting flywheels rev up

    // Rotate CW 180 degrees (pushing dart out of barrel)
    myStepper.step(stepsPerRevolution*0.55);
    delay(100);

    calAct = 1; //requires code to re calibrate at the start of the loop
    digitalWrite(MOSFET, LOW);

  } else {
    digitalWrite(LED_BUILTIN, LOW);  // LED is off unless button is pressed
    digitalWrite(MOSFET, LOW);
  }
}
