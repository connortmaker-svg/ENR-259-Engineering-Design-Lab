#include "DGMotor.h"

DGMotor leftMotor(Serial7, 1);
DGMotor rightMotor(Serial6,1);

void setup() {
  Serial.begin(115200); // For debug output

  leftMotor.begin(115200);
  rightMotor.begin(115200);
  delay(10);

  Serial.println("Motors Initialized!");
}

void loop() {
  leftMotor.setMotorSpeed(300);
  rightMotor.setMotorSpeed(-300);
}
