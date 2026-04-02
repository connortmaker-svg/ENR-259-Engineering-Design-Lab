// ---------------------------------------------
// main_code.ino
// ---------------------------------------------
// This is the main code for the project!
// Everything not fully implemented will remain commented.
// :D

// ---------------------------------------------
//  Libraries
// ---------------------------------------------

// standard C libraries
#include <stdio.h>

// Teensy Multitasking
#include <TeensyThreads.h>

// for SSD1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// for Pixy2
// #include <Pixy2SPI_SS.h>

// for servo and vacuum
// #include <Servo.h>

// ---------------------------------------------
// Symbolic Constants
// ---------------------------------------------

// OLED display height and width in pixels
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1  // reset pin #
#define SCREEN_ADDRESS 0x3C

// Color signature integer values for Pixy
#define PIXY_RED 1
#define PIXY_WHITE 2
#define PIXY_BLUE 3


// for debug display and vacuum
#define BAUD_RATE_0 9600

// for the servo wheels, pixy, and servo sort
// #define BAUD_RATE_1 115200

// ---------------------------------------------
// Global Variables
// ---------------------------------------------


// void thread_func(){
// }

// ---------------------------------------------
// Define Objects
// ---------------------------------------------

// Declaration for an SSD1306 display connected to I2C (SDA, SCL)
// For Arduino Mega: 20 (SDA) and 21 (SCL)
// For Teensy 4.1: 17 (SDA) and 16 (SCL)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// For sorting:
// Pixy2 pixy;
// Servo sorting_servo;

// Vacuum: (assuming we're using Servo library)
// Servo vacuum;
//
// Or using turbine control
// TurbineControl turbine(9)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE_0);
  delay(500);  // wait for display
               // Configure display:
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  Serial.println("Display ready");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Display ready");
  display.display();

//  threads.addThread(thread_func, 1);

}

void loop() {
  // put your main code here, to run repeatedly:

}

// ---------------------------------------------
// Display Functions
// ---------------------------------------------

// display color detected by pixy (might remove and replace with existing pixy function)
//void display_color_detected(int color) {

//  display.println("Color detected: ");
//  display.setCursor(100, 10);
//  switch (signature) {
//    case RED:
//      display.println("R");
//      break;
//    case WHITE:
//      display.println("W");
//      break;
//    case BLUE:
//      display.println("B");
//      break;
//    default:
//      display.println("N");
//      break;
//  }
//
//  display.display();
//}

void display_vacuum_speed(float pulse) {
  float conversion_factor = 0.06;
  float conversion_intercept = 20;
  float level;
  level = floor(pulse * conversion_factor - conversion_intercept);

  display.println("Motor level: ");
  display.setCursor(85, 0);
  display.println(level);
  display.display();
}

  // display current battery percentage
  void battery_level(float percentage) {
    display.println("Batt level: ");
    display.setCursor(80, 20);
    display.println(percentage);
    display.display();
  }
