

float input_voltage = 0.0;
float temp=0.0;

const int analogIn = A1;
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module


void setup()
{
   Serial.begin(9600);     //  opens serial port, sets data rate to 9600 bps
//   lcd.begin(16, 2);       //// set up the LCD's number of columns and rows: 
//   lcd.print("DIGITAL VOLTMETER");
}
void loop()
{

//Conversion formula for voltage
   
   int analog_value = analogRead(A0);
   input_voltage = (analog_value * 5.0) / 1024.0; 

   
   if (input_voltage < 0.1) 
   {
     input_voltage=0.0;
   } 
   Serial.println("My voltameter");
    Serial.print("v= ");
    Serial.println(input_voltage);

 
     RawValue = analogRead(analogIn);
     Voltage = (RawValue / 1023.0) * 5000; // Gets you mV
     Amps = ((Voltage - ACSoffset) / mVperAmp); 
     Serial.print("Current = "); // shows the current measured
     Serial.println(Amps,3); // the '3' after voltage allows you to display 3 digits after decimal point
     delay(1000);    
  
    
//    lcd.setCursor(0, 1);
//    lcd.print("Voltage= ");
//    lcd.print(input_voltage);

}
