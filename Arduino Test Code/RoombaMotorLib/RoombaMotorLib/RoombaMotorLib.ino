#include "DGMotor.h"


DGMotor leftMotor(Serial1, 1);
DGMotor rightMotor(Serial2, 1);

void setup() {
  Serial.begin(115200); // For debug output

  // Initialize motor serial communication
  leftMotor.begin(115200);
  rightMotor.begin(115200);
    Serial.println("meow");
  delay(1000); // Allow motors to initialize

}

// Function to brake both tires
void driveBreak() {
  leftMotor.brake();
  rightMotor.brake();
}

// Helper to switch tires to position mode
void setBothTiresToPosMode() {
  driveBreak();
  leftMotor.setPositionMode();
  rightMotor.setPositionMode();
}

// Helper to switch tires to velocity mode
void setBothTiresToVelocityMode() {
  driveBreak();
  leftMotor.setVelocityMode();
  rightMotor.setVelocityMode();
}

// Turn Left: Set Right motor to an angle
void turnLeft(int deg) {
  driveBreak();
  rightMotor.setMotorDegrees(deg);
}

// Turn Right: Set Left motor to an angle
void turnRight(int deg) {
  driveBreak();
  leftMotor.setMotorDegrees(deg);
}

// Set both tires on (Velocity Mode Demo)
void setBothTiresOn() {
  setBothTiresToVelocityMode();
  leftMotor.setMotorSpeed(100);
  rightMotor.setMotorSpeed(100);

  delay(2000);

  leftMotor.setMotorSpeed(0);
  rightMotor.setMotorSpeed(0);
}

void loop() {
  // Implement your control logic here via Serial, sensors, etc.
  // Example: simple serial menu echoing original logic

  if (Serial.available() > 0) {
    int sel = Serial.parseInt();
    // Clear buffer
    while (Serial.available()) {
      Serial.read();
    }

    switch (sel) {
      case 1:
        // Set Speed (Example: 100 RPM)
        rightMotor.setMotorSpeed(100);
        leftMotor.setMotorSpeed(100);
        delay(100);
        
        Serial.println("Running Motors");
        break;
      case 2:
        driveBreak();
        break;
      case 3:
        setBothTiresToPosMode();
        leftMotor.setMotorDegrees(30);
        rightMotor.setMotorDegrees(30);

      break;
      case 5:
        setBothTiresOn();
        break;
      default:
        Serial.println("Invalid choice.");
        break;
    }
  }
}