/*
 * EE490 Homework/ project #2
 * Due : Septermber 13, 2018 at 8am
 * 
 * 2. Write Arduino code to read the input state of pin 3 
 *    and output the value to the onboard LED.
 *    a. create hardware that causes pin 3 to read high value
 *       when a button switch is pushed and low value when the
 *       switch isn't pushed
 *    b. compile and verify the code.
 *    c. download the code to your Arduino.
 *    d. send a video of your completed project tp your instructor
 *       or you can demonstrate it in person.
 */


const int button_switch = 3;       // assigns pin 3 as the inputPin from the button switch
const int led_onboard = 13;        // assigns pin 13 to the onboardBoard LED

void setup() 
{
  pinMode(button_switch, INPUT);   // assigns the button_switch as an input signal 
  pinMode(led_onboard, OUTPUT);    // assigns the onboardLED as an output signal
}

void loop() 
{
  // writes a digital signal to the onboard LED that is determined on the logical 
  //   state of the digitalread value of button_switch. Uses ternary operator
  digitalWrite( led_onboard, digitalRead(button_switch) ? HIGH : LOW );
  delay(100);
}
