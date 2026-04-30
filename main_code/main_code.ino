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

#include <Teensy_PWM.h>

// for SSD1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//for gyro and sensor
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_VL53L0X.h"

//for wheels
#include "DGMotor.h"
#include "HardwareSerial.h"

// for Teensy interrupts
#include <avr/io.h>
#include <avr/interrupt.h>

// for Pixy2
#include <Pixy2SPI_SS.h>

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

//GPIO pins for distance interrupts
#define VL53LOX_INT_PIN 26  //interrupt pin
#define VL53LOX_SHUT_PIN 27 //shutdown pin
#define LOW_THREASHOLD 50
#define HIGH_THREASHOLD 100

// for debug display and vacuum
#define BAUD_RATE_0 9600

//PWM pin and values for intake motor
#define PWM_PIN  4
#define PWM_FREQ  47400.0f
#define PWM_DC  50.0f //Duty cycle in in percentage (0-100%)

// for the servo wheels, pixy, and servo sort
#define BAUD_RATE_1 115200

#define RAD2DEG (180.0/PI);

// ---------------------------------------------
// Global Variables
// ---------------------------------------------

//volatile uint16_t signature;
volatile uint8_t VL53LOX_State = LOW;

//Variables for gyro PID 
float GzError = 0; // z calib offset
float yaw = 0;     // yaw
int TpLeft = 190;  // Target base power for left motor
int TpRight = 190; // Target base power for right motor
float Kp = 2.5; 

int speedLeft = 0;
int speedRight = 0;

unsigned long lastTime = 0;

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
Pixy2SPI_SS pixy;
Servo seat;
Servo redServo;
Servo whiteServo;
Servo blueServo;

//Wheels
DGMotor leftMotor(Serial6, 1);
DGMotor rightMotor(Serial7, 1);

//Gyro
sensors_event_t a, g, temp;
Adafruit_MPU6050 mpu;

//Distance Sensor (I2C address: 0x29)
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

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

  seat.attach(SORTING_SERVO_PIN);
  redServo.attach(RED_SERVO_PIN);
  blueServo.attach(BLUE_SERVO_PIN);
  whiteServo.attach(WHITE_SERVO_PIN);

  leftMotor.begin(BAUD_RATE_1);
  rightMotor.begin(BAUD_RATE_1);

  Wire.begin();
  Wire.setClock(400000);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }
  Serial.println("MPU6050 Found!");

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  //Serial.println("Stay Still");
  delay(1000);

  leftMotor.setVelocityMode();
  rightMotor.setVelocityMode();
  
  //grab 100 samples of Gz to determine the mean gyro offset
  for(int i = 0; i < 100; i++) {
    mpu.getEvent(&a, &g, &temp);
    GzError += g.gyro.z;
    delay(10);
  }
  GzError /= 100.0;
  //end
  
  //Serial.println("Calibration Completed!");
  lastTime = millis(); //start calculating time passed
  
  pinMode(VL53LOX_SHUT_PIN, INPUT_PULLUP);
  pinMode(VL53LOX_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(VL53LOX_INT_PIN), VL53LOXISR, CHANGE);

  // if lox.begin failes its becasue it was a warm boot and the VL53LOX is in
  // continues mesurement mode we can use an IO pin to reset the device in case
  // we get stuck in this mode
  while (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    Serial.println("Adafruit VL53L0X XShut set Low to Force HW Reset");
    digitalWrite(VL53LOX_SHUT_PIN, LOW);
    delay(100);
    digitalWrite(VL53LOX_SHUT_PIN, HIGH);
    Serial.println("Adafruit VL53L0X XShut set high to Allow Boot");
    delay(100);
  }
  //Set GPIO config to trigger when range is lower then LowThreshold
  lox.setGpioConfig(VL53L0X_DEVICEMODE_CONTINUOUS_RANGING,
                    VL53L0X_GPIOFUNCTIONALITY_THRESHOLD_CROSSED_LOW,
                    VL53L0X_INTERRUPTPOLARITY_LOW);

  //Quantize threasholds
  FixPoint1616_t LowThreashHold = (LOW_THREASHOLD * 65536.0);
  FixPoint1616_t HighThreashHold = (HIGH_THREASHOLD * 65536.0);
  Serial.println("Set Interrupt Threasholds... ");
  lox.setInterruptThresholds(LowThreashHold, HighThreashHold, true);
  lox.setDeviceMode(VL53L0X_DEVICEMODE_CONTINUOUS_RANGING, false);

  //Initialize pixy camera and interrupt timer
  pixy.init();
  pixy.setLamp(255, 255);
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

void VL53LOXISR() {
  // Read if we are high or low (low = in range)
  VL53LOX_State = digitalRead(VL53LOX_INT_PIN);
}

// ---------------------------------------------
// Driving Functions
// ---------------------------------------------

void drive() {
  // calculate dt
  long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; //ms to s
  lastTime = currentTime; //update

  mpu.getEvent(&a, &g, &temp);
  
  float Gz_deg = (g.gyro.z - GzError) * RAD2DEG; //calculate Gz by accounting for offset and converting to deg/s

  //integrate to get position
  yaw += Gz_deg * dt; 

  // neg yaw: ccw
  // pos yaw: cw
  double correction = Kp * yaw; 

  double powerLeft = TpLeft - correction; 
  double powerRight = TpRight + correction;

  powerLeft = constrain(powerLeft, 0, 300);
  powerRight = constrain(powerRight, 0, 300);

  //correct
  leftMotor.setMotorSpeed(-1 * powerLeft);
  rightMotor.setMotorSpeed(powerRight);
}

void driveBreak() {
  leftMotor.brake();
  rightMotor.brake();
}

void timedDrive(unsigned long stopTime) {
  unsigned long startDriveTime = millis();
  lastTime = millis();

  while(millis() - startDriveTime <= stopTime) {
    drive();
  }
  driveBreak(); 
  
}

void sensorDrive(){
  
  while(VL530LOX_STATE == HIGH) {
    drive();
  }
  //Clear interrupt
  driveBreak();
  lox.clearInterruptMask(false);
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

/*
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
*/

// ---------------------------------------------
// Sorting Functions
// ---------------------------------------------

void detectBalls() {
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks > 0) {
    //Find area and age of block
    uint16_t area = pixy.ccc.blocks[0].area();

    // if the area of the block is above a certain threshold, read the signature
    if (area > 4000) {
      uint16_t signature = pixy.ccc.blocks[0].m_signature;
      switch (signature) {

        //Red ball detected
      case 1:
        //Serial.println("Red ball detected");
        pixy.ccc.blocks[0].printInfo();
        seat.write(50);
        break;

        //White ball detected
      case 2:
        //Serial.println("White ball detected");
        pixy.ccc.blocks[0].printInfo();
        seat.write(100);
        break;
        //Blue ball detected
      case 3:
        //Serial.println("Blue ball detected");
        pixy.ccc.blocks[0].printInfo();
        seat.write(150);
        break;
      default:

        break;
      }
    }

  }
}

void drop_blue(){
  blueServo.write(90);
}
void drop_red() {
  redServo.write(90);
}
void drop_white() {
  whiteServo.write(90);
}
