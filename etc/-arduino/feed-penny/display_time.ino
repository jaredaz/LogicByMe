#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// set the display settings
const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 906, TS_RT = 116, TS_TOP = 92, TS_BOT = 952;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button up_btn, down_btn, switch_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars


int count = 0; // for setting HH:MM:ss 0= hours, 1=mins

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

unsigned long time = 0;
int hour_offset, min_offset, sec_offset = 0;

String getTime(unsigned long tm)
{
  int secs,  mins, hours;
  String secStr, minStr, hourStr, amPmStr;
  bool am_pm = false; // false = am, true = pm;  

  secs = ((tm % 3600000) % 60000) / 1000;
  mins = ((tm % 3600000) / 60000) + min_offset;

  while (mins >= 60)
  {
    hour_offset += 1;
    mins -= 60  ;
  }
  hours = (tm / 3600000) + hour_offset;

  if (hours > 12)
  {
    am_pm = am_pm ? false : true;
    hours -= 12;  
  }
  else
  {
    am_pm = false;
  }

  hourStr = hours < 9 ? "0" + String(hours) : String(hours);
  minStr = mins < 9 ? "0" + String(mins) : String(mins);
  secStr = mins < 9 ? "0" + String(secs) : String(secs);
  amPmStr = am_pm ? "PM" : "AM";
  
  return hourStr + ":" + minStr + "." + secStr + amPmStr;
}

void set_hours_plus(){
  hour_offset += 1;  
}
void set_hours_minus(){
  hour_offset -= 1;  
}
void set_mins_plus(){
  min_offset += 1;  
}
void set_mins_minus(){
  min_offset -= 1;  
}

bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, tft.width(), 0); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, tft.height(), 0);
    }
    return pressed;
}

void setup(void)
{
    Serial.begin(9600); 
    
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    tft.fillScreen(BLACK);

    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(2);     // System font is 8 pixels.  ht = 8*2=16
    tft.setCursor(150, 40);
    tft.print("00:00");

    
    up_btn.initButton(&tft,  60, 200, 100, 40, WHITE, CYAN, BLACK, "UP", 2);
    down_btn.initButton(&tft, 60, 250, 100, 40, WHITE, CYAN, BLACK, "DOWN", 2);
    switch_btn.initButton(&tft, 170, 200, 50, 90, WHITE, CYAN, BLACK, "->", 2);

    up_btn.drawButton(false);
    down_btn.drawButton(false);
    switch_btn.drawButton(false);

    tft.fillRect(40, 300, 160, 80, RED);
    tft.setTextColor(WHITE, BLACK);
};


void loop(void)
{

    bool down = Touch_getXY();
    up_btn.press(down && up_btn.contains(pixel_x, pixel_y));
    down_btn.press(down && down_btn.contains(pixel_x, pixel_y));
    switch_btn.press(down && switch_btn.contains(pixel_x, pixel_y));

    if (up_btn.justReleased()){
        down_btn.drawButton(false);
    }
    if (down_btn.justReleased()){
        down_btn.drawButton(false);
    }
    if (switch_btn.justReleased()){
        switch_btn.drawButton(false);
    }
    
    if (up_btn.justPressed()) {
        up_btn.drawButton(true);
        tft.fillRect(40, 300, 160, 80, GREEN);
        if (count == 0){set_hours_plus();}
        if (count == 1){set_mins_plus();}
    }
    if (down_btn.justPressed()) {
        down_btn.drawButton(true);  
        tft.fillRect(40, 300, 160, 80, YELLOW);  
        if (count == 0){set_hours_minus();}
        if (count == 1){set_mins_minus();}
    }
    if (switch_btn.justPressed()) {
        switch_btn.drawButton(true);  
        tft.fillRect(40, 300, 160, 80, MAGENTA);  
        count++;
    }

    tft.setCursor(150, 40);
    tft.setTextColor(WHITE, BLACK);
    tft.print(getTime(millis()));
}
