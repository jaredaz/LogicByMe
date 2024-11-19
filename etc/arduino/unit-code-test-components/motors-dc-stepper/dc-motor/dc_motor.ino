#include <Time.h>

int motorPin = 5;
const unsigned long event1=2000;
const unsigned long event2=10000;
long int t1 = 0;
  long int t2 = 0;
  
void setup() {
  Serial.begin(9600);
  pinMode(5,OUTPUT);
  long int t1 = millis();
}

void loop() {
  digitalWrite(motorPin, LOW);
//  t2 = millis();
//
//  if( t2 - t1 >= 2000 )
//  {
//    t1 = t2;
//    digitalWrite(motorPin, LOW);
//  }
}
