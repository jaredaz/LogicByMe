#include <Wire.h>

void setup() {
  Wire.begin();

  Serial.begin(9600);
  while (!Serial); // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");


  Wire.beginTransmission( 0x3C);
int error = Wire.endTransmission();
if( error != 0)
{
  Serial.println( "The sensor is not at 0x3C");
}
else
{
  Serial.println( "The sensor is found, changing I2C address");
  Wire.beginTransmission( 0x3C);
  Wire.write( 0x53);  // password register
  Wire.write( 0xAA);  // password
  Wire.endTransmission();

  delay(10);    // not described somewhere, just for safety

  Wire.beginTransmission( 0x3C);
  Wire.write( 0x00);  // I2C address register
  Wire.write( 0x3E);  // new I2C address
  Wire.endTransmission();
}
}

void loop() {
//  int nDevices = 0;
//
//  Serial.println("Scanning...");
//
//  for (byte address = 1; address < 127; ++address) {
//    // The i2c_scanner uses the return value of
//    // the Write.endTransmisstion to see if
//    // a device did acknowledge to the address.
//    Wire.beginTransmission(address);
//    byte error = Wire.endTransmission();
//
//    if (error == 0) {
//      Serial.print("I2C device found at address 0x");
//      if (address < 16) {
//        Serial.print("0");
//      }
//      Serial.print(address, HEX);
//      Serial.println("  !");
//
//      // change address maybe??
//      Wire.write( 0x3C << 1);
//      Serial.print("changed");
//      Serial.println("  !");
//
//      ++nDevices;
//    } else if (error == 4) {
//      Serial.print("Unknown error at address 0x");
//      if (address < 16) {
//        Serial.print("0");
//      }
//      Serial.println(address, HEX);
//    }
//  }
//  if (nDevices == 0) {
//    Serial.println("No I2C devices found\n");
//  } else {
//    Serial.println("done\n");
//  }
//  delay(5000); // Wait 5 seconds for next scan

  
 }