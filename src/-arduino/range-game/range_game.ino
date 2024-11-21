// https://docs.arduino.cc/learn/built-in-libraries/software-serial/
#include <Stepper.h>
#include <SoftwareSerial.h>
#include <string>
#include <iostream> // cout << "text here to console"

//-----------------------------
const int powerSwitch_pin = 1; // switch
const int powerLed_pin = 2; // if game is turned on?

const int gameStatusLed_pin = 3; // on/off visual LED indicator
const int gameStatusButton_pin = 3; // on/off start game

const int rangeFinderLed_pin = 4; // button to stop range finger
const int rangeFinderButton_pin = 5; // control range finder

const int maxSteps = 50950; // max number of times the stepper should rotate to prevent crash
const int stepperSpeed = 15;
//-----------------------------------

enum GameStatus {
  OFF = 0,
  CHILLIN,
  ACTIVE,
  FROZEN,
  RESET
};

enum GameStatus stat;

// stepper motor control ----------------------
/* 5V Stepper Motor - ULN2003 Driver Board
   Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence */
const int stepsPerRevolution = 2038;
Stepper rangeFinder = Stepper(stepsPerRevolution, 10, 12, 11, 13);


int rangeFinderPos_init = 0;
int rangeFinderPos = 0; // put read ir value here
int currentSteps = 0;

bool freezeOverride = false;

void setup() {
  Serial.begin(9600); // serial comm 9600 buad rate
  stat = RESET;
  // stepper motor pins set in library

  pinMode(powerSwitch_pin, INPUT); // power switch
  pinMode(powerLed_pin, OUTPUT); // power light indicator

  pinMode(gameStatusLed_pin, OUTPUT); // is in game light indicator
  pinMode(gameStatusButton_pin, INPUT); // enum states

  pinMode(rangeFinderLed_pin, OUTPUT); // led light for range finder control btn
  pinMode(rangeFinderButton_pin, INPUT); // RangeFinder Button
}

void loop() {
  // !!jaa: figure out how to read isGamePowered all the time
  isGamePowered = getPowerStatus();
  Serial.print("game power status: ");
  Serial.println(isGamePowered);


  switch (stat) {
    case GameStatus.OFF:
      Serial.println("game is turned off.");
      // listen for power on switch
      isGamePowered = getPowerStatus();
      if (isGamePowered == 1) {
        goto reset_label; // game just turned on
      }
      break;
    case GameStatus.CHILLIN:
      getPowerStatus();
      getStatusButtonStatus();
      // chillin at the bottom of the scale
      // -- check position of range finder if not at bottom goto RESET
      // if not at bottom goto reset or set enum to reset
      digitalWrite(gameStatusLed_pin, LOW); // game status LED = OFF or super slow blink??
      digitalWrite(rangeFinderLed_pin, HIGH);


      break;
    case GameStatus.ACTIVE:
      getPowerStatus();
      getStatusButtonStatus();
      // note: should we check current position to make sure at reset
      // turn on game button lights
      digitalWrite(gameStatusLed_pin, HIGH);
      digitalWrite(rangeFinderLed_pin, HIGH);
      // start moving RangeFinder
      moveStepper(150);
    case GameStatus.FROZEN:
      isGamePowered = getPowerStatus();
      // after game is active -> calls reset
      // slowly flash game control LED
      break;
    case GameStatus.RESET:
reset_label:
      getPowerStatus();
      getStatusButtonStatus();
      if (stat != GameStatus.RESET) {
        stat = GameStatus.RESET;
      }
      // quickly flash game control LED
      mode 
      digitalWrite(gameStatusLed_pin, HIGH);
      break;
  }
} // main loop


bool getPowerStatus() {
  bool powerSwitch = digitalRead(powerSwitch_pin) == 1;
  if (powerSwitch == 1) {
    stat = (powerSwitch == 1) ? GameStatus.Reset : GameStatus.OFF
  }
  return powerSwitch;
}

int moveStepper(int steps)
{
  // do some math here, don't need to pass in revolutions (could pass in the delay) then calculate how far it has moved?
  // could also use this method with up or down
  // add logic here for checking for not going lower than 0 steps
  if (currentSteps < maxSteps) {
    currentSteps += steps;
    rangeFinder.setSpeed(stepperSpeed);
    rangeFinder.step(-stepsPerRevolution);
    delay(150); // calculate this
  }
}

bool getRangeFinderButtonStatus() {
  // called when in active mode to check when to freeze motor
  bool isBtnPressed = digitalRead(rangeFinderButton_pin) == 1;
  if (stat = GameStatus.ACTIVE) { // only works when actively playing
    freezeOverride = true;
    stat =  (isBtnPressed == 1) ? GameStatus.FROZEN : GameStatus.ACTIVE;
  } else {
    // do nothing?
  }
  return isBtnPressed;
}

bool getStatusButtonStatus() {
  // returns if stat got changed
  bool isBtnPressed = digitalRead(gameStatusButton_pin) == 1;
  bool rtnVal = false;
  switch (stat) {
    case GameStatus.OFF:
      rtnVal = false;
      break;
    case GameStatus.CHILLIN:
      stat = GameStatus.ACTIVE;
      rtnVal = true;
      break;
    case GameStatus.ACTIVE:
      stat = GameStatus.FROZEN;
      rtnVal = true;
      break;
    case GameStatus.FROZEN:
      stat = GameStatus.RESET;
      rtnVal = true;
      break;
    case GameStatus.RESET:
      // continue reseting?
      rtnVal = false
               break;
  }
  return rtnVal;
}

/*
  switch (stat) {
    case GameStatus.OFF:
      break;
    case GameStatus.CHILLIN:
      break;
    case GameStatus.ACTIVE:
      break;
    case GameStatus.FROZEN:
      break;
    case GameStatus.RESET:
      break;
  }
*/