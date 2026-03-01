/*
  SimpleTest.ino - Example usage of the TurbineControl library.
  
  Controls:
  - '1' or 'ARM' or 'LOW': Arm/Stop the motor (1000us)
  - '2' or 'HIGH': Turn ON the motor (2000us)
  - Custom values between 1000 and 20000 can also be entered.
*/

#include <TurbineControl.h>

// Initialize turbine on pin 9
TurbineControl turbine(9);

void setup() {
  Serial.begin(9600);
  
  turbine.begin();
  
  // Arm the motor on startup
  turbine.arm();
  
  Serial.println("=========================================");
  Serial.println("       TURBINE CONTROL LIBRARY READY     ");
  Serial.println("=========================================");
}

void loop() {
  // Use debugControl to handle Serial input for testing
  // In a real application, you would just call turbine.turnOn() or turbine.turnOff() based on your logic.
  turbine.debugControl();
}
