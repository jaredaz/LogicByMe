#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#include <Stepper.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// -Color Variables-----------------------------------
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define PINK 0xF81F

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// -Stepper Variables-----------------------------------
bool isDispensing = false;
const int stepsPerRevolution = 2038; // Defines the number of steps per rotation

bool moveMotor = false; 

int dispense_time = 2500;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 10, 12,11, 13);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~DISPLAY VARIABLES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Show Screen ----> 0=home, 1=Menu, 2=setDispenseTime, 3=deleteDispenseTime, 
// 4=DispenseTime, 5=SetClock, 6=about
int show_screen = 2; 

bool islistening = false; // true=listeningFunction, false=setUpFunction

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~Screen Variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//touch sensitivity for press
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~set the display settings~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 952, TS_RT = 92, TS_TOP = 906, TS_BOT = 92;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// menu icon top left corner to activate menu screen
Adafruit_GFX_Button menu_btn;
Adafruit_GFX_Button menu_0_btn, menu_2_btn, menu_3_btn, menu_4_btn, menu_5_btn, menu_6_btn;
// home page buttons
Adafruit_GFX_Button up_btn, down_btn, switch_btn;
// set clock buttons
Adafruit_GFX_Button up_clock_hour_btn, down_clock_hour_btn, up_clock_min_btn, down_clock_min_btn, up_clock_ampm_btn, down_clock_ampm_btn, set_clock_btn;
// set about buttons
Adafruit_GFX_Button feed_NOW_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars


int count = 0; // for setting HH:MM:ss 0= hours, 1=mins


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~Clock Variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

unsigned long time = 0;
int hour_offset, min_offset, sec_offset = 0;
bool amPm_master = false;

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

void setAmPm()
{
  amPm_master = !amPm_master;
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
        pixel_x = map(p.y, TS_TOP, TS_BOT, tft.width(), 0);  // for rotate(3) LANDSCAPE 270deg
        pixel_y = map(p.x, TS_LEFT, TS_RT, tft.height(), 0); // for rotate(3) LANDSCAPE 270deg
    }
    if(pressed){
      Serial.print("touched!:");
      Serial.println(pressed);
      Serial.print("X:");
      Serial.print(p.x);
      Serial.print(", Y:");
      Serial.println(p.y);

      Serial.print("tft.width(");
      Serial.print(tft.width());
      Serial.print("), tft.height(");
      Serial.print(tft.height());
      Serial.println(")");
    }
    return pressed;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~SHOW MENU BUTTON~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void dispense_food_now() //move_motor
{
  if (isDispensing){
    // Rotate CW 15 RPM
    myStepper.setSpeed(15);
    myStepper.step(stepsPerRevolution);
    delay(dispense_time);
    isDispensing = false;
  }
}

void show_munu_btn()
{
  // menu btn top left
  menu_btn.initButton(&tft, 33, 33, 50, 50, BLACK, NAVY, WHITE, "MENU", 1);
  menu_btn.drawButton(false);
}

