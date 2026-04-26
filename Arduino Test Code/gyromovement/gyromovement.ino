#include "DGMotor.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define RAD2DEG (180.0/PI)

sensors_event_t a, g, temp;

// z calib offset
float GzError = 0;
// yaw
float yaw = 0;
int TpLeft = 190;  // Target base power for left motor
int TpRight = 190; // Target base power for right motor

float Kp = 2.5; 

// timing var for dt
unsigned long lastTime = 0;

DGMotor leftMotor(Serial6, 1);
DGMotor rightMotor(Serial7, 1);
Adafruit_MPU6050 mpu;

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);
  while(!Serial) {}

  leftMotor.begin(115200);
  rightMotor.begin(115200);
  Serial.println("meow");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }
  Serial.println("MPU6050 Found!");

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("Stay Still");
  delay(1000);
  
  //grab 100 samples of Gz to determine the mean gyro offset
  for(int i = 0; i < 100; i++) {
    mpu.getEvent(&a, &g, &temp);
    GzError += g.gyro.z;
    delay(10);
  }
  GzError /= 100.0;
  //end
  
  Serial.println("Calibration Completed!");
  lastTime = millis(); //start calculating time passed
}

void loop() {
  if(Serial.available() > 1){
      int choice = Serial.parseInt();
      switch(choice){
        case 1:
          {
          Serial.read(); //clear buffer
          while(Serial.available() < 1 ); //wait until input
          int speed = Serial.parseInt();
          leftMotor.setMotorSpeed(speed);
          }
        break;
        case 2: 
          {
          Serial.read(); //clear buffer
          while(Serial.available() < 1 ); //wait until input
          int speed = Serial.parseInt();
          rightMotor.setMotorSpeed(speed);
          }
        break;
        case 3:
          {
          Serial.read(); //clear buffer
          while(Serial.available() < 1 ); //wait until input
          Kp = Serial.parseInt();
          Serial.print("New Kp is: ");
          Serial.println(Kp);
          }
        break;
        case 4:
          {
          unsigned long startDriveTime = millis();
          lastTime = millis(); // update lastTime to reflect new start time of function
          
          while(millis() - startDriveTime <= 3000) {
            drive();
          }
          driveBreak(); // stop moving
          }
        break;
        case 5:
          {
          Serial.read(); //clear buffer
          while(Serial.available() < 1 ); //wait until input
          TpLeft = Serial.parseInt();
          }
        break;
        case 6:
          {
          Serial.read(); //clear buffer
          while(Serial.available() < 1 ); //wait until input
          TpRight = Serial.parseInt();
          }
        break;
        case 7:
          driveBreak();
        break;
        case 8:
          String buffer = "Kp: " + (String) Kp + " TpLeft: " + (String) TpLeft + " TpRight: " + (String) TpRight;
          Serial.println(buffer);
        break;
      }
  }
}

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

void menu() {
  Serial.println("1.)  Change Left Motor Speed");
  Serial.println("2.) Change Right Motor Speed");
  Serial.println("3.)                Change Kp");
  Serial.println("4.)                    Drive");
  Serial.println("5.)            Change TpLeft");
  Serial.println("6.)           Change TpRight");
  Serial.println("7.)                    Break");
  Serial.println("8.)               Print Data");
}