/*******************************************************************************
 **** AUTOMATIC DOG FEEDER *****************************************************
 *******************************************************************************
 **** By: Jessica Atkinson *****************************************************
 **** Date: September 8, 2023 **************************************************
 *******************************************************************************
 **** Description: *************************************************************
 **** Software for Arduino Uno R3 with stepper motor & 3.5" LCD Touch screen ***
 **** LCD: 3.5" TFT LCD Color Touch Panel Display Screen Module ( 480 x 320 )px*
 **** Motor: 28NYJ-48 ULN2003 5V Stepper Motor + ULN2003 Driver board **********
 **** Software capabitlities: **************************************************
 ************** 1. Dispense dog food at click of a button **********************
 ************** 2. Set clock ***************************************************
 ************** 3. Set multiple schedules to dispense dog food *****************
 ************** 4. Activate dog feeding on a schedule **************************
 ************** 5. Change dog food dispense time (milliseconds) ****************
 ************** 6. Delete scheduled dog food time ******************************
 ************** 7. Organize schedule array from earliest to latest *************
 ************** 7. About page **************************************************
 **** Notes : ******************************************************************
 ************ Program storage is at 98% using 31694 bytes **********************
 ************ Global variables dynamic memory is at 77% using 1579 bytes *******
 ************ Max program storage: 32256 bytes *********************************
 ************ Max Global variables dynamic memory: 2048 ************************
 *******************************************************************************
 ** Version 5.0 notes: *********************************************************
 ****** Fixed Bug: does not display the 12 hour it displays 0 instead **********
 ****** 
 *******************************************************************************
 ****** TODO: ******************************************************************
 ******** Delete all commented out code ****************************************
 ******** Rename variables to complement shared code pages *********************
 ******** Verify conditions for max schedules set in array *********************
 ******** Check what would need error handling *********************************
 ******** Test motor direction, speed & figure out why it only runs for 4 secs *
 *******************************************************************************/

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#include <Stepper.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// -Color Variables----------------------------------------------------------
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
// -Stepper Variables--------------------------------------------------------
bool isDispensing = false;
const int stepsPerRevolution = 2038; // Defines the number of steps per rotation

bool moveMotor = false; 

long dispense_time = 15500;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 10, 12,11, 13);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~DISPLAY VARIABLES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Show Screen --> 0=home, 1=Menu, 2=setDispenseTime, 3=deleteDispenseTime, 
// 4=DispenseTime, 5=SetClock, 6=about
int show_screen = 0; 

bool islistening = false; // true=listeningFunction, false=setUpFunction

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~Screen Variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//touch sensitivity for press
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~DISPLAY settings~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 952, TS_RT = 92, TS_TOP = 906, TS_BOT = 92;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// 1 menu icon top left corner to activate menu screen
Adafruit_GFX_Button menu_btn;
Adafruit_GFX_Button menu_0_btn, menu_2_btn, menu_3_btn, menu_4_btn, menu_5_btn, menu_6_btn;
// 0 home page buttons
Adafruit_GFX_Button up_btn, down_btn, switch_btn;
// 5 set clock buttons
Adafruit_GFX_Button up_clock_hour_btn, down_clock_hour_btn, up_clock_min_btn, down_clock_min_btn, up_clock_ampm_btn, down_clock_ampm_btn, set_clock_btn;
// 6 set about buttons
Adafruit_GFX_Button feed_NOW_btn;
// 4 setDispenseTime buttons
Adafruit_GFX_Button up_dispenseTime_btn, down_dispenseTime_btn;
// 2 Add schedule buttons
// Adafruit_GFX_Button up_schedule_hour_btn, down_schedule_hour_btn, up_schedule_min_btn, down_schedule_min_btn, up_schedule_ampm_btn, down_schedule_ampm_btn, add_schedule_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars

int count = 0; // for setting HH:MM:ss 0= hours, 1=mins

char deleteSchedule_title[] = "Pick a schedule to delete:";
char setFood_title[] = "Adjust dispense time (millisecs)";
char setClock_title[] = "Set clock: ";
char addSchedule_title[] = "Add schedule:";
char scheduleMax_title[] = "Max schedules already set";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~Clock Variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MS_in_HOUR 3600000
#define MS_in_MIN 60000
#define MS_in_SEC 1000
#define MS_in_PM 3600000 * 12
#define MS_in_DAY 3600000 * 24
#define MAX_HOUR 12
#define MIN_HOUR 1
#define MAX_MINUTE 59
#define MIN_MINUTE 0

struct time_struct {
    char s_master_time[12];
    char s_hour[3];
    int hour;
    char s_min[3];
    int minute;
    char s_sec[3];
    int sec;
    char s_ampm[3];
    bool b_ampm; // false = am, true = pm
    int ampm_overflowMultiple;
    long int ms_runTime_when_offset_was_set; // time offset was set
    int offset_hour;
    char s_offset_hour[3];
    int offset_min;
    char s_offset_min[3];
    int offset_sec;
    char s_offset_ampm[3];
    bool offset_ampm;
    long int offset_ms; 
    long int master_ms_clock; // ((ms_since_device_turned_on - ms_runTime_when_offset_was_set) + ms_offset)% MS_in_DAY
    char s_offset[9];
} master_time_struct;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX_SCHEDULES 5
int schedule_counter = 0; // how many schedules set

