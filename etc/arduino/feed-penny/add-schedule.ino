#include <Adafruit_GFX.h>

Adafruit_GFX_Button menu_btn;

void addSchedule()
{
    schedule[schedule_counter].hour = master_time_struct.offset_hour;
    schedule[schedule_counter].minute = master_time_struct.offset_min;

    schedule[schedule_counter].s_hour = master_time_struct.s_offset_hour;
    schedule[schedule_counter].s_min = master_time_struct.s_offset_min;

    schedule[schedule_counter].b_ampm = master_time_struct.offset_ampm;
    schedule[schedule_counter].s_ampm = master_time_struct.s_offset_ampm;

    sprintf(schedule[schedule_counter].s_display_schedule_time, "%s:%s %s", schedule[schedule_counter].s_hour, schedule[schedule_counter].s_min, schedule[schedule_counter].s_ampm);
    // calculate ms 
    int ms = 0;
    ms += schedule[schedule_counter].hour * MS_in_HOUR;
    ms += schedule[schedule_counter].minute * MS_in_MIN;
    ms += schedule[schedule_counter].b_ampm ? MS_in_PM : 0;

    schedule[schedule_counter].set_ms_schedule = ms;
    schedule[schedule_counter].isActive = true;

    schedule_counter++;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
