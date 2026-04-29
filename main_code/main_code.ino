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
// #include <TeensyThreads.h>

#include <Teensy_PWM.h>

// for SSD1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//for gyro
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

//for wheels
#include "DGMotor.h"
#include "HardwareSerial.h"

// for Teensy interrupts
#include <avr/io.h>
#include <avr/interrupt.h>

// for Pixy2
// #include <Pixy2SPI_SS.h>

// for servo and vacuum
#include <Servo.h>

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

#define PIXY_SAMPLE_PERIOD 20000 //measured in microseconds
#define PIXY_ISR_PRIORITY 128

//Pins for sorting and chute servos
#define SORTING_SERVO_PIN 9
#define RED_SERVO_PIN 4
#define WHITE_SERVO_PIN 5
#define BLUE_SERVO_PIN 6


// for debug display and vacuum
#define BAUD_RATE_0 9600

//PWM pin and values for intake motor
#define PWM_PIN  4
#define PWM_FREQ  47400.0f
#define PWM_DC  50.0f //Duty cycle in in percentage (0-100%)

// for the servo wheels, pixy, and servo sort
#define BAUD_RATE_1 115200

// ---------------------------------------------
// Global Variables
// ---------------------------------------------

//volatile uint16_t signature;

// void thread_func(){
// }

// ---------------------------------------------
// Define Objects
// ---------------------------------------------

// Declaration for an SSD1306 display connected to I2C (SDA, SCL)
// For Arduino Mega: 20 (SDA) and 21 (SCL)
// For Teensy 4.1: 17 (SDA) and 16 (SCL)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//PWM object for intake motor
Teensy_PWM* PWM_Instance;
//Timer for pixy interrupts
//IntervalTimer pixyTimer;

// For sorting:
// Pixy2 pixy;
Servo sortingServo;
Servo redServo;
Servo whiteServo;
Servo blueServo;

//Wheels
DGMotor leftMotor(Serial6, 1);
DGMotor rightMotor(Serial7, 1);


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

  //Initialize SPI channel
  pinMode(8, OUTPUT);
  SPI.begin();
  SPI.usingInterrupt(128);

  sortingServo.attach(SORTING_SERVO_PIN);
  redServo.attach(RED_SERVO_PIN);
  blueServo.attach(BLUE_SERVO_PIN);
  whiteServo.attach(WHITE_SERVO_PIN);

  leftMotor.begin(BAUD_RATE_1);
  rightMotor.begin(BAUD_RATE_1);


  //Initialize pixy camera and interrupt timer
  //pixy.init();
  //pixy.setLamp(255, 255);
  //pixyTimer.priority(128);
  //pixyTimer.begin(pixyISR, PIXY_SAMPLE_PERIOD);

  PWM_Instance = new Teensy_PWM(PWM_PIN, PWM_FREQ, PWM_DC);
//  threads.addThread(thread_func, 1);

}

void loop() {
  // put your main code here, to run repeatedly:

}

// ---------------------------------------------
// Interrupts 
// ---------------------------------------------
//void pixyISR(){
//  pixy.ccc.getBlocks();
//  if(pixy.ccc.numBlocks > 0){
//    signature = pixy.ccc.blocks[0].m_signature;
//  }
//}


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

// ---------------------------------------------
// Other Functions
// ---------------------------------------------
void drop_blue(){
  blueServo.write(90);
}
void drop_red() {
  redServo.write(90);
}
void drop_white() {
  whiteServo.write(90);
}