struct schedule_struct {
  char s_display_schedule_time[12];
  long int ms_schedule;
  char s_hour[3];
  int hour;
  char s_min[3];
  int minute;
  char s_ampm[3];
  bool b_ampm; // false = am, true = pm
  bool isActive;
};
struct schedule_struct schedule[MAX_SCHEDULES];
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void update_hour_and_ampm(int hr)
{
    char c_hr[3]; // string to display
    
    // AM/PM logic
    // 0 = am - false, 1 = pm - true
    int overflowHourMultiplier = hr / 12;
    bool ampm = ( overflowHourMultiplier % 2 ) == 1;
    // update global AM/PM variables
    master_time_struct.ampm_overflowMultiple = overflowHourMultiplier;
    master_time_struct.b_ampm = ampm;
    strcpy(master_time_struct.s_ampm, ampm ? "PM" : "AM");
    
    
    // HOUR logic
    int reducedHour = hr % 12;

    if (reducedHour == 0){
        strcpy(c_hr, "12");
    }
    else if (reducedHour < 10) 
    {
        sprintf(c_hr, "0%d", reducedHour);
    }
    else 
    {
        sprintf(c_hr, "%d", reducedHour);
    }
    
    // update global variables
    strcpy(master_time_struct.s_hour, c_hr);
    master_time_struct.hour = hr;
}

void update_min(int minute)
{
    char c_min[3];
    if (minute < 10) 
    { 
        sprintf(c_min, "0%d", minute);
    }
    else 
    {
        sprintf(c_min, "%d", minute);
    }
    
    // update global variables
    strcpy(master_time_struct.s_min, c_min);
    master_time_struct.minute = minute;
}

void update_sec(int sec)
{
    char c_sec[4];
    if (sec < 10) 
    { 
        sprintf(c_sec, "0%d", sec);
    }
    else 
    {
        sprintf(c_sec, "%d", sec);
    }
    
    // update global variables
    strcpy(master_time_struct.s_sec, c_sec);
    master_time_struct.sec = sec;
    printf("\nSec: %s", c_sec);
}

void update_master_time_string()
{
    sprintf(master_time_struct.s_master_time, "%s:%s.%s %s", master_time_struct.s_hour, master_time_struct.s_min, master_time_struct.s_sec, master_time_struct.s_ampm);
}

void update_struct() // ms = runtime ms
{
    long int ms = millis();
    long int diff_ms = ms - master_time_struct.ms_runTime_when_offset_was_set;
    long int total_ms = diff_ms + master_time_struct.offset_ms; // add the offset_ms
    master_time_struct.master_ms_clock = total_ms;
    
    // calculate hours, mins, secs values
    int hour = total_ms / MS_in_HOUR;
    int minute = ( total_ms % MS_in_HOUR ) / MS_in_MIN;
    int sec = (( total_ms % MS_in_HOUR ) % MS_in_MIN ) / MS_in_SEC;
    
    // update struct values
    update_hour_and_ampm(hour);
    update_min(minute);
    update_sec(sec);
    
    update_master_time_string();
}

// CLOCK - OFFSET FUNCTIONS /////////////////////////////////

bool isInitialized_offset = false;
void get_offset_display_str()
{
  sprintf(master_time_struct.s_offset, "%s:%s %s", master_time_struct.s_offset_hour, master_time_struct.s_offset_min, master_time_struct.s_offset_ampm);
}

void initialize_offset()
{
  if (!isInitialized_offset)
  {
    master_time_struct.offset_hour = 1;
    strcpy(master_time_struct.s_offset_hour, "01");
    master_time_struct.offset_min = 0;
    strcpy(master_time_struct.s_offset_min, "00");
    master_time_struct.offset_sec = 0;
    master_time_struct.offset_ampm = false;
    strcpy(master_time_struct.s_offset_ampm, "AM");
    master_time_struct.offset_ms = 0;

    get_offset_display_str();

    isInitialized_offset = true;
  }
}

void update_offset() // SET BUTTON PRESSED on SET CLOCK SCREEN
{
    master_time_struct.ms_runTime_when_offset_was_set = millis();
    
    // adds offset hours, mins, secs -> converts to ms
    // update struct ms
    long int total_ms = 0; //master_time_struct.offset_ms;
    
    total_ms += master_time_struct.offset_hour * MS_in_HOUR;
    total_ms += master_time_struct.offset_min * MS_in_MIN;
    total_ms += master_time_struct.offset_sec * MS_in_SEC;
    total_ms += master_time_struct.offset_ampm ? MS_in_PM : 0;
    
    // update global variables
    master_time_struct.offset_ms = total_ms;
    
    update_struct();
}