void show_munu_btn_listener(bool down)
{
    menu_btn.press(down && menu_btn.contains(pixel_x, pixel_y));

    // check if released
    if (menu_btn.justReleased()){
        menu_btn.drawButton(false);
        show_screen = 1; //open menu
        islistening = false;
    }
  
    // check if pressed
    if (menu_btn.justPressed()) {
        menu_btn.drawButton(true);
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~DISPLAY MENU PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void display_menu()
{
  islistening = true;
  tft.fillScreen(BLACK);
   tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);     // System font is 8 pixels.  ht = 8*2=16
  tft.setCursor(150, 40);
  tft.print("TEST123456");
  // menu options
  menu_0_btn.initButton(&tft, 210, 33, 350, 50, BLACK, NAVY, WHITE, "HOME", 2);
  menu_0_btn.drawButton(false);
//  menu_1_btn.initButton(&tft, 210, 96, 350, 50, BLACK, NAVY, WHITE, "MENU", 2);
//  menu_1_btn.drawButton(false);
  menu_2_btn.initButton(&tft, 210, 96, 350, 50, BLACK, NAVY, WHITE, "SCHEDULE", 2);
  menu_2_btn.drawButton(false);
  menu_3_btn.initButton(&tft, 210, 149, 350, 50, BLACK, NAVY, WHITE, "DELETE", 2);
  menu_3_btn.drawButton(false);
  menu_4_btn.initButton(&tft, 210, 200, 350, 50, BLACK, NAVY, WHITE, "FOOD SIZE", 2);
  menu_4_btn.drawButton(false);
  menu_5_btn.initButton(&tft, 210, 250, 350, 50, BLACK, NAVY, WHITE, "SET CLOCK", 2);
  menu_5_btn.drawButton(false);
  menu_6_btn.initButton(&tft, 210, 300, 350, 50, BLACK, NAVY, WHITE, "ABOUT", 2);
  menu_6_btn.drawButton(false);
  
}

void display_menu_listener()
{
  bool down = Touch_getXY(); 
  
  menu_0_btn.press(down && menu_0_btn.contains(pixel_x, pixel_y));
  menu_2_btn.press(down && menu_2_btn.contains(pixel_x, pixel_y));
  menu_3_btn.press(down && menu_3_btn.contains(pixel_x, pixel_y));
  menu_4_btn.press(down && menu_4_btn.contains(pixel_x, pixel_y));
  menu_5_btn.press(down && menu_5_btn.contains(pixel_x, pixel_y));
  menu_6_btn.press(down && menu_6_btn.contains(pixel_x, pixel_y));

  // btn released
  if (menu_0_btn.justReleased()){
      menu_0_btn.drawButton(false);
  }
  if (menu_2_btn.justReleased()){
      menu_2_btn.drawButton(false);
  }
  if (menu_3_btn.justReleased()){
      menu_3_btn.drawButton(false);
  }
  if (menu_4_btn.justReleased()){
      menu_4_btn.drawButton(false);
  }
  if (menu_5_btn.justReleased()){
      menu_5_btn.drawButton(false);
  }
  if (menu_6_btn.justReleased()){
      menu_6_btn.drawButton(false);
  }
  
  // btn pressed
  if (menu_0_btn.justPressed()) {
      menu_0_btn.drawButton(true);

      show_screen = 0;
      islistening = false;
  }
  if (menu_2_btn.justPressed()) {
      menu_2_btn.drawButton(true);

      show_screen = 2;
      islistening = false;
  }
  if (menu_3_btn.justPressed()) {
      menu_3_btn.drawButton(true);

      show_screen = 3;
      islistening = false;
  }
  if (menu_4_btn.justPressed()) {
      menu_4_btn.drawButton(true);

      show_screen = 4;
      islistening = false;
  }
  if (menu_5_btn.justPressed()) {
      menu_5_btn.drawButton(true);

      show_screen = 5;
      islistening = false;
  }
  if (menu_6_btn.justPressed()) {
      menu_6_btn.drawButton(true);

      show_screen = 6;
      islistening = false;
  }
  
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~DISPLAY HOME PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void display_home()
{
  islistening = true;
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

  
  show_munu_btn();

  
  tft.fillRect(40, 300, 160, 80, RED);
  tft.setTextColor(WHITE, BLACK);  
}

void display_home_listener()
{
  bool down = Touch_getXY();
  up_btn.press(down && up_btn.contains(pixel_x, pixel_y));
  down_btn.press(down && down_btn.contains(pixel_x, pixel_y));
  switch_btn.press(down && switch_btn.contains(pixel_x, pixel_y));

  if (up_btn.justReleased()){
      up_btn.drawButton(false);
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

  show_munu_btn_listener(down);
  
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~SET CLOCK PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int hours = 0;
int mins = 0;
bool amPm = false; // false=am, true=pm

void display_setClock()
{
  // set clock buttons
//Adafruit_GFX_Button up_clock_hour_btn, down_clock_hour_btn, 
//          up_clock_min_btn, down_clock_min_btn, up_clock_ampm_btn,
//          down_clock_ampm_btn, set_clock_btn;
  islistening = true;
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);

  tft.setTextColor(WHITE, DARKGREY);  
  tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
  tft.setCursor(90, 111);
  tft.print("00:00 AM");
  
  // 30x48
  // initBtn(&tft, x-init, y-init, width, height, outline, fill, textColor, *label, text size)
  // hour modifier
  up_clock_hour_btn.initButton(&tft,  126, 76, 90, 50, WHITE, CYAN, BLACK, "UP", 2);
  up_clock_hour_btn.drawButton(false);
  down_clock_hour_btn.initButton(&tft, 126, 195, 90, 50, WHITE, CYAN, BLACK, "DOWN", 2);
  down_clock_hour_btn.drawButton(false);
  // minute modified
  up_clock_min_btn.initButton(&tft, 231, 76, 90, 50, WHITE, CYAN, BLACK, "UP", 2);
  up_clock_min_btn.drawButton(false);
  down_clock_min_btn.initButton(&tft, 231, 195, 90, 50, WHITE, CYAN, BLACK, "DOWN", 2);
  down_clock_min_btn.drawButton(false);
  // am/pm modifier
  up_clock_ampm_btn.initButton(&tft, 341, 76, 90, 50, WHITE, CYAN, BLACK, "UP", 2);
  up_clock_ampm_btn.drawButton(false);
  down_clock_ampm_btn.initButton(&tft, 341, 195, 90, 50, WHITE, CYAN, BLACK, "DOWN", 2);
  down_clock_ampm_btn.drawButton(false);

  set_clock_btn.initButton(&tft, 395, 280, 110, 60, WHITE, GREEN, BLACK, "SET", 2);
  set_clock_btn.drawButton(false);

  show_munu_btn();
}


void display_setClock_listener()
{
  bool down = Touch_getXY();
  up_clock_hour_btn.press(down && up_clock_hour_btn.contains(pixel_x, pixel_y));
  down_clock_hour_btn.press(down && down_clock_hour_btn.contains(pixel_x, pixel_y));
  up_clock_min_btn.press(down && up_clock_min_btn.contains(pixel_x, pixel_y));
  down_clock_min_btn.press(down && down_clock_min_btn.contains(pixel_x, pixel_y));
  up_clock_ampm_btn.press(down &&  up_clock_ampm_btn.contains(pixel_x, pixel_y));
  down_clock_ampm_btn.press(down &&  down_clock_ampm_btn.contains(pixel_x, pixel_y));
  set_clock_btn.press(down && set_clock_btn.contains(pixel_x, pixel_y));

  // check if released
  if (up_clock_hour_btn.justReleased()){
      up_clock_hour_btn.drawButton(false);
  }
  if (down_clock_hour_btn.justReleased()){
      down_clock_hour_btn.drawButton(false);
  }
  if (up_clock_min_btn.justReleased()){
      up_clock_min_btn.drawButton(false);
  }
  if (down_clock_min_btn.justReleased()){
      down_clock_min_btn.drawButton(false);
  }
  if (up_clock_ampm_btn.justReleased()){
      up_clock_ampm_btn.drawButton(false);
  }
  if (down_clock_ampm_btn.justReleased()){
      down_clock_ampm_btn.drawButton(false);
  }

  // check if pressed
  if (up_clock_hour_btn.justPressed()) {
      up_clock_hour_btn.drawButton(true);
//      set_hours_plus();
      hours++;
  }
  if (down_clock_hour_btn.justPressed()) {
      down_clock_hour_btn.drawButton(true);
//      set_hours_minus();
      hours--;
  }
  if (up_clock_min_btn.justPressed()) {
      up_clock_min_btn.drawButton(true);
//      set_mins_plus();
      mins++;
  }
  if (down_clock_min_btn.justPressed()) {
      down_clock_min_btn.drawButton(true);
//      set_mins_minus();
      mins--;
  }
    if (up_clock_ampm_btn.justPressed()) {
      up_clock_ampm_btn.drawButton(true);
      amPm = !amPm;
  }
  if (down_clock_ampm_btn.justPressed()) {
      down_clock_ampm_btn.drawButton(true);
      amPm = !amPm;
  }
  if (set_clock_btn.justPressed()) {
      set_clock_btn.drawButton(true);
      show_screen = 0; // go to home screen
      islistening = false;
  }

  show_munu_btn_listener(down);

  tft.setTextColor(WHITE, DARKGREY);  
  tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
  tft.setCursor(90, 111);
  tft.print(hours);
  
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ABOUT PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void display_about()
{
  islistening = true;

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  
  feed_NOW_btn.initButton(&tft, 100, 255, 100, 100, BLACK, NAVY, WHITE, "FEED", 2);
  feed_NOW_btn.drawButton(false);

  show_munu_btn();
  
}

void display_about_listener()
{
  bool down = Touch_getXY(); 
  feed_NOW_btn.press(down && feed_NOW_btn.contains(pixel_x, pixel_y));
  
  
  if (feed_NOW_btn.justReleased()){
        feed_NOW_btn.drawButton(false);
    }
  
  if (feed_NOW_btn.justPressed()) {
        feed_NOW_btn.drawButton(true);
        isDispensing = true;
    }
  
  show_munu_btn_listener(down);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~DELETE CLOCK SCHEDULE PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void display_deleteSchedule()
{
  islistening = true;

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  
  feed_NOW_btn.initButton(&tft, 100, 255, 100, 100, BLACK, NAVY, WHITE, "FEED", 2);
  feed_NOW_btn.drawButton(false);

  show_munu_btn();
  
}

void display_deleteSchedule_listener()
{
  bool down = Touch_getXY(); 
  feed_NOW_btn.press(down && feed_NOW_btn.contains(pixel_x, pixel_y));
  
  
  if (feed_NOW_btn.justReleased()){
        feed_NOW_btn.drawButton(false);
    }
  
  if (feed_NOW_btn.justPressed()) {
        feed_NOW_btn.drawButton(true);
        isDispensing = true;
    }
  
  show_munu_btn_listener(down);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~setDispenseTime PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void display_setDispenseTime()
{
  islistening = true;

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  
  example_btn.initButton(&tft, 100, 255, 100, 100, BLACK, NAVY, WHITE, "FEED", 2);
  example_btn.drawButton(false);

  show_munu_btn();
  
}

void display_setDispenseTime_listener()
{
  bool down = Touch_getXY(); 
  example_btn.press(down && feed_NOW_btn.contains(pixel_x, pixel_y));
  
  
  if (feed_NOW_btn.justReleased()){
        example_btn.drawButton(false);
    }
  
  if (example_btn.justPressed()) {
        example_btn.drawButton(true);
        isDispensing = true;
    }
  
  show_munu_btn_listener(down);
}


//   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   ~~START PROGRAM HERE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup(void) {
  Serial.begin(9600); 
    
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(3);            //0=PORTRAIT, 1=landscape
    tft.fillScreen(BLACK);
}


void loop(void) {

  if (isDispensing)
  {
    dispense_food_now();  
  }

  switch (show_screen) {
    case 0: // home/idle screen
      if (!islistening)
      {
        display_home();
        islistening = true;
      } 
      else 
      {
        display_home_listener();
      }
      break;
    case 1: // menu screen
      if (!islistening)
      {
        display_menu();
      }
      else
      {
        display_menu_listener();
      }
      break;
    case 2: // SET/ADD schedule
      
      if (!islistening)
      {
        display_setClock();
        islistening = true;
      } 
      else 
      {
        display_setClock_listener();
      }
      break;
    case 3: // DELETE schedule
      if (!islistening)
      {
        display_deleteSchedule();
        islistening = true;
      } 
      else 
      {
        display_deleteSchedule_listener();
      }
      break;
    case 4: // SET DOG FOOD DISPENSING TIME
      if (!islistening)
      {
        display_setDispenseTime();
        islistening = true;
      } 
      else 
      {
        display_setDispenseTime_listener();
      }        
      break;
    case 5: // SET clock screen

         if (!islistening)
      {
        display_setClock();
        islistening = true;
      } 
      else 
      {
        display_setClock_listener();
      }
      break;
    case 6: // ABOUT
      if (!islistening)
      {
        display_about();
        islistening = true;
      } 
      else 
      {
        display_about_listener();
      }
      break;
    default: // HOME
        show_screen = 0;
        islistening = false;
      break;
  }
  
  

}