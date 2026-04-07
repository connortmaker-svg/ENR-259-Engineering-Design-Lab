#include "BricoGeek_DirectDriveServo.h"

// Create two servo objects on different ports
BricoGeek_DirectDriveServo servoA(Serial1, 0x01); // Servo A on Serial1
BricoGeek_DirectDriveServo servoB(Serial2, 0x02); // Servo B on Serial2

void setup() {
    Serial.begin(115200);

    // Initialize both
    servoA.begin(115200);
    servoB.begin(115200);

    servoA.setMode(0);
    servoB.setMode(0);
}

void loop() {
    // Move Servo A
    servoA.setPosition(1500);

    // Move Servo B to the opposite position
    servoB.setPosition(2500);

    delay(2000);
    
    getMode();
    // Swap positions
    servoA.setPosition(2500);
    servoB.setPosition(1500);

    delay(2000);
}
