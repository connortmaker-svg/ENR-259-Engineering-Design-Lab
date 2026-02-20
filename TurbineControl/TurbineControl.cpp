#include "Arduino.h"
#include "TurbineControl.h"

TurbineControl::TurbineControl(int pin) {
  _pin = pin;
}

void TurbineControl::begin() {
  // Attach to pin with wide range (500-20000us) as per original code
  _esc.attach(_pin, 500, 20000);
}

void TurbineControl::arm() {
  // Send STOP signal immediately to arm (stop the beeping)
  _esc.writeMicroseconds(1000);
  Serial.println("Turbine Armed (1000us sent).");
}

void TurbineControl::turnOn() {
  _esc.writeMicroseconds(2000); // Max Throttle
  Serial.println("Turbine ON (2000us sent).");
}

void TurbineControl::turnOff() {
  _esc.writeMicroseconds(1000); // Min Throttle / Stop
  Serial.println("Turbine OFF (1000us sent).");
}

void TurbineControl::debugControl() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // remove whitespace
        
        if (input == "HIGH" || input == "2") {
            turnOn();
        }
        else if (input == "LOW" || input == "1" || input == "ARM") {
            arm(); // arm and turnOff are essentially the same signal
        }
        else {
            int val = input.toInt();
            // Safety: Don't allow extremely low values that differ from 0/stop might confuse ESC
            if (val >= 1000 && val <= 20000) {
                _esc.writeMicroseconds(val);
                Serial.print("Speed set to: ");
                Serial.println(val);
            }
            else if (val > 0 && val < 1000) {
                 Serial.println("Value too low! Sending 1000 (Stop) to be safe.");
                 _esc.writeMicroseconds(1000);
            }
            else {
                 Serial.println("Ignored invalid value. Type '1' to ARM/STOP.");
            }
        }
    }
}
