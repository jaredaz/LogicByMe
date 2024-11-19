#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Default OLED address, usually
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D); // Second OLED address, via onboard jumper
  
  
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Display A");
  display.display();
  
  display2.clearDisplay();
  display2.setTextSize(1); 
  display2.setTextColor(WHITE);
  display2.setCursor(0,0);
  display2.print("Display B");
  display2.display();
}

void loop() {
  }
