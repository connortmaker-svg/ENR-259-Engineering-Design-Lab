add // ---------------------------------------------
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

// for SSD1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// for Pixy2
// #include <Pixy2.h>

// for servo
// #include <Servo.h>

// ---------------------------------------------
// Symbolic Constants
// ---------------------------------------------

// OLED display height and width in pixels
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // reset pin #
#define SCREEN_ADDRESS 0x3C

// Color signature integer values for Pixy
#define RED 1
#define WHITE 2
#define BLUE 3

// ---------------------------------------------
// Global Variables
// ---------------------------------------------

// ---------------------------------------------
// Define Objects (?)
// ---------------------------------------------

// Declaration for an SSD1306 display connected to I2C (SDA, SCL)
// For Arduino Mega: 20 (SDA) and 21 (SCL)
// For Teensy 4.1: 17 (SDA) and 16 (SCL)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// For sorting:
// Pixy2 pixy;
// Servo sorting_servo;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
