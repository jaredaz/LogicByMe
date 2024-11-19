
#include <Servo.h> 

Servo five;
Servo six;
Servo seven;
Servo ten;
Servo eleven;
Servo twelve;
 int pos;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); 

five.attach(5);
six.attach(9);
seven.attach(4);
ten.attach(10);
eleven.attach(11);
twelve.attach(12);
}

void loop() {
  // put your main code here, to run repeatedly:


five.write(120);
six.write(90);
seven.write(90);
ten.write(90);
eleven.write(90);
twelve.write(90);
delay(1000);
/*
five.write(90);
six.write(90);
seven.write(90);
ten.write(90);
eleven.write(90);
twelve.write(90);
delay(1000);
*/

}
