/*
 * The Mukava by Nissnass temperature controller
 * 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * 
 * TMP36 temperature sensor analogue A0
 * TMP36 temperature sensor analogue A0
 * 
*/

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int sensePin = A0;  //This is the Arduino Pin that will read the sensor output
int temp2Input;
int temp1Input;    //The variable we will use to store the sensor input


const int arrSize = 10;
int counter = 0;
// Hot side
double tempHotF;
double tempHotC;
double tempHotMax;
double tempHotMin;
double tempHotArray[arrSize];
double tempHotAvg;
// Cold side
double tempColdF;
double tempColdC;
double tempColdMax;
double tempColdMin;
double tempColdArray[arrSize];
double tempColdAvg;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(" Temp Sensor");
  lcd.setCursor(0,1); lcd.print("  by Nissnass ");
  delay(1500);
  lcd.clear();
  lcd.print("CurTF:C:Avg:Max");
  lcd.setCursor(0,1); lcd.print("CurTF:C:Avg:Min");
  delay(1000);

  tempHotMin =  99;
  tempColdMin = 99;
  
  Serial.begin(9600);
  Serial.println("\n");
  Serial.println("\t\t\t\t=========================================");
  Serial.println("\t\t\t\t==                                     ==");
  Serial.println("\t\t\t\t==     Temperature Readings            ==");
  Serial.println("\t\t\t\t==            provided by Nissnass     ==");
  Serial.println("\t\t\t\t==                                     ==");
  Serial.println("\t\t\t\t=========================================");
  Serial.print("---(1)--->TMP36 Sensor 1:");   Serial.println("\t\t\t\t\t\t\t\t\t\t\t      ---(2)--->TMP36 Sensor 2:");
}

void loop() {
  // get temperature signal
  temp1Input = analogRead(A0);    //read the analog sensor and store it
  temp2Input = analogRead(A1);    //read the analog sensor and store it
  // calculate temperature
  tempHotC = ((((double) temp1Input / 1024) * 5 ) - 0.5) * 100;
  tempColdC = ((((double)temp2Input / 1024) * 5 ) - 0.5) * 100;       //find percentage of input reading
  // convert to degrees Fagrenheight
  tempHotF =  ( tempHotC  * 1.8)  + 32; 
  tempColdF = ( tempColdC * 1.8) + 32; 
  // set min and max temps
  tempHotMax = ( tempHotF > tempHotMax) ? tempHotF : tempHotMax ;
  tempHotMin = ( tempHotF < tempHotMin) ? tempHotF : tempHotMin ;

   tempColdMax = ( tempColdF > tempColdMax) ? tempColdF : tempColdMax ;
   tempColdMin = ( tempColdF < tempColdMin) ? tempColdF : tempColdMin ;

  // save for average
  tempHotArray[counter] = tempHotF;
  tempColdArray[counter] = tempColdF;
  
  counter = ( counter == arrSize ) ? 0 : counter; // check if over array size
  double tempHAvg = 0;
  double tempCAvg = 0;
  int tempMax = 0;
  for (int idx = 0; idx < arrSize; idx++)
  {
     if (tempHotArray[idx] == 0)
        { break;}
     else
     { tempMax++; tempHAvg += tempHotArray[idx]; tempCAvg += tempColdArray[idx]; }
  }
  counter++;
  // assigns average values
  tempHotAvg = tempHAvg / tempMax;
  tempColdAvg = tempCAvg / tempMax;
  
  ////////////////////////////
  lcd.clear();    // clears the lcd

  // first line
  lcd.setCursor(0, 0);
  lcd.print(tempHotF,1);   lcd.print(":"); lcd.print(tempHotC,0);   lcd.print(":"); lcd.print(tempHotAvg,1); lcd.print(":"); lcd.print(tempHotMax,0);
  // second line
  lcd.setCursor(0, 1);    // bottom left
  lcd.print(tempColdF,1);  lcd.print(":"); lcd.print(tempColdC,0);  lcd.print(":"); lcd.print(tempColdAvg,1); lcd.print(":"); lcd.print(tempColdMin,0);

  int dec = 1;
  //Print to serial
    Serial.print("---(1)--->Fahrenheit : "); Serial.print(tempHotF, dec);
    Serial.print(" : Celsius : ");Serial.print(tempHotC, dec);
    Serial.print(" : MaxF: ");Serial.print(tempHotMax, dec);  
    Serial.print(" : MinF: ");Serial.print(tempHotMin, dec);
    Serial.print(" : AvgF : ");Serial.print(tempHotAvg, dec);  
    Serial.print(" : AvgArr : "); Serial.print("[");
    for (int idx = 0; idx < arrSize; idx++){Serial.print(tempHotArray[idx],0); (idx == arrSize -1)? Serial.print("]") : Serial.print(",");}   

    Serial.print("---(2)--->Fahrenheit: "); Serial.print(tempColdF);
    Serial.print(" : Celsius : ");Serial.print(tempColdC);
    Serial.print(" : MaxF : ");Serial.print(tempColdMax);  
    Serial.print(" : MinF : ");Serial.print(tempColdMin);
    Serial.print(" : AvgF : ");Serial.print(tempColdAvg);  
    Serial.print(" : AvgArr : "); Serial.print("[");
    for (int idx = 0; idx < arrSize; idx++){Serial.print(tempColdArray[idx],0); (idx == arrSize -1)? Serial.print("]") : Serial.print(",");}   

  Serial.println();

/*
  //Print to serial
  Serial.println("TMP36 Sensor 1:");
    Serial.print("\tFahrenheit: "); Serial.println(tempHotF);
    Serial.print("\tCelsius   : ");Serial.println(tempHotC);
    Serial.print("\tF Max Temp: ");Serial.println(tempHotMax);  
    Serial.print("\tF Min Temp: ");Serial.println(tempHotMin);
    Serial.print("\tF Avg     : ");Serial.println(tempHotAvg);  
    Serial.print("\tAvg Array : "); Serial.print("[");
    for (int idx = 0; idx < arrSize; idx++){Serial.print(tempHotArray[idx],1); (idx == arrSize -1)? Serial.println("]") : Serial.print(", ");}   
  Serial.println("TMP36 Sensor 2:");
    Serial.print("\tFahrenheit: "); Serial.println(tempColdF);
    Serial.print("\tCelsius   : ");Serial.println(tempColdC);
    Serial.print("\tF Max Temp: ");Serial.println(tempColdMax);  
    Serial.print("\tF Min Temp: ");Serial.println(tempColdMin);
    Serial.print("\tF Avg     : ");Serial.println(tempColdAvg);  
    Serial.print("\tAvg Array : "); Serial.print("[");
    for (int idx = 0; idx < arrSize; idx++){Serial.print(tempColdArray[idx],1); (idx == arrSize -1)? Serial.println("]") : Serial.print(", ");}   

*/
//    lcd.scrollDisplayLeft();  // scroll one space left
   
  delay(500);
}