void update_offset_hour_plus()
{
    char c_hour_offset[3]; 
    master_time_struct.offset_hour++;
    
    if (master_time_struct.offset_hour > 12)
    {
        master_time_struct.offset_hour = 1;
    }

    if (master_time_struct.offset_hour < 10) 
    { 
        sprintf(c_hour_offset, "0%d", master_time_struct.offset_hour);
    }
    else 
    {
        sprintf(c_hour_offset, "%d", master_time_struct.offset_hour);
    }
    
    // update global variables
    strcpy(master_time_struct.s_offset_hour, c_hour_offset);
}
void update_offset_hour_minus()
{
    char c_hour_offset[3]; 
    master_time_struct.offset_hour--;
    
    if (master_time_struct.offset_hour < 1)
    {
        master_time_struct.offset_hour = 12;
    }
    if (master_time_struct.offset_hour < 10) 
    { 
        sprintf(c_hour_offset, "0%d", master_time_struct.offset_hour);
    }
    else 
    {
        sprintf(c_hour_offset, "%d", master_time_struct.offset_hour);
    }
    
    // update global variables
    strcpy(master_time_struct.s_offset_hour, c_hour_offset);
}
void update_offset_min_plus()
{
    char c_min_offset[3]; 
    master_time_struct.offset_min++;
    
    if (master_time_struct.offset_min > 59)
    {
        master_time_struct.offset_min = 0;
    }
    if (master_time_struct.offset_min < 10) 
    { 
        sprintf(c_min_offset, "0%d", master_time_struct.offset_min);
    }
    else 
    {
        sprintf(c_min_offset, "%d", master_time_struct.offset_min);
    }
    
    // update global variables
    strcpy(master_time_struct.s_offset_min, c_min_offset);
}
void update_offset_min_minus()
{
    char c_min_offset[3];
    master_time_struct.offset_min--;
    
    if (master_time_struct.offset_min < 0)
    {
        master_time_struct.offset_min = 59;
    }
    if (master_time_struct.offset_min < 10) 
    { 
        sprintf(c_min_offset, "0%d", master_time_struct.offset_min);
    }
    else 
    {
        sprintf(c_min_offset, "%d", master_time_struct.offset_min);
    }
    
    // update global variables
    strcpy(master_time_struct.s_offset_min, c_min_offset);
}
void update_offset_ampm()
{
    master_time_struct.offset_ampm = !master_time_struct.offset_ampm;
    strcpy(master_time_struct.s_offset_ampm, master_time_struct.offset_ampm ? "PM" : "AM");
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct schedule_struct get_schedule_display_str(struct schedule_struct st)
{
  sprintf(st.s_display_schedule_time, "%s:%s %s", st.s_hour, st.s_min, st.s_ampm);
  return st;
}
struct schedule_struct initialize_schedule(struct schedule_struct st)
{
    st.isActive = false;
    st.hour = 1;
    strcpy(st.s_hour, "01");
    st.minute = 0;
    strcpy(st.s_min, "00");
    st.b_ampm = false;
    strcpy(st.s_ampm, "AM");
    st.ms_schedule = 0;

    st = get_schedule_display_str(st);

    return st;
}
//struct schedule_struct update_schedule_hour(int hr, struct schedule_struct st)
//{
//    // account for over max or under max
//    if (hr > MAX_HOUR)
//        hr = MIN_HOUR;
//    else if (hr < MIN_HOUR)
//        hr = MAX_HOUR;
//
//    st.hour = hr; // update hour in struct
//
//    // display string formatting
//    if (hr < 10) 
//        sprintf(st.s_hour, "0%d", hr);
//    else 
//        sprintf(st.s_hour, "%d", hr);
//
//    st = get_schedule_display_str(st);
//
//    return st;
//}
//struct schedule_struct update_schedule_min(int minute, struct schedule_struct st)
//{
//    // account for over max or under max
//    if (minute > MAX_MINUTE)
//        minute = MIN_MINUTE;
//    else if (minute < MIN_MINUTE)
//        minute = MAX_MINUTE;
//
//    st.minute = minute; // update minute in struct
//
//    // display string formatting
//    if (minute < 10) 
//        sprintf(st.s_min, "0%d", minute);
//    else 
//        sprintf(st.s_min, "%d", minute);
//
//    st = get_schedule_display_str(st);
//
//    return st;
//}
//struct schedule_struct update_schedule_ampm(bool ampm_val, struct schedule_struct st)
//{
//    st.b_ampm = ampm_val;
//    strcpy(st.s_ampm, ampm_val ? "PM" : "AM");
//    st = get_schedule_display_str(st);
//
//    return st;
//}
//void addSchedule()
//{
//    // calculate ms 
//    int ms = 0;
//    ms += schedule[schedule_counter].hour * MS_in_HOUR;
//    ms += schedule[schedule_counter].minute * MS_in_MIN;
//    ms += schedule[schedule_counter].b_ampm ? MS_in_PM : 0;
//
//    schedule[schedule_counter].ms_schedule = ms;
//    schedule[schedule_counter].isActive = true;
//
//    schedule_counter++;
//}

// reorganize the schedules from earliest to latest
void organizeSchedules()
{
    bool wasUpdated = true;
    
    while (wasUpdated)
    {
        wasUpdated = false;
        for (int i = 0; i < schedule_counter - 1; i++)
        {
            if (schedule[i].isActive && schedule[i+1].isActive )
            {
                if (schedule[i].ms_schedule > schedule[i+1].ms_schedule)
                {
                    struct schedule_struct tmp = schedule[i];
                    schedule[i] = schedule[i+1];
                    schedule[i+1] = tmp;
                    wasUpdated = true;
                    break;
                }
            }
        }
    }
}

void addSchedule()
{
    if (schedule_counter < MAX_SCHEDULES)
    {
        schedule[schedule_counter].hour = master_time_struct.offset_hour;
        schedule[schedule_counter].minute = master_time_struct.offset_min;
    
        strcpy(schedule[schedule_counter].s_hour, master_time_struct.s_offset_hour);
        strcpy(schedule[schedule_counter].s_min, master_time_struct.s_offset_min);
    
        schedule[schedule_counter].b_ampm = master_time_struct.offset_ampm;
        strcpy(schedule[schedule_counter].s_ampm, master_time_struct.s_offset_ampm);
    
        sprintf(schedule[schedule_counter].s_display_schedule_time, "%s:%s %s", schedule[schedule_counter].s_hour, schedule[schedule_counter].s_min, schedule[schedule_counter].s_ampm);
        // calculate ms 
        long int ms = 0;
        
        ms = ms + schedule[schedule_counter].hour * MS_in_HOUR;
        ms = ms + schedule[schedule_counter].minute * MS_in_MIN;
        ms += schedule[schedule_counter].b_ampm ? MS_in_PM : 0;
    
        schedule[schedule_counter].ms_schedule = ms;
        schedule[schedule_counter].isActive = true;
    
        schedule_counter++;
    
        organizeSchedules();
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ DELETE from SCHEDULE ARRAY ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// deletes a schedule from the schedule array
void deleteSchedule(int idx)
{
    if (schedule[idx].isActive)
    {
      // clear the desired schedule to be deleted
      schedule[idx] = initialize_schedule(schedule[idx]);
      
      // reposition the array in order
      
      // check if idx+1 is active & move it down an index
      int curr_idx = idx;
      while (curr_idx < MAX_SCHEDULES - 1)
      {
          if (schedule[curr_idx + 1].isActive)
          {
              schedule[curr_idx] = schedule[curr_idx+1];
              schedule[curr_idx + 1] = initialize_schedule(schedule[curr_idx]);
          }
          curr_idx++;
      }
      schedule_counter--; 

      // refresh the screen
      islistening = false;
    }
}


void display_title()
{
  tft.setTextSize(2);
  tft.setCursor(80, 40);
  tft.print(show_screen == 4 ? "Adjust dispense time (milliseconds):" : (show_screen == 3 ? "Pick a schedule to delete: " : ""));
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ TOUCH SCREEN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
    return pressed;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~SHOW MENU BUTTON~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void dispense_food_now() // aka move_motor
{
  if (isDispensing){
    islistening = true;

//    tft.fillScreen(BLACK);
//    tft.setTextColor(GREEN, BLACK);
//    tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16
//    tft.setCursor(170, 40);
//    tft.print("NOW");
//    tft.setCursor(60, 90);
//    tft.print("Dispensing!");
//    tft.setCursor(180, 130);
//    tft.print(":D");
  
    // Rotate CW 15 RPM
    myStepper.setSpeed(15);
    myStepper.step(-stepsPerRevolution);

    isDispensing = false;
    islistening = false; // refresh screen ??
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

void display_1_menu_setup()
{
  islistening = true;
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);

  if (show_screen == 3)
  {
    tft.setTextSize(2);
    tft.setCursor(80, 40);
    tft.print(deleteSchedule_title);
  }
  else{
    menu_0_btn.initButton(&tft, 210, 33, 350, 50, BLACK, NAVY, WHITE, "HOME", 2);
    menu_0_btn.drawButton(false);
  }

  // menu options
  menu_2_btn.initButton(&tft, 210, 96, 350, 50, BLACK, NAVY, WHITE, show_screen == 1 ? "SCHEDULE" : (schedule[0].isActive ? schedule[0].s_display_schedule_time : "Not Set"), 2);
  menu_2_btn.drawButton(show_screen == 1 ? false : !schedule[0].isActive);
  menu_3_btn.initButton(&tft, 210, 149, 350, 50, BLACK, NAVY, WHITE, show_screen == 1 ? "DELETE" : (schedule[1].isActive ? schedule[1].s_display_schedule_time : "Not Set"), 2);
  menu_3_btn.drawButton(show_screen == 1 ? false : !schedule[1].isActive);
  menu_4_btn.initButton(&tft, 210, 200, 350, 50, BLACK, NAVY, WHITE, show_screen == 1 ? "FOOD SIZE" : (schedule[2].isActive ? schedule[2].s_display_schedule_time : "Not Set"), 2);
  menu_4_btn.drawButton(show_screen == 1 ? false : !schedule[2].isActive);
  menu_5_btn.initButton(&tft, 210, 250, 350, 50, BLACK, NAVY, WHITE, show_screen == 1 ? "SET CLOCK" : (schedule[3].isActive ? schedule[3].s_display_schedule_time : "Not Set"), 2);
  menu_5_btn.drawButton(show_screen == 1 ? false : !schedule[3].isActive);
  menu_6_btn.initButton(&tft, 210, 300, 350, 50, BLACK, NAVY, WHITE, show_screen == 1 ? "ABOUT" : (schedule[4].isActive ? schedule[4].s_display_schedule_time : "Not Set"), 2);
  menu_6_btn.drawButton(show_screen == 1 ? false : !schedule[4].isActive);

  if (show_screen == 3)
    show_munu_btn();
}

void display_1_menu_listener()
{
  bool down = Touch_getXY(); 

  if (show_screen == 1)
    menu_0_btn.press(down && menu_0_btn.contains(pixel_x, pixel_y));
  if (show_screen == 1 || (show_screen == 3 & schedule[0].isActive))
    menu_2_btn.press(down && menu_2_btn.contains(pixel_x, pixel_y));
  if (show_screen == 1 || (show_screen == 3 & schedule[1].isActive))
    menu_3_btn.press(down && menu_3_btn.contains(pixel_x, pixel_y));
  if (show_screen == 1 || (show_screen == 3 & schedule[2].isActive))
    menu_4_btn.press(down && menu_4_btn.contains(pixel_x, pixel_y));
  if (show_screen == 1 || (show_screen == 3 & schedule[3].isActive))
    menu_5_btn.press(down && menu_5_btn.contains(pixel_x, pixel_y));
  if (show_screen == 1 || (show_screen == 3 & schedule[4].isActive))
    menu_6_btn.press(down && menu_6_btn.contains(pixel_x, pixel_y));

  // btn released
  if (show_screen == 1)
  {
    if (menu_0_btn.justReleased()){
        menu_0_btn.drawButton(false);
    }
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
  if (show_screen == 1)
  {
    if (menu_0_btn.justPressed()) {
        menu_0_btn.drawButton(true);
        show_screen = 0;
        islistening = false;
    }
  }
  if (menu_2_btn.justPressed()) {
      menu_2_btn.drawButton(true);
      if (show_screen == 1){
          show_screen = 2;
          islistening = false;
      }
      else // del
          deleteSchedule(0);
  }
  if (menu_3_btn.justPressed()) {
      menu_3_btn.drawButton(true);
      if (show_screen == 1){
          show_screen = 3;
          islistening = false;
      }
      else // del
          deleteSchedule(1);
  }
  if (menu_4_btn.justPressed()) {
      menu_4_btn.drawButton(true);
      if (show_screen == 1){
          show_screen = 4;
          islistening = false;
      }
      else // del
          deleteSchedule(2);
  }
  if (menu_5_btn.justPressed()) {
      menu_5_btn.drawButton(true);
      if (show_screen == 1){
          show_screen = 5;
          islistening = false;
      }
      else // del
          deleteSchedule(3);
  }
  if (menu_6_btn.justPressed()) {
      menu_6_btn.drawButton(true);
      if (show_screen == 1){
          show_screen = 6;
          islistening = false;
      }
      else // del
          deleteSchedule(4);
  }

  if (show_screen == 3)
    show_munu_btn_listener(down);
  
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ ( 0 ) DISPLAY HOME PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void display_0_home_setup()
{
  islistening = true;

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  
  feed_NOW_btn.initButton(&tft, 100, 255, 100, 100, BLACK, NAVY, WHITE, "FEED", 2);
  feed_NOW_btn.drawButton(false);

  show_munu_btn();

  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);     // System font is 8 pixels.  ht = 8*2=16
  tft.setCursor(150, 40);
  tft.print(master_time_struct.s_master_time);

  tft.setTextSize(2);     // System font is 8 pixels.  ht = 8*2=16
  tft.setCursor(200, 130);
  tft.print("Schedules set:");
  tft.print(schedule_counter);
}

void display_0_home_listener()
{
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);     // System font is 8 pixels.  ht = 8*2=16
  tft.setCursor(150, 40);
  tft.print(master_time_struct.s_master_time);

  if (schedule_counter > 0) {
    for (int i = 0; i < schedule_counter; i++)
    {
      tft.setTextSize(2);     // System font is 8 pixels.  ht = 8*2=16
      tft.setCursor(250, 160 + (20 * i));
      tft.print(schedule[i].s_display_schedule_time);
    }
  }
  
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


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ ( 2 ) ADD SCHEDULE PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// **************** Commented out because ran of program space 103 % used if this page had its
// **************** own designated buttons and screen

//int hours = 0;
//int mins = 0;
//bool amPm = false; // false=am, true=pm
//
//void display_2_addSchedule_setup()
//{
//  // set clock buttons
////Adafruit_GFX_Button up_clock_hour_btn, down_clock_hour_btn, 
////          up_clock_min_btn, down_clock_min_btn, up_clock_ampm_btn,
////          down_clock_ampm_btn, set_clock_btn;
//  islistening = true;
//  tft.fillScreen(BLACK);
//  tft.setTextColor(WHITE, BLACK);
//
//  // check if max amount of schedules
// if (schedule_counter == MAX_SCHEDULES){
//    tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
//    tft.setCursor(90, 111);
//    printf("Max schedules reached, please delete a schedule to add a new one");
//  }
//  else
//  {
//      tft.setTextColor(WHITE, DARKGREY);  
//      tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
//      tft.setCursor(90, 111);
//      schedule[schedule_counter] = initialize_schedule(schedule[schedule_counter]); // initialize it 01:00 AM
//      tft.print(schedule[schedule_counter].s_display_schedule_time);
//      
//      // initBtn(&tft, x-init, y-init, width, height, outline, fill, textColor, *label, text size)
//      // hour modifier
//      up_schedule_hour_btn.initButton(&tft,  126, 76, 90, 50, WHITE, CYAN, BLACK, "UP", 2);
//      up_schedule_hour_btn.drawButton(false);
//      down_schedule_hour_btn.initButton(&tft, 126, 195, 90, 50, WHITE, CYAN, BLACK, "DOWN", 2);
//      down_schedule_hour_btn.drawButton(false);
//      // minute modified
//      up_schedule_min_btn.initButton(&tft, 231, 76, 90, 50, WHITE, CYAN, BLACK, "UP", 2);
//      up_schedule_min_btn.drawButton(false);
//      down_schedule_min_btn.initButton(&tft, 231, 195, 90, 50, WHITE, CYAN, BLACK, "DOWN", 2);
//      down_schedule_min_btn.drawButton(false);
//      // am/pm modifier
//      up_schedule_ampm_btn.initButton(&tft, 341, 76, 90, 50, WHITE, CYAN, BLACK, "UP", 2);
//      up_schedule_ampm_btn.drawButton(false);
//      down_schedule_ampm_btn.initButton(&tft, 341, 195, 90, 50, WHITE, CYAN, BLACK, "DOWN", 2);
//      down_schedule_ampm_btn.drawButton(false);
//    
//      add_schedule_btn.initButton(&tft, 395, 280, 110, 60, WHITE, GREEN, BLACK, "ADD", 2);
//      add_schedule_btn.drawButton(false);
//  }
//  show_munu_btn();
//}
//
//
//void display_2_addSchedule_listener()
//{
//  bool down = Touch_getXY();
//    // check if max amount of schedules
// if (schedule_counter == MAX_SCHEDULES){
//    tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
//    tft.setCursor(90, 111);
//    printf("Max schedules reached, please delete a schedule to add a new one");
//  }
//  else
//  {
//      up_schedule_hour_btn.press(down && up_schedule_hour_btn.contains(pixel_x, pixel_y));
//      down_schedule_hour_btn.press(down && down_schedule_hour_btn.contains(pixel_x, pixel_y));
//      up_schedule_min_btn.press(down && up_schedule_min_btn.contains(pixel_x, pixel_y));
//      down_schedule_min_btn.press(down && down_schedule_min_btn.contains(pixel_x, pixel_y));
//      up_schedule_ampm_btn.press(down &&  up_schedule_ampm_btn.contains(pixel_x, pixel_y));
//      down_schedule_ampm_btn.press(down &&  down_schedule_ampm_btn.contains(pixel_x, pixel_y));
//      add_schedule_btn.press(down && add_schedule_btn.contains(pixel_x, pixel_y));
//    
//      // check if released
//      if (up_schedule_hour_btn.justReleased()){
//          up_schedule_hour_btn.drawButton(false);
//      }
//      if (down_schedule_hour_btn.justReleased()){
//          down_schedule_hour_btn.drawButton(false);
//      }
//      if (up_schedule_min_btn.justReleased()){
//          up_schedule_min_btn.drawButton(false);
//      }
//      if (down_schedule_min_btn.justReleased()){
//          down_schedule_min_btn.drawButton(false);
//      }
//      if (up_schedule_ampm_btn.justReleased()){
//          up_schedule_ampm_btn.drawButton(false);
//      }
//      if (down_schedule_ampm_btn.justReleased()){
//          down_schedule_ampm_btn.drawButton(false);
//      }
//    
//      // check if pressed
//      if (up_schedule_hour_btn.justPressed()) {
//          up_schedule_hour_btn.drawButton(true);
//          update_schedule_hour(schedule[schedule_counter].hour++, schedule[schedule_counter]);
//      }
//      if (down_schedule_hour_btn.justPressed()) {
//          down_schedule_hour_btn.drawButton(true);
//          update_schedule_hour(schedule[schedule_counter].hour--, schedule[schedule_counter]);
//      }
//      if (up_schedule_min_btn.justPressed()) {
//          up_schedule_min_btn.drawButton(true);
//          update_schedule_min(schedule[schedule_counter].minute++, schedule[schedule_counter]);
//      }
//      if (down_schedule_min_btn.justPressed()) {
//          down_schedule_min_btn.drawButton(true);
//          update_schedule_min(schedule[schedule_counter].minute--, schedule[schedule_counter]);
//      }
//        if (up_schedule_ampm_btn.justPressed()) {
//          up_schedule_ampm_btn.drawButton(true);
//          update_schedule_ampm(!schedule[schedule_counter].b_ampm, schedule[schedule_counter]);
//      }
//      if (down_schedule_ampm_btn.justPressed()) {
//          down_schedule_ampm_btn.drawButton(true);
//          update_schedule_ampm(!schedule[schedule_counter].b_ampm, schedule[schedule_counter]);
//      }
//      if (add_schedule_btn.justPressed()) {
//          add_schedule_btn.drawButton(true);
//          addSchedule();
//          show_screen = 0; // go to home screen
//          islistening = false;
//      }
//
//      
//      tft.setTextColor(WHITE, DARKGREY);  
//      tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
//      tft.setCursor(90, 111);
//      tft.print(schedule[schedule_counter].s_display_schedule_time);
//  }
//  show_munu_btn_listener(down);  
//}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ ( 5 ) SET CLOCK PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int tmp_hours = 0;
int tmp_mins = 0;
bool tmp_amPm = false; // false=am, true=pm

void display_5_setClock_setup()
{
  // set clock buttons
//Adafruit_GFX_Button up_clock_hour_btn, down_clock_hour_btn, 
//          up_clock_min_btn, down_clock_min_btn, up_clock_ampm_btn,
//          down_clock_ampm_btn, set_clock_btn;
  islistening = true;
  initialize_offset();
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.setCursor(80, 20);
  tft.print(show_screen == 5 ? setClock_title : ( schedule_counter == MAX_SCHEDULES ? scheduleMax_title : addSchedule_title));

  tft.setTextColor(WHITE, DARKGREY);  
  tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
  tft.setCursor(90, 111);
  tft.print(master_time_struct.s_offset);
  
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
  set_clock_btn.drawButton(schedule_counter == MAX_SCHEDULES);

  show_munu_btn();
}


void display_5_setClock_listener()
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
      update_offset_hour_plus();
  }
  if (down_clock_hour_btn.justPressed()) {
      down_clock_hour_btn.drawButton(true);
      update_offset_hour_minus();
  }
  if (up_clock_min_btn.justPressed()) {
      up_clock_min_btn.drawButton(true);
      update_offset_min_plus();
  }
  if (down_clock_min_btn.justPressed()) {
      down_clock_min_btn.drawButton(true);
      update_offset_min_minus();
  }
  if (up_clock_ampm_btn.justPressed()) {
      up_clock_ampm_btn.drawButton(true);
      update_offset_ampm();
  }
  if (down_clock_ampm_btn.justPressed()) {
      down_clock_ampm_btn.drawButton(true);
      update_offset_ampm();
  }
  if (set_clock_btn.justPressed()) {
      set_clock_btn.drawButton(true);
      show_screen == 2 ? addSchedule() : update_offset();
      show_screen = 0; // go to home screen
      islistening = false;
  }
  show_munu_btn_listener(down);

  tft.setTextColor(WHITE, DARKGREY);  
  tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
  tft.setCursor(90, 111);
  get_offset_display_str();
  tft.print(master_time_struct.s_offset);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ ( 3 ) DELETE CLOCK SCHEDULE PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void display_3_deleteSchedule_setup()
//{
//  islistening = true;
//
//  tft.fillScreen(BLACK);
//  tft.setTextColor(WHITE, BLACK);
//  tft.setTextSize(2);     // System font is 8 pixels.  ht = 8*2=16
//  tft.setCursor(50, 40);
//  tft.print("Pick a schedule to delete: ");
////  display_title()
//
//  // use same buttons from menu 
//  menu_2_btn.initButton(&tft, 210, 96, 350, 50, BLACK, NAVY, WHITE, schedule[0].isActive ? schedule[0].s_display_schedule_time : "Not Set", 2);
//  menu_2_btn.drawButton(false);
//  menu_3_btn.initButton(&tft, 210, 149, 350, 50, BLACK, NAVY, WHITE, schedule[1].isActive ? schedule[1].s_display_schedule_time : "Not Set", 2);
//  menu_3_btn.drawButton(false);
//  menu_4_btn.initButton(&tft, 210, 200, 350, 50, BLACK, NAVY, WHITE, schedule[2].isActive ? schedule[2].s_display_schedule_time : "Not Set", 2);
//  menu_4_btn.drawButton(false);
//  menu_5_btn.initButton(&tft, 210, 250, 350, 50, BLACK, NAVY, WHITE, schedule[3].isActive ? schedule[3].s_display_schedule_time : "Not Set", 2);
//  menu_5_btn.drawButton(false);
//  menu_6_btn.initButton(&tft, 210, 300, 350, 50, BLACK, NAVY, WHITE, schedule[4].isActive ? schedule[4].s_display_schedule_time : "Not Set", 2);
//  menu_6_btn.drawButton(false);
//
//  show_munu_btn();
//}
//
//void display_3_deleteSchedule_listener()
//{
//  bool down = Touch_getXY();
//
//  menu_2_btn.press(down && menu_2_btn.contains(pixel_x, pixel_y));
//  menu_3_btn.press(down && menu_3_btn.contains(pixel_x, pixel_y));
//  menu_4_btn.press(down && menu_4_btn.contains(pixel_x, pixel_y));
//  menu_5_btn.press(down && menu_5_btn.contains(pixel_x, pixel_y));
//  menu_6_btn.press(down && menu_6_btn.contains(pixel_x, pixel_y));
//
//  // btn released
//  if (menu_2_btn.justReleased()){
//      menu_2_btn.drawButton(false);
//  }
//  if (menu_3_btn.justReleased()){
//      menu_3_btn.drawButton(false);
//  }
//  if (menu_4_btn.justReleased()){
//      menu_4_btn.drawButton(false);
//  }
//  if (menu_5_btn.justReleased()){
//      menu_5_btn.drawButton(false);
//  }
//  if (menu_6_btn.justReleased()){
//      menu_6_btn.drawButton(false);
//  }
//  
//  // btn pressed
//  if (menu_2_btn.justPressed()) {
//      menu_2_btn.drawButton(true);
//      deleteSchedule(0);
//      islistening = false;
//  }
//  if (menu_3_btn.justPressed()) {
//      menu_3_btn.drawButton(true);
//      deleteSchedule(1);
//      islistening = false;
//  }
//  if (menu_4_btn.justPressed()) {
//      menu_4_btn.drawButton(true);
//      deleteSchedule(2);
//      islistening = false;
//  }
//  if (menu_5_btn.justPressed()) {
//      menu_5_btn.drawButton(true);
//      deleteSchedule(3);
//      islistening = false;
//  }
//  if (menu_6_btn.justPressed()) {
//      menu_6_btn.drawButton(true);
//      deleteSchedule(4);
//      islistening = false;
//  }
//  show_munu_btn_listener(down);
//}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ ( 4 ) setDispenseTime PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void display_4_setDispenseTime_setup()
{
  islistening = true;

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.setCursor(80, 40);
  tft.print(setFood_title);
  
  tft.setTextColor(WHITE, DARKGREY);  
  tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
  tft.setCursor(160, 141);
  tft.print(dispense_time);
  up_dispenseTime_btn.initButton(&tft, 235, 95, 150, 60, BLACK, NAVY, WHITE, "UP", 2);
  up_dispenseTime_btn.drawButton(false);
  down_dispenseTime_btn.initButton(&tft, 235, 230, 150, 60, BLACK, NAVY, WHITE, "DOWN", 2);
  down_dispenseTime_btn.drawButton(false);

  show_munu_btn();
}

void display_4_setDispenseTime_listener()
{
  // update the dispense time display value
  tft.setTextColor(WHITE, DARKGREY);  
  tft.setTextSize(6);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
  tft.setCursor(160, 141);
  tft.print(dispense_time);
  
  bool down = Touch_getXY(); 
  up_dispenseTime_btn.press(down && up_dispenseTime_btn.contains(pixel_x, pixel_y));
  down_dispenseTime_btn.press(down && down_dispenseTime_btn.contains(pixel_x, pixel_y));

  // disable button if at min or max
//  if (dispense_time < 1000){
//    down_dispenseTime_btn.drawButton(true);  
//  }
//  if (dispense_time > 950000){
//    up_dispenseTime_btn.drawButton(true);  
//  }

  // check button status
  if (up_dispenseTime_btn.justReleased()){
        up_dispenseTime_btn.drawButton(false);
  }
  if (down_dispenseTime_btn.justReleased()){
        down_dispenseTime_btn.drawButton(false);
  }
  
  if (up_dispenseTime_btn.justPressed()) {
        up_dispenseTime_btn.drawButton(true);
        dispense_time += 500;
        dispense_time = dispense_time > 999500 ? 999500 : dispense_time; // max seconds = 999.5s
//        if(dispense_time == 10000 || dispense_time == 100000)
//          islistening = false;
  }
  if (down_dispenseTime_btn.justPressed()) {
      down_dispenseTime_btn.drawButton(true);
      dispense_time -= 500;
      dispense_time = dispense_time < 1000 ? 1000 : dispense_time; // min seconds = 1s
//      islistening = false; // TODO : maybe??
      if(dispense_time == 9500 || dispense_time == 99500) // refresh screen so no extra zero
          islistening = false;
  }
  
  show_munu_btn_listener(down);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ ( 6 ) ABOUT PAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void display_6_about_setup()
{
  islistening = true;

  tft.fillScreen(BLACK);
  tft.setTextColor(MAGENTA, BLACK);
 
  tft.setTextSize(2);     // System font is 8 pixels.  ht = 8*2=16 wdth = 5/6 * size
  tft.setCursor(90, 80);
  tft.print("Automatic Dog Feeder");
  tft.setCursor(200, 110);
  tft.print("By");
  tft.setCursor(105, 140);
  tft.print("Jessica Atkinson");
  tft.setCursor(190, 170);
  tft.print("2023");
  
  feed_NOW_btn.initButton(&tft, 100, 255, 100, 100, BLACK, NAVY, WHITE, "FEED", 2);
  feed_NOW_btn.drawButton(false);

  show_munu_btn();
}

void display_6_about_listener()
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
  update_struct();

  if (schedule_counter > 0) {
    for (int i = 0; i < schedule_counter; i++)
    {
      if ( schedule[i].isActive && schedule[i].ms_schedule < master_time_struct.master_ms_clock + (schedule_counter * 32) && schedule[i].ms_schedule > master_time_struct.master_ms_clock - (schedule_counter * 32))
      {
        isDispensing = true;
      }
    }
  }

  if (isDispensing)
  {
    dispense_food_now();  
  }

  switch (show_screen) {
    case 0: // home/idle screen
      if (!islistening)
      {
        display_0_home_setup();
        islistening = true;
      } 
      else 
      {
        display_0_home_listener();
      }
      break;
    case 1: // menu screen
      if (!islistening)
      {
        display_1_menu_setup();
        islistening = true;
      }
      else
      {
        display_1_menu_listener();
      }
      break;
    case 2: // SET/ADD schedule
      
      if (!islistening)
      {
//        display_2_addSchedule_setup();
        display_5_setClock_setup();
        islistening = true;
      } 
      else 
      {
//        display_2_addSchedule_listener();
        display_5_setClock_listener();
      }
      break;
    case 3: // DELETE schedule
      if (!islistening)
      {
//        display_3_deleteSchedule_setup();
        display_1_menu_setup();
        islistening = true;
      } 
      else 
      {
//        display_3_deleteSchedule_listener();
          display_1_menu_listener();
      }
      break;
    case 4: // SET DOG FOOD DISPENSING TIME
      if (!islistening)
      {
        display_4_setDispenseTime_setup();
        islistening = true;
      } 
      else 
      {
        display_4_setDispenseTime_listener();
      }        
      break;
    case 5: // SET clock screen
      if (!islistening)
      {
        display_5_setClock_setup();
        islistening = true;
      } 
      else 
      {
        display_5_setClock_listener();
      }
      break;
    case 6: // ABOUT
      if (!islistening)
      {
        display_6_about_setup();
        islistening = true;
      } 
      else 
      {
        display_6_about_listener();
      }
      break;
    default: // HOME
        show_screen = 0;
        islistening = false;
      break;
  }
}
