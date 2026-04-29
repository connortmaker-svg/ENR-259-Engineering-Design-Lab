#include "DGMotor.h"


int input1 = 14;
int input2 = 15;
int enable = 36;


DGMotor leftMotor(Serial7, 1);
DGMotor rightMotor(Serial6,1);


void setup() {
  Serial.begin(115200); // For debug output

  // Initialize motor serial communication
  leftMotor.begin(115200);
  rightMotor.begin(115200);
  delay(10);
  Serial.println("meow");
  delay(1000); // Allow motors to initialize
 
  
  
  // pinMode(enable, OUTPUT);
  // pinMode(input1, OUTPUT);
  // pinMode(input2, OUTPUT);

  // digitalWrite(enable, HIGH);
  // digitalWrite(input2, LOW);

  // analogWrite(input1, 300);

  // setBothTiresOn();

 
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
  leftMotor.setMotorSpeed(-300);
  rightMotor.setMotorSpeed(300);

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
      // Set Speed1 (Example: 100 RPM)
      rightMotor.setMotorSpeed(0);
      leftMotor.setMotorSpeed(400);
      delay(100);

      Serial.println("Running Motors");
      break;
    case 2:
      driveBreak();
      break;
    case 3:
      setBothTiresToVelocityMode();
      rightMotor.setMotorSpeed(-300);
      leftMotor.setMotorSpeed(300);
      delay(100);

      break;
    case 4:
      leftMotor.setMotorSpeed(300);
      //delay(500);
      //leftMotor.setMotorSpeed(0);
      break;
    case 5:
      break;
    case 6:
    
      setBothTiresOn();
      break;
    default:
      Serial.println("Invalid choice.");
      break;
    }
  }
}